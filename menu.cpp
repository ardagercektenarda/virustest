#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#define NOMINMAX

#include <Windows.h>
#include <chrono>
#include "Menu.hpp"

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "ui.hpp"
#include "config.hpp"
#include "features/Notification.h"
#include "features/Misc.h"

#include "render.hpp"
#include <experimental/filesystem>

#include "hooks.hpp"

namespace fs = std::experimental::filesystem;


void NewSeparator(const char* label)
{
	auto size = ImGui::CalcTextSize(label);
	auto draw_list = ImGui::GetWindowDrawList();
	const auto width = ImGui::GetContentRegionAvailWidth();
	const auto win_pos = ImGui::GetWindowPos();
	const auto cursor = ImVec2(ImGui::GetCursorPosX() + win_pos.x, ImGui::GetCursorPosY() + win_pos.y);
	const auto pos = ImVec2(cursor.x + width / 2 - size.x / 2, cursor.y);
	const auto col = ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text));
	draw_list->AddText(pos, col, label);
	draw_list->AddLine(ImVec2(cursor.x, cursor.y + size.y / 2), ImVec2(pos.x - 5.f, cursor.y + size.y / 2), col);
	draw_list->AddLine(ImVec2(pos.x + size.x + 5.f, cursor.y + size.y / 2), ImVec2(cursor.x + width, cursor.y + size.y / 2), col);
	ImGui::NewLine();
}

const char* ___(const char* english, const char* turkish)
{
	return g_Options.menu_language ? turkish : english;
}

void pipi(float otzubir) {
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + otzubir);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + otzubir);
}

const char* const KeyNames[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"VK_TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU"
};

static bool IsKeyPressedMap(ImGuiKey key, bool repeat = true)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}

bool KeyBind(const char* label, int& k, const ImVec2& size_arg = ImVec2(0, 0))
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), label_size.y + style.FramePadding.y * 2.0f);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	const bool focus_requested = ImGui::FocusableItemRegister(window, g.ActiveId == id);
	//const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	//const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool hovered = ImGui::ItemHoverable(frame_bb, id);

	if (hovered) {
		ImGui::SetHoveredID(id);
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	const bool user_clicked = hovered && io.MouseClicked[0];

	if (focus_requested || user_clicked) {
		if (g.ActiveId != id) {
			// Start edition
			memset(io.MouseDown, 0, sizeof(io.MouseDown));
			memset(io.KeysDown, 0, sizeof(io.KeysDown));
			k = 0;
		}
		ImGui::SetActiveID(id, window);
		ImGui::FocusWindow(window);
	}
	else if (io.MouseClicked[0]) {
		// Release focus when we click outside
		if (g.ActiveId == id)
			ImGui::ClearActiveID();
	}

	bool value_changed = false;
	int key = k;

	if (g.ActiveId == id) {
		for (auto i = 0; i < 5; i++) {
			if (io.MouseDown[i]) {
				switch (i) {
				case 0:
					key = VK_LBUTTON;
					break;
				case 1:
					key = VK_RBUTTON;
					break;
				case 2:
					key = VK_MBUTTON;
					break;
				case 3:
					key = VK_XBUTTON1;
					break;
				case 4:
					key = VK_XBUTTON2;
					break;
				}
				value_changed = true;
				ImGui::ClearActiveID();
			}
		}
		if (!value_changed) {
			for (auto i = VK_BACK; i <= VK_RMENU; i++) {
				if (io.KeysDown[i]) {
					key = i;
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
		}

		if (IsKeyPressedMap(ImGuiKey_Escape)) {
			k = 0;
			ImGui::ClearActiveID();
		}
		else {
			k = key;
		}
	}

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

	char buf_display[64] = "None";

	ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_FrameBg]), true, style.FrameRounding);

	if (k != 0 && g.ActiveId != id) {
		strcpy_s(buf_display, KeyNames[k]);
	}
	else if (g.ActiveId == id) {
		strcpy_s(buf_display, "<Press a key>");
	}

	const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;
	ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);
	//RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, GetColorU32(ImGuiCol_Text), style.ButtonTextAlign, &clip_rect);
	//draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, NULL, 0.0f, &clip_rect);

	if (label_size.x > 0)
		ImGui::RenderText(ImVec2(total_bb.Max.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}

void Kick(int userid)
{
	char command[1024]; sprintf(command, "callvote kick %d", userid);

	g_EngineClient->ExecuteClientCmd(command);
}

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"
#include "features/visuals.hpp"



static char* boxes[] = { "Off", "Regular", "Edge", "Rounded" };

namespace ImGuiEx
{
	inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
	{
		auto clr = ImVec4{
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};

		if (ImGui::ColorEdit4(label, &clr.x, show_alpha)) {
			v->SetColor(clr.x, clr.y, clr.z, clr.w);
			return true;
		}
		return false;
	}
	inline bool ColorEdit3(const char* label, Color* v)
	{
		return ColorEdit4(label, v, false);
	}
}

template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h)
{
	bool values[N] = { false };
	values[activetab] = true;
	for (auto i = 0; i < N; ++i) {
		if (ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) activetab = i;
		if (i < N - 1) ImGui::SameLine();
	}
}


void RenderEspTab()
{
	char* chamstypes[] = { "Material", "Flat", "Glass", "Crystal", "Gold", "Dark Crystal", "Silver" };

	ImGui::BeginGroup();
	ImGui::BeginChild(u8"##ESP1", ImVec2(250, 475), true);
	{
		NewSeparator("ESP");
		ImGui::NewCheckbox(___(u8"Aktif##ESP", u8"Enabled##ESP"), &g_Options.esp_enabled);
		ImGui::ColorEdit3(___(u8"Görünürken##31", u8"Visible##31"), g_Options.color_esp_visible);
		ImGui::ColorEdit3(___(u8"Görünmezken##31", u8"Occluded##31"), g_Options.color_esp_occluded);
		ImGui::NewCheckbox(___(u8"Sadece Görünürken", u8"Visible only"), &g_Options.esp_visible_only);
		ImGui::NewCheckbox(___(u8"Sadece Düşman", u8"Enemy only"), &g_Options.esp_enemy_only);
		ImGui::Combo(___(u8"Kutu", u8"Box"), &g_Options.esp_player_box, boxes, IM_ARRAYSIZE(boxes), 4);
		ImGui::NewCheckbox(___(u8"İskelet", u8"Skeleton"), &g_Options.esp_player_skeleton);
		ImGui::ColorEdit4(___(u8"İskelet Rengi", u8"Skeleton Color"), g_Options.color_esp_skeleton);
		ImGui::NewCheckbox(___(u8"İsim", u8"Name"), &g_Options.esp_player_names);
		ImGui::NewCheckbox(___(u8"Can", u8"Health"), &g_Options.esp_player_health);
		ImGui::NewCheckbox(___(u8"Armor", u8"Armour"), &g_Options.esp_player_armour);
		ImGui::NewCheckbox(___(u8"Silah", u8"Weapon"), &g_Options.esp_player_weapons);
		ImGui::NewCheckbox(___(u8"Mermi", u8"Ammo"), &g_Options.show_ammo);
		ImGui::NewCheckbox(___(u8"Düşmüş Silahlar", u8"Dropped Weapons"), &g_Options.esp_dropped_weapons);
		ImGui::NewCheckbox(___(u8"İmha Kiti", u8"Defuse Kit"), &g_Options.esp_defuse_kit);
		ImGui::NewCheckbox(___(u8"Kurulmuş C4", u8"Planted C4"), &g_Options.esp_planted_c4);
		ImGui::NewCheckbox(___(u8"İtem Esp", u8"Item Esp"), &g_Options.esp_items);
		if (g_Options.esp_items)
			ImGui::SliderFloat(___(u8"Uzaklığı", u8"Distance"), &g_Options.esp_distance_dz, 64.0f, 8192.f, "%.1fu");
		if (ImGui::BeginCombo(___(u8"Flaglar", u8"Flags"), "..."))
		{
			ImGui::Selectable((___(u8"Armor", u8"Armour")), &g_Options.esp_flags_armor, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable((___(u8"Çözüyor", u8"Defusing")), &g_Options.esp_flags_defusing, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable((___(u8"İmha Kiti", u8"Defuse Kit")), &g_Options.esp_flags_kit, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable((___(u8"Rehine Taşıyıcısı", u8"Hostage Carrier")), &g_Options.esp_flags_hostage, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable((___(u8"Dürbün Açık", u8"Scoped")), &g_Options.esp_flags_scoped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable((___(u8"Para", u8"Money")), &g_Options.esp_flags_money, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::Selectable((___(u8"Bomba", u8"Bomb")), &g_Options.esp_flags_c4, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			ImGui::EndCombo();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(u8"##ESP2", ImVec2(250, 475), true);
	{
		NewSeparator(u8"Chams");
		ImGui::NewCheckbox(___(u8"Aktif Et##3224", u8"Enabled##2234"), &g_Options.chams_player_enabled);
		if (g_Options.chams_player_enabled)
		{
			ImGui::NewCheckbox(___(u8"Sadece Düşman##3232", u8"Enemy Only##23232"), &g_Options.chams_player_enemies_only);
			ImGui::ColorEdit4(___(u8"Görünürken##123123", u8"Visible##123123"), g_Options.color_chams_player_visible);
			ImGui::ColorEdit4(___(u8"Duvar Arkası##123123", u8"Occluded##12321"), g_Options.color_chams_player_occluded);
			ImGui::NewCheckbox(___(u8"Çizgili##23123", u8"WireFrame##12321"), &g_Options.chams_player_wireframe);
			ImGui::NewCheckbox(___(u8"Düz##123123", u8"Flat##123123"), &g_Options.chams_player_flat);
			ImGui::NewCheckbox(___(u8"Sadece Görünürken##312312", u8"Visible Only##123123"), &g_Options.chams_player_visibleonly);
		}
		/*NewSeparator(u8"Kol Chams");
		ImGui::NewCheckbox(___(u8"Aktif Et##32323", u8"Enabled##321321"), &g_Options.chams_arms_enabled);
		if (g_Options.chams_arms_enabled)
		{
			ImGui::Combo("Materials##1", &g_Options.chams_arms_mat, chamstypes, IM_ARRAYSIZE(chamstypes));
			ImGui::NewCheckbox(___(u8"Çizgili##123123", u8"WireFrame##123213"), &g_Options.chams_arms_wireframe);
			ImGui::NewCheckbox(___(u8"Material##123123", u8"Reflective##123123"), &g_Options.chams_arms_reflect);
		}
		ImGui::SameLine(150);
		ImGui::ColorEdit4(___(u8"##ArmColor", u8"##ArmColor"), g_Options.chams_arms_color);*/
		NewSeparator(u8"Silah Chams");
		ImGui::NewCheckbox(___(u8"Aktif Et##1231234", u8"Enabled##412321321"), &g_Options.chams_weapon_enabled);
		if (g_Options.chams_weapon_enabled)
		{
			ImGui::ColorEdit4(___(u8"Renk##WeaponColor", u8"##WeaponColor"), g_Options.chams_weapon_color);
			ImGui::Combo("Materials##21", &g_Options.chams_weapon_mat, chamstypes, IM_ARRAYSIZE(chamstypes));
			ImGui::NewCheckbox(___(u8"Çizgili", u8"WireFrame"), &g_Options.chams_weapon_wireframe);
			ImGui::NewCheckbox(___(u8"Material", u8"Reflective"), &g_Options.chams_weapon_reflect);
		}
		NewSeparator(u8"Kıyıfet Chams");
		ImGui::NewCheckbox(___(u8"Aktif Et##341231238", u8"Enabled##341231238"), &g_Options.chams_sleeves_enabled);
		if (g_Options.chams_sleeves_enabled)
		{
			ImGui::ColorEdit4(___(u8"Renk##SleevesColor", u8"##SleevesColor"), g_Options.chams_sleeves_color);
			ImGui::Combo("Materials##441", &g_Options.chams_sleeves_mat, chamstypes, IM_ARRAYSIZE(chamstypes));
			ImGui::NewCheckbox(___(u8"Çizgili##123123", u8"WireFrame##123213"), &g_Options.chams_sleeves_wireframe);
			ImGui::NewCheckbox(___(u8"Material##123123", u8"Reflective##12312321"), &g_Options.chams_sleeves_reflect);
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(u8"##ESP3", ImVec2(250, 475), true);
	{
		NewSeparator(u8"Hasar Yardımcısı");
		ImGui::NewCheckbox(___(u8"Hasar göstergesi ", u8"Damage Indicator"), &g_Options.esp_damageindicator);
		if (g_Options.esp_damageindicator)
			ImGui::NewCheckbox(___(u8"HP'yi göster", u8"Show HP"), &g_Options.esp_damageindicator_color);
		NewSeparator(u8"Ses ESP");
		ImGui::NewCheckbox(___(u8"Aktif Et##ESPSOUND", u8"Enabled##ESPSOUND"), &g_Options.esp_sounds);
		if (g_Options.esp_sounds)
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Zamanı", u8"Duration"), &g_Options.esp_sounds_time, 0.5f, 3.0f, "%.3f ms");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Büyüklüğü", u8"Radius"), &g_Options.esp_sounds_radius, 15.0f, 150.0f, "%.1f");
			ImGui::SameLine();
			ImGui::ColorEdit4("##Sound color", g_Options.color_esp_sounds, ImGuiColorEditFlags_NoInputs);
		}
		NewSeparator("Thirdperson");
		ImGui::NewCheckbox(___(u8"Aktif Et##TP", u8"Enabled##TP"), &g_Options.misc_thirdperson);
		if (g_Options.misc_thirdperson)
		{
			pipi(10);
			ImGui::SliderFloat(___(u8"Thirdperson Uzaklığı", u8"ThirdPerson Distance"), &g_Options.misc_thirdperson_dist, 50.f, 500.f);
			pipi(10);
			KeyBind(___(u8"Tuş##TP", u8"Bind##TP"), g_Options.misc_thirdperson_bind);
		}
		NewSeparator(u8"Viewmodel");
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		ImGui::SliderInt("Viewmodel FOV", &g_Options.misc_viewmodel_fov, 68, 120, "%d");
		pipi(10);
		ImGui::SliderInt("Override FOV", &g_Options.misc_fov, 90, 150, "%d");
		NewSeparator(u8"Glow");
		ImGui::NewCheckbox(___(u8"Glow Aktif Et##33124", u8"Glow Enabled##3421321"), &g_Options.glow_enabled);
		if (g_Options.glow_enabled)
		{
			ImGui::NewCheckbox("Pulsing", &g_Options.glow_pulsing);
			ImGui::NewCheckbox(___(u8"Sadece Düşmanlar##3234", u8"Enemies Only##3324"), &g_Options.glow_enemy_only);
			ImGui::ColorEdit4(___(u8"Glow Rengi##3234", u8"Glow Color##3324"), g_Options.color_glow);

		}
	}
	ImGui::EndChild();
	ImGui::EndGroup();
}


void RenderMiscTab()
{
	static int selected_Tab = 0;
	ImGui::PushFont(TitleFont);
	if (ImGui::OwOButton(u8"Eklentiler##tab", selected_Tab == 0 ? true : false, ImVec2(400, 40)))selected_Tab = 0;
	ImGui::SameLine(350);
	if (ImGui::OwOButton(u8"Oyuncu Listesi##tab", selected_Tab == 1 ? true : false, ImVec2(400, 40)))selected_Tab = 1;
	ImGui::PopFont();
	ImGui::PushID(selected_Tab);
	switch (selected_Tab)
	{
	case 0:
		ImGui::BeginGroup();
		ImGui::BeginChild(u8"##MISC1", ImVec2(350, 430), true);
		{
			NewSeparator(u8"Hareket Ayarları");
			ImGui::NewCheckbox(___(u8"Otomatik Bunny", u8"Bunny Hop"), &g_Options.misc_bhop);
			ImGui::NewCheckbox(___(u8"Otomatik Strafe", u8"Auto Strafe"), &g_Options.misc_autostrafer);
			if (g_Options.misc_autostrafer)
				ImGui::SliderFloat(___(u8"Tepki Süresi", u8"Retrack Speed"), &g_Options.retrack, 1.f, 15.f);
			ImGui::NewCheckbox(___(u8"Edge Zıplama", u8"Edge Jump"), &g_Options.edgejump);
			if (g_Options.edgejump)
				KeyBind(___(u8"Tuş##EDGE", u8"Bind##EDGE"), g_Options.edgejump_bind);
			ImGui::NewCheckbox(___(u8"Sahte Eğilme", u8"Fake-Duck"), &g_Options.fakeduck);
			if (g_Options.fakeduck)
				KeyBind(___(u8"Sahte Eğilme Tuşu", u8"Fake Duck Bind"), g_Options.fakeduck_bind);
			ImGui::NewCheckbox(___(u8"Otomatik Bloklama", u8"Auto Block"), &g_Options.autoblock);
			if (g_Options.autoblock)
				KeyBind(___(u8"Tuş##AB", u8"Bind##AB"), g_Options.autoblock_bind);
			ImGui::NewCheckbox(___(u8"Sahte Lag", u8"Fake Lag"), &g_Options.fakelag_enabled);
			if (g_Options.fakelag_enabled)
			{
				ImGui::NewCheckbox(___(u8"Ayakta", u8"Standing"), &g_Options.fakelag_standing);
				ImGui::SameLine(150);
				ImGui::NewCheckbox(___(u8"Yürürken", u8"Moving"), &g_Options.fakelag_moving);
				ImGui::SameLine(250);
				ImGui::NewCheckbox("Unducking", &g_Options.fakelag_unducking);
				ImGui::Combo(___(u8"Mod##FK", u8"Mode##FK"), &g_Options.fakelag_mode, std::vector<std::string>{"Factor", "Switch", "Adaptive", "Random", "Legit peek"});
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::SliderInt(___(u8"Faktör##FK", u8"Factor##FK"), &g_Options.fakelag_factor, 1, 15);
			}
			NewSeparator(u8"Dünya");
			ImGui::NewCheckbox(___(u8"Gece Modu", u8"Nightmode"), &g_Options.other_nightmode);
			if (g_Options.other_nightmode)
			{
				ImGui::SliderFloat(___(u8"Büyüklüğü##NG", u8"Size##NG"), &g_Options.other_nightmode_size, 0.05f, 1.0f, "%.3f");
			}
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::NewCheckbox(___(u8"Gökkuşağı Modu", u8"Rainbow"), &g_Options.other_mat_ambient_light_rainbow);

		}
		ImGui::EndChild();
		ImGui::SameLine(375);
		ImGui::BeginChild(u8"##MISC2", ImVec2(350, 430), true);
		{
			NewSeparator(u8"Diğer Eklentiler");
			ImGui::NewCheckbox("Watermark", &g_Options.misc_watermark);
			ImGui::NewCheckbox(___(u8"Oto Maç Kabul", u8"Auto-Accept"), &g_Options.misc_autoaccept);
			ImGui::NewCheckbox("Rank reveal", &g_Options.misc_rankreveal);
			ImGui::NewCheckbox(___(u8"İzleyici Listesi", u8"Spectator List"), &g_Options.esp_speclist);
			ImGui::NewCheckbox(___(u8"Sol El Bıçak", u8"Left Hand Knife"), &g_Options.solbicak);
			ImGui::NewCheckbox(___(u8"Bıçak Bot", u8"Knife Bot"), &g_Options.kbot);
			ImGui::NewCheckbox(___(u8"Bomba Yardımcısı", u8"Grenade Prediction"), &g_Options.esp_grenade_prediction);
			ImGui::NewCheckbox(___(u8"Kalıcı Crosshair", u8"Crosshair"), &g_Options.crosshair);
			ImGui::NewCheckbox(___(u8"Vuruş Sesi", u8"Hit Sound"), &g_Options.hitsound);
			NewSeparator(u8"Siliciler");
			ImGui::NewCheckbox(___(u8"Elleri Sil", u8"No Hands"), &g_Options.other_no_hands);
			ImGui::NewCheckbox("No Smoke", &g_Options.nosmoke);
			ImGui::SliderFloat(___(u8"Flash Opaklığı", u8"Flash Ammount"), &g_Options.flash, 0.f, 255.f);
			ImGui::NewCheckbox("FPS Optimize", &g_Options.esp_optimise);
			ImGui::NewCheckbox(___(u8"Dürbünü Kaldır", u8"Remove Scope"), &g_Options.nozoom);
			/*const char* items_theme[] = { "Default", "Old" };
if (ImGui::Combo("Theme", &g_Options.misc_theme, items_theme, 2))
{
	switch (g_Options.misc_theme)
	{
	case 0:
		ImGui::StyleColorsDark();
		break;
	case 1:
		ImGui::StyleColorsClassic();
		break;
	}
}
if (ImGui::Button(___(u8"Türkçe", u8"Turkish")))
	g_Options.menu_language = false;
ImGui::SameLine();
if (ImGui::Button(___(u8"İngilizce", u8"English")))
	g_Options.menu_language = true;*/
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		break;
	case 1:
		ImGui::BeginGroupBox("##trigger");
		{

			ImGui::BeginChild("##players", ImVec2(0, 0), true);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
			static const char* teams[4] = { "None", "Spec", "T", "CT" };
			ImGui::Columns(7, NULL, false);

			ImGui::SetColumnWidth(0, 20.f); ImGui::Text((___(u8"ID", "ID"))); ImGui::NextColumn();
			ImGui::SetColumnWidth(1, 245.f); ImGui::Text((___(u8"İsim", "Name"))); ImGui::NextColumn();
			ImGui::SetColumnWidth(2, 40.f); ImGui::Text(("Live")); ImGui::NextColumn();
			ImGui::SetColumnWidth(3, 40.f); ImGui::Text((___(u8"Takım", "Team"))); ImGui::NextColumn();
			ImGui::SetColumnWidth(4, 50.f); ImGui::Text((___(u8"Para", "Money"))); ImGui::NextColumn();
			ImGui::SetColumnWidth(5, 140.f); ImGui::Text(("SteamID")); ImGui::NextColumn();
			ImGui::SetColumnWidth(6, 100.f); ImGui::Text((___(u8"Özellikler", "Features"))); ImGui::NextColumn();
			ImGui::Separator();
			if (g_EngineClient->IsInGame()) {
				for (int i = 0; i < g_EngineClient->GetMaxClients(); i++) {
					C_BasePlayer* pPlayer = C_BasePlayer::GetPlayerByIndex(i);
					player_info_t info;
					if (pPlayer && g_EngineClient->GetPlayerInfo(i, &info)) {
						int teamNum = pPlayer->m_iTeamNum();
						if (teamNum > 3) teamNum = 0;
						ImGui::Text(std::to_string(info.userId).data()); ImGui::NextColumn();
						std::string name = pPlayer->GetName(false);
						if (name.find("\n") != std::string::npos)
						{
							ImGui::TextColored(ImVec4(255, 255, 0, 255), "Bruh Moment");
						}
						else
						{
							ImGui::Text(name.c_str());
						}
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::Text(___(u8"İsim: %s", "Name: %s"), (name.c_str()));
							ImGui::Text(___(u8"Sürat: %.3f", "Velocity: %.3f"), pPlayer->m_vecVelocity().Length2D());
							if (pPlayer->m_bHasDefuser()) {
								ImGui::Text(___(u8"-=İmha Ediyor=-", "-=HasDefuser=-"));
							}
							if (pPlayer->m_bGunGameImmunity()) {
								ImGui::Text("-=GunGameImmunity=-");
							}
							if (pPlayer->m_bIsScoped()) {
								ImGui::Text(___(u8"-=Dürbün Açık=-", "-=Scoped=-"));
							}
							ImGui::EndTooltip();
						}
						ImGui::NextColumn();

						if (pPlayer->m_iHealth())
						{
							if (pPlayer->m_iHealth() > 100) {
								ImGui::TextColored(ImVec4(0, 0, 100, 100), "Over");
							}
							else if (pPlayer->m_iHealth() >= 25) {
								ImGui::TextColored(ImVec4(0, 100, 0, 100), ___(u8"Canlı", "Alive"));
							}
							else if (pPlayer->m_iHealth() >= 1) {
								ImGui::TextColored(ImVec4(100, 0, 0, 100), ___(u8"Az", "Low"));
							}
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::Text(Utils::Format("%d", pPlayer->m_iHealth()));
								ImGui::EndTooltip();
							}
							ImGui::NextColumn();
						}
						else
						{
							ImGui::TextColored(ImVec4(100, 100, 100, 100), ___(u8"Ölü", "Dead")); ImGui::NextColumn();
						}

						if (teamNum == 1) {
							ImGui::TextColored(ImVec4(0, 255, 0, 255), teams[teamNum]); ImGui::NextColumn();
						}
						else if (teamNum == 2) {
							ImGui::TextColored(ImVec4(255, 0, 0, 255), teams[teamNum]); ImGui::NextColumn();
						}
						else if (teamNum == 3) {
							ImGui::TextColored(ImVec4(0, 128, 255, 255), teams[teamNum]); ImGui::NextColumn();
						}
						else {
							ImGui::Text(teams[teamNum]); ImGui::NextColumn();
						}
						ImGui::Text(Utils::Format("$%d", static_cast<int>(pPlayer->m_iAccount()))); ImGui::NextColumn();
						ImGui::Text(info.szSteamID); ImGui::NextColumn();
						if (pPlayer != g_LocalPlayer)
						{
							if (ImGui::ButtonEx((___(u8"İsmi Çal ", "Steal") + std::to_string(i)).data())) {
								char name[1024]; sprintf(name, "%s ", info.szName);
								Utils::SetName(name);
							}
							ImGui::SameLine();
						}

						if (ImGui::ButtonEx((___(u8"At ", "Kick##") + std::to_string(i)).data())) {
							Misc::Get().Kick(info.userId);
						}
						ImGui::NextColumn();

					}

				}
				ImGui::Separator();
			}
			ImGui::Columns(1, NULL, false);

			ImGui::EndChild();
		}
		ImGui::EndGroupBox();
		break;
	}
}




Vector RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck, bool angleInRadians)
{
	float r_1, r_2;
	float x_1, y_1;

	r_1 = -(EntityPos.y - LocalPlayerPos.y);
	r_2 = EntityPos.x - LocalPlayerPos.x;
	float Yaw = angle - 90.0f;

	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

	*viewCheck = y_1 < 0;

	x_1 *= zoom;
	y_1 *= zoom;

	int sizX = sizeX / 2;
	int sizY = sizeY / 2;

	x_1 += sizX;
	y_1 += sizY;

	if (x_1 < 5)
		x_1 = 5;

	if (x_1 > sizeX - 5)
		x_1 = sizeX - 5;

	if (y_1 < 5)
		y_1 = 5;

	if (y_1 > sizeY - 5)
		y_1 = sizeY - 5;


	x_1 += posX;
	y_1 += posY;


	return Vector(x_1, y_1, 0);
}


void RadarRender()
{
	if (ImGui::Begin("Radar", &g_Options.Act, ImVec2(200, 200), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
	{
		ImVec2 siz = ImGui::GetWindowSize();
		ImVec2 pos = ImGui::GetWindowPos();

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddLine(ImVec2(pos.x + siz.x * 0.5f, pos.y), ImVec2(pos.x + siz.x * 0.5f, pos.y + siz.y), ImColor(70, 70, 70, 255), 1.f);
		draw_list->AddLine(ImVec2(pos.x, pos.y + siz.y * 0.5f), ImVec2(pos.x + siz.x, pos.y + siz.y * 0.5f), ImColor(70, 70, 70, 255), 1.f);

		if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected())
		{
			auto pLocalEntity = g_LocalPlayer;
			if (pLocalEntity)
			{
				Vector LocalPos = pLocalEntity->GetEyePos();
				QAngle ang;
				g_EngineClient->GetViewAngles(&ang);
				for (int i = 1; i < g_EngineClient->GetMaxClients(); i++)
				{
					C_BasePlayer* pBaseEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

					if (!pBaseEntity)
						continue;
					if (pBaseEntity->IsDormant() || !pBaseEntity->IsAlive())
						continue;
					if (pBaseEntity == pLocalEntity)
						continue;

					bool bIsEnemy = pLocalEntity->m_iTeamNum() != pBaseEntity->m_iTeamNum();
					if (!bIsEnemy)
						continue;

					bool viewCheck = false;
					Vector EntityPos = RotatePoint(pBaseEntity->m_vecOrigin(), LocalPos, pos.x, pos.y, siz.x, siz.y, ang.yaw, g_Options.Zoom, &viewCheck, 0.F);

					int s = g_Options.Size;
					switch (g_Options.RadarStyle)
					{
					case 0:
					{
						draw_list->AddRect(ImVec2(EntityPos.x - s, EntityPos.y - s), ImVec2(EntityPos.x + s, EntityPos.y + s), ImColor(254, 24, 110, 255));
						break;
					}
					case 1:
					{
						draw_list->AddRectFilled(ImVec2(EntityPos.x - s, EntityPos.y - s), ImVec2(EntityPos.x + s, EntityPos.y + s), ImColor(254, 24, 110, 255));
						break;
					}
					case 2:
					{
						draw_list->AddCircle(ImVec2(EntityPos.x, EntityPos.y), s, ImColor(254, 24, 110, 255));
						break;
					}
					case 3:
					{
						draw_list->AddCircleFilled(ImVec2(EntityPos.x, EntityPos.y), s, ImColor(254, 24, 110, 255));
						break;
					}
					default:
						break;
					}
				}
			}
		}
	}
	ImGui::End();
}



void RenderAimbotTab()
{
	static int ayimbottab = 0;
	const char* Hitboxx[] =
	{
		___(u8"Lütfen Hitbox Seçiniz", "Please Select Hitbox"),
		___(u8"Kafa", "Head"),
		___(u8"Boyun", "Neck"),
		___(u8"Göğüs", "Chest"),
		___(u8"Mide", "Stomach"),
		___(u8"Çoklu Nokta", "Multipoint")
	};
	const char* items[] = { ___(u8"Kapalı", "Off"), ___(u8"Statik", "Static"), ___(u8"Dengeli", "Balance") };
	ImGui::BeginGroup();
	ImGui::BeginChild(u8"##LEGIT1", ImVec2(350, 225), true);
	{
		NewSeparator(u8"Aimbot Ayarları");
		ImGui::NewCheckbox(___(u8"Aktif Et##Aimbot", "Enabled##Aimbot"), &g_Options.enable_legitbot);
		ImGui::NewCheckbox(___(u8"Dangerzone Modu", "Dangerzone Mode"), &g_Options.dangerzone);
		NewSeparator("Backtrack");
		ImGui::NewCheckbox("Aktif Et##Backtrack", &g_Options.misc_backtrack);
		if (g_Options.misc_backtrack)
		{
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
				ImGui::SliderInt("Backtrack Tik", &g_Options.legit_backtrack, 0, 18);
		}
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		NewSeparator(u8"Aimbot Tuşu");
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
		KeyBind(u8"##AimbotKey", g_Options.aimkey);
	}
	ImGui::EndChild();
	ImGui::SameLine(375);
	ImGui::BeginChild(u8"##LEGIT2", ImVec2(350, 225), true);
	{
		ImGui::PushFont(ExAimbot);
		if (ImGui::OwOButton(u8"T##tab", ayimbottab == 0 ? true : false, ImVec2(55, 50)))ayimbottab = 0;
		ImGui::SameLine(55);
		if (ImGui::OwOButton(u8"A##tab", ayimbottab == 1 ? true : false, ImVec2(55, 50)))ayimbottab = 1;
		ImGui::SameLine(110);
		if (ImGui::OwOButton(u8"a##tab", ayimbottab == 2 ? true : false, ImVec2(55, 50)))ayimbottab = 2;
		ImGui::SameLine(165);
		if (ImGui::OwOButton(u8"P##tab", ayimbottab == 3 ? true : false, ImVec2(55, 50)))ayimbottab = 3;
		ImGui::SameLine(220);
		if (ImGui::OwOButton(u8"c##tab", ayimbottab == 4 ? true : false, ImVec2(55, 50)))ayimbottab = 4;
		ImGui::SameLine(275);
		if (ImGui::OwOButton(u8"f##tab", ayimbottab == 5 ? true : false, ImVec2(55, 50)))ayimbottab = 5;
		ImGui::PopFont();
		ImGui::PushID(ayimbottab);
		switch (ayimbottab)
		{
		case 0:
			ImGui::NewCheckbox(___(u8"Tüfekler İçin Aimbot Aktif##31", "Aimbot Enable For Rifles##31"), &g_Options.aim_LegitBotRifles);
			ImGui::Combo(___(u8"Vuruş Bölgesi", "Hitbox"), &g_Options.hitbox_rifles, Hitboxx, ARRAYSIZE(Hitboxx));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Görüş Açısı##31", "FOV##31"), &g_Options.legit_fov_rifles, 1.00f, 30.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Yumuşaklık##31", "Smooth##31"), &g_Options.legit_smooth_rifles, 1.00f, 100.00f, "%.2f");
			/*ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Min RCS##31", &g_Options.legit_rcsmin_rifles, 1.00f, 100.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Max RCS##31", &g_Options.legit_rcsmax_rifles, 1.00f, 100.00f, "%.2f");*/
			break;
		case 1:
			ImGui::NewCheckbox(___(u8"Tabancalar İçin Aimbot Aktif##32", "Aimbot Enable For Pistol##32"), &g_Options.aim_LegitBotPistols);
			ImGui::Combo(___(u8"Vuruş Bölgesi##32", "Hitbox##32"), &g_Options.hitbox_pistols, Hitboxx, ARRAYSIZE(Hitboxx));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Görüş Açısı##32", "FOV##32"), &g_Options.legit_fov_pistols, 1.00f, 30.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Yumuşaklık##32", "Smooth##32"), &g_Options.legit_smooth_pistols, 1.00f, 100.00f, "%.2f");
			/*ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Min RCS##32", &g_Options.legit_rcsmin_pistols, 1.00f, 100.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Max RCS##32", &g_Options.legit_rcsmax_pistols, 1.00f, 100.00f, "%.2f");*/
			break;
		case 2:
			ImGui::NewCheckbox(___(u8"Dürbünlü İçin Aimbot Aktif##33", "Aimbot Enable For Snipers##33"), &g_Options.aim_LegitBotSnipers);
			ImGui::Combo(___(u8"Vuruş Bölgesi##33", "Hitbox##33"), &g_Options.hitbox_snipers, Hitboxx, ARRAYSIZE(Hitboxx));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Görüş Açısı##33", "FOV##33"), &g_Options.legit_fov_Snipers, 1.00f, 30.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Yumuşaklık##33", "Smooth##33"), &g_Options.legit_smooth_Snipers, 1.00f, 100.00f, "%.2f");
			/*ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Min RCS##33", &g_Options.legit_rcsmin_Snipers, 1.00f, 100.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Max RCS##33", &g_Options.legit_rcsmax_Snipers, 1.00f, 100.00f, "%.2f");*/
			break;
		case 3:
			ImGui::NewCheckbox(___(u8"Hafif Makinalı İçin Aimbot Aktif##34", "Aimbot Enable For SMGs##34"), &g_Options.aim_LegitBotsmg);
			ImGui::Combo(___(u8"Vuruş Bölgesi##34", "Hitbox##34"), &g_Options.hitbox_smg, Hitboxx, ARRAYSIZE(Hitboxx));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Görüş Açısı##34", "FOV##34"), &g_Options.legit_fov_smg, 1.00f, 30.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Yumuşaklık##34", "Smooth##34"), &g_Options.legit_smooth_smg, 1.00f, 100.00f, "%.2f");
			/*ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Min RCS##34", &g_Options.legit_rcsmin_smg, 1.00f, 100.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Max RCS##34", &g_Options.legit_rcsmax_smg, 1.00f, 100.00f, "%.2f");*/
			break;
		case 4:
			ImGui::NewCheckbox(___(u8"Pompalı İçin Aimbot Aktif##35", "Aimbot Enable For Shotgun##35"), &g_Options.aim_LegitBotShotgun);
			ImGui::Combo(___(u8"Vuruş Bölgesi##35", "Hitbox##35"), &g_Options.hitbox_Shotgun, Hitboxx, ARRAYSIZE(Hitboxx));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Görüş Açısı##35", "FOV##35"), &g_Options.legit_fov_Shotgun, 1.00f, 30.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Yumuşaklık##35", "Smooth##35"), &g_Options.legit_smooth_Shotgun, 1.00f, 100.00f, "%.2f");
			/*ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Min RCS##35", &g_Options.legit_rcsmin_Shotgun, 1.00f, 100.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Max RCS##35", &g_Options.legit_rcsmax_Shotgun, 1.00f, 100.00f, "%.2f");*/
			break;
		case 5:
			ImGui::NewCheckbox(___(u8"Ağır Makineli İçin Aimbot Aktif##36", "Aimbot Enable For Machinegun##36"), &g_Options.aim_LegitBotMachinegun);
			ImGui::Combo(___(u8"Vuruş Bölgesi##36", "Hitbox##36"), &g_Options.hitbox_Machinegun, Hitboxx, ARRAYSIZE(Hitboxx));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Görüş Açısı##36", "FOV##36"), &g_Options.legit_fov_Machinegun, 1.00f, 30.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat(___(u8"Yumuşaklık##36", "Smooth##36"), &g_Options.legit_smooth_Machinegun, 1.00f, 100.00f, "%.2f");
			/*ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Min RCS##36", &g_Options.legit_rcsmin_Machinegun, 1.00f, 100.00f, "%.2f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SliderFloat("Max RCS##36", &g_Options.legit_rcsmax_Machinegun, 1.00f, 100.00f, "%.2f");*/
			break;
		}
		ImGui::PopID();
	}
	ImGui::EndChild();
	ImGui::EndGroup();
	ImGui::NewLine();
	ImGui::BeginGroup();
	ImGui::BeginChild(u8"##LEGIT3", ImVec2(350, 225), true);
	{
		NewSeparator(u8"TriggerBot Ayarları");
		ImGui::NewCheckbox(___(u8"Aktif Et##Trigger", "Enabled##Trigger"), &g_Options.trigenable);
		ImGui::SameLine();
		if (g_Options.trigenable)
		{
			ImGui::SameLine(150);
			ImGui::NewCheckbox(___(u8"Tuşlu Trigger##444", "Trigger On Key##444"), &g_Options.onkey);
			if (g_Options.onkey)
			{
				KeyBind(___(u8"Tuş##123", "Key##123"), g_Options.trigbind);
			}
			NewSeparator(u8"Trigger Eklentileri");
			ImGui::NewCheckbox(___(u8"Sis Kontrol##123", "Check Smoke##123"), &g_Options.trigchecksmoke);
			ImGui::SameLine(150);
			ImGui::NewCheckbox(___(u8"Flash Kontrol##123", "Check Flash##123"), &g_Options.trigcheckflash);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SliderFloat(___(u8"Vuruş Şansı##123", "Hitchance##123"), &g_Options.trighitchance, 0.f, 100.f, "%.1f");
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SliderFloat(___(u8"Gecikme##123", "Delay##123"), &g_Options.trigdelay, 0.f, 150.f, "%.1f");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			if (ImGui::BeginCombo(___(u8"Bölgeler##123", "Hitgroups##123"), "..."))
			{
				ImGui::Selectable(___(u8"Kafa", "Head"), &g_Options.trighead, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
				ImGui::Selectable(___(u8"Göğüs", "Chest"), &g_Options.trigchest, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
				ImGui::Selectable(___(u8"Mide", "Stomach"), &g_Options.trigstomach, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
				ImGui::Selectable(___(u8"Sol Kol", "Left Arm"), &g_Options.trigleftarm, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
				ImGui::Selectable(___(u8"Sağ Kol", "Right Arm"), &g_Options.trigrightarm, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
				ImGui::Selectable(___(u8"Sol Bacak", "Left Leg"), &g_Options.trigleftleg, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
				ImGui::Selectable(___(u8"Sağ Bacak", "Right Leg"), &g_Options.trigrightleg, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
				ImGui::EndCombo();
			}
		}
	}
	ImGui::EndChild();
	ImGui::EndGroup();
	ImGui::SameLine(375);
	ImGui::BeginGroup();
	ImGui::BeginChild(u8"##LEGIT4", ImVec2(350, 225), true);
	{
		NewSeparator("Legit Anti-Aim");
		ImGui::Combo("Aktif Et##LegitAA", &g_Options.misc_desync, items, 3);
		if (g_Options.misc_desync)
		{
			ImGui::NewCheckbox(___(u8"Otomatik Yönlendirme", "Auto Direction"), &g_Options.misc_desync_ad);
			KeyBind(___(u8"Legit AA Tuşu", "Legit AA Bind"), g_Options.misc_desync_bind);
			ImGui::NewCheckbox(___(u8"Sahte Açı Çizgileri", u8"Angle Lines"), &g_Options.esp_angle_lines);
		}
	}
	ImGui::EndChild();
	ImGui::EndGroup();
}


struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}

const char* GetWeaponNameById(int id)
{
	switch (id)
	{
	case 1:
		return "deagle";
	case 2:
		return "elite";
	case 3:
		return "fiveseven";
	case 4:
		return "glock";
	case 7:
		return "ak47";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 10:
		return "famas";
	case 11:
		return "g3sg1";
	case 13:
		return "galilar";
	case 14:
		return "m249";
	case 60:
		return "m4a1_silencer";
	case 16:
		return "m4a1";
	case 17:
		return "mac10";
	case 19:
		return "p90";
	case 23:
		return "mp5sd";
	case 24:
		return "ump45";
	case 25:
		return "xm1014";
	case 26:
		return "bizon";
	case 27:
		return "mag7";
	case 28:
		return "negev";
	case 29:
		return "sawedoff";
	case 30:
		return "tec9";
	case 32:
		return "hkp2000";
	case 33:
		return "mp7";
	case 34:
		return "mp9";
	case 35:
		return "nova";
	case 36:
		return "p250";
	case 38:
		return "scar20";
	case 39:
		return "sg556";
	case 40:
		return "ssg08";
	case 61:
		return "usp_silencer";
	case 63:
		return "cz75a";
	case 64:
		return "revolver";
	case 508:
		return "knife_m9_bayonet";
	case 500:
		return "bayonet";
	case 505:
		return "knife_flip";
	case 506:
		return "knife_gut";
	case 507:
		return "knife_karambit";
	case 509:
		return "knife_tactical";
	case 512:
		return "knife_falchion";
	case 514:
		return "knife_survival_bowie";
	case 515:
		return "knife_butterfly";
	case 516:
		return "knife_push";

	case 519:
		return "knife_ursus";
	case 520:
		return "knife_gypsy_jackknife";
	case 522:
		return "knife_stiletto";
	case 523:
		return "knife_widowmaker";

	case 5027:
		return "studded_bloodhound_gloves";
	case 5028:
		return "t_gloves";
	case 5029:
		return "ct_gloves";
	case 5030:
		return "sporty_gloves";
	case 5031:
		return "slick_gloves";
	case 5032:
		return "leather_handwraps";
	case 5033:
		return "motorcycle_gloves";
	case 5034:
		return "specialist_gloves";
	case 5035:
		return "studded_hydra_gloves";

	default:
		return "";
	}
}

std::vector<wskin> g_skins;





void safesave()
{
	///p fix
	CreateDirectory("C:\\AxePrimePremium", NULL);
	unsigned size = g_skins.size();
	std::ofstream output("C:\\AxePrimePremium\\inventory.data", std::ios::binary);
	output.write(reinterpret_cast<char*>(&size), sizeof(size));
	output.write(reinterpret_cast<char*>(g_skins.data()), sizeof(wskin)* size);
	/*size = music_id.size();
	std::ofstream _output("C:\\Shonax\\music.data", std::ios::binary);
	_output.write(reinterpret_cast<char*>(&size), sizeof(size));
	_output.write(reinterpret_cast<char*>(music_id.data()), sizeof(sdhsdhsdffh)* size);
	size = med_id.size();
	std::ofstream __output("C:\\Shonax\\medal.data", std::ios::binary);
	__output.write(reinterpret_cast<char*>(&size), sizeof(size));
	__output.write(reinterpret_cast<char*>(med_id.data()), sizeof(sdhsdhsdffh)* size);*/
	///p fix
}
void safeload()
{
	///p fix
	std::ifstream readme("C:\\AxePrimePremium\\inventory.data", std::ios::binary);
	unsigned size;
	readme.read(reinterpret_cast<char*>(&size), sizeof(size));
	std::vector<wskin> one(size);
	readme.read(reinterpret_cast<char*>(one.data()), sizeof(wskin)* size);
	g_skins = one;
	/*std::ifstream _readme("C:\\Shonax\\music.data", std::ios::binary);
	_readme.read(reinterpret_cast<char*>(&size), sizeof(size));
	std::vector<sdhsdhsdffh> two(size);
	_readme.read(reinterpret_cast<char*>(two.data()), sizeof(sdhsdhsdffh)* size);
	music_id = two;
	std::ifstream __readme("C:\\Shonax\\medal.data", std::ios::binary);
	__readme.read(reinterpret_cast<char*>(&size), sizeof(size));
	std::vector<sdhsdhsdffh> three(size);
	__readme.read(reinterpret_cast<char*>(three.data()), sizeof(sdhsdhsdffh)* size);
	med_id = three;*/
	///p fix
}



void RenderInventoryTab() {

	static int   tt = 0;
	static char* tts[] = { "INVENTORY" };
	static auto definition_vector_index = 0;
	static int kmodel;
	static int paintkit;
	static int kglove;
	static int rarity;
	static int stickers[4];
	static int switchf[4];
	ImGui::BeginGroupBox("##xdez");
	{
		ImGui::Columns(2);

		ImGui::BeginChild("##11115434");
		{
			ImGui::PushItemWidth(-1);
			const auto wnd = ImGui::GetCurrentWindowRead();
			float height = (wnd->Pos.y + wnd->Size.y) - wnd->DC.CursorPos.y - 18.0f - ImGui::GetStyle().WindowPadding.y - ImGui::GetStyle().FramePadding.y * 2.0f;

			ImGui::ListBoxHeader("##wtf", ImVec2(0.f, height));
			{
				for (size_t w = 0; w < weapon_names.size(); w++) {
					if (ImGui::Selectable(weapon_names[w].name, definition_vector_index == w)) {
						definition_vector_index = w;
					}
				}

			}
			ImGui::ListBoxFooter();
			ImGui::PopItemWidth();
		}
		ImGui::EndChild();
		ImGui::NextColumn();

		ImGui::BeginChild("##5423142");
		{
			auto& selected_entry = g_Options.skins.m_items[weapon_names[definition_vector_index].definition_index];
			if (weapon_names[definition_vector_index].definition_index == WEAPON_KNIFE)
			{
				ImGui::Combo(___(u8"Bıçak##ENV", "Knife##ENV"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
					*out_text = k_knife_names.at(idx).name;
					return true;
				}, nullptr, k_knife_names.size(), 10);
				kmodel = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
			}
			if (weapon_names[definition_vector_index].definition_index == GLOVE_T_SIDE)
			{
				ImGui::Combo(___(u8"Skinler##ENV", "Paint Kit##ENV"), &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
					*out_text = k_gloves[idx].name.c_str();
					return true;
				}, nullptr, k_gloves.size(), 20);
				paintkit = k_gloves[selected_entry.paint_kit_vector_index].id;

				ImGui::Combo(___(u8"Eldiven##ENV", "Gloves##ENV"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
					*out_text = k_glove_names.at(idx).name;
					return true;
				}, nullptr, k_glove_names.size(), 10);
				kglove = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
			}
			if (weapon_names[definition_vector_index].definition_index != GLOVE_T_SIDE)
			{
				ImGui::Combo(___(u8"Skinler##ENV", "Paint Kit##ENV"), &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
					*out_text = k_skins[idx].name.c_str();
					return true;
				}, nullptr, k_skins.size(), 20);
				paintkit = k_skins[selected_entry.paint_kit_vector_index].id;
			}


			static float seed, wear;
			//ImGui::SliderFloat("Seed", &seed, 0, 1);
			pipi(10);
			ImGui::SliderFloat(___(u8"Eskimişlik##ENV", "Wear##ENV"), &wear, 0, 1);


			if (weapon_names[definition_vector_index].definition_index != GLOVE_T_SIDE && weapon_names[definition_vector_index].definition_index != WEAPON_KNIFE)
			{

				ImGui::Combo("Sticker Kit 1", &switchf[0], [](void* data, int idx, const char** out_text) {
					*out_text = k_stickers[idx].name.c_str();
					return true;
				}, nullptr, k_stickers.size(), 20);

				ImGui::Combo("Sticker Kit 2", &switchf[1], [](void* data, int idx, const char** out_text) {
					*out_text = k_stickers[idx].name.c_str();
					return true;
				}, nullptr, k_stickers.size(), 20);

				ImGui::Combo("Sticker Kit 3", &switchf[2], [](void* data, int idx, const char** out_text) {
					*out_text = k_stickers[idx].name.c_str();
					return true;
				}, nullptr, k_stickers.size(), 20);

				ImGui::Combo("Sticker Kit 4", &switchf[3], [](void* data, int idx, const char** out_text) {
					*out_text = k_stickers[idx].name.c_str();
					return true;
				}, nullptr, k_stickers.size(), 20);

				stickers[0] = k_stickers[switchf[0]].id;
				stickers[1] = k_stickers[switchf[1]].id;
				stickers[2] = k_stickers[switchf[2]].id;
				stickers[3] = k_stickers[switchf[3]].id;
			}

			static float sticker_wear,
				sticker_scale = 1,
				sticker_rotaion;
			pipi(10);
			ImGui::SliderFloat(___(u8"Stiker Eskiliği##ENV", "Sticker Wear##ENV"), &sticker_wear, 0, 1);
			pipi(10);
			ImGui::SliderFloat(___(u8"Stiker Ölçeği##ENV", "Sticker Scale##ENV"), &sticker_scale, 0, 1);
			pipi(10);
			ImGui::SliderFloat(___(u8"Stiker Rotasyonu##ENV", "Sticker Rotaion##ENV"), &sticker_rotaion, 0, 1);


			const char* raritynames[] = { "Default (Gray)", "Consumer Grade (White)", "Industrial Grade (Light Blue)", "Mil-Spec (Darker Blue)", "Restricted (Purple)", "Classified (Pinkish Purple)", "Covert (Red)", "Exceedingly Rare (Gold)" };

			ImGui::Combo(___(u8"Enderlik##ENV", "Rarity##ENV"), &rarity, raritynames, IM_ARRAYSIZE(raritynames));
			static char shon_nam[32];
			ImGui::InputText("Inventory name", shon_nam, sizeof(shon_nam));

			if (ImGui::Button(___(u8"Ekle##ENV", "Add##ENV")))
			{
				if (weapon_names[definition_vector_index].definition_index != WEAPON_KNIFE && weapon_names[definition_vector_index].definition_index != GLOVE_T_SIDE)
					g_skins.insert(g_skins.end(), { weapon_names[definition_vector_index].definition_index , paintkit, rarity, {stickers[0],stickers[1],stickers[2],stickers[3]},shon_nam,seed,wear,sticker_wear,sticker_scale,sticker_rotaion });
				else if (weapon_names[definition_vector_index].definition_index == WEAPON_KNIFE)
					g_skins.insert(g_skins.end(), { kmodel , paintkit, rarity, {0,0,0,0},shon_nam,seed,wear,0,0,0 });
				else if (weapon_names[definition_vector_index].definition_index == GLOVE_T_SIDE)
					g_skins.insert(g_skins.end(), { kglove , paintkit, rarity, {0,0,0,0},shon_nam,seed,wear,0,0,0 });

			}


			ImGui::SameLine();
			if (ImGui::Button(___(u8"Güncelle##ENV", "Update##ENV")))
			{
				ProtoFeatures.SendClientHello();
				ProtoFeatures.SendMatchmakingClient2GCHello();
			}



			if (ImGui::Button(___(u8"Kaydet##ENV", "Save##ENV")))
				safesave();
			ImGui::SameLine();
			if (ImGui::Button(___(u8"Yükle##ENV", "Load##ENV")))
				safeload();


			int wpind = 0;
			//static int pk_fixed = paintkit;


			std::string pk_name;
			std::string stick_name[4];

			for (auto weapon : g_skins)
			{


				if (weapon_names[definition_vector_index].definition_index == GLOVE_T_SIDE)
				{
					for (auto i = 0; i < k_gloves.size(); i++)
					{
						if (k_gloves.at(i).id == weapon.paintkit)
							pk_name = k_gloves.at(i).name;

					}
				}

				else
				{
					for (auto i = 0; i < k_gloves.size(); i++)
					{
						if (k_skins.at(i).id == weapon.paintkit)
							pk_name = k_skins.at(i).name;

					}
				}

				/* if (weapon_names[definition_vector_index].definition_index != GLOVE_T_SIDE && weapon_names[definition_vector_index].definition_index != WEAPON_KNIFE)
				 {
					 for (auto i = 0; i < k_stickers.size(); i++)
					 {
						 for (auto j = 0; j < 4; j++)
						 {
							 if (k_stickers.at(i).id == weapon.stickers[j])
								 stick_name[j] = k_gloves.at(i).name;
						 }
					 }
				 }
				 */
				std::string fix = GetWeaponNameById(weapon.wId);

				if (ImGui::Selectable(std::string("Weapon: " + fix + " | Skin: " + pk_name /*+ " rarity: " + raritynames[weapon.rarity] + "stickers: " + stick_name[0] + "|" + stick_name[1] + "|" + stick_name[2] + "|" + stick_name[3]*/).c_str())) {
					g_skins.erase(g_skins.begin() + wpind);
				}
				wpind++;
			}



		}
		ImGui::EndChild();
	}
	ImGui::EndGroupBox();
}


const char* DZRank[] = {
	"Ignored",
	"Lab Rat I",
	"Lab Rat II",
	"Sprinting Hare I",
	"Sprinting Hare II",
	"Wild Scout I",
	"Wild Scout II",
	"Wild Scout Elite",
	"Hunter Fox I",
	"Hunter Fox II",
	"Hunter Fox III",
	"Hunter Fox Elite",
	"Timber Wolf",
	"Ember Wolf",
	"Wildfire Wolf",
	"The Howling Alpha"
};


const char* MMRank[] = {
						"Ignored",
						"Silver I",
						"Silver II",
						"Silver III",
						"Silver IV",
						"Silver Elite",
						"Silver Elite Master",

						"Gold Nova I",
						"Gold Nova II",
						"Gold Nova III",
						"Gold Nova Master",

						"Master Guardian I",
						"Master Guardian II",
						"Master Guardian Elite",
						"Distinguished Master Guardian",

						"Legendary Eagle",
						"Legendary Eagle Master",
						"Supreme Master First Class",
						"The Global Elite" };

static const char* bans[] =
{
	"none",
	"you were kicked from the last match (competitive cooldown)",
	"you killed too many teammates (competitive cooldown)",
	"you killed a teammate at round start (competitive cooldown)",
	"you failed to reconnect to the last match (competitive cooldown)",
	"you abandoned the last match (competitive cooldown)",
	"you did too much damage to your teammates (competitive cooldown)",
	"you did too much damage to your teammates at round start (competitive cooldown)",
	"this account is permanently untrusted (global cooldown)",
	"you were kicked from too many recent matches (competitive cooldown)",
	"convicted by overwatch - majorly disruptive (global cooldown)",
	"convicted by overwatch - minorly disruptive (global cooldown)",
	"resolving matchmaking state for your account (temporary cooldown)",
	"resolving matchmaking state after the last match (temporary cooldown)",
	"this account is permanently untrusted (global cooldown)",
	"(global cooldown)",
	"you failed to connect by match start. (competitive cooldown)",
	"you have kicked too many teammates in recent matches (competitive cooldown)",
	"congratulations on your recent competitive wins! before you play competitive matches further please wait for matchmaking servers to calibrate your skill group placement based on your lastest performance. (temporary cooldown)",
	"a server using your game server login token has been banned. your account is now permanently banned from operating game servers, and you have a cooldown from connecting to game servers. (global cooldown)"
};


void RenderSkinsTab() {
	static int selected_Tab = 0;
	auto& entries = g_Options.skins.m_items;
	static auto definition_vector_index = 0;
	auto& selected_entry = entries[weapon_names[definition_vector_index].definition_index];
	auto& satatt = g_Options.statrack_items[weapon_names[definition_vector_index].definition_index];
	selected_entry.definition_index = weapon_names[definition_vector_index].definition_index;
	selected_entry.definition_vector_index = definition_vector_index;
	const char* Slot[] =
	{
		"1",
		"2",
		"3",
		"4"
	};
	ImGui::PushFont(TitleFont);
	if (ImGui::OwOButton(u8"Skin Değiştirici##tab", selected_Tab == 0 ? true : false, ImVec2(250, 40)))selected_Tab = 0;
	ImGui::SameLine(250);
	if (ImGui::OwOButton(u8"Rank Değiştirici##tab", selected_Tab == 1 ? true : false, ImVec2(250, 40)))selected_Tab = 1;
	ImGui::SameLine(500);
	if (ImGui::OwOButton(u8"Envanter Değiştirici##tab", selected_Tab == 2 ? true : false, ImVec2(250, 40)))selected_Tab = 2;
	ImGui::PopFont();
	ImGui::PushID(selected_Tab);
	switch (selected_Tab)
	{
	case 0:
		ImGui::BeginGroup();
		ImGui::BeginChild(u8"##CHANGER1", ImVec2(350, 430), true);
		{
			const auto wnd = ImGui::GetCurrentWindowRead();
			float height = (wnd->Pos.y + wnd->Size.y) - wnd->DC.CursorPos.y - 18.0f - ImGui::GetStyle().WindowPadding.y - ImGui::GetStyle().FramePadding.y * 2.0f;
			ImGui::ListBoxHeader("##config", ImVec2(0, height));
			{
					for (size_t w = 0; w < weapon_names.size(); w++) {
						if (ImGui::Selectable(weapon_names[w].name, definition_vector_index == w)) {
							definition_vector_index = w;
						}
					}
			}
			ImGui::ListBoxFooter();
		}
		ImGui::EndChild();
		ImGui::SameLine(375);
		ImGui::BeginChild(u8"##CHANGER2", ImVec2(350, 430), true);
		{

			ImGui::NewCheckbox(___(u8"Aktif##Skin", "Enabled##Skin"), &selected_entry.enabled);
			if (selected_entry.enabled)
			{
				ImGui::InputText("Name Tag", selected_entry.custom_name, 32);
				if (selected_entry.definition_index != GLOVE_T_SIDE)
				{
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
					ImGui::Combo(___(u8"Skinler##Skin", "Paintkits##Skin"), &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
						*out_text = k_skins[idx].name.c_str();
						return true;
					}, nullptr, k_skins.size(), 20);
					selected_entry.paint_kit_index = k_skins[selected_entry.paint_kit_vector_index].id;
				}
				else {
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
					ImGui::Combo(___(u8"Skinler##Skin2", "Paintkits##Skin2"), &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
						*out_text = k_gloves[idx].name.c_str();
						return true;
					}, nullptr, k_gloves.size(), 20);
					selected_entry.paint_kit_index = k_gloves[selected_entry.paint_kit_vector_index].id;
				}
				if (selected_entry.definition_index == WEAPON_KNIFE) {
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
					ImGui::Combo(___(u8"Bıçak##Skin", "Knife##Skin"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
						*out_text = k_knife_names.at(idx).name;
						return true;
					}, nullptr, k_knife_names.size(), 10);
					selected_entry.definition_override_index = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;
				}
				else if (selected_entry.definition_index == GLOVE_T_SIDE) {
					ImGui::Combo(___(u8"Eldiven##Skin", "Gloves##Skin"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
						*out_text = k_glove_names.at(idx).name;
						return true;
					}, nullptr, k_glove_names.size(), 10);
					selected_entry.definition_override_index = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;
				}
				else {
					static auto unused_value = 0;
					selected_entry.definition_override_vector_index = 0;
				}
				ImGui::NewCheckbox("StatTrak##2", &selected_entry.stat_trak);
				if (selected_entry.stat_trak)
				{
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
					ImGui::InputInt("StatTrak", &satatt.statrack_new.counter);
				}
				pipi(10);
				ImGui::SliderFloat(u8"Eskimişlik", &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5);
				/*ImGui::InputInt("Seed", &selected_entry.seed);
					ImGui::NewCheckbox("Stickers", &selected_entry.enabled_stickers);
		ImGui::SliderInt("Slot", &selected_entry.stickers_place, 0, 4);
		ImGui::InputInt("Index", &selected_entry.stickers[selected_entry.stickers_place].stickers_id);*/
			}
			pipi(10);
			if (ImGui::Button(___(u8"Güncelle##Skin", "Update##Skin"))) {
				static auto clear_hud_weapon_icon_fn =
					reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
						Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

				auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

				if (!element)
					return;

				auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xa0);
				if (hud_weapons == nullptr)
					return;

				if (!*hud_weapons->get_weapon_count())
					return;

				for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
					i = clear_hud_weapon_icon_fn(hud_weapons, i);

				typedef void(*ForceUpdate) (void);
				static ForceUpdate FullUpdate = (ForceUpdate)Utils::PatternScan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
				FullUpdate();

				//g_ClientState->ForceFullUpdate();
			}
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		break;
	case 1:
		ImGui::BeginGroup();
		ImGui::BeginChild(u8"##CHANGER1", ImVec2(350, 430), true);
		{
			NewSeparator("MM Rank");
			ImGui::Combo("MM Rank", &g_Options.Profile_Info_Rank_Combo, MMRank, IM_ARRAYSIZE(MMRank));
			ImGui::InputInt(___(u8"Kazanma##MM", "Win##MM"), &g_Options.Profile_Info_Win);
			if (ImGui::Button("Update"))
			{
				ProtoFeatures.SendClientHello();
				ProtoFeatures.SendMatchmakingClient2GCHello();
			}
			NewSeparator("Yoldaş Rank");
			ImGui::Combo(___(u8"Yoldaş Rank", "Wingman Rank"), &g_Options.wingman_rank, MMRank, IM_ARRAYSIZE(MMRank));
			ImGui::InputInt(___(u8"Kazanma##MM2", "Wins##MM2"), &g_Options.wingman_wins);
			if (ImGui::Button("Update windman")) ProtoFeatures.SendClientGcRankUpdate();
			NewSeparator("Dangerzone Rank");
			ImGui::Combo(___(u8"Tehlikeli Bölge Rankı", "Danger Zone Rank"), &g_Options.danger_zone_rank, DZRank, IM_ARRAYSIZE(DZRank));
			ImGui::InputInt(___(u8"Kazanma##MM3", "Wins##MM3"), &g_Options.danger_zone_wins);
			if (ImGui::Button("Update danger zone")) ProtoFeatures.FixSendClientGcRankUpdate();
		}
		ImGui::EndChild();
		ImGui::SameLine(375);
		ImGui::BeginChild(u8"##CHANGER2", ImVec2(350, 430), true);
		{
			NewSeparator(u8"Diğer Lobi Eklentileri");
			ImGui::NewCheckbox(___(u8"Aktif Et##Rank", "Enabled##Rank"), &g_Options.Profile_enable);
			pipi(10);
			ImGui::SliderInt(___(u8"Seviye", "Level"), &g_Options.Profile_Info_Rank, 0, 40);
			pipi(10);
			ImGui::SliderInt("XP", &g_Options.Profile_Info_XP, 0, 5000);
			ImGui::InputInt(___(u8"Liderlik", "Leader"), &g_Options.Profile_Info_Leader);
			ImGui::InputInt(___(u8"Öğretici", "Teacher"), &g_Options.Profile_Info_Teacher);
			ImGui::InputInt(___(u8"Arkadaşça", "Friendly"), &g_Options.Profile_Info_Friendly);
			ImGui::Combo(___(u8"Sahte Ban", "Fake Ban"), &g_Options.ban, bans, IM_ARRAYSIZE(bans));
			ImGui::InputInt(___(u8"Sahte Ban Süresi", "Fake Ban Time"), &g_Options.time);
			NewSeparator(u8"Prime Exploit");
			if (ImGui::Button("Prime Ol"))
			{

				static uint8_t* fakePrimeStatus = Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "83 F8 05 5E 0F 94 C0 C3");
				DWORD old_protect;

				VirtualProtect(fakePrimeStatus, 3, PAGE_EXECUTE_READWRITE, &old_protect);        char patch[] = { 0x83, 0xF8, 0x00 };
				memcpy(fakePrimeStatus, patch, 3);
				VirtualProtect(fakePrimeStatus, 3, old_protect, nullptr);
			}
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		break;
	case 2:
		RenderInventoryTab();
		break;
	}
}



void RenderOptionsTab() {
	const char* Style[] =
	{
		"1 style",
		"2 style",
		"3 style",
		"4 style"
	};

	const char* radio[] = {
("Off"),
("Rock"),
("Techno"),
("HvH"),
//("Rap"),
("Teatime"),
("Clubtime"),
("Housetime"),
("Iluvradio"),
("8bit"),
(u8"ğàäèî ğåêîğä"),
("DFM"),
("Radio Energy"),
(u8"ğåòğî FM"),
(u8"Äîğîæíîå ğàäèî"),
(u8"Íàøå ğàäèî"),
(u8"LikeFM"),
(u8"Åâğîïà Ïëşñ"),
(u8"AxePrime Premium Radio")
	};
	ImGui::BeginGroup();
	ImGui::BeginChild(u8"##CONFIG1", ImVec2(350, 475), true);
	{
		NewSeparator("Config");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
		static std::vector<std::string> configs;

		static auto load_configs = []() {
			std::vector<std::string> items = {};

			std::string path("C:\\AxePrimePremium");
			if (!fs::is_directory(path))
				fs::create_directories(path);

			for (auto& p : fs::directory_iterator(path))
				items.push_back(p.path().string().substr(path.length() + 1));

			return items;
		};

		static auto is_configs_loaded = false;
		if (!is_configs_loaded) {
			is_configs_loaded = true;
			configs = load_configs();
		}

		static std::string current_config;

		static char config_name[32];
		ImGui::Spacing();

		ImGui::InputText(u8"##config_name", config_name, sizeof(config_name));
		ImGui::SameLine();
		if (ImGui::Button(___(u8"Oluştur", "Create"))) {
			current_config = std::string(config_name);

			Config->Save(current_config + ".ini");
			is_configs_loaded = false;
			memset(config_name, 0, 32);
		}

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.50f, 0.00f, 1.00f, 1.00f));
		ImGui::BeginChild("##configs", ImVec2(250, 150), true);
		ImGui::ListBoxHeader("##configs");
		{
			for (auto& config : configs) {
				if (ImGui::Selectable(config.c_str(), config == current_config)) {
					current_config = config;
				}
			}
		}
		ImGui::ListBoxFooter();
		ImGui::EndChild();
		ImGui::PopStyleColor();

		if (!current_config.empty()) {

			if (ImGui::Button(___(u8"Yükle", "Load")))
				Config->Load(current_config);
			ImGui::SameLine();

			if (ImGui::Button(___(u8"Kaydet", "Save")))
				Config->Save(current_config);
			ImGui::SameLine();

			if (ImGui::Button(___(u8"Sil", "Delete")) && fs::remove("C:\\AxePrimePremium\\" + current_config)) {
				current_config.clear();
				is_configs_loaded = false;
			}

			ImGui::SameLine();
		}

		if (ImGui::Button(___(u8"Yenile", "Refresh")))
			is_configs_loaded = false;
	}
	ImGui::EndChild();
	ImGui::SameLine(375);
	ImGui::BeginGroup();
	ImGui::BeginChild(u8"##CONFIG2", ImVec2(350, 475), true);
	{
		NewSeparator("Lobi");
		ImGui::NewCheckbox(___(u8"İsim Çalıcı", u8"Name-Stealer"), &g_Options.namestealer);
		ImGui::Combo(___(u8"Spamlayıcı", "Chat Spam"), &g_Options.misc_chatspam, "Off\0\rAxePrime\0\rCustom");
		if (g_Options.misc_chatspam == 2)
		{
			ImGui::InputText(u8"##CustomSpam", g_Options.spam, 128);
			ImGui::Spacing();
		}
		ImGui::NewCheckbox(___(u8"Klan Tag", u8"Clan Tag"), &g_Options.misc_clantag);
		if (g_Options.misc_clantag)
		{
			ImGui::NewCheckbox("Custom", &g_Options.misc_customclan);
			if (g_Options.misc_customclan) {
				ImGui::NewCheckbox("Animated", &g_Options.misc_clantaga);
				ImGui::InputText("Custom Clantag", g_Options.customclan, sizeof(g_Options.customclan) - 1);
				if (!g_Options.misc_clantaga)
					if (ImGui::Button("Set Clantag"))
						Utils::SetClantag(g_Options.customclan);
			}
		}
		NewSeparator("Radar");
		ImGui::NewCheckbox(___(u8"Aktif Et##Radar", u8"Enabled##Radar"), &g_Options.Act);
		if (g_Options.Act)
		{
			ImGui::Combo(___(u8"Stil##Radar", u8"Style##Radar"), &g_Options.RadarStyle, Style, ARRAYSIZE(Style));
			pipi(10);
			ImGui::SliderFloat(___(u8"Uzaklık##Radar", u8"Range##Radar"), &g_Options.Zoom, 0, 4);
			pipi(10);
			ImGui::SliderInt(___(u8"Büyüklük##Radar", u8"Size##Radar"), &g_Options.Size, 0, 10);
		}
		NewSeparator("Radyo");
		ImGui::Combo(___(u8"Radyo Kanalı", "Radio Channel"), &g_Options.radiochannel, radio, IM_ARRAYSIZE(radio));
		pipi(10);
		ImGui::SliderInt(___(u8"Ses", "Volume"), &g_Options.radiovolume, 0, 100);
		pipi(10);
		KeyBind(___(u8"Sesi Kapat", "Mute"), g_Options.radiomute);
	}
	ImGui::EndChild();
	ImGui::EndGroup();
}



void Menu::Initialize()
{
	CreateStyle();

	_visible = false;
}

void Menu::Shutdown()
{
	_visible = false;
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Menu::OnDeviceLost()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

#include "BASS/bass.h"
#include "sounds/sounds.h"
#include "xorstr.h"

void RenderRadio() {
	if (strlen(BASS::bass_metadata) > 0 && g_Options.radiochannel) {

		ImGui::Begin(u8"Şuan Çalıyor", nullptr, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text(radio_muted ? "Durduruldu" : BASS::bass_metadata);
		ImGui::End();
	}
}

void Menu::Render()
{
	ImGui::GetIO().MouseDrawCursor = _visible;

	if (g_Options.esp_speclist)
		Visuals::Get().RenderSpectatorList();
	if (g_Options.Act)
		RadarRender();

	RenderRadio();

	if (!_visible)
		return;

	static int selected_Tab = 0;


	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	float x = w * 0.5f - 300.0f;
	float y = h * 0.5f - 200.0f;
	ImGui::SetNextWindowSize(ImVec2{ 800, 600 }, ImGuiCond_Once);
	std::string tessst = _xor_("AxePrimePremium##V5").c_str();
	static int AxeTab = 0;
	if (ImGui::Begin(tessst.c_str(), &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::GetWindowDrawList()->AddImage(arkaplan, ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), ImVec2(ImGui::GetWindowPos().x + 800, ImGui::GetWindowPos().y + 600), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
		ImGui::BeginGroup();
		ImGui::BeginChild("##MENU1", ImVec2(0, 50));
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 225);
			ImGui::PushFont(TitleFont);
			if (ImGui::OwOButton(u8"LEGITBOT##tab", selected_Tab == 0 ? true : false, ImVec2(100, 35)))selected_Tab = 0;
			ImGui::SameLine();
			if (ImGui::OwOButton(u8"VISUALS##tab", selected_Tab == 1 ? true : false, ImVec2(100, 35)))selected_Tab = 1;
			ImGui::SameLine();
			if (ImGui::OwOButton(u8"MISC##tab", selected_Tab == 2 ? true : false, ImVec2(100, 35)))selected_Tab = 2;
			ImGui::SameLine();
			if (ImGui::OwOButton(u8"CHANGERS##tab", selected_Tab == 3 ? true : false, ImVec2(100, 35)))selected_Tab = 3;
			ImGui::SameLine();
			if (ImGui::OwOButton(u8"CONFIG##tab", selected_Tab == 4 ? true : false, ImVec2(100, 35)))selected_Tab = 4;
			ImGui::PopFont();
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		ImGui::BeginGroup();
		ImGui::BeginChild("##MENU2", ImVec2(0, 475));
		{
			ImGui::BeginGroup();
			ImGui::PushID(selected_Tab);
			switch (selected_Tab)
			{
			case 0:
				RenderAimbotTab();
				break;
			case 1:
				RenderEspTab();
				break;
			case 2:
				RenderMiscTab();
				break;
			case 3:
				RenderSkinsTab();
				break;
			case 4:
				RenderOptionsTab();
				break;
			}
			ImGui::PopID();
			ImGui::EndGroup();
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 35);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.30f, 0.30f, 0.30f, 0.30f));
		ImGui::BeginGroup();
		ImGui::BeginChild("##MENU3", ImVec2(0, 25));
		{
			ImGui::PushFont(TitleFont);
			char buf[128];
			sprintf(buf, "axeprimeyazilim.xyz %c", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3]);	
			ImGui::Text(buf);
			ImGui::SameLine();
			ImGui::Text("V5.2");
			ImGui::PopFont();
		}
		ImGui::EndChild();
		ImGui::EndGroup();
		ImGui::PopStyleColor();
	}
	ImGui::End();
}

void Visuals::RenderSpectatorList() {
	int specs = 0;
	std::string spect = "";

	if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected()) {
		int localIndex = g_EngineClient->GetLocalPlayer();
		C_BasePlayer* pLocalEntity = C_BasePlayer::GetPlayerByIndex(localIndex);
		if (pLocalEntity) {
			for (int i = 1; i < g_EngineClient->GetMaxClients(); i++) {
				C_BasePlayer* pBaseEntity = C_BasePlayer::GetPlayerByIndex(i);
				if (!pBaseEntity)										     continue;
				if (pBaseEntity->m_iHealth() > 0)							 continue;
				if (pBaseEntity == pLocalEntity)							 continue;
				if (pBaseEntity->IsDormant())								 continue;
				if (!pLocalEntity->IsAlive()) {
					if (pBaseEntity->m_hObserverTarget() != pLocalEntity->m_hObserverTarget())
						continue;
				}
				else {
					if (pBaseEntity->m_hObserverTarget() != pLocalEntity)
						continue;
				}

				player_info_t pInfo;
				g_EngineClient->GetPlayerInfo(pBaseEntity->EntIndex(), &pInfo);
				if (pInfo.ishltv) continue;

				spect += pInfo.szName;
				spect += "\n";
				specs++;
			}
		}
	}
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin(u8"İzleyici Listesi", nullptr, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
		if (specs > 0) spect += "\n";

		ImVec2 size = ImGui::CalcTextSize(spect.c_str());
		ImGui::SetWindowSize(ImVec2(200, 25 + size.y));
		ImGui::Text(spect.c_str());
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void Menu::Toggle()
{
	if (!g_Unload)
		_visible = !_visible;
}

void Menu::CreateStyle()
{
	ImGui::StyleColorsDark();
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX);
	ImGui::GetStyle() = _style;
}