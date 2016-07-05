#include "engine_scripts.h"
#include "overworld_effects.h"

void oei_cut_finish(void);

// 08097874
/* ... */

// 080979A0
bool oei_cut() {
	u8 tid = oei_task_add();
	TASK_PRIV_ASSIGN32(task[tid], 8, (uintptr_t)&oei_cut_finish);
	sav_xor_incr(0x12);
	return false;
}

// 080979D0
void hm_cut_run_scr() {
	u8 index = brm_get_pokemon_selection();
	*(u32*)&dp03 = index;
	sub_08069AE4(&scr_cut2);
}

// 080979F0
bool oei_cut2() {
	u8 tid = oei_task_add();
	TASK_PRIV_ASSIGN32(task[tid], 8, (uintptr_t)&oei_cut2_finish);
	sav_xor_incr(0x12);
	return false;
}

void oei_cut_finish() {
	oe_active_list_remove(1);
	if (byte_2039874 == 1)
		cut();
	else
		oe_exec(0x3A);
}

// 08097A48
bool oei_3A_08097A48() {
	audio_play(0x79);
	player_get_pos_to(
		&coords16_02039A04.x,
		&coords16_02039A04.y
	);

	for (u8 i=0; i<2; i++) {
		/* ... */
	}

	cur_mapdata_full_redraw();
	dword_2039870 = mem_alloc(8);

	/* ... */

	return false;
}

// 08097B50
/* ... */

// 08097BA8
void objc_08097BA8(struct obj *obj) {
	obj->priv0 = 8;
	obj->priv1 = 0;
	obj->priv3 = 0;
	obj->callback = &objc_08097BBC;
}

// 08097BBC
/* ... */

// 08097C18
/* ... */

// 08097C6C
void oei_cut2_finish() {
	audio_play(0x79);
	oe_active_list_remove(2);
	script_env_2_enable_and_set_ctx_running();
}