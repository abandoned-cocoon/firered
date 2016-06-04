#pragma once
#include "types.h"

// alternative names:
//  prev quest
//  quest log

struct prev_quest_story {
	char active;
	char field_1[1639];
};

// 0203ADF9
extern u8 prev_quest_display_number;
// 0203ADFA
extern u8 prev_quest_mode; // 0: normal
                           // 1: unknown
                           // 2: flashback
                           // 3: fade flashback to normal

// 03005E90
extern u32 dword_3005E90;


void c2_8056938();
void prev_quest_mode_set(u8 mode);
bool questlog_active_at_cursor();
void game_continue(u8 task_id);
void flashback();
void hmp1_8111000();
void hmp1_8111038();
void flashback_2();
void render_prev_quest_text_if_appropriate();
void prev_quest_write_x10_union_room(u8 *cursor);
void prev_quest_read_x10_union_room(u8 *cursor);
void prev_quest_write_x11_chat_with_trainers(u8 *cursor);
void prev_quest_read_x11_chat_with_trainers(u8 *cursor);
