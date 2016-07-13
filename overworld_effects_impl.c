#include "engine_scripts.h"
#include "overworld_effects.h"
#include "hm.h" // for cut_ruin_valley

struct oe_state_t {
	union {
		struct {
			u32 to_x;
			u32 to_y;
			u32 height_lsr_4;
		};
		struct {
			u32 local_id;
			u32 local_mapnumber;
			u32 local_mapgroup2;
		};
		struct {
			u8 byte0;
		};
	};
	u32 field_C;
	u32 local_id_and_mapnumber;
	u32 local_mapgroup;
	u32 sav1_location;
	u32 field_1C;
};

// 020386E0
struct oe_state_t oe_state;

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
	oe_state.byte0 = index;
	script_env_12_start_and_stuff(scr_cut2);
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
	if (cut_ruin_valley == 1)
		cut();
	else
		oe_exec(0x3A);
}

// 02039A04
extern struct coords16 coords16_in_front_of_player;

// 08097A48
bool oei_3A_08097A48() {
	audio_play(0x79);
	player_get_pos_to(
		&coords16_in_front_of_player.x,
		&coords16_in_front_of_player.y
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

// TODO
void objc_08097BBC(struct obj *obj);

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