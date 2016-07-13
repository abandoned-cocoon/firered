#include "continuegame.h"
#include "helpsystem.h"
#include "overworld.h"
#include "save.h"
#include "task.h"
#include "u0807d_overworld.h"

#ifndef NO_RAM
// 0203ADF8
u8 flashback_cursor;
// 0203ADF9
u8 prev_quest_display_number;
// 0203ADFA
u8 prev_quest_mode; // 0: normal
					// 1: unknown
					// 2: flashback
					// 3: fade flashback to normal
// 0203ADFC
u16 word_203ADFC;
// 0203ADFE
u8 byte_203ADFE;
// 0203ADFF
u8 byte_203ADFF;
// 0203AE00
u8 pq_rbox;
// 0203AE04
u8 *prev_quest_postbuffer_cursor_backup; // points into sav1-area
// 0203AE08
u8 *prev_quest_postbuffer_cursor; // points into sav1-area
// 0203AE0C
u8 prev_quest_postbuffer[0x80];
// 0203AE8C
void (*funcptr_203AE8C)();
// 0203AE90
u16 *ptr_to_palette_backup;
// 0203AE94
u8 byte_203AE94;
// 0203AE95
u8 byte_203AE95;
// 0203AE96
u8 byte_203AE96;
// 0203AE97
u8 byte_203AE97;
// 0203AE98
u8 byte_203AE98[0x100];
// 0203AF98
u16 pq_npc_cursor;
// 0203AF90
u8 byte_203AF90[0x80];
// 0203B01A
u16 pq_npc__script_env_2_is_enabled;
// 0203B01C
u16 word_203B01C;
// 0203B01E
u16 word_203B01E;
// 0203B020
u8 word_203B020;
// 0203B024
u16 prev_quest_prebuffer_length[0x10];
// ...


// 03005E88
u8 pq_npc_recording;
// 03005E8C
u16 pq_npc_end;
// 03005E94
struct pq_npc_entry *pq_npc_buffer;

// 03005E90
u32 dword_3005E90;

#endif

// 08056938 (move to overworld.c/overworld_loading.c probably)
void c2_8056938() {
	sub_80569BC();
	sub_8071A94();
	flag_clear_safari_zone();
	cur_mapheader_from_sav1();
	load_sav1_rom_npc_scripts();
	sub_8068A5C();
	// TODO
}

// 081109CC
void prev_quest_mode_set(u8 mode) {
	prev_quest_mode = mode;
	j5 = (mode == 1)
	   ? &j5_prev_quest_mode_1
	   : &j5_08110A3C;
}

// 08110A00
// void j5_prev_quest_mode_1() {
// 	if (sub_8110E68(&unk_203AE98) != 1) {
// 		pq_npc_recording = 0;
// 		sub_8110E3C(&pq_npc_recording);
// 		prev_quest_mode = 0;
// 		j5 = 0;
// 	}
// }

// 08110A3C
void j5_08110A3C() {
    if ((byte_203AE94 & 0xF) == 2)
        byte_203AE94 &= 0xF0u;

    if (!(byte_203AE94 & 0xC0)) {
        if (pq_npc_recording
          || (byte_203AE94 & 0xF) == 1
          || byte_203AE95 <= 0x1Fu && prev_quest_postbuffer[byte_203AE95]) {
            prev_quest_get_story();
        } else {
            byte_203AE94 = byte_203AE94 & 0x3F | 0x80;
            script_env_2_enable();
            sub_81118F4(0);
        }
    }
}

// 08110AC8
bool questlog_active_at_cursor() {
	return sav1i->questlog[flashback_cursor].active;
}

// 08110F14
void game_continue(task_id c) {
	quest_logger_filter_disable();
	prev_quest_display_number = 0;

	for (uint i=0; i<4; i++)
		if (sav1i->questlog[i].active)
			prev_quest_display_number++;

	if (prev_quest_display_number > 0) {
		help_system_enabled = 0;
		flashback();
	} else
		set_callback2(&c2_8056938);

	task_del(c);
}

// 08110F90
void flashback() {
	// Route 1
	sav1i->location.group = 3;
	sav1i->location.map = 19;
	sav1i->location.warpid = 0xFF;

	flashback_cursor = 0;
	warp_sound_disable = 1;
	set_20386DC(1);
	flashback_2();
}

// 08111000
bool hmp1_8111000() {
	pal_bg_patch(stdpal_get(4), 0xF0, 0x20);
	prev_quest_mode_set(2);
	mapldr_default();
	byte_203AE94 = 2;
	byte_203AE95 = 0;
	byte_203AE96 = 0;
	byte_203AE97 = 0;
	return true;
}

// 08111038
bool hmp1_8111038() {
	pal_bg_patch(stdpal_get(4), 0xF0, 0x20);
	prev_quest_mode_set(2);
	mapldr_for_new_game();
	byte_203AE94 = 2;
	byte_203AE95 = 0;
	byte_203AE96 = 0;
	byte_203AE97 = 0;
	return true;
}

// 08111368
void flashback_2() {
	prev_quest_mode = 1;
	init_vars();
	bag_809A2DC();
	sub_809A2A4();
	if (questlog_active_at_cursor()) {
		sub_8111274(flashback_cursor, 0);
		hm_phase_1 = &hmp1_8111038;
		set_callback2(&c2_080572A8);
	} else {
		sub_8111274(flashback_cursor, 1);
		warp_in_metadata();
		hm_phase_1 = &hmp1_8111000;
		set_callback2(&c2_0805726C);
	}
}

// 08111CF0
void render_prev_quest_text_if_appropriate() {
	if (prev_quest_mode == 2)
		render_previous_quest_text(prev_quest_display_number);
}

// 0811246C
//void objc_npc_alternative(struct obj *obj) {
//	// TODO
//}

// 0203AF98
extern u16 pq_npc_cursor;

// 08114710
u8 *prev_quest_write_x10_union_room(u8 *cursor) {
	cursor[0] = 0x10;
	cursor[2] = pq_npc_cursor & 0xFF;
	cursor[3] = pq_npc_cursor >> 8;
	return cursor+4;
}

// 08114724
u8 *prev_quest_read_x10_union_room(u8 *cursor) {
	fdecoder(displayed_string, "Mingled with other TRAINERS in the UNION ROOM.");
	return cursor + 4;
}

// 08114744
u8 *prev_quest_write_x11_chat_with_trainers(u8 *cursor) {
	cursor[0] = 0x11;
	cursor[2] = pq_npc_cursor & 0xFF;
	cursor[3] = pq_npc_cursor >> 8;
	return cursor+4;
}

// 08114758
u8 *prev_quest_read_x11_chat_with_trainers(u8 *cursor) {
	fdecoder(displayed_string, "Chatted with many TRAINERS.");
	return cursor + 4;
}