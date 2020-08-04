#include "config.hpp"
#include "item_definitions.hpp"
#include "helpers/math.hpp"
#include "helpers/utils.hpp"
#include "menu.hpp"
#include "helpers/input.hpp"
#include "options.hpp"

// For ints, chars, floats, and bools
void CConfig::SetupValue(int& value, int def, std::string category, std::string name) { value = def; ints.push_back(new ConfigValue< int >(category, name, &value, def)); }
void CConfig::SetupValue(char* value, char* def, std::string category, std::string name) { value = def; chars.push_back(new ConfigValue< char >(category, name, value, *def)); }
void CConfig::SetupValue(float& value, float def, std::string category, std::string name) { value = def; floats.push_back(new ConfigValue< float >(category, name, &value, def)); }
void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name) { value = def; bools.push_back(new ConfigValue< bool >(category, name, &value, def)); }
void CConfig::SetupColor(float value[4], std::string name) {
	SetupValue(value[0], value[0], ("colors"), name + "_r");
	SetupValue(value[1], value[1], ("colors"), name + "_g");
	SetupValue(value[2], value[2], ("colors"), name + "_b");
	SetupValue(value[3], value[3], ("colors"), name + "_a");
}

void CConfig::SetupLegit() {
	/*LegitBot*/
	///////////////////////////////////
	SetupValue(g_Options.enable_legitbot, false, ("legit"), ("enabled"));
	SetupValue(g_Options.misc_backtrack, false, ("legitbt"), ("enabled"));
	SetupValue(g_Options.legit_backtrack, 0, ("legitbttk"), ("enable"));
	SetupValue(g_Options.resolver, 0, ("reso"), ("enable"));
	SetupValue(g_Options.dangerzone, false, ("legit"), ("dangerzone"));
	/*Legitbot - rifles*/
	///////////////////////////////////
	SetupValue(g_Options.aim_LegitBotRifles, false, ("rifleaim"), ("enabled"));
	SetupValue(g_Options.aimkey, false, ("rkey"), ("enabled"));
	SetupValue(g_Options.hitbox_rifles, false, ("rhitbox"), ("enabled"));
	SetupValue(g_Options.legit_smooth_rifles, false, ("rsmooth"), ("enabled"));
	SetupValue(g_Options.legit_fov_rifles, false, ("rfov"), ("enabled"));
	SetupValue(g_Options.legit_rcsmax_rifles, false, ("rsrcsmax"), ("enabled"));
	SetupValue(g_Options.legit_rcsmin_rifles, false, ("rsrcsmin"), ("enabled"));
	/*Legitbot - pistol*/
	///////////////////////////////////
	SetupValue(g_Options.aim_LegitBotPistols, false, ("ppistol"), ("enabled"));
	SetupValue(g_Options.hitbox_pistols, false, ("phitbox"), ("enabled"));
	SetupValue(g_Options.legit_smooth_pistols, false, ("spmooth"), ("enabled"));
	SetupValue(g_Options.legit_fov_pistols, false, ("cfov"), ("enabled"));
	SetupValue(g_Options.legit_rcsmax_pistols, false, ("pistolsrcsmax"), ("enabled"));
	SetupValue(g_Options.legit_rcsmin_pistols, false, ("pistolsrcsmin"), ("enabled"));
	/*Legitbot - snipers*/
	///////////////////////////////////
	SetupValue(g_Options.aim_LegitBotSnipers, false, ("sl"), ("enabled"));
	SetupValue(g_Options.hitbox_snipers, false, ("shitbox"), ("enabled"));
	SetupValue(g_Options.legit_smooth_Snipers, false, ("ssmooth"), ("enabled"));
	SetupValue(g_Options.legit_fov_Snipers, false, ("sfov"), ("enabled"));
	SetupValue(g_Options.legit_rcsmax_Snipers, false, ("srcsmax"), ("enabled"));
	SetupValue(g_Options.legit_rcsmin_Snipers, false, ("srcsmin"), ("enabled"));
	/*Legitbot - smgs*/
///////////////////////////////////
	SetupValue(g_Options.aim_LegitBotsmg, false, ("smgaim"), ("enabled"));
	SetupValue(g_Options.hitbox_smg, false, ("smghitbox"), ("enabled"));
	SetupValue(g_Options.legit_smooth_smg, false, ("smgsmooth"), ("enabled"));
	SetupValue(g_Options.legit_fov_smg, false, ("smgfov"), ("enabled"));
	SetupValue(g_Options.legit_rcsmax_smg, false, ("smgrcsmax"), ("enabled"));
	SetupValue(g_Options.legit_rcsmin_smg, false, ("smgrcsmin"), ("enabled"));
	/*Legitbot - Shotgun*/
///////////////////////////////////
	SetupValue(g_Options.aim_LegitBotShotgun, false, ("Shotgunaim"), ("enabled"));
	SetupValue(g_Options.hitbox_Shotgun, false, ("Shotgunhitbox"), ("enabled"));
	SetupValue(g_Options.legit_smooth_Shotgun, false, ("Shotgunsmooth"), ("enabled"));
	SetupValue(g_Options.legit_fov_Shotgun, false, ("Shotgunfov"), ("enabled"));
	SetupValue(g_Options.legit_rcsmax_Shotgun, false, ("Shotgunrcsmax"), ("enabled"));
	SetupValue(g_Options.legit_rcsmin_Shotgun, false, ("Shotgunrcsmin"), ("enabled"));
	/*Legitbot - Machinegun*/
///////////////////////////////////
	SetupValue(g_Options.aim_LegitBotMachinegun, false, ("Machinegunaim"), ("enabled"));
	SetupValue(g_Options.hitbox_Machinegun, false, ("Machinegunhitbox"), ("enabled"));
	SetupValue(g_Options.legit_smooth_Machinegun, false, ("Machinegunsmooth"), ("enabled"));
	SetupValue(g_Options.legit_fov_Machinegun, false, ("Machinegunfov"), ("enabled"));
	SetupValue(g_Options.legit_rcsmax_Machinegun, false, ("Machinegunrcsmax"), ("enabled"));
	SetupValue(g_Options.legit_rcsmin_Machinegun, false, ("Machinegunrcsmin"), ("enabled"));
	/*TRIGGER*/

	SetupValue(g_Options.trigenable, false, ("trigger"), ("trigenable"));
	SetupValue(g_Options.trigchecksmoke, false, ("trigger"), ("trigchecksmoke"));
	SetupValue(g_Options.onkey, false, ("trigger"), ("onkey"));
	SetupValue(g_Options.trigcheckflash, false, ("trigger"), ("trigcheckflash"));
	SetupValue(g_Options.trighitchance, false, ("trigger"), ("trighitchance"));
	SetupValue(g_Options.trigdelay, false, ("trigger"), ("trigdelay"));
	SetupValue(g_Options.legit_rcsmin_rifles, false, ("trigger"), ("enabled"));
	SetupValue(g_Options.trigbind, 0, ("trigger"), ("trigbind"));


	/*RCS STANDALONE*/
	/*Rifles*/
	/////////////////////////////////////////////////
	SetupValue(g_Options.legit_rcsmin_rifles, 0.f, ("lrcs"), ("rrcs"));
	SetupValue(g_Options.legit_rcsmax_rifles, 0.f, ("lrcs"), ("rrcsm"));
	/////////////////////////////////////////////////
	/*Pistols*/
	SetupValue(g_Options.legit_rcsmin_pistols, 0.f, ("lrcs"), ("prcs"));
	SetupValue(g_Options.legit_rcsmax_pistols, 0.f, ("lrcs"), ("prcsm"));
	////////////////////////////////////////////////
	/*Snipers*/
	SetupValue(g_Options.legit_rcsmin_Snipers, 0.f, ("lrcs"), ("Snrcs"));
	SetupValue(g_Options.legit_rcsmax_Snipers, 0.f, ("lrcs"), ("Snrcsm"));
	/*Shotgun*/
	SetupValue(g_Options.legit_rcsmin_Shotgun, 0.f, ("lrcs"), ("Srcs"));
	SetupValue(g_Options.legit_rcsmax_Shotgun, 0.f, ("lrcs"), ("Srcsm"));
	/*Machine*/
	SetupValue(g_Options.legit_rcsmin_Machinegun, 0.f, ("lrcs"), ("MGrcs"));
	SetupValue(g_Options.legit_rcsmax_Machinegun, 0.f, ("lrcs"), ("MGrcsm"));
}

void CConfig::SetupVisuals() {
	SetupValue(g_Options.esp_enabled, false, ("esp"), ("enabled"));
	SetupValue(g_Options.esp_visible_only, false, ("esp"), ("esp_visible_only"));
	SetupValue(g_Options.show_ammo, false, ("esp"), ("show_ammo"));
	SetupValue(g_Options.crosshair, false, ("esp"), ("crosshair"));
	SetupValue(g_Options.nozoom, false, ("esp"), ("nozoom"));
	SetupValue(g_Options.esp_enemy_only, false, ("esp"), ("enemies_only"));
	SetupValue(g_Options.esp_player_skeleton, false, ("esp"), ("player_skeleton"));
	SetupValue(g_Options.esp_player_box, 0, ("esp"), ("player_boxes_type"));
	SetupValue(g_Options.esp_player_names, false, ("esp"), ("player_names"));
	SetupValue(g_Options.esp_player_health, false, ("esp"), ("player_health"));
	SetupValue(g_Options.esp_player_armour, false, ("esp"), ("player_armour"));
	SetupValue(g_Options.esp_player_weapons, false, ("esp"), ("player_weapons"));
	SetupValue(g_Options.esp_dropped_weapons, false, ("esp"), ("dropped_weapons"));
	SetupValue(g_Options.esp_defuse_kit, false, ("esp"), ("defuse_kit"));
	SetupValue(g_Options.esp_planted_c4, false, ("esp"), ("planted_c4"));
	SetupValue(g_Options.esp_items, false, ("esp"), ("items"));
	SetupValue(g_Options.esp_grenade_prediction, false, ("esp"), ("grenade_prediction"));
	SetupValue(g_Options.esp_sounds, false, ("esp"), ("sounds"));
	SetupValue(g_Options.esp_sounds_time, 0.5f, ("esp"), ("sounds_time"));
	SetupValue(g_Options.esp_sounds_radius, 15.0f, ("esp"), ("sounds_radius"));
	SetupValue(g_Options.esp_angle_lines, false, ("esp"), ("angle_lines"));
	//SetupValue( g_Options.esp_hitmarker, false, ( "esp" ), ( "hitmarker" ) );

	SetupValue(g_Options.glow_enabled, false, ("glow"), ("enabled"));


	SetupValue(g_Options.chams_player_enabled, false, ("chams"), ("player_enabled"));
	SetupValue(g_Options.chams_player_enemies_only, false, ("chams"), ("player_enemies_only"));
	SetupValue(g_Options.chams_player_wireframe, false, ("chams"), ("player_wireframe"));
	SetupValue(g_Options.chams_player_flat, false, ("chams"), ("player_flat"));
	SetupValue(g_Options.chams_player_glass, false, ("chams"), ("player_glass"));
	SetupValue(g_Options.chams_sleeves_enabled, false, ("chams"), ("chams_sleeves_enabled"));
	SetupValue(g_Options.chams_weapon_enabled, false, ("chams"), ("chams_weapon_enabled"));
	SetupValue(g_Options.chams_arms_enabled, false, ("chams"), ("chams_arms_enabled"));
	SetupValue(g_Options.chams_weapon_wireframe, false, ("chams"), ("chams_weapon_wireframe"));
	SetupValue(g_Options.chams_weapon_reflect, false, ("chams"), ("chams_weapon_reflect"));


	SetupValue(g_Options.other_nightmode, false, ("other"), ("nightmode"));
	SetupValue(g_Options.other_nightmode_size, 0.2f, ("other"), ("nightmode_size"));
	SetupValue(g_Options.other_mat_ambient_light_r, false, ("other"), ("mat_ambient_light_r"));
	SetupValue(g_Options.other_mat_ambient_light_g, false, ("other"), ("mat_ambient_light_g"));
	SetupValue(g_Options.other_mat_ambient_light_b, false, ("other"), ("mat_ambient_light_b"));
	SetupValue(g_Options.esp_speclist, false, ("other"), ("esp_speclist"));
	SetupValue(g_Options.esp_damageindicator, false, ("other"), ("esp_damageindicator"));
	SetupValue(g_Options.esp_damageindicator_color, false, ("other"), ("esp_damageindicator_color"));
	SetupValue(g_Options.hitsound, false, ("other"), ("hitsound"));
	SetupValue(g_Options.Act, false, ("other"), ("Act"));

	SetupValue(g_Options.other_drawfov, false, ("other"), ("drawfov"));
	SetupValue(g_Options.other_no_hands, false, ("other"), ("no_hands"));

}

void CConfig::SetupMisc() {
	SetupValue(g_Options.misc_bhop, false, ("misc"), ("bhop"));
	SetupValue(g_Options.misc_autostrafer, false, ("misc"), ("misc_autostrafer"));
	SetupValue(g_Options.misc_rankreveal, false, ("misc"), ("misc_rankreveal"));
	SetupValue(g_Options.namestealer, false, ("misc"), ("namestealer"));
	SetupValue(g_Options.edgejump, false, ("misc"), ("edgejump"));
	SetupValue(g_Options.autoblock, false, ("misc"), ("autoblock"));
	SetupValue(g_Options.kbot, false, ("misc"), ("kbot"));
	SetupValue(g_Options.esp_optimise, false, ("misc"), ("esp_optimise"));
	SetupValue(g_Options.misc_thirdperson, false, ("misc"), ("thirdperson"));
	SetupValue(g_Options.misc_thirdperson_bind, 0, ("misc"), ("thirdperson_bind"));
	SetupValue(g_Options.misc_clantag, false, ("misc"), ("clantag"));
	SetupValue(g_Options.solbicak, false, ("misc"), ("Knifes2s"));
	SetupValue(g_Options.misc_autoaccept, false, ("misc"), ("autoaccept"));
	SetupValue(g_Options.misc_desync, 0, ("misc"), ("desync"));
	SetupValue(g_Options.misc_desync_bind, 0, ("misc"), ("desync_bind"));
	SetupValue(g_Options.misc_thirdperson_dist, 50.f, ("misc"), ("thirdperson_dist"));
	SetupValue(g_Options.misc_viewmodel_fov, 68, ("misc"), ("viewmodel_fov"));
	SetupValue(g_Options.fakelag_enabled, false, ("fakelag"), ("enabled"));
	SetupValue(g_Options.fakelag_standing, false, ("fakelag"), ("standing"));
	SetupValue(g_Options.fakelag_moving, false, ("fakelag"), ("moving"));
	SetupValue(g_Options.fakelag_unducking, false, ("fakelag"), ("unducking"));
	SetupValue(g_Options.fakelag_mode, 0, ("fakelag"), ("mode"));
	SetupValue(g_Options.fakelag_factor, 0, ("fakelag"), ("factor"));
}

void CConfig::SetupSkins() {
	//for (auto&[key, val] : k_weapon_names) {
	for (auto& key : weapon_names) {
		SetupValue(g_Options.skins.m_items[key.definition_index].name, "Default", (key.name), ("name"));
		SetupValue(g_Options.skins.m_items[key.definition_index].enabled, 0, (key.name), ("enabled"));
		SetupValue(g_Options.skins.m_items[key.definition_index].definition_vector_index, 0, (key.name), ("definition_vector_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].definition_index, 1, (key.name), ("definition_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].paint_kit_vector_index, 0, (key.name), ("paint_kit_vector_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].paint_kit_index, 0, (key.name), ("paint_kit_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].definition_override_vector_index, 0, (key.name), ("definition_override_vector_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].definition_override_index, 0, (key.name), ("definition_override_index"));
		SetupValue(g_Options.skins.m_items[key.definition_index].seed, 0, (key.name), ("seed"));
		SetupValue(g_Options.skins.m_items[key.definition_index].stat_trak, 0, (key.name), ("stat_trak"));
		SetupValue(g_Options.skins.m_items[key.definition_index].wear, 0.0f, (key.name), ("wear"));
		SetupValue(g_Options.skins.m_items[key.definition_index].custom_name, "", (key.name), ("custom_name"));
	}
}

void CConfig::SetupColors() {
	SetupColor(g_Options.color_esp_visible, "color_esp_visible");
	SetupColor(g_Options.color_esp_occluded, "color_esp_occluded");
	SetupColor(g_Options.color_esp_skeleton, "color_esp_skeleton");
	SetupColor(g_Options.color_glow, "color_glow");
	SetupColor(g_Options.color_chams_player_visible, "color_chams_player_visible");
	SetupColor(g_Options.color_chams_player_occluded, "color_chams_player_occluded");
	SetupColor(g_Options.chams_arms_color, "chams_arms_color");
	SetupColor(g_Options.chams_sleeves_color, "chams_sleeves_color");
	SetupColor(g_Options.chams_weapon_color, "chams_weapon_color");
	SetupColor(g_Options.color_esp_sounds, "color_esp_sounds");
	SetupColor(g_Options.color_esp_sounds, "color_esp_sounds");
}

void CConfig::Setup() {
	CConfig::SetupLegit();
	CConfig::SetupVisuals();
	CConfig::SetupMisc();
	CConfig::SetupSkins();
	CConfig::SetupColors();
}

#include "helpers/utils.hpp"
void CConfig::Save(const std::string& name) {
	if (name.empty())
		return;

	CreateDirectoryA(u8"C:\\AxePrimePremium\\", NULL);
	std::string file = u8"C:\\AxePrimePremium\\" + name;

	for (auto value : ints) {
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	}

	for (auto value : floats) WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
	for (auto value : bools) WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(const std::string& name) {
	if (name.empty())
		return;

	g_ClientState->ForceFullUpdate();

	CreateDirectoryA(u8"C:\\AxePrimePremium\\", NULL);
	std::string file = u8"C:\\AxePrimePremium\\" + name;

	char value_l[32] = { '\0' };
	for (auto value : ints) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0", value_l, 32, file.c_str()); *value->value = atoi(value_l);
	}

	for (auto value : floats) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "0.0f", value_l, 32, file.c_str()); *value->value = atof(value_l);
	}

	for (auto value : bools) {
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "false", value_l, 32, file.c_str()); *value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config = new CConfig();