#include "continuegame.h"
#include "save.h"

// 0203ADF9
u8 prev_quest_display_number;
// 0203ADFA
u8 prev_quest_mode; // 0: normal
					// 1: unknown
					// 2: flashback
					// 3: fade flashback to normal
// 03005E90
u32 dword_3005E90;

// 08056938
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

// 08110AC8
bool questlog_active_at_cursor() {
	return sav1i->questlog[flashback_cursor].active;
}

// 08110F14
void game_continue(u8 task_id) {
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

	task_del(task_id);
}

// 08110F90
void flashback() {
	// Route 1
	sav1i->location.bank = 3;
	sav1i->location.map = 19;
	sav1i->location.warpid = 0xFF;

	quest_log_id_in_saveblock1 = 0;
	warp_sound_disable = 1;
	set_20386DC(1);
	flashback_2();
}

// 08111000
void hmp1_8111000() {
	pal_bg_patch(stdpal_get(4), 0xF0, 0x20);
	prev_quest_mode_set(2);
	mapldr_default();
	// dword_203AE94 = 0;
	// dword_203AE94 &= 0xF0;
	// dword_203AE94 |= 0x02;
	dword_203AE94 = 2;
	return 1;
}

// 08111038
void hmp1_8111038() {
	pal_bg_patch(stdpal_get(4), 0xF0, 0x20);
	prev_quest_mode_set(2);
	mapldr_for_new_game();
	// dword_203AE94 = 0;
	// dword_203AE94 &= 0xF0;
	// dword_203AE94 |= 0x02;
	dword_203AE94 = 2;
	return 1;
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

// 0203AF98
extern u16 pq_npc_cursor;

// 08114710
void prev_quest_write_x10_union_room(u8 *cursor) {
	cursor[0] = 0x10;
	cursor[2] = pq_npc_cursor & 0xFF;
	cursor[3] = pq_npc_cursor >> 8;
	return cursor+4;
}

// 08114724
void prev_quest_read_x10_union_room(u8 *cursor) {
	fdecoder(displayed_string, "Mingled with other TRAINERS in the UNION ROOM.");
	return cursor + 4;
}

// 08114744
void prev_quest_write_x11_chat_with_trainers(u8 *cursor) {
	cursor[0] = 0x11;
	cursor[2] = pq_npc_cursor & 0xFF;
	cursor[3] = pq_npc_cursor >> 8;
	return cursor+4;
}

// 08114758
void prev_quest_read_x11_chat_with_trainers(u8 *cursor) {
	fdecoder(displayed_string, "Chatted with many TRAINERS.");
	return cursor + 4;
}