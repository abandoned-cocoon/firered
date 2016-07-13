#pragma once
#include "types.h"

#define TAI_PHASE_NEXT_MOVE     0
#define TAI_PHASE_EVALUATE_MOVE 1
#define TAI_PHASE_QUIT          2

#define TAI_FLAG_EVAL_COMPLETE 1
#define TAI_FLAG_USE_MOVE_4    2
#define TAI_FLAG_USE_MOVE_5    4
#define TAI_FLAG_QUIT          8

struct trainer_ai {
	u8  phase;
	u8  moveset_index;
	u16 move_id;
	u8  viabilities[4];
	u32 field_8;
	u32 script_usage_mask;
	u8  flags;
	u8  script_id;
	u16 field_12;
	u8  field_14;
	u8  field_15;
	u8  field_16;
	u8  field_17;
	u8  move_damage_multipliers[4];
};

extern void (*tai_move_consideration_cmds[0x5E])();

// 02039A00
extern u8 *tai_cursor;

// 081D9BF4
extern u8 *tai_scripts[32];

u8 tai_pick_move();
void tai_run_script(void);
void b_movehistory_add_defenders_move();
int b_movehistory_clear(int a1);
void b_history__record_ability_usage_of_player(u8 a1, char a2);
void b_history__record_item_x12_of_player(u8 a1, char a2);
void tai_cmd_random_goto__high_param_likely();
void tai_cmd_random_goto__low_param_likely();
void tai_cmd_random_goto__1_in_256_chance();
void tai_cmd_random_goto__255_in_256_chance();
void tai_cmd_viability_score();
void tai_cmd_jump_if_health_percentage_lt();
void tai_cmd_jump_if_health_percentage_ge();
void tai_cmd_jump_if_health_percentage_eq();
void tai_cmd_jump_if_health_percentage_ne();
void tai_cmd_jump_if_any_status1_bit();
void tai_cmd_jump_if_no_status1_bit();
