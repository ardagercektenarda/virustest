#define NOMINMAX
#include <Windows.h>
#include "config.hpp"
#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"
#include "helpers/input.hpp"
#include "xorstr.h"
#include "BASS/API.h"
#include "hooks.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "render.hpp"
#include "security.hpp"
#include <chrono>
#include <thread>
#include "changer_p/steam_shonax.h"
#include <Wininet.h>
#include <string>
#include "eireneprotect.h"
#pragma comment(lib, "wininet.lib")
#include "Atakan75p.h"

void* saved_hwnd = nullptr;
void silent_crash();
using namespace std;
string replaceAll(string subject, const string& search, const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

string DownloadString(string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

char* HashString(const char* szString)
{
	int iLen = strlen(szString);
	char* szText = new char[iLen + 1];

	for (int i = 0; i < iLen; ++i)
	{
		szText[i] = szString[i] + 1;
	}

	szText[iLen] = 0;
	return szText;
}
std::string lol = DownloadString("http://destinycheats.xyz/Version.txt");

auto CheckRemoteDebuggerPresentAPI()
{
	BOOL bIsDbgPresent = FALSE;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &bIsDbgPresent);
	return bIsDbgPresent;
}
void lockcursor()
{
	if (Menu::Get().IsVisible()) {
		g_VGuiSurface->UnlockCursor();
	}
	else {
		g_VGuiSurface->LockCursor();
	}
	g_Input->m_mouse_initiated = !Menu::Get().IsVisible();

	g_InputSystem->EnableInput(!Menu::Get().IsVisible());

	g_Input->m_mouse_active = Menu::Get().IsVisible();

	g_InputSystem->ResetInputState();
}

void swap_hwnd() {
	if (Menu::Get().IsVisible())
	{
		if (!saved_hwnd)
			swap(saved_hwnd, g_InputSystem->GetWindow());
	}
	else if (saved_hwnd)
		swap(saved_hwnd, g_InputSystem->GetWindow());
}

char g_szAppPath[MAX_PATH] = { 0 };


BOOL SetAppPath(LPSTR lpszOut)
{
	//Write application path to string

	if (!lpszOut)
		return FALSE;

	//Get full file name
	if (!GetModuleFileNameA(NULL, lpszOut, MAX_PATH))
		return FALSE;

	//Remove file name from string
	for (size_t i = strlen(lpszOut); i >= 3; i--) {
		if (lpszOut[i] == '\\')
			break;

		lpszOut[i] = 0;
	}

	return TRUE;
}

vfunc_hook gc_hook;
using GCRetrieveMessage = EGCResult(__thiscall*)(void*, uint32_t * punMsgType, void* pubDest, uint32_t cubDest, uint32_t * pcubMsgSize);
using GCSendMessage = EGCResult(__thiscall*)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);

EGCResult __fastcall hkGCRetrieveMessage(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	static auto oGCRetrieveMessage = gc_hook.get_original<GCRetrieveMessage>(2);
	auto status = oGCRetrieveMessage(ecx, punMsgType, pubDest, cubDest, pcubMsgSize);

	if (status == k_EGCResultOK)
	{

		void* thisPtr = nullptr;
		__asm mov thisPtr, ebx;
		auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

		uint32_t messageType = *punMsgType & 0x7FFFFFFF;
		ProtoFeatures.ReceiveMessage(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);
	}

	return status;
}

EGCResult __fastcall hkGCSendMessage(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)
{
	static auto oGCSendMessage = gc_hook.get_original<GCSendMessage>(0);

	bool sendMessage = ProtoFeatures.PreSendMessage(unMsgType, const_cast<void*>(pubData), cubData);

	if (!sendMessage)
		return k_EGCResultOK;

	return oGCSendMessage(ecx, unMsgType, const_cast<void*>(pubData), cubData);
}

#include "features/lazy_importer.hpp"
void autoaccept();

DWORD WINAPI OnDllAttach(LPVOID base)
{
	while (!FindWindowA("Valve001", NULL))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}

	while (!GetModuleHandleA("serverbrowser.dll"))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}




#ifdef _DEBUG
	Utils::AttachConsole();
#endif

	try {
		Config->Setup();

		Utils::ConsolePrint(_xor_("Initializing...\n").c_str());
		Interfaces::Initialize();

		/*if (auto const n = lol.find("client_panorama.dll") != std::string::npos)
		{

		}
		else
		{
			LI_FN(MessageBoxA).get()(NULL, _xor_("Maintenance, repair work!").c_str(), _xor_("AxePrime Premium").c_str(), NULL);
			exit(-1);
		}*/

		Interfaces::Dump();

		/*if (g_EngineClient->GetEngineBuildNumber() != 13744) {
			LI_FN(MessageBoxA).get()(NULL, _xor_("New CS:GO update has been released, wait until cheat will be updated!").c_str(), _xor_("AxePrime Premium").c_str(), NULL);
			silent_crash();
		}*/



		InputSys::Get().Initialize();
		NetvarSys::Get().Initialize();
		Render::Get().Initialize();


		Menu::Get().Initialize();
		Hooks::Initialize();
		if (!SetAppPath(g_szAppPath)) {
			return 0;
		}
		g_oSteamAPI.Initialize(g_szAppPath);
		pSteamFriends = g_oSteamAPI.SteamFriendsInterface();
		SteamName = pSteamFriends->GetPersonaName();



		auto SteamClient = ((_ISteamClient * (__cdecl*)(void))GetProcAddress(GetModuleHandleA("steam_api.dll"), "SteamClient"))();
		g_SteamGameCoordinator = (_ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface((void*)1, (void*)1, "SteamGameCoordinator001");
		g_SteamUser = SteamClient->GetISteamUser((void*)1, (void*)1, "SteamUser019");



		gc_hook.setup(g_SteamGameCoordinator);
		gc_hook.hook_index(0, hkGCSendMessage);
		gc_hook.hook_index(2, hkGCRetrieveMessage);

		ProtoFeatures.SendClientHello();
		ProtoFeatures.SendMatchmakingClient2GCHello();
		ProtoFeatures.SendClientGcRankUpdate();
		ProtoFeatures.FixSendClientGcRankUpdate();




		if (k_skins.size() == 0) {
			initialize_kits();
		}

		BASS::bass_lib_handle = BASS::bass_lib.LoadFromMemory(bass_dll_image, sizeof(bass_dll_image));

		if (BASS_INIT_ONCE())
			BASS::bass_init = TRUE;

		static auto bass_needs_reinit = false;

		static std::string channels[] = {
			(" "),
			("http://www.rockantenne.de/webradio/channels/alternative.m3u"),
			("http://listen.technobase.fm/dsl.pls"),
			("http://streams.bigfm.de/bigfm-deutschrap-128-aac"),
			//("http://16983.live.streamtheworld.com/WMGCFMAACIHR.aac?pname=StandardPlayerV4&pversion=4.19.2-014&dist=triton-web&tdsdk=js-2.9&banners=none&sbmid=7710ab11-f99c-4b12-b9c0-3874777aec40"),
			("http://mp3.stream.tb-group.fm/tt.mp3"),
			("http://mp3.stream.tb-group.fm/clt.mp3"),
			("http://mp3.stream.tb-group.fm/ht.mp3"),
			("http://www.iloveradio.de/iloveradio.m3u"),
			("http://8bit.fm:8000/live"),
			("http://air.radiorecord.ru:8101/rr_320"),
			("https://dfm.hostingradio.ru/dfm96.aacp"),
			("http://ic2.101.ru:8000/v1_1"),
			("http://retroserver.streamr.ru:8043/retro256.mp3"),
			("http://dorognoe.hostingradio.ru:8000/radio"),
			("http://nashe1.hostingradio.ru/nashespb128.mp3"),
			("http://ic4.101.ru:8000/a219"),
			("http://ep256.hostingradio.ru:8052/europaplus256.mp3"),
			("http://radio.m0ne0n.cf:8000/stream")

		};

		InputSys::Get().RegisterHotkey(VK_INSERT, [base]() {
			Menu::Get().Toggle();
			swap_hwnd();

		});

		Utils::ConsolePrint("Finished.\n");

		while (!g_Unload) {

			if (IsDebuggerPresent() || CheckRemoteDebuggerPresentAPI())
			{
#ifndef _DEBUG
				silent_crash();
#endif
			}
			if (g_Options.misc_autoaccept)
				autoaccept();
			radio_muted = GetKeyState(g_Options.radiomute);

			const auto desired_channel = g_Options.radiochannel;

			if (BASS::bass_init && desired_channel)
			{
				static auto current_channel = 0;

				if (current_channel != desired_channel || bass_needs_reinit)
				{
					bass_needs_reinit = false;
					BASS_Start();
					auto channel = channels[desired_channel];
					BASS_OPEN_STREAM(channel.c_str());
					current_channel = desired_channel;
				}

				BASS_SET_VOLUME(BASS::stream_handle, radio_muted ? 0.f : g_Options.radiovolume / 100.f);
				BASS_PLAY_STREAM();
			}
			else if (BASS::bass_init)
			{
				bass_needs_reinit = true;
				BASS_StreamFree(BASS::stream_handle);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		FreeLibraryAndExitThread((HMODULE)base, 0);
		return false;
	}
	catch (const std::exception & ex) {
		Utils::ConsolePrint("An error occured during initialization:\n");
		Utils::ConsolePrint("%s\n", ex.what());
		Utils::ConsolePrint("Press any key to exit.\n");
		Utils::ConsoleReadKey();
		Utils::DetachConsole();

		FreeLibraryAndExitThread((HMODULE)base, 0);
	}
	return false;
}

BOOL WINAPI OnDllDetach()
{
#ifdef _DEBUG
	Utils::DetachConsole();
#endif

	return TRUE;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDll, _In_ DWORD fdwReason, _In_opt_ LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDll);
		CreateThread(nullptr, NULL, OnDllAttach, hinstDll, NULL, nullptr);
		return TRUE;
	case DLL_PROCESS_DETACH:
		if (lpvReserved == nullptr)
			return OnDllDetach();
		return TRUE;
	default:
		return TRUE;
	}
}