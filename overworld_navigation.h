#pragma once
#include "types.h"
#include "npc.h"

struct walkrun {
	u8  bitfield;
	u8  bike;
	u8  running2;
	u8  running1;
	u8  oamid;
	u8  npcid;
	u8  lock;
	u8  gender;
	u8  xmode;
	u8  field_9;
	u8  field_A;
	u8  field_B;
	u32 field_C;
	u32 field_10;
	u32 field_14;
	u8  field_18;
	u8  field_19;
	u16 field_1A;
	u16 most_recent_override_tile;
};

//* 02037078
extern struct walkrun walkrun;

void player_step_by_keypad(u8 direction, u16 keypad_new, u16 keypad_held);
void sub_805BCEC(u16 x, u16 y, u8 direction);
void player_step(u8 direction, u16 keypad_new, u16 keypad_held);
void player_get_pos_to(u16 x, u16 y);
u8 player_get_direction();
u8 player_get_height();
void script_env_2_apply_keypad_override(u8 *ignored, u16 *keypad_new, u16 *keypad_held);
void sub_805CCD0(u8 npc_id, u8 direction);
void input_process(u8 *d, u16 keypad_new, u16 keypad_held);
bool sub_806CAC8(u8 *d);
void sub_806CD30(u8 *d);
void player_get_pos_to_and_height(struct npc_position *n);
u8 cur_mapdata_block_role_at_player_pos();
bool onpress_a(struct npc_position *n, u8 role, u8 direction);
u8 *onpress_a_get_script(struct npc_position *n, u8 role, u8 direction);
u32 onpress_a_get_script_npc(struct npc_position *n, u8 role, u8 direction);
u32 onpress_a_get_script_block(struct npc_position *n, u8 role, u8 direction);
bool per_step_2(struct npc_state *npc, u16 role, u8 direction);
bool is_tile_XX_prevent_per_step_scripts(u16 role);
void sub_805CCD0(u8 npc_id, u8 direction);
void taskFF_805CD0C(u8 task_id);
bool sub_805CD64_mode_0(struct task_t* c, struct npc_state* player_npc, struct npc_state* other_npc);
bool sub_805CD84_mode_1(struct task_t* c, struct npc_state* player_npc, struct npc_state* other_npc);
bool sub_805CE20_mode_2(struct task_t* c, struct npc_state* player_npc, struct npc_state* other_npc);
