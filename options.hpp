#pragma once
#include <set>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include <limits>
#include "valve_sdk/Misc/Color.hpp"
#include "kit_parser.hpp"
#include "item_definitions.hpp"
#define A( s ) #s
#define OPTION(type, var, val) type var = val

extern std::map<int, const char*> k_weapon_names;

class Sticker_t
{
public:
	int stickers_id = 0;
};

struct item_setting
{
	char name[32] = "Default";
	bool enabled = false;
	Sticker_t stickers[5];
	int stickers_place = 0;
	int definition_vector_index = 0;
	int definition_index = 0;
	bool   enabled_stickers = 0;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	bool stat_trak = 0;
	float wear = FLT_MIN;
	char custom_name[32] = "";
};


template <typename T = bool>
class Var {
public:
	std::string name;
	std::shared_ptr<T> value;
	int32_t size;
	Var(std::string name, T v) : name(name) {
		value = std::make_shared<T>(v);
		size = sizeof(T);
	}
	operator T() { return *value; }
	operator T*() { return &*value; }
	operator T() const { return *value; }
	//operator T*() const { return value; }
};
struct statrack_setting
{

	int definition_index = 1;
	struct
	{
		int counter = 0;
	}statrack_new;
};
struct funstr {
	OPTION(float, playback, 0.f);
	OPTION(float, weight, 0.f);
	OPTION(int, order, 0);
	OPTION(int, sequence, 0);
	OPTION(float, cycle, 0.f);
	OPTION(float, weightdelta, 0.f);
	OPTION(float, prevcycle, 0.f);
	OPTION(float, posparam, 0.f);
	OPTION(float, feetrate, 0.f);
	OPTION(bool, funenable, false);
};

struct skinInfo
{
	int seed = -1;
	int paintkit;
	std::string tagName;
};


class Options
{
public:
		std::map<int, funstr> funstuff = {};
		std::map<int, statrack_setting> statrack_items = { };
		bool Profile_enable;
		int Profile_Info_Rank = 0;
		int Profile_Info_Rank_Combo;
		int Profile_Info_Level = 1;
		int Profile_Info_XP = 1;
		int Profile_Info_Win = 1;
		int Profile_Info_Friendly = 1;
		int Profile_Info_Leader = 1;
		int Profile_Info_Teacher = 1;

		int wingman_rank;
		int wingman_wins;
		int danger_zone_rank;
		int danger_zone_wins;

		int ban;
		int time;

	
		/*LEGITBOT*/
	/////////////////////////////////////////////
		OPTION(bool, enable_legitbot, false);
		OPTION(bool, Friendly_Fire, false);
		OPTION(bool, misc_watermark, false);
		//OPTION(bool, legit_backtrack, false);
		OPTION(int, legit_backtrack, false);
		bool CanAutoPistol = false;
		OPTION(bool, misc_autopistol, false);
		OPTION(bool, keytrigger, false);
		OPTION(bool, showbacktrack, false);
		OPTION(bool, dangerzone, false);
		OPTION(bool, legit_enabled, false);
		OPTION(int, aimkey, 0);
		//pistols
		OPTION(int, hitbox_pistols, 0);
		OPTION(bool, aim_LegitBotPistols, false);
		OPTION(float, legit_fov_pistols, 1.f);
		OPTION(float, legit_smooth_pistols, 1.f);
		OPTION(float, legit_rcsmin_pistols, 0.f);
		OPTION(float, legit_rcsmax_pistols, 0.f);
		// rifles
		OPTION(int, hitbox_rifles, 0);
		OPTION(bool, aim_LegitBotRifles, false);
		OPTION(float, legit_smooth_rifles, 1.f);
		OPTION(float, legit_fov_rifles, 1.f);
		OPTION(float, legit_rcsmin_rifles, 0.f);
		OPTION(float, legit_rcsmax_rifles, 0.f);
		//snipers
		OPTION(int, hitbox_snipers, 0);
		OPTION(bool, aim_LegitBotSnipers, false);
		OPTION(float, legit_smooth_Snipers, 1.f);
		OPTION(float, legit_fov_Snipers, 1.f);
		OPTION(float, legit_rcsmin_Snipers, 0.f);
		OPTION(float, legit_rcsmax_Snipers, 0.f);
		OPTION(float, hitchance_amount, 0.f);
		//smg
		OPTION(int, hitbox_smg, 0);
		OPTION(bool, aim_LegitBotsmg, false);
		OPTION(float, legit_fov_smg, 1.f);
		OPTION(float, legit_smooth_smg, 1.f);
		OPTION(float, legit_rcsmin_smg, 0.f);
		OPTION(float, legit_rcsmax_smg, 0.f);
		//Shotgun
		OPTION(int, hitbox_Shotgun, 0);
		OPTION(bool, aim_LegitBotShotgun, false);
		OPTION(float, legit_fov_Shotgun, 1.f);
		OPTION(float, legit_smooth_Shotgun, 1.f);
		OPTION(float, legit_rcsmin_Shotgun, 0.f);
		OPTION(float, legit_rcsmax_Shotgun, 0.f);
		// Machinegun
		OPTION(int, hitbox_Machinegun, 0);
		OPTION(bool, aim_LegitBotMachinegun, false);
		OPTION(float, legit_smooth_Machinegun, 1.f);
		OPTION(float, legit_fov_Machinegun, 1.f);
		OPTION(float, legit_rcsmin_Machinegun, 0.f);
		OPTION(float, legit_rcsmax_Machinegun, 0.f);

		//ESP
		float color_esp_sounds[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		OPTION(bool, esp_sounds, false);
		OPTION(bool, esp_optimise, false);
		OPTION(float, esp_sounds_time, 0.5f);
		OPTION(float, esp_sounds_radius, 15.0f);
		OPTION(bool, esp_flags_armor, false);
		OPTION(bool, esp_flags_kit, false);
		OPTION(bool, esp_flags_hostage, false);
		OPTION(bool, esp_flags_scoped, false);
		OPTION(bool, esp_flags_money, false);
		OPTION(bool, esp_flags_c4, false);
		OPTION(bool, esp_flags_defusing, false);
		int   RadarType = 0;
		bool Act = 0;
		int	  RadarStyle = 0;
		float Zoom = 1;
		int   Size = 1;

		OPTION(int, layer, 0);
		OPTION(int, radiochannel, 0);
		OPTION(int, radiovolume, 0);
		OPTION(int, radiomute, 0);
		int dista = 0;
		int distb = 0;
		OPTION(bool, kbot, false);
		OPTION(bool, nosmoke, false);
		OPTION(bool, ANIMFIX, false);
		OPTION(bool, nozoom, false);
		OPTION(bool, debug_showposes, false);
		OPTION(bool, debug_showactivities, false);
		OPTION(int, fakelags, 0);

		OPTION(bool, trigchecksmoke, false);
		OPTION(float, trighitchance, 0.f);
		OPTION(float, trigdelay, 0.f);
		OPTION(bool, trigcheckflash, false);
		OPTION(bool, trighead, false);
		OPTION(bool, trigleftarm, false);
		OPTION(bool, trigrightarm, false);
		OPTION(bool, trigleftleg, false);
		OPTION(bool, trigrightleg, false);
		OPTION(bool, trigchest, false);
		OPTION(bool, trigstomach, false);

		OPTION(bool, trigenable, false);
		OPTION(bool, onkey, false);
		OPTION(int, trigbind, 0);

		OPTION(bool, baimlethal, false);
		OPTION(bool, baimfast, false);
		OPTION(bool, baimair, false);
		OPTION(bool, baimsw, false);
		OPTION(int, baimhp, 0);

		///

		OPTION(float, flash, 255.f);

		OPTION(bool, antiscreen, false);
		OPTION(bool, resolver,false);
		OPTION(float , retrack, 15.f);
		///
		OPTION(bool, solbicak, false);
		OPTION(int, misc_chatspam, false);
		char spam[128];
		OPTION(bool, use_all_weapons, false);
		OPTION(bool, esp_angle_lines, false);
		OPTION(bool, esp_speclist, false);
		OPTION(bool, esp_damageindicator, false);
		OPTION(bool, hitsound, false);
		OPTION(bool, esp_damageindicator_color, false);
		OPTION(bool, show_ammo, false);
		OPTION(bool, misc_autoaccept, false);
		OPTION(bool, edgejump, false);
		OPTION(int, edgejump_bind, 0);
		OPTION(bool, namestealer, false);
		OPTION(bool, fakeduck, false);
		OPTION(int, fakeduck_bind, 0);
		OPTION(bool, misc_desync_ad, false);
		
		OPTION(int, misc_theme, 0);

		OPTION(int, misc_desync, 0);
		OPTION(int, misc_desync_bind, 0);

		OPTION(bool, autoblock, false);
		OPTION(int, autoblock_bind, 0);
		OPTION(bool, misc_customclan, false);
		char customclan[16];
		OPTION(bool, menu_language, false);

		struct {
			std::map<int, item_setting> m_items;
			std::unordered_map<std::string, std::string> m_icon_overrides;
			auto get_icon_override(const std::string original) const -> const char*
			{
				return m_icon_overrides.count(original) ? m_icon_overrides.at(original).data() : nullptr;
			}
		} skins;
		// 
		// ESP
		// 
		OPTION(bool, esp_enabled, false);
		OPTION(bool, esp_visible_only, false);
		OPTION(bool, esp_enemy_only, false);
		OPTION(bool, esp_farther, false);
		OPTION(int, esp_player_box, 0);
		OPTION(bool, esp_player_skeleton, false);
		OPTION(bool, esp_player_skeleton_bt, false);
		OPTION(bool, esp_player_names, false);
		OPTION(bool, esp_player_health, false);
		OPTION(bool, esp_player_armour, false);
		OPTION(bool, esp_player_weapons, false);

		OPTION(bool, chams_arms_enabled, false);
		OPTION(bool, chams_arms_wireframe, false);
		OPTION(bool, chams_arms_reflect, false);
		OPTION(int, chams_arms_mat, false);
		float chams_arms_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

		OPTION(bool, chams_sleeves_enabled, false);
		OPTION(bool, chams_sleeves_wireframe, false);
		OPTION(bool, chams_sleeves_reflect, false);
		OPTION(int, chams_sleeves_mat, false);
		float chams_sleeves_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

		OPTION(bool, chams_weapon_enabled, false);
		OPTION(bool, chams_weapon_wireframe, false);
		OPTION(bool, chams_weapon_reflect, false);
		OPTION(int, chams_weapon_mat, false);
		float chams_weapon_color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

		OPTION(bool, esp_dropped_weapons, false);
		OPTION(bool, crosshair, false);
		
		OPTION(bool, esp_defuse_kit, false);
		OPTION(bool, esp_planted_c4, false);
		OPTION(bool, esp_items, false);
		OPTION(float, esp_distance_dz, 1024.f);

		float color_esp_visible[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		float color_esp_skeleton[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		float color_esp_occluded[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

		//
		// CHAMS
		//
		OPTION(bool, chams_player_enabled, false);
		OPTION(bool, chams_player_enemies_only, false);
		OPTION(bool, chams_player_wireframe, false);
		OPTION(bool, chams_player_fake, false);
		OPTION(bool, chams_player_flat, false);
		OPTION(bool, chams_player_visibleonly, false);
		OPTION(bool, chams_player_glass, false);
		float color_chams_player_visible[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		float color_chams_player_occluded[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		float color_chams_real[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		// 
		// GLOW
		// 
		OPTION(bool, glow_enabled, false);
		OPTION(bool, glow_pulsing, false);
		OPTION(bool, glow_enemy_only, false);
		float color_glow[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

		// 
		// OTHER
		// 
		OPTION(bool, other_drawfov, false);
		OPTION(float, other_drawfov_fov, 0.f);
		OPTION(bool, other_no_hands, false);
		OPTION(bool, esp_grenade_prediction, false);
		OPTION(bool, other_nightmode, false);
		OPTION(float, other_nightmode_size, 0.05f);
		OPTION(float, other_mat_ambient_light_r, 0.0f);
		OPTION(float, other_mat_ambient_light_g, 0.0f);
		OPTION(float, other_mat_ambient_light_b, 0.0f);
		OPTION(bool, other_mat_ambient_light_rainbow, false);

		//
		// MISC
		//
		OPTION(bool, DiscordRichPresense, false);
		OPTION(bool, misc_bhop, false);
		OPTION(bool, misc_autostrafer, false);
		OPTION(bool, misc_clantag, false);
		OPTION(bool, misc_clantaga, false);
		OPTION(bool, misc_thirdperson, false);
		OPTION(int, misc_thirdperson_bind, 0);
		OPTION(bool, misc_rankreveal, false);
		OPTION(float, misc_thirdperson_dist, 50.f);
		OPTION(int, misc_viewmodel_fov, 68);
		OPTION(int, misc_fov, 90);	

		OPTION(bool, fakelag_enabled, false);
		OPTION(bool, fakelag_standing, false);
		OPTION(bool, fakelag_moving, false);
		OPTION(bool, fakelag_unducking, false);
		OPTION(int, fakelag_mode, 0);
		OPTION(int, fakelag_factor, 0);


		std::unordered_map<std::string, std::set<std::string>> weaponSkins;
		std::unordered_map<std::string, skinInfo> skinMap;
		std::unordered_map<std::string, std::string> skinNames;
};

struct wskin
{
	int wId;
	int paintkit;
	int rarity;
	int stickers[4];
	std::string name;
	float seed;
	float wear;
	float sticker_wear;
	float sticker_scale;
	float sticker_rotaion;
};

extern std::vector<wskin> g_skins;
extern Options g_Options;
extern bool   g_Unload;
