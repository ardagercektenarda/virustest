#include "render.hpp"

#include <mutex>

#include "features/visuals.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "menu.hpp"
#include "hooks.hpp"
#include "options.hpp"
#include "fonts/fonts.hpp"
#include "images.hpp"
#include "helpers/math.hpp"
#include "xorstr.h"
ImFont* g_pDefaultFont;
ImFont* g_pSecondFont;
ImFont* g_Font1;
ImFont* g_Font2;
ImFont* AimbotFont;
ImFont* TitleFont;

ImDrawListSharedData _data;

std::mutex render_mutex;

void Render::Initialize()
{
	ImGui::CreateContext();

	if (arkaplan == nullptr)
		D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &background, sizeof(background), 900, 600, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &arkaplan);

	if (profileimage == nullptr)
		D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &profilepic, sizeof(profilepic), 236, 236, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &profileimage);

	ImGui_ImplWin32_Init(InputSys::Get().GetMainWindow());
	ImGui_ImplDX9_Init(g_D3DDevice9);

	draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_rendering = new ImDrawList(ImGui::GetDrawListSharedData());

	GetFonts();

	ImGuiFreeType::BuildFontAtlas(ImGui::GetIO().Fonts);
}

void Render::GetFonts() {
	// menu font
	ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
		Fonts::bahnschrift,
		sizeof(Fonts::bahnschrift),
		13.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()
	);

	// esp font
	g_pDefaultFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
		Fonts::bahnschrift,
		sizeof(Fonts::bahnschrift),
		14.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()
	);

	// font for watermark; just example
	g_pSecondFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
		Fonts::bahnschrift,
		sizeof(Fonts::bahnschrift),
		24.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()
	);

	AimbotFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::tabs_compressed_data,
		Fonts::tabs_compressed_size,
		25.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()
	);

	ExAimbot = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::tabs_compressed_data,
		Fonts::tabs_compressed_size,
		18.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()
	);


	TitleFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::title_compressed_data,
		Fonts::title_compressed_size,
		18.f,
		nullptr,
		ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()
	);

}

void Render::ClearDrawList() {
	render_mutex.lock();
	draw_list_act->Clear();
	render_mutex.unlock();
}
float frameRate = 0.f;

std::string GetTimeString()
{
	time_t current_time;
	struct tm* time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}

void watermark() {
	if (g_Options.misc_watermark)
	{
		int width, height;
		g_EngineClient->GetScreenSize(width, height);




		frameRate = 0.9f * frameRate + (1.0 - 0.9f) * g_GlobalVars->absoluteframetime;
		std::stringstream ss;

		auto net_channel = g_EngineClient->GetNetChannelInfo();
		auto latency = (net_channel && g_EngineClient->IsInGame() && !g_EngineClient->IsPlayingDemo()) ? net_channel->GetAvgLatency(FLOW_OUTGOING) : 0.0f;

		static auto cl_updaterate = g_CVar->FindVar("cl_updaterate");
		latency -= 0.5f / cl_updaterate->GetFloat();

		auto ping = std::to_string((int)(std::fmax(0.0f, latency) * 1000.0f));


		ss << "AxePrime Premium | FPS : " << static_cast<int>(1.f / frameRate) << " | PING:  " << ping.c_str() << "ms " << "|  TIME:  ";
		ss << GetTimeString().c_str();

		Render::Get().RenderText(ss.str(), ImVec2(10, 5), 14.f, Color(255, 255, 255, 255), false, false);
	}
}

extern float side;
extern float delta_time;
void Arrows(float screen_width, float screen_height) {
	auto client_viewangles = QAngle();
	g_EngineClient->GetViewAngles(&client_viewangles);
	const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);

	constexpr auto radius = 350.f;
	auto draw_arrow = [&](float rot, Color color) -> void
	{
		std::vector<ImVec2> vertices;
		vertices.push_back((ImVec2(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
		vertices.push_back((ImVec2(screen_center.x + cosf(rot + DEG2RAD(4)) * (radius - 12), screen_center.y + sinf(rot + DEG2RAD(4)) * (radius - 12)))); //25
		vertices.push_back((ImVec2(screen_center.x + cosf(rot - DEG2RAD(4)) * (radius - 12), screen_center.y + sinf(rot - DEG2RAD(4)) * (radius - 12)))); //25

		Render::Get().RenderTriangle(vertices.at(0), vertices.at(1), vertices.at(2), Color::White, 2.f);
	};

	static auto alpha = 0.f; static auto plus_or_minus = false;
	if (alpha <= 0.f || alpha >= 255.f) plus_or_minus = !plus_or_minus;
	alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015); alpha = std::clamp(alpha, 0.f, 255.f);

	auto fake_color = Color(200, 0, 0, 170);
	const auto fake_rot = DEG2RAD((side < 0.0f ? 90 : -90) - 90);
	draw_arrow(fake_rot, fake_color);


}

void Render::BeginScene() {
	draw_list->Clear();
	draw_list->PushClipRectFullScreen();

	static float alpha;
	if (Menu::Get().IsVisible() && alpha < 0.5)			alpha += 0.01;
	else if (!Menu::Get().IsVisible())		alpha = 0.01;

	int w, h;
	g_EngineClient->GetScreenSize(w, h);

	draw_list->AddRectFilled(ImVec2(0, 0), ImVec2(w, h), ImGui::GetColorU32(ImVec4(0, 0, 0, alpha)));

	float x = w * 0.5f;
	float y = h * 0.5f;

	//if (Menu::Get().IsVisible())
	//	draw_list->AddImage(bgbrand_img, ImVec2(x, y), ImVec2(x + 200, y + 140));

	watermark();

	if (g_EngineClient->IsInGame() && g_LocalPlayer)
	{
		if (g_Options.misc_desync)
			Arrows(w, h);

		if (g_Options.nozoom && g_LocalPlayer->m_bIsScoped() && g_LocalPlayer->m_hActiveWeapon() &&
			g_LocalPlayer->m_hActiveWeapon()->m_iItemDefinitionIndex() != WEAPON_SG556 &&
			g_LocalPlayer->m_hActiveWeapon()->m_iItemDefinitionIndex() != WEAPON_AUG)
		{
			Render::Get().RenderLine((float)0, y, (float)w, y, Color::Black);
			Render::Get().RenderLine(x, (float)0, x, (float)h, Color::Black);
		}
		//if (g_Options.misc_desync)
		//	Render::Get().RenderText(side < 0.0f ? ">" : " <", side > 0.0f ? x - 120 : x + 120, y - 7.5F, 24.f, Color::White, true, g_pSecondFont);

		Visuals::Get().AddToDrawList();
	}
	render_mutex.lock();
	*draw_list_act = *draw_list;
	render_mutex.unlock();
}

ImDrawList* Render::RenderScene()
{
	if (render_mutex.try_lock())
	{
		*draw_list_rendering = *draw_list_act;
		render_mutex.unlock();
	}

	return draw_list_rendering;
}


float Render::RenderText(const std::string& text, ImVec2 pos, float size, Color color, bool center, bool outline, ImFont* pFont)
{
	ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());
	if (!pFont->ContainerAtlas) return 0.f;
	draw_list->PushTextureID(pFont->ContainerAtlas->TexID);

	if (center)
		pos.x -= textSize.x / 2.0f;

	if (outline) {
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
	}

	draw_list->AddText(pFont, size, pos, GetU32(color), text.c_str());

	draw_list->PopTextureID();

	return pos.y + textSize.y;
}

void Render::RenderCircle3D(Vector position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d))
			return;

		RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
	}
}