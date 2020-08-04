#include <algorithm>
#include "kit_parser.hpp"
#include "helpers\utils.hpp"
std::vector<paint_kit> k_skins;
std::vector<paint_kit> k_gloves;
std::vector<paint_kit> k_stickers;

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;
template <typename Key, typename Value>
struct Node_t
{
	int previous_id; //0x0000
	int next_id; //0x0004
	void* _unknown_ptr; //0x0008
	int _unknown; //0x000C
	Key key; //0x0010
	Value value; //0x0014
};
template <typename Key, typename Value>
struct Head_t
{
	Node_t<Key, Value>* memory; //0x0000
	int allocation_count; //0x0004
	int grow_size; //0x0008
	int start_element; //0x000C
	int next_available; //0x0010
	int _unknown; //0x0014
	int last_element; //0x0018
}; //Size=0x001C
   // could use CUtlString but this is just easier and CUtlString isn't needed anywhere else
struct String_t
{
	char* buffer; //0x0000
	int capacity; //0x0004
	int grow_size; //0x0008
	int length; //0x000C
}; //Size=0x0010
struct CPaintKit
{
	int id; //0x0000
	String_t name; //0x0004
	String_t description; //0x0014
	String_t item_name; //0x0024
	String_t material_name; //0x0034
	String_t image_inventory; //0x0044
	char pad_0x0054[0x8C]; //0x0054
}; //Size=0x00E0

struct CStickerKit
{
	int id;

	int item_rarity;

	String_t name;
	String_t description;
	String_t item_name;
	String_t material_name;
	String_t image_inventory;

	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool is_custom_sticker_material;

	float rotate_end;
	float rotate_start;

	float scale_min;
	float scale_max;

	float wear_min;
	float wear_max;

	String_t image_inventory2;
	String_t image_inventory_large;

	std::uint32_t pad0[4];
};

auto get_export(const char* module_name, const char* export_name) -> void*
{
	HMODULE mod;
	while (!((mod = GetModuleHandleA(module_name))))
		Sleep(100);
	return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
}
auto initialize_kits() -> void
{
	std::ifstream items{ "csgo/scripts/items/items_game_cdn.txt" };
	const std::string gameItems{ std::istreambuf_iterator<char>{ items }, std::istreambuf_iterator<char>{ } };
	items.close();
	const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(get_export("vstdlib.dll", "V_UCS2ToUTF8"));
	// Search the relative calls
	// call    ItemSystem
	// push    dword ptr [esi+0Ch]
	// lea     ecx, [eax+4]
	// call    CEconItemSchema::GetPaintKitDefinition
	const auto sig_address = Utils::PatternScan(GetModuleHandle("client_panorama.dll"), "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");
	// Skip the opcode, read rel32 address
	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(sig_address + 1);
	// Add the offset to the end of the instruction
	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem * (*)()>(sig_address + 5 + item_system_offset);
	// Skip VTable, first member variable of ItemSystem is ItemSchema
	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(item_system_fn()) + sizeof(void*));
	// Dump paint kits
	{
		// Skip the instructions between, skip the opcode, read rel32 address
		const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(sig_address + 11 + 1);
		// Add the offset to the end of the instruction
		const auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit * (__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);
		// The last offset is start_element, we need that
		// push    ebp
		// mov     ebp, esp
		// sub     esp, 0Ch
		// mov     eax, [ecx+298h]
		// Skip instructions, skip opcode, read offset
		const auto start_element_offset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(get_paint_kit_definition_fn) + 8 + 2);
		// Calculate head base from start_element's offset
		const auto head_offset = start_element_offset - 12;
		const auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(std::uintptr_t(item_schema) + head_offset);
		for (auto i = 0; i <= map_head->last_element; ++i)
		{
			const auto paint_kit = map_head->memory[i].value;
			if (paint_kit->id == 9001)
				continue;
			const auto wide_name = g_Localize->Find(paint_kit->item_name.buffer + 1);

			//size_t retval;
			//wcstombs_s(&retval, name, sizeof(name) - 1, wide_name, sizeof(name) - 1);
			//g_localize->ConvertUnicodeToANSI(wide_name, name, sizeof(name));


			//V_UCS2ToUTF8(wide_name, name, sizeof(name));
			const int itemNameLength = WideCharToMultiByte(CP_UTF8, 0, wide_name, -1, nullptr, 0, nullptr, nullptr);
			if (std::string name(itemNameLength, 0); WideCharToMultiByte(CP_UTF8, 0, wide_name, -1, &name[0], itemNameLength, nullptr, nullptr)) {
				if (paint_kit->id < 10000) {

					if (auto pos = gameItems.find('_' + std::string{ paint_kit->name.buffer } +'='); pos != std::string::npos && gameItems.substr(pos + paint_kit->name.length).find('_' + std::string{ paint_kit->name.buffer } +'=') == std::string::npos) {
						if (auto weaponName = gameItems.rfind("weapon_", pos); weaponName != std::string::npos) {
							name.back() = ' ';
							name += '(' + gameItems.substr(weaponName + 7, pos - weaponName - 7) + ')';
						}
					}
					k_skins.push_back({ paint_kit->id, name });
				}

				else
					k_gloves.push_back({ paint_kit->id, name });
			}
			/*k_skins.push_back({ paint_kit->id, name });
			for (int i = 0; i < KNIFE_DATA_SIZE; i++) {
				KnifeSkins[i].SkinNames.push_back(name);
				KnifeSkins[i].SkinPaintKit.push_back(paint_kit->id);
			}
			for (int i = 0; i < WEAPON_DATA_SIZE; i++) {
				WeaponSkins[i].SkinNames.push_back(name);
				WeaponSkins[i].SkinPaintKit.push_back(paint_kit->id);
			}*/
		}
		std::sort(k_skins.begin(), k_skins.end());
		std::sort(k_gloves.begin(), k_gloves.end());
	}

	// Dump sticker kits
	{
		const auto sticker_sig = Utils::PatternScan(GetModuleHandle("client_panorama.dll"), "53 8D 48 04 E8 ? ? ? ? 8B 4D 10") + 4;

		// Skip the opcode, read rel32 address
		const auto get_sticker_kit_definition_offset = *reinterpret_cast<std::intptr_t*>(sticker_sig + 1);

		// Add the offset to the end of the instruction
		const auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit * (__thiscall*)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);

		// The last offset is head_element, we need that

		//	push    ebp
		//	mov     ebp, esp
		//	push    ebx
		//	push    esi
		//	push    edi
		//	mov     edi, ecx
		//	mov     eax, [edi + 2BCh]

		// Skip instructions, skip opcode, read offset
		const auto start_element_offset = *reinterpret_cast<intptr_t*>(std::uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		const auto head_offset = start_element_offset - 12;

		const auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(std::uintptr_t(item_schema) + head_offset);

		for (auto i = 0; i <= map_head->last_element; ++i)
		{
			const auto sticker_kit = map_head->memory[i].value;

			char sticker_name_if_valve_fucked_up_their_translations[64];

			auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;

			if (strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas"))
			{
				strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
				strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
				sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
			}

			const auto wide_name = g_Localize->Find(sticker_name_ptr);
			char name[256];
			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			k_stickers.push_back({ sticker_kit->id, name });
		}

		std::sort(k_stickers.begin(), k_stickers.end());

		k_stickers.insert(k_stickers.begin(), { 0, "None" });
	}

}