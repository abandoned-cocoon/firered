#pragma once

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

struct br00 {
	u32 field_0[40];
};

struct br04 {
	u32 field_0;
	u32 field_4;
	u32 field_8;
	u32 field_C;
};

struct br08 {
	u32 stack_data[8];
	u32 stack_height;
};

struct br0C {
	u32 stack_data[8];
	u32 stack_height;
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

struct br1C {
	int stack_data[8];
	int stack_height;
};

struct brb; // v

struct b_resources_t {
	struct br00 *field_0;
	struct br04 *field_4;
	struct br08 *_8_move_script_stack;
	struct br0C *_C_bc_stack;
	struct br10 *field_10;
	struct trainer_ai *tai_state;
	struct br18 *history;
	struct br1C *_1C_move_consider_stack;
};


extern struct b_resources_t *b_resources;
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
