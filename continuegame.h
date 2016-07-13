#pragma once
#include "types.h"

// alternative names:
//  prev quest
//  quest log

struct prev_quest_story {
	char active;
	char field_1[1639];
};

struct pq_npc_entry {
	u8 zero;
	u8 field_1;
	u8 field_2;
	u8 eventtype;
	u16 script_env_2_is_enabled;
	u8 one;
	u8 field_7;
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

// 0203ADFC
extern u16 word_203ADFC;
// 0203ADFE
extern u8 byte_203ADFE;
// 0203ADFF
extern u8 byte_203ADFF;
// 0203AE00
extern u8 pq_rbox;
// 0203AE04
extern u8 *prev_quest_postbuffer_cursor_backup; // points into sav1-area
// 0203AE08
extern u8 *prev_quest_postbuffer_cursor; // points into sav1-area
// 0203AE0C
extern u8 prev_quest_postbuffer[0x80];

// 0203AE8C
#define j5 funcptr_203AE8C
extern void (*funcptr_203AE8C)();

// 0203AE94
extern u8 byte_203AE94;
// 0203AE95
extern u8 byte_203AE95;
// 0203AE96
extern u8 byte_203AE96;
// 0203AE97
extern u8 byte_203AE97;
// 0203AE98
extern u8 byte_203AE98[0x100];
// 0203AF98
extern u16 pq_npc_cursor;
// 0203AF90
extern u8 byte_203AF90[0x80];
// 0203B01A
extern u16 pq_npc__script_env_2_is_enabled;
// 0203B01C
extern u16 word_203B01C;
// 0203B01E
extern u16 word_203B01E;
// 0203B020
extern u8 word_203B020;
// 0203B024
extern u16 prev_quest_prebuffer_length[0x10];
// ...

// 03005E88
extern u8 pq_npc_recording;
// 03005E8C
extern u16 pq_npc_end;
// 03005E94
struct pq_npc_entry *pq_npc_buffer;

// 03005E90
extern u32 dword_3005E90;


void c2_8056938(void);
void prev_quest_mode_set(u8 mode);
void j5_prev_quest_mode_1();
void j5_08110A3C();
bool questlog_active_at_cursor();
void game_continue(task_id c);
void flashback(void);
bool hmp1_8111000(void);
bool hmp1_8111038(void);
void flashback_2(void);
void render_prev_quest_text_if_appropriate();

// split into headers here?

void objc_npc_alternative(struct obj *obj);

// split into headers here?

u8 *prev_quest_write_x10_union_room(u8 *cursor);
u8 *prev_quest_read_x10_union_room(u8 *cursor);
u8 *prev_quest_write_x11_chat_with_trainers(u8 *cursor);
u8 *prev_quest_read_x11_chat_with_trainers(u8 *cursor);
