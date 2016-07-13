#include "battle.h"
#include "battle_rpc.h"
#include "pokemon.h"
#include "vars.h"
#include "u08044.h"

#ifndef NO_RAM
	u8 b_rpc_scratchpad[0x100]; // 02022874
	u16 b_lcd_BG0HOFS; // 02022974
	u16 b_lcd_BG0VOFS; // 02022976
	u16 b_lcd_BG1HOFS; // 02022978
	u16 b_lcd_BG1VOFS; // 0202297A
	u16 b_lcd_BG2HOFS; // 0202297C
	u16 b_lcd_BG2VOFS; // 0202297E
	u16 b_lcd_BG3HOFS; // 02022980
	u16 b_lcd_BG3VOFS; // 02022982
	u16 b_lcd_WIN0H;   // 02022984
	u16 b_lcd_WIN0V;   // 02022986
	u16 b_lcd_WIN1H;   // 02022988
	u16 b_lcd_WIN1V;   // 0202298A
	u8 string_buffer_maybe[300]; // 0202298C
	u8 b_outcome_A;    // 02022AB8
	u8  byte_02022AB9; // 02022AB9
	u16 word_02022ABA; // 02022ABA
	u8  byte_02022ABC; // 02022ABC
	u8  byte_02022ABD; // 02022ABD
	u8  byte_02022ABE; // 02022ABE
	u8  byte_02022ABF; // 02022ABF
	u8  byte_02022AB0; // 02022AB0
	// 02022AB1:02022AC8 ?
	u8 b_outcome_B;    // 02022AC8
	u32 dword_02022AE8;
	// 02022AEC:02022B4C ?
	u32 b_type_flags; // 02022B4C
	u8 idx_for_battle_screen_elements_by_env; // 02022B50
	u32 dword_2022B54; // 02022B54
	struct pokemon_summary ps_2022B58[3]; // 02022B58
	void *b_tilemap_C; // 02022BB8
	void *b_tilemap_D; // 02022BBC
	u32 dword_2022BC0; // 02022BC0
	u8  b_buffer_A[4][0x200]; // 02022BC4
	u8  b_buffer_B[4][0x200]; // 020233C4
	u8  b_active_side; // 02023BC4
	u32 b_buffers_awaiting_execution_bitfield; // 02023BC8
		// Controls execution of b_buffer_A.
		// Four bits for four battle sides.
		// = (b_type & wireless) ? (value>>0x1C) : (value)
	u8  b_num_active_sides; // 02023BCC
	u16 b_pokemon_team_id_by_side[4]; // 02023BCE
	u8  b_side_status[4]; // 02023BD6
		// bit 0x01: is enemy pokemon
		// val 0xff: is dead
	u8  bs_mode_pbs[4]; // 02023BDA
	u8  b_attackers_in_order[4]; // 02023BDE
	u8  bs_mode_pbs_index; // 02023BE2
	u8  bs_mode; // 02023BE3
	struct b_data b_data[4]; // 02023BE4
	u8 b_oamid[4]; // 02023D44
		// 02023D44 battle_side_objid_P_and_priv5_for_dp11b3 in db
	u8 b_moveset_index; // 02023D48
	u8 b_moveset_index_; // 02023D49
		// never read
	u16 b_move_to_execute_A; // 02023D4A
	u16 b_move_to_execute_B; // 02023D4C
	u16 b_move_to_execute_C; // 02023D4E
	u32 b_move_damage; // 02023D50
		// *0x02023F50 and *0x02024020 are read in the function called
		// by atk05_base_damage_calculator and written to here.
	u32 b_old_hp; // 02023D54
	u32 b_damages_pbs[4]; // 02023D58
	u16 b_2023D68_held_item; // 02023D68
	u8 b_last_copied_ability; // 02023D6A
	u8 b_attacker; // 02023D6B
	u8 b_defender; // 02023D6C
	u8 b_attacker_partner; // 02023D6D
	u8 b_defender_partner; // 02023D6E
	u8 b_2023D6F_side; // 02023D6F
	u8 b_bits_pbs_2023D70; // 02023D70
	u8 b_critical_multiplier; // 02023D71
	u8 b_move_loop_counter; // 02023D72
	// ?
	u8 *b_movescr_cursor; // 02023D74
	// ?
	u8 battle_top_menu_chosen_item; // 02023D7C
		// 0xFF = show menu
		// 0..3 = fight, bag, pokemon, run
	u8 byte_2023D7D; // 02023D7C
	u8 battle_top_menu_chosen_item; // 02023D7C
	u8 byte_2023D7D; // 02023D7D
	u32 dword_2023D80[4]; // 02023D80
	u16 move_to_execute_B_pbs_2[4]; // 02023D90
	u16 move_to_execute_B_pbs[4]; // 02023D98
	u16 word2023DA0_pbs[4]; // 02023DA0
	u16 word_2023DA8[4]; // 02023DA8
	u16 move_to_execute_A_pbs[4]; // 02023DB0
	u16 word_2023DB8[4]; // 02023DB8
	u8 byte_2023DC0[4]; // 02023DC0
	u16 moveid_2023DC4_pbs; // 02023DC4
	u8 b_attack_effectivity; // 02023DCC
	u32 b_features_bitfield; // 02023DD0
	u32 pbs_byte_2023DD4; // 02023DD4
	u8 b_defender_pbs[4]; // 02023DD8
	u8 byte_2023DDC; // 02023DDC
	u16 b_flags4_per_side_status[4]; // 02023DDE
	u8 unk_2023DE4; // 02023DE4
	u8 byte_2023DEC; // 02023DEC
	u8 byte_2023DF8; // 02023DF8
	u32 b_status3_bits_pbs[4]; // 02023DFC
	struct disable b_disable_data_pbs[4]; // 02023E0C
	u16 atk39_counter; // 02023E7C
	u16 money_to_give; // 02023E7E
	u16 word_2023E80; // 02023E80
	u8 b_preparation_phase; // 02023E82 // TODO: rename to b_…
	u8 b_preparation_sub_phase; // 02023E83 // TODO: rename to b_…
	u8 byte_2023E84; // 02023E84
		// bit  x___ ____ see 08021170
		// bit  _x__ ____ see 0801FF20
		// bit  __xx xxxx see below
		// (JPAN)
		// 0x1 -> Sleep
		// 0x2 -> poison (08020656)
		// 0x3 -> burn
		// 0x4 -> freeze
		// 0x5 -> paralyze
		// 0x6 -> badly poison
		// 0x07 -> Confusion
		// 0x08 -> Flinch
		// 0x09 -> Tri Attack
		// 0x0a -> uproar start
		// 0x0b -> payday
		// 0x0c -> set Repeating attack (0x1000 flag)
		// 0x0d -> wrap
		// 0x0e -> recoil
		// 0x0f to 0x15 -> raise stat by 1 (same order as battle data)
		// 0x16 to 0x1c -> lower stat by 1
		// 0x1d -> recharge needed set
		// 0x1e -> rage set
		// 0x1f -> steal Item
		// 0x20 -> prevent escape
		// 0x21 -> nightmare
		// 0x22 -> ancientpower raise
		// 0x23 -> rapid spin
		// 0x24 -> remove prlz if paralized
		// 0x25 -> superpower
		// 0x26 -> recoil and paralyze

		// <0x27 - move_scripts_effects

		// 0x27 to 0x2d -> raise stat by 2
		// 0x2e to 0x34 -> lower stat by 2
		// 0x35 -> ??
		// 0x36 -> knock off
		// 0x37 to 0x39 --> None
		// 0x3a -> reduce 2 sp Attack (overheat)

		// 0xe6 -> volt tackle

	u8 effect_to_apply; // 02023E85
	u8 bc_menu_quit_phase; // 02023E86
	u8 byte_2023E87; // 02023E87
	u8 battle_message_to_show_; // 02023E88
	u8 b_buffers_awaiting_execution_bitfield__copied_after_displaying_x_used_y_message; // 02023E89
	u8 bc_bs_followup_bc_func_index; // 02023E8A
	struct protect_struct protect_structs[4]; // 02023E8C
	struct dp16 dp16_array[4]; // 02023ECC
	u16 b_weather; // 02023F1C
	u8 byte_2023F44[4]; // 02023F44
	u8 unk_2023F48; // 02023F48
	u8 byte_2023F49[3]; // 02023F49
	u16 word_2023F4C; // 02023F4C
	u8 byte_2023F4E; // 02023F4E
	u8 byte_2023F4F; // 02023F4F
	u16 b_move_power_override; // 02023F50
	u16 word_2023F52; // 02023F52
	struct berry_info sav1_copied_berries_pbs[4]; // 02023F54
	u16 word_02023F4C; // 02023F4C
	u8 byte_2023FC4; // 02023FC4
	u8 byte_2023FC5; // 02023FC5
	u8 byte_2023FC6; // 02023FC6
	u8 byte_2023FC7; // 02023FC7
	u8 unk_2023FC8; // 02023FC8
	u8 byte_2023FCC; // 02023FCC
	u8 byte_2023FCD; // 02023FCD
	u8 byte_2023FCE; // 02023FCE
	u8 byte_2023FCF; // 02023FCF
	u8 byte_2023FD0; // 02023FD0
	u8 byte_2023FD1; // 02023FD1
	u8 b_dmg_multiplier; // 02023FD2
	u8 unk_2023FD3; // 02023FD3
	u8 byte_2023FD4; // 02023FD4
	u8 byte_2023FD5; // 02023FD5
	u8 byte_2023FD8; // 02023FD8
	u8 byte_2023FD9; // 02023FD9
	u8 byte_2023FDA; // 02023FDA
	u8 b_side_unknown; // 02023FDB
	u8 byte_2023FDC; // 02023FDC
	u8 byte_2023FDD; // 02023FDD
	u8 stat_modification_spec; // 02023FDE
	u8 byte_2023FDF; // 02023FDF
	u8 b_end_stage; // 02023FE0
	u8 byte_2023FE1; // 02023FE1
	u8 byte_2023FE2; // 02023FE2
	u8 byte_2023FE3; // 02023FE3
	u8 unk_2023FE7; // 02023FE7
	struct dp08 *b_dp08_ptr; // 02023FE8
	u32 b_tilemap_A; // 02023FEC
	u32 b_tilemap_B; // 02023FF0
	struct b_resources_t *b_resources; // 02023FF4
	u8 byte_2023FF8[4]; // 02023FF8
	u32 move_index_chosen_per_side; // 02023FFC
	u8 taskids_02024000[4]; // 02024000
	u8 byte_2024004; // 02024004
	u8 bx_busy; // 02024005
	u32 pokemon_personality_overrides_pbs_maybe[4]; // 02024008
	struct dp11 *dp11_; // 02024018
	u32 ptr_plus_0x14_uns_table_pokemon_copy; // 0202401C
		// Also see "uns_table_pokemon" and "init_uns_table_pokemon_copy"
	u16 b_cur_move_power; // 02024020
	u8 unk_2024022; // 02024022
	u8 unk_2024024[4]; // 02024024
	u8 unk_2024027; // 02024027
	u8 byte_2024028; // 02024028

	void (*b_callback1_backup)(void); // 03004F80
	void (*bc)(void); // 03004F84
	string11 b_pokemon_name_1; // 03004F98
	u8       b_turn_counter; // 03004FA3
	string11 b_pokemon_name_2; // 03004FA4
	void (*bx[4])(); // 03004FE0
	u8 b_selected_side; // 03004FF4

	void (*bt1_cmds[])(void);
	void (*bt2_cmds[])(void);
	void (*bt3_cmds[])(void);
	void (*bt4_cmds[])(void);
	void (*bt5_cmds[])(void);
	void (*bt6_cmds[])(void);
	void (*bt7_cmds[])(void);
#endif

// 0800D30C
void b_setup_bx() {
	if (battle_type_flags & BATTLE_LINK)
		b_setup_bx_link();
	else
		b_setup_bx_local();

	sub_800D768();

	if (battle_type_flags & BATTLE_TRAINER2)
		for (u32 i=0; i<b_num_active_sides; i++)
			sub_8127DA8(i, 0);

}

// 0800D364
void b_setup_bx_local() {
	if (battle_type_flags & BATTLE_DOUBLE) {
		bc = &bc_clear_state;

		b_side_status[0] = 0;
		b_side_status[1] = 1;
		b_side_status[2] = 2;
		b_side_status[3] = 3;
		b_num_active_sides = 4;

		if (battle_type_flags & BATTLE_POKE_DUDE) {
			bx[0] = &bx5_goto_buffer_A;
			bx[1] = &bx5_goto_buffer_A;
			bx[2] = &bx5_goto_buffer_A;
			bx[3] = &bx5_goto_buffer_A;
		} else {
			bx[0] = &bx1_goto_buffer_A;
			bx[1] = &bx2_goto_buffer_A;
			bx[2] = &bx1_goto_buffer_A;
			bx[3] = &bx2_goto_buffer_A;
		}

	} else {
		bc = &bc_clear_state;

		b_side_status[0] = 0;
		b_side_status[1] = 1;
		b_num_active_sides = 2;

		if (battle_type_flags & BATTLE_POKE_DUDE) {
			bx[0] = &bx5_goto_buffer_A;
			bx[1] = &bx5_goto_buffer_A;
		} else if (battle_type_flags & BATTLE_SAFARI) {
			bx[0] = &bx6_goto_buffer_A;
			bx[1] = &bx2_goto_buffer_A;
		} else if (battle_type_flags & (BATTLE_OAK_COMMENTS|0x200)) {
			bx[0] = &bx3_goto_buffer_A;
			bx[1] = &bx2_goto_buffer_A;
		} else {
			bx[0] = &bx1_goto_buffer_A;
			bx[1] = &bx2_goto_buffer_A;
		}
	}
}
// 080123C0
void bc_clear_state() {
	b_clear_state();
	b_preparation_sub_phase = 0;
	bc = &bc_8012FAC;
}

// 080123E4
void c1_battle_exec_bc_and_bx() {
	(*bc)();
	for (b_active_side = 0; b_active_side < b_num_active_sides; b_active_side++) {
		(*bx[b_active_side])();
	}
}

// 08012FAC
void bc_8012FAC() {
	if (b_preparation_phase == 0) {

		b_active_side = 0;
		bt_00_emit(0, 0, 0);
		b_active_side_mark_buffer_for_execution(b_active_side);

		b_preparation_phase++;

	} else if (b_preparation_phase == 1) {

		if (b_buffers_awaiting_execution_bitfield == 0) {
			b_preparation_sub_phase++;

			if (b_preparation_sub_phase == b_num_active_sides)
				bc = &bc_load_battlefield;
			else
				b_preparation_phase = 0;
		}
	}
}

// 08013020
void bc_load_battlefield() {
	if (b_buffers_awaiting_execution_bitfield == 0) {
		b_active_side = battle_get_side_with_given_state(0);
		bt_2E_emit(0, idx_for_battle_screen_elements_by_env);
		b_active_side_mark_buffer_for_execution(b_active_side);
		bc = &bc_read_battle_data_from_buffer_B;
		b_preparation_phase = 0;
		b_preparation_sub_phase = 0;
	}
}

// 08013070
void bc_read_battle_data_from_buffer_B() {
	const u32 XFLAGS =
		BATTLE_POKE_DUDE |
		BATTLE_GHOST |
		BATTLE_TRAINER_TOWER_MAYBE_2 |
		BATTLE_OLD_MAN |
		BATTLE_LINK |
		0x2000;

	if (b_buffers_awaiting_execution_bitfield == 0) {
		for (b_active_side = 0; b_active_side < b_num_active_sides; b_active_side++) {
			if (battle_type_flags & BATTLE_SAFARI == 0 || battle_side_get_owner(b_active_side) << 24 ) {
				memcpy(&b_data[b_active_side], &b_buffer_B[b_active_side][4], sizeof(struct battle_data));

				b_data[b_active_side].type1 = base_stats[b_data[b_active_side].species].type1;
				b_data[b_active_side].type2 = base_stats[b_data[b_active_side].species].type2;
				b_data[b_active_side].ability_id =
					b_copy_species_ability(
						b_data[b_active_side].species,
						(unsigned int)BYTE3(b_data[b_active_side].ivs) >> 7);
				*(u16 *)((char *)b_dp08_ptr->hp_by_party_A8 + (2 * battle_side_get_owner(b_active_side) & 0x1FF)) = b_data[b_active_side].current_hp;
				int v4 = 0;
				do
					b_data[b_active_side].stat_buffs[v4++] = 6;
				while (v4 <= 7 );
				b_data[b_active_side].status2 = 0;
			} else {
				memset(&b_data[b_active_side], 0, sizeof(struct battle_data));
			}

			if (battle_get_per_side_status(b_active_side) == 0) {
				bt_07_emit(0);
				b_active_side_mark_buffer_for_execution(b_active_side);
			}

			u16 index = species_to_pokedex_index(b_data[b_active_side].species);

			if (battle_type_flags & BATTLE_TRAINER) {
				if (battle_get_per_side_status(b_active_side) == 1) {
					bt_07_emit(0);
					b_active_side_mark_buffer_for_execution(b_active_side);
				}
				if (battle_side_get_owner(b_active_side) == 1 && !(battle_type_flags & XFLAGS) ) {
					//u16 index = species_to_pokedex_index(b_data[b_active_side].species);
					dex_flag_08044898(index, 2, b_data[b_active_side].pid);
				}

			} else if (battle_side_get_owner(b_active_side) == 1) {

				const u32 YFLAGS =
					BATTLE_GHOST |
					0x2000;

				if ((battle_type_flags & YFLAGS) ? (battle_type_flags & YFLAGS != BATTLE_GHOST) : ((battle_type_flags & XFLAGS) == 0)) {
					//u16 index = species_to_pokedex_index(b_data[b_active_side].species);
					dex_flag_08044898(index, 2, b_data[b_active_side].pid);
				}

				bt_04_emit(0);
				b_active_side_mark_buffer_for_execution(b_active_side);
			}

			if (battle_type_flags & BATTLE_TRAINER2) {
				u8 status = battle_get_per_side_status(b_active_side);
				if (status == 2 || status == 3) {
					bt_07_emit(0);
					b_active_side_mark_buffer_for_execution(b_active_side);
				}
			}
			++b_active_side;
		}
		bc = &bc_send_hp_and_status_ailments;
	}
}

// 0801333C
void bc_send_hp_and_status_ailments() {
	// TODO
}


// TODO: Cleanup

// bx1: 080307B4
// bx2: 080361FC
// bx3: 080E8948
// bx4: 080D4730
// bx5: 08156F00
// bx7: 0803ADF0

void bx1_blink_for_damage() {
	u8 oamid = b_oamid[b_active_side];
	struct obj *o = &objects[oamid];
	u8 t = o->priv1;  // time that passed
	if (t >= 0x20) {  // for 20 frames
		if (t%4 == 0) // every fourth frame
			o->bitfield2 ^= OBJ_BIT2_INVISIBLE;
		o->priv1++;
	} else {
		o->priv1 = 0;
		o->bitfield2 &= ~OBJ_BIT2_INVISIBLE;
		bx_busy = false;
		bx1_done();
	}
}

// bx1: 08030778
// bx6: 080DD7F0
void bx1_wait() {
	// TODO
}

// bx3: 080E7970
// bx5: 08156484
void bx3_wait() {
	if (!bx_busy)
		bx3_done();
}

// Seven execs

// 0802E3B4
void bx1_exec_buffer_A_ch0() {
	if ((b_bitfield & (1<<b_active_side)) == 0) return;
	u8 cmd = b_buffer_A[b_active_side][0];
	if (cmd >= 0x38) bx1_done();
	else             bt1_cmds[cmd]();
}

// 08035A98
void bx2_exec_buffer_A_ch0() {
	if ((b_bitfield & (1<<b_active_side)) == 0) return;
	u8 cmd = b_buffer_A[b_active_side][0];
	if (cmd >= 0x38) bx2_done();
	else             bt2_cmds[cmd]();
}

// 080E8A9C
void bx3_exec_buffer_A_ch0() {
	if ((b_bitfield & (1<<b_active_side)) == 0) return;
	u8 cmd = b_buffer_A[b_active_side][0];
	if (cmd >= 0x38) bx3_done();
	else             bt3_cmds[cmd]();
}

// 080D49E8
void bx4_exec_buffer_A_ch0() {
	if ((b_bitfield & (1<<b_active_side)) == 0) return;
	u8 cmd = b_buffer_A[b_active_side][0];
	if (cmd >= 0x38) bx4_done();
	else             bt4_cmds[cmd]();
}

// 081560DC
void bx5_exec_buffer_A_ch0() {
	if ((b_bitfield & (1<<b_active_side)) == 0) return;
	u8 cmd = b_buffer_A[b_active_side][0];
	if (cmd >= 0x38) bx5_done();
	else             bt5_cmds[cmd]();
}

// 080DD554
void bx6_exec_buffer_A_ch0() {
	if ((b_bitfield & (1<<b_active_side)) == 0) return;
	u8 cmd = b_buffer_A[b_active_side][0];
	if (cmd >= 0x38) bx6_done();
	else             bt6_cmds[cmd]();
}

// 0803A684
void bx7_exec_buffer_A_ch0() {
	if ((b_bitfield & (1<<b_active_side)) == 0) return;
	u8 cmd = b_buffer_A[b_active_side][0];
	if (cmd >= 0x38) bx7_done();
	else             bt7_cmds[cmd]();
}

// 0802E314
void bx1_goto_buffer_A() {
	bx[b_active_side] = bx1_exec_buffer_A_ch0;
	
	bx_busy = false;
}

// 08035A7C
void bx2_goto_buffer_A() {
	bx[b_active_side] = bx2_exec_buffer_A_ch0;
}

void bx3_goto_buffer_A() {
	bx[b_active_side] = bx3_exec_buffer_A_ch0;

	b_dp08_ptr->byte_94 = 0;
	b_dp08_ptr->byte_95 = 0;
	b_dp08_ptr->index_96_pokedude_script_id = 0;
	b_dp08_ptr->subindex_97_rpc_replace_pc = 0;
}

void bx4_goto_buffer_A() {
	bx[b_active_side] = bx4_exec_buffer_A_ch0;
}

void bx5_goto_buffer_A() {
	bx[b_active_side] = bx5_exec_buffer_A_ch0;

	b_dp08_ptr->index_96_pokedude_script_id = var_8004;
	b_dp08_ptr->subindex_97_rpc_replace_pc = 0;
}

void bx6_goto_buffer_A() {
	bx[b_active_side] = bx6_exec_buffer_A_ch0;
}

void bx7_goto_buffer_A() {
	bx[b_active_side] = bx7_exec_buffer_A_ch0;
}

// bx1: 08030610
// bx2: 080361E4
// bx3: 080E7918
// bx4: 080D4718
// bx5: 0815623C
// bx6: 080DD760
// bx7: 0803ADD8
void bx1_button_a() {
	if (button_a_pressed(0) == 0)
		bx1_done();
}

// 0803A684
void bx3_oak_inflicting_damage() {
	bx_text("OAK: Inflicting damage on the foe is the key to any battle.", 0x01);
}

// 080E8584
void bx3_oak_lowering_the_foes_stats() {
	bx_text("OAK: Lowering the foe`s stats will put you at an advantage.", 0x40);
}

// 080E8598
void bx3_oak_there_is_no_running_away() {
	bx_text("OAK: No! There`s no running away from a TRAINER POKeMON battle!", 0x01);
}

// 080E85AC
void bx3_oak_hm_excellent() {
	bx_text("OAK: Hm! Excellent!\nIf you win, you earn prize money, ...", 0x40);
}

// 080E85C0
void bx3_oak_how_disappointing() {
	bx_text("OAK: Hm. How disappointing.\nIf you win, you earn prize money, ...", 0x40);
}

// 080E85D4
void bx3_text(const char *message, u8 _) {
	/* ... */
}
