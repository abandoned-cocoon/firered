#pragma once
#include "types.h"
#include "pokemon.h"

// these are not confirmed
// enum enum_battle_type in db
#define BATTLE_DOUBLE 0x1
#define BATTLE_LINK 0x2
#define BATTLE_WILD 0x4
#define BATTLE_TRAINER 0x8
#define BATTLE_OAK_COMMENTS 0x10
#define BATTLE_x20 0x20
#define BATTLE_TRAINER2 0x40
#define BATTLE_SAFARI 0x80
#define BATTLE_TRAINER_TOWER_MAYBE 0x100
#define BATTLE_OLD_MAN 0x200
#define BATTLE_ROAMING 0x400
#define BATTLE_TRAINER_TOWER_MAYBE_2 0x800
#define BATTLE_x1000 0x1000
#define BATTLE_x2000 0x2000
#define BATTLE_x4000 0x4000
#define BATTLE_GHOST 0x8000
#define BATTLE_POKE_DUDE 0x10000
#define BATTLE_x20000 0x20000
#define BATTLE_IN_SECRET_BASE_PROBABLY 0x40000
#define BATTLE_TRAINER_TOWER_MAYBE_3 0x80000

#define B_S2_CONFUSION_MASK 0x7
#define B_S2_FLINCHING 0x8
#define B_S2_UPROAR 0x70
#define B_S2_BIDE 0xF00
#define B_S2_MULTI_TURN_FLAG 0x1000
#define B_S2_MULTI_TURN_COUNTER 0xE000
#define B_S2_ATTRACTION_INFATUATION 0xF0000
#define B_S2_FOCUS_CRITICAL 0x100000
#define B_S2_TRANSFORMED 0x200000
#define B_S2_NEEDS_RECHARGE 0x400000
#define B_S2_ATK_UP_IF_HIT 0x800000
#define B_S2_SUBSTITUTE 0x1000000
#define B_S2_DESTINY_BOND 0x2000000
#define B_S2_PREVENT_ESCAPE 0x4000000
#define B_S2_NIGHTMARES 0x8000000
#define B_S2_CURSED 0x10000000
#define B_S2_NEXT_ATTACK_WILL_HIT 0x20000000
#define B_S2_DEFENSE_CURL_USED 0x40000000

#define B_F_DESTINY_BOND                0x00000040
#define B_F_SKIP_MOVE_ANIMATIONS        0x00000080
#define B_F_INHIBIT_X_USED_Y_MESSAGE    0x00000200
#define B_F_COMPLETED_X_USED_Y_MESSAGE  0x00000400
#define B_F_PP_DEDUCTION_OUTSTANDING    0x00000800
#define B_F_DEFLECT_DESTINY_BOND        0x01000000 // among other things

#define B_AE_MISSED       0x01
#define B_AE_NOT_AFFECTED 0x08
#define B_AE_FAILED       0x20

#define B_PR1_DO_STRUGGLE         0x04
#define B_PR1_FIFTY_PERC_MORE_DMG 0x08
#define B_PR1_MAGIC_COAT          0x10

// not used anywhere because changing this doesn't trivially allow more pokemon to fight
#define NUM_BATTLE_SIDES 4

enum bd_status_1 {
	sleep_turns = 0x7,
	poison_ = 0x8,
	burn_ = 0x10,
	freeze_ = 0x20,
	paralyze_ = 0x40,
	badly_poisoned_ = 0x80,
};

#define STAT_HP      0
#define STAT_ATK     1
#define STAT_DEF     2
#define STAT_SPD     3
#define STAT_SPATK   4
#define STAT_SPDEF   5
#define STAT_ACC     6
#define STAT_EVASION 7

struct battle_data {
	u16  species;
	u16  stats[5];
	u16  move_ids[4];
	u32  ivs;
	u8   stat_buffs[8]; // hp/atk/def/spd/spatk/spdef/accuracy/evasion
		// min 0 -- 6 -- 12 max
	u8   ability_id;
	u8   type1;
	u8   type2;
	u8   padding;
	u8   current_pp[4];
	u16  current_hp;
	u8   level;
	u8   happiness;
	u16  max_hp;
	u16  held_item;
	char name[11];
	u8   field_3B;
	char trainer_name[8];
	u32  padding3;
	u32  pid;
	u32  status1;
		// 0x00000007  -> Sleep (up to 7 turns)
		// 0x00000008  -> poison
		// 0x00000010  -> burn
		// 0x00000020  -> freeze
		// 0x00000040  -> paralyze
		// 0x00000080  -> badly poisoned
	u32  status2;
		// see http://www.pokecommunity.com/archive/index.php/t-215644.html

		// 0x00000007 -> confusion: 7 turns max, decreases each turn before use. snaps out when 1->0)
		// 0x00000008 -> flinching
		// 0x00000070 -> uproar
		// 0x00000f00 -> charging energy (bide) when 1, spit all energy. 3 total, as each flag is a different state
		 
		// 0x00001000 -> multiple attack over several turns.
		// This one is very important, as it is used by any attack that has two turns.
		// From Rollout to dig, solarbeam to petal dance, any attack that forces the user to use it a second turn (skip attack selection) sets this flag.
		// The attack must then know how to deal with it, and unset it when time is up.
		 
		// 0x0000e000 -> multiple attack (warp) target by up to 7 turns.
		// This is the target's side, so its the timer for the one suffering from the attack. 
		// 0x000f0000 -> attraction, infatuation.
		// The bit indicates the slot of who is the target. 
		// 1 is your pokemon,2 is opponent, 3rd bit is your second out pokemon, 4th is your opponent 2nd pokemon out.
		// ex: 0x000a0000 means the pokemon is in love with both of your opponent pokemon. The one sho
	u32  otid;
};

struct pokemon_summary {
	u16 species;
	u16 held_item;
	u8 nick[11];
	u8 level;
	u16 current_hp;
	u16 max_hp;
	u32 status_ailment;
	u32 pokemon_id_or_personality;
	u8 gender;
	u8 byte1D;
	u8 byte1E;
	u8 byte1F;
};

static_assert_sizeof(struct pokemon_summary, 0x20);

struct dp16 { // distinct from protect__struct
	u8 flags; // __m_____ m = magic_coat
	u8 gap_1[3];
	u32 field_4;
	u32 some_damage_amount__phys_damage_only_maybe;
	u32 field_C;
	u8 field_10;
	u8 side_11;
	u16 field_12;
};

struct disable {
	u32 field_0;
	u16 disabled_move;
	u16 encore_move;
	u8 protect_or_endure_is_active;
	u8 stockpile_counter;
	u8 dp16_default_field4;
	u8 disable_timer;
	u8 field_C;
	u8 field_D;
	u8 encore_timer;
	u8 perish_timer;
	u8 fury_cutter_counter;
	u8 rollout_timer;
	u8 charge_timer;
	u8 taunt_timer;
	u8 field_14;
	u8 always_hit;
	u8 battle_start;
	u8 field_17;
	u8 field_18;
	u8 field_19;
	u16 field_1A;
};

struct protect_struct {
	u8 flags; // uuu123uu 1_magic_coat 2_50perc_more_damage 3_do_struggle;
	u8 protect_flags;
	u16 field_2;
	u16 phys_damage_received;
	u16 spec_damage_received;
	u32 field_8;
	u8 field_C;
	u8 side_D;
	u16 field_E;
};

struct berry_info {
	u8 field_0[6];
	u8 padding_6;
	u8 item_x12;
	u8 field_8[17];
	u8 field_19;
	u8 item_quality;
	u8 field_1B;
};

struct dp08_d3 {
	char data[3];
};

struct dp08 {
	u8 padding_0[16];
	u8 party_index_10;
	u8 field_11;
	u8 field_12;
	enum pokemon_type move_to_execute_type_override;
	u8 padding_14[53];
	u8 byte_49;
	u8 byte_4A;
	u8 bs_mode_pbs_index;
	u8 padding_4C[4];
	u8 byte_50;
	u8 gap_51[1];
	u8 party_index_52;
	u8 field_53;
	u8 padding_54[4];
	u8 team_id_by_side_58[4];
	u8 team_id_by_side_5C[4];
	struct dp08_d3 blobs_60[4];
	u8 padding_6C[12];
	u8 byte_78;
	u8 byte_79_randparam;
	u8 byte_7A_randparam;
	u8 byte_7B_randparam;
	u8 byte_7C;
	u8 objid_7D;
	u8 objid_7E;
	u8 byte_7F;
	u8 moveset_indices_pbs[11];
	u8 byte_8B;
	u8 byte_8C;
	u8 byte_8D;
	u8 type_8E;
	u8 byte_8F;
	u8 field_90;
	u8 side_bits_91;
	u8 switchout_index_maybe[2];
	u8 byte_94;
	u8 byte_95;
	u8 index_96_pokedude_script_id;
	u8 subindex_97_rpc_replace_pc;
	u8 padding_98[16];
	u16 hp_by_party_A8[2];
	u8 byte_AC;
	u8 byte_AD;
	u8 battle_type;
	u8 pading_AF;
	void (*callback2_backup_backup)(void);
	u8 field_B4;
	u8 multiplayer_id_B5;
	u8 field_B6;
	u8 field_B7;
	u16 items_pbs[4];
	u8 item_usage[24];
	u8 byte_D8;
	u8 side_D9;
	u8 byte_DA;
	u8 field_DB;
	u8 padding_DC[3];
	u8 field_DF;
	u8 padding_E0[32];
	u8 pading_100[132];
	struct pokemon_summary party3summary[3];
	u8 field_1E4[28];
};


struct br00 {
	u32 field_0[40];
};

struct br04 {
	u32 field_0;
	u32 field_4;
	u32 field_8;
	u32 field_C;
};

struct br_stack {
	u32 values[8];
	u8 height;
};

struct br10 {
	u16 field_0;
	u16 field_2;
	u16 field_4;
	u16 field_6;
	u16 field_8;
	u16 field_A;
};

struct br18 {
	u16 movehistory_1[8];
	u16 movehistory_2[8];
	u8 abilites_used[2];
	u8 item_x12[2];
	u16 t24[4];
	u8 t24count;
	u8 field_2D;
	u8 field_2E;
	u8 field_2F;
};

struct trainer_ai;

struct b_resources_t {
	struct br00 *field_0;
	struct br04 *field_4;
	struct br_stack *move_script_stack;
	struct br_stack *bc_stack;
	struct br10 *field_10;
	struct trainer_ai *tai_state;
	struct br18 *history;
	struct br_stack *_1C_move_consider_stack;
};

#define b_bitfield b_buffers_awaiting_execution_bitfield

extern u8 b_rpc_scratchpad[0x100]; // 02022874
extern u16 b_lcd_BG0HOFS; // 02022974
extern u16 b_lcd_BG0VOFS; // 02022976
extern u16 b_lcd_BG1HOFS; // 02022978
extern u16 b_lcd_BG1VOFS; // 0202297A
extern u16 b_lcd_BG2HOFS; // 0202297C
extern u16 b_lcd_BG2VOFS; // 0202297E
extern u16 b_lcd_BG3HOFS; // 02022980
extern u16 b_lcd_BG3VOFS; // 02022982
extern u16 b_lcd_WIN0H;   // 02022984
extern u16 b_lcd_WIN0V;   // 02022986
extern u16 b_lcd_WIN1H;   // 02022988
extern u16 b_lcd_WIN1V;   // 0202298A
extern u8 string_buffer_maybe[300]; // 0202298C
extern u8 b_outcome_A;    // 02022AB8
extern u8  byte_02022AB9; // 02022AB9
extern u16 word_02022ABA; // 02022ABA
extern u8  byte_02022ABC; // 02022ABC
extern u8  byte_02022ABD; // 02022ABD
extern u8  byte_02022ABE; // 02022ABE
extern u8  byte_02022ABF; // 02022ABF
extern u8  byte_02022AB0; // 02022AB0
// 02022AB1:02022AC8 ?
extern u8 b_outcome_B;    // 02022AC8
extern u32 dword_02022AE8;
// 02022AEC:02022B4C ?

#define battle_type_flags b_type_flags

extern u32 b_type_flags; // 02022B4C
extern u8 idx_for_battle_screen_elements_by_env; // 02022B50
extern u32 dword_2022B54; // 02022B54
extern struct pokemon_summary ps_2022B58[3]; // 02022B58
extern void *b_tilemap_C; // 02022BB8
extern void *b_tilemap_D; // 02022BBC
extern u32 dword_2022BC0; // 02022BC0
extern u8  b_buffer_A[4][0x200]; // 02022BC4
extern u8  b_buffer_B[4][0x200]; // 020233C4
extern u8  b_active_side; // 02023BC4
extern u32 b_buffers_awaiting_execution_bitfield; // 02023BC8
	// Controls execution of b_buffer_A.
	// Four bits for four battle sides.
	// = (b_type & wireless) ? (value>>0x1C) : (value)
extern u8  b_num_active_sides; // 02023BCC
extern u16 b_pokemon_team_id_by_side[4]; // 02023BCE
extern u8  b_side_status[4]; // 02023BD6
	// bit 0x01: is enemy pokemon
	// val 0xff: is dead
extern u8  bs_mode_pbs[4]; // 02023BDA
extern u8  b_attackers_in_order[4]; // 02023BDE
extern u8  bs_mode_pbs_index; // 02023BE2
extern u8  bs_mode; // 02023BE3
extern struct battle_data b_data[4]; // 02023BE4
extern u8 b_oamid[4]; // 02023D44
	// 02023D44 battle_side_objid_P_and_priv5_for_dp11b3 in db
extern u8 b_moveset_index; // 02023D48
extern u8 b_moveset_index_; // 02023D49
	// never read
extern u16 b_move_to_execute_A; // 02023D4A
extern u16 b_move_to_execute_B; // 02023D4C
extern u16 b_move_to_execute_C; // 02023D4E
extern u32 b_move_damage; // 02023D50
	// *0x02023F50 and *0x02024020 are read in the function called
	// by atk05_base_damage_calculator and written to here.
extern u32 b_old_hp; // 02023D54
extern u32 b_damages_pbs[4]; // 02023D58
extern u16 b_2023D68_held_item; // 02023D68
extern u8 b_last_copied_ability; // 02023D6A
extern u8 b_attacker; // 02023D6B
extern u8 b_defender; // 02023D6C
extern u8 b_attacker_partner; // 02023D6D
extern u8 b_defender_partner; // 02023D6E
extern u8 b_2023D6F_side; // 02023D6F
extern u8 b_bits_pbs_2023D70; // 02023D70
extern u8 b_critical_multiplier; // 02023D71
extern u8 b_move_loop_counter; // 02023D72
// ?
#define b_move_cursor b_movescr_cursor // TODO
extern u8 *b_movescr_cursor; // 02023D74
// ?
extern u8 battle_top_menu_chosen_item; // 02023D7C
	// 0xFF = show menu
	// 0..3 = fight, bag, pokemon, run
extern u8 byte_2023D7D; // 02023D7C
extern u8 battle_top_menu_chosen_item; // 02023D7C
extern u8 byte_2023D7D; // 02023D7D
extern u32 dword_2023D80[4]; // 02023D80
extern u16 move_to_execute_B_pbs_2[4]; // 02023D90
extern u16 move_to_execute_B_pbs[4]; // 02023D98
extern u16 word2023DA0_pbs[4]; // 02023DA0
extern u16 word_2023DA8[4]; // 02023DA8
extern u16 move_to_execute_A_pbs[4]; // 02023DB0
extern u16 word_2023DB8[4]; // 02023DB8
extern u8 byte_2023DC0[4]; // 02023DC0
extern u16 moveid_2023DC4_pbs; // 02023DC4
extern u8 b_attack_effectivity; // 02023DCC
extern u32 b_features_bitfield; // 02023DD0
extern u32 pbs_byte_2023DD4; // 02023DD4
extern u8 b_defender_pbs[4]; // 02023DD8
extern u8 byte_2023DDC; // 02023DDC
extern u16 b_flags4_per_side_status[4]; // 02023DDE
extern u8 unk_2023DE4; // 02023DE4
extern u8 byte_2023DEC; // 02023DEC
extern u8 byte_2023DF8; // 02023DF8
extern u32 b_status3_bits_pbs[4]; // 02023DFC
extern struct disable b_disable_data_pbs[4]; // 02023E0C
extern u16 atk39_counter; // 02023E7C
extern u16 money_to_give; // 02023E7E
extern u16 word_2023E80; // 02023E80
extern u8 b_preparation_phase; // 02023E82
extern u8 b_preparation_sub_phase; // 02023E83
extern u8 byte_2023E84; // 02023E84
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

#define b_weather weather

extern u8 effect_to_apply; // 02023E85
extern u8 bc_menu_quit_phase; // 02023E86
extern u8 byte_2023E87; // 02023E87
extern u8 battle_message_to_show_; // 02023E88
extern u8 b_buffers_awaiting_execution_bitfield__copied_after_displaying_x_used_y_message; // 02023E89
extern u8 bc_bs_followup_bc_func_index; // 02023E8A
extern struct protect_struct protect_structs[4]; // 02023E8C
extern struct dp16 dp16_array[4]; // 02023ECC
extern u16 b_weather; // 02023F1C
extern u8 byte_2023F44[4]; // 02023F44
extern u8 unk_2023F48; // 02023F48
extern u8 byte_2023F49[3]; // 02023F49
extern u16 word_2023F4C; // 02023F4C
extern u8 byte_2023F4E; // 02023F4E
extern u8 byte_2023F4F; // 02023F4F
extern u16 b_move_power_override; // 02023F50
extern u16 word_2023F52; // 02023F52
extern struct berry_info sav1_copied_berries_pbs[4]; // 02023F54
extern u16 word_02023F4C; // 02023F4C
extern u8 byte_2023FC4; // 02023FC4 struct {
extern u8 byte_2023FC5; // 02023FC5
extern u8 byte_2023FC6; // 02023FC6
extern u8 byte_2023FC7; // 02023FC7
extern u8 unk_2023FC8; // 02023FC8
extern u8 byte_2023FCC; // 02023FCC
extern u8 byte_2023FCD; // 02023FCD
extern u8 byte_2023FCE; // 02023FCE
extern u8 byte_2023FCF; // 02023FCF
extern u8 byte_2023FD0; // 02023FD0
extern u8 byte_2023FD1; // 02023FD1
extern u8 b_dmg_multiplier; // 02023FD2
extern u8 unk_2023FD3; // 02023FD3
extern u8 byte_2023FD4; // 02023FD4
extern u8 byte_2023FD5; // 02023FD5
extern u8 byte_2023FD8; // 02023FD8
extern u8 byte_2023FD9; // 02023FD9
extern u8 byte_2023FDA; // 02023FDA
extern u8 b_side_unknown; // 02023FDB }?
extern u8 byte_2023FDC; // 02023FDC
extern u8 byte_2023FDD; // 02023FDD
extern u8 stat_modification_spec; // 02023FDE
extern u8 byte_2023FDF; // 02023FDF
extern u8 b_end_stage; // 02023FE0
extern u8 byte_2023FE1; // 02023FE1
extern u8 byte_2023FE2; // 02023FE2
extern u8 byte_2023FE3; // 02023FE3
extern u8 unk_2023FE7; // 02023FE7
extern struct dp08 *b_dp08_ptr; // 02023FE8
extern u32 b_tilemap_A; // 02023FEC
extern u32 b_tilemap_B; // 02023FF0
extern struct b_resources_t *b_resources; // 02023FF4
extern u8 byte_2023FF8[4]; // 02023FF8
extern u32 move_index_chosen_per_side; // 02023FFC
extern u8 taskids_02024000[4]; // 02024000
extern u8 byte_2024004; // 02024004
extern u8 bx_busy; // 02024005
extern u32 pokemon_personality_overrides_pbs_maybe[4]; // 02024008
extern struct dp11 *dp11_; // 02024018
extern u32 ptr_plus_0x14_uns_table_pokemon_copy; // 0202401C
	// Also see "uns_table_pokemon" and "init_uns_table_pokemon_copy"
extern u16 b_cur_move_power; // 02024020
extern u8 unk_2024022; // 02024022
extern u8 unk_2024024[4]; // 02024024
extern u8 unk_2024027; // 02024027
extern u8 byte_2024028; // 02024028
// end (continue in pokemon.h)

extern void (*b_callback1_backup)(void); // 03004F80
extern void (*bc)(void); // 03004F84
extern string11 b_pokemon_name_1; // 03004F98
extern u8       b_turn_counter; // 03004FA3
extern string11 b_pokemon_name_2; // 03004FA4
extern void (*bx[4])(); // 03004FE0
extern u8 b_selected_side; // 03004FF4

extern void (*bt1_cmds[])(void);
extern void (*bt2_cmds[])(void);
extern void (*bt3_cmds[])(void);
extern void (*bt4_cmds[])(void);
extern void (*bt5_cmds[])(void);
extern void (*bt6_cmds[])(void);
extern void (*bt7_cmds[])(void);

void b_setup_bx();
void b_setup_bx_local();
void bc_clear_state();
void c1_battle_exec_bc_and_bx();
void bc_8012FAC();
void bc_load_battlefield();
void bc_read_battle_data_from_buffer_B();
void bc_send_hp_and_status_ailments();

void bx1_blink_for_damage();
void bx1_wait();
void bx3_wait();
void bx1_exec_buffer_A_ch0();
void bx2_exec_buffer_A_ch0();
void bx3_exec_buffer_A_ch0();
void bx4_exec_buffer_A_ch0();
void bx5_exec_buffer_A_ch0();
void bx6_exec_buffer_A_ch0();
void bx7_exec_buffer_A_ch0();
void bx1_goto_buffer_A();
void bx2_goto_buffer_A();
void bx3_goto_buffer_A();
void bx4_goto_buffer_A();
void bx5_goto_buffer_A();
void bx6_goto_buffer_A();
void bx7_goto_buffer_A();
void bx1_button_a();
void bx3_oak_inflicting_damage();
void bx3_oak_lowering_the_foes_stats();
void bx3_oak_there_is_no_running_away();
void bx3_oak_hm_excellent();
void bx3_oak_how_disappointing();
void bx3_text(const char *message, u8 _);
