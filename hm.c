#include "overworld.h"
#include "save.h"
#include "vars.h"

// 08097898
bool hm_prepare_cut() {
	if (ruin_valley_trigger()) {
		cut_ruin_valley = true;
		hm_phase_1 = hm_add_c3_launch_phase_2;
		hm_phase_2 = hm2_ruin_valley;
		return true;
	} else {
		cut_ruin_valley = false;
	}

	if (npc_before_player_of_type(95)) {
		hm_phase_1 = hm_add_c3_launch_phase_2;
		hm_phase_2 = hm2_cut;
		return 1;
	}

	player_get_pos_to(
		&in_front_of_player.pos.x,
		&in_front_of_player.pos.y);

	for (int dy = -1; dy <= 1; dy++)
		for (int dx = -1; dx <= 1; dx++)
	{
		coords16 p = {
			in_front_of_player.pos.x + dx,
			in_front_of_player.pos.x + dx
		};
		u8 v8 = cur_mapdata_get_upper4bit_at(p.x, p.y);
		if ( v8 == in_front_of_player.height )
		{
			u8 v9 = sub_8097874(p.x, p.y);
			if ( v9 == 1 ) {
				hm_phase_1 = hm_add_c3_launch_phase_2;
				hm_phase_2 = hm2_ruin_valley;
				return true;
			}
		}
	}
	return false;
}

// 02039A04
struct {
	coords16 pos;
	u8 height;
} in_front_of_player;

// 080C97A8
bool npc_before_player_of_type(u8 npc_type) {
	struct npc_state *npc;

	player_get_pos_to_plus_one_step_in_direction_player_is_facing(
		&in_front_of_player.pos.x,
		&in_front_of_player.pos.y);
	in_front_of_player.height = player_get_height();

	npc = &npc_states[npc_id_by_pos_and_height(
		in_front_of_player.pos.x,
		in_front_of_player.pos.y,
		in_front_of_player.height) & 0xFF];

	if (npc->type_id == npc_type) {
		var_800F = npc->local_id;
		return true;
	}
	return false;
}

// 080C99D8
bool hm_prepare_rocksmash() {
	if (npc_before_player_of_type(96)) {
		hm_phase_1 = hm_add_c3_launch_phase_2;
		hm_phase_2 = hm_rocksmash_run_scr;
		return true;
	}
	return false;
}

// 080C9A78
bool hm_prepare_dig() {
	if (cur_map_can_dig()) {
		hm_phase_1 = hm_add_c3_launch_phase_2;
		hm_phase_2 = hm2_dig;
		return true;
	}
	return false;
}

// 080C9B2C
bool hm_prepare_flash() {
	if (current_mapheader.cave == 1 && !flag_check(2054)) {
		hm_phase_1 = hm_add_c3_launch_phase_2;
		hm_phase_2 = hm2_flash;
		return true;
	}
	return false;
}

// 080CCD84
bool ruin_valley_trigger() {
	return flag_check(739) != 1
		&& saveblock1_mapdata->location.bank == 0x03
		&& saveblock1_mapdata->location.map  == 0x3D
		&& saveblock1_mapdata->camera_position.x == 24
		&& saveblock1_mapdata->camera_position.y == 25
		&& player_get_direction__sp1AA(1638424) == 2;
}

// 080D07EC
signed int hm_prepare_strength() {
	if ( walkrun_bitfield_and_r0(8) << 24 || npc_before_player_of_type(97) != 1 ) {
		return false;
	} else {
		LOBYTE(var_800D) = brm_get_pokemon_selection();
		var_800D = var_800D;
		hm_phase_1 = hm_add_c3_launch_phase_2;
		hm_phase_2 = hm_strenght_run_scr;
		return true;
	}
}

// 080DE0C8
bool hm_prepare_sweet_scent() 	{
	hm_phase_1 = hm_add_c3_launch_phase_2;
	hm_phase_2 = hm2_sweet_scent;
	return true;
}

// 080E5684
bool hm_prepare_dive_probably() {
	struct pokemon *pkmn = &party_player[brm_get_pokemon_selection()];

	u16   total_hp = pokemon_getattr(pkmn, req_total_hp);
	u16 current_hp = pokemon_getattr(pkmn, req_current_hp);
	return current_hp > (total / 5);
}

// 080F66F0
bool hm_prepare_teleport() {
	if (is_light_level_1_2_3_or_6__opensky(current_mapheader.light)) {
		hm_phase_1 = hm_add_c3_launch_phase_2;
		hm_phase_2 = hm_teleport_run_dp02scr;
		return true;
	}
	return false;
}

// 08124998
signed int hm_prepare_surf()
{
	coords16 target;

	player_get_pos_to_plus_one_step_in_direction_player_is_facing(
		&target.x,
		&target.y);

	u8 role = cur_mapdata_block_role_at(v2, *(&v2 + 1));
	if (sub_8059CC8(role))
		return false;
	if (!party_has_pokemon_with_surf())
		return false;
	if (!sub_805C8B0())
		return false;
	hm_phase_1 = hm_add_c3_launch_phase_2;
	hm_phase_2 = hm_surf_run_dp02scr;
	return true;
}

// 08124A8C
bool hm_prepare_fly() {
	return is_light_level_1_2_3_or_6__opensky(current_mapheader.light);
}

// 08124AF8
bool hm_prepare_waterfall() {
	coords16 target;

	player_get_pos_to_plus_one_step_in_direction_player_is_facing(
		&target.x,
		&target.y);

	u8 role = cur_mapdata_block_role_at(target.x, target.y);

	if (!is_tile_x13_waterfall(role))
		return false;

	if (!sub_805C88C())
		return false;

	hm_phase_1 = hm_add_c3_launch_phase_2;
	hm_phase_2 = sub_8124ADC;
	return true;
}

// 0845A788
struct hm_overworld {
	bool (*prepare)(void);
	u32 number;
} hm_overworld_table[] = {
	{hm_prepare_flash, 0xD},
	{hm_prepare_cut, 7},
	{hm_prepare_fly, 0xD},
	{hm_prepare_strength, 0xD},
	{hm_prepare_surf, 8},
	{hm_prepare_rocksmash, 0xD},
	{hm_prepare_waterfall, 0xD},
	{hm_prepare_teleport, 0xD},
	{hm_prepare_dig, 0xD},
	{hm_prepare_dive_probably, 0x10},
	{hm_prepare_dive_probably, 0x10},
	{hm_prepare_sweetscent_maybe, 0xD}
};
