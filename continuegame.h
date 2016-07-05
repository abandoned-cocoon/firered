#pragma once
#include "types.h"

// alternative names:
//  prev quest
//  quest log

struct prev_quest_story {
	char active;
	char field_1[1639];
};

// 0203ADF8
extern u8 flashback_cursor;
// 0203ADF9
extern u8 prev_quest_display_number;
// 0203ADFA
extern u8 prev_quest_mode; // 0: normal
                           // 1: unknown
                           // 2: flashback
                           // 3: fade flashback to normal

// 0203AE8C
#define j5 funcptr_203AE8C
extern void (*funcptr_203AE8C)();

// 03005E90
extern u32 dword_3005E90;


void c2_8056938(void);
void prev_quest_mode_set(u8 mode);
void j5_prev_quest_mode_1();
bool questlog_active_at_cursor();
void game_continue(u8 task_id);
void flashback(void);
bool hmp1_8111000(void);
bool hmp1_8111038(void);
void flashback_2(void);
void render_prev_quest_text_if_appropriate();
u8 *prev_quest_write_x10_union_room(u8 *cursor);
u8 *prev_quest_read_x10_union_room(u8 *cursor);
u8 *prev_quest_write_x11_chat_with_trainers(u8 *cursor);
u8 *prev_quest_read_x11_chat_with_trainers(u8 *cursor);
