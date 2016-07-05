#include "object.h"
#include "overworld_navigation.h"
#include "task.h"

/*
Assumptions: 
void *memcpy(void *dst, const void *src, char n)
*/

#define P ((struct fishing_priv*)&t->priv)

struct fishing_priv {
	u16 state;
};

// 0805D2C0
void fishing_start(u8 rod_quality) {
	u8 taskid = task_add(&taskFF_fishing, 0xFF);
	tasks[taskid].priv[15] = rod_quality;
	taskFF_fishing(taskid);
	if (sub_08150474(2) == 1)
		sub_08112C9C();
}

// 0805D304
void taskFF_fishing(u8 taskid) {
	struct task_t *t = &tasks[taskid];
	while (fishing_states[P->state](t));
}

// 0805D33C
bool fishing0(struct task_t *t) {
	script_env_2_enable();
	walkrun_state.lock = true;
	t->priv[0]++;
	return false;
}

// 0805D35C
bool fishing1(struct task_t *t) {
	// used to be using memcpy 
	u16 wait_base[] = {1, 1, 1}; // 1 per rod
	u16 wait_variance[] = {1, 3, 6};
	struct npc_state *player_npc = &npc_states[walkrun_state.npc_id];
	
	t->priv[12] = 0;
	t->priv[13] = unk_data[t->priv[15]] + rand() % some_unk_data[t->priv[15]];
	t->priv[14] = player_npc->type_id;
	npc_unset_movement_behaviour_if_pause_stash_locked(player_npc)
	player_npc->obj_anim_and_vis_control |= 8;
	sub_805CC2C(player_npc->direction & 0xF);
	t->priv[0]++;
	return false; 
}

// 0805D3F4
bool fishing2(struct task_t *t) {
	sub_0805D9C4(objects[walkrun_state.objid]);
	if ((++t->priv[1]) >= 60) // wait one second
		t->priv[0]++; // go to state 3
	return false;
}

// 0805D430
bool fishing3(struct task_t *t) {
	box_border_curve_load_tiles_and_pal();
	u16 *priv = t->priv;
	priv[0]++;
	priv[1] = 0;
	priv[2] = 0;
	priv[3] = (rand() % 10) + 1;
	if (priv[12] == 0)
		priv[3] += 3;
	if (priv[3] > 10)
		priv[3] = 10;
	return true;
}

// 0805D47C
bool fishing4(struct task_t *t) {
	sub_805D9C4(&objects[walkrun_state.oamid]);
	t->priv[1]++;
	if (t->priv[1] < 19) {
		t->priv[1] = 0;
		if (t->priv[2] < t->priv[3]) {
			// textbox related
			sub_8002C48(0, 2, (u32 *)0x835B918, t->priv[2] * 12, 1, 0, 0);
		} else {
			t->priv[0]++;
			if (!t->priv[12])
				t->priv[0]++;
			t->priv[12]++;
		}
	}
	return false;
}

// 0805D508
/* get_fishing_exists_by_map = sub_8082F78 // returns true if fishing exists on map, else false
	sub_8063510 seems to be getting an animation number/id from a table depending on player's dir
*/

bool fishing5(struct task_t *t) {
	struct obj *player_obj = &objects[walkrun_state.oamid];
	sub_805D9C4(player_obj); // animation. I think it's the shaking animation before hitting A
	t->priv[0]++;
	if ((get_wilddata_fishing_by_map() << 0x18) && !(rand() &1)) {
		u8 anim_id = sub_8063510(player_get_direction_sp1AA(walkrun_state.oamid));
		obj_anim_image_start(player_obj, anim_id);
	} else {
		t->priv[0] = 11;
	}
	
	return true;
}

// 0805D574
bool fishing6(struct task_t *t) {
	t->priv[0] += 3; // go to state 9
	return false;
}

// 0805D580
/*sub_805D9C4 does some animation. Rod shaking maybe.*/
bool fishing7(struct task_t *t) {
	// used to be memcpy
	u16 rod_something[] = {0x24, 0x21, 0x1E};
	sub_805D9C4(&objects[walkrun_state.oamid]);
	t->priv[1]++;
	if (t->priv[1] >= rod_something[t->priv[15]]) {
		t->priv[0] = 12;
		return false;
	}
	if (super.buttons_new_remapped & 1) {
		t->priv[0]++;
	}
	return false;
}

// 0805D5EC
/*sub_805D9C4 does some animation. Rod shaking maybe.*/
bool fishing8(struct task_t *t) {
	u16 rod_something[] = {0x0, 0x0, 0x28, 0xA, 0x46, 0x1E};
	sub_805D9C4(&objects[walkrun_state.oamid]);	
	t->priv[0]++;
	if ((t->priv[12] < t->priv[13]) || (t->priv[12] <= 1) && 
	(rod_something[2 * t->priv[15] + t->priv[12]]) > rand() % 100)
		t->priv[0] = 3;
	return false;
}

// 0805D66C
/*sub_805D9C4 does some animation. Rod shaking maybe.*/
bool fishing9(struct task_t *t) {
	sub_805D9C4(&objects[walkrun_state.oamid]);
	rboxid_clear_pixels(0, 0x11); 
	// box_related_one(unk, font, string, text_speed, unk, text_colour, text_bg_maybe, shadow);*/
	box_related_one(0, 2, (char *)0x841D14E, 1, 0, 2, 1, 3);
	t->priv[0]++;
	t->priv[1] = 0;
	return false;
}

// 0805D6C8
/*sub_805D9C4 does some animation. Rod shaking maybe.
sub_8082FB0 = setup_wild_battle_rod; gens a wild pokemon by rod type & does battle preparations */
bool fishingA(struct task_t *t) {
	struct obj *player_obj = &objects[walkrun_state.oamid];

	if (!t->priv[1])
		sub_805D9C4(player_obj);
	dialogboxes_upload_tilesets(player_obj);
	bool a_pressed = dialogid_was_acknowledged(0);
	if (a_pressed && !(t->priv[1])) {
		return false;
	} else if (a_pressed) {
		walkrun_state.lock = 0;
		script_env_2_disable();
		setup_wild_battle_rod(t->priv[15] & 0xFF);
		return false;
	} else {
		struct npc_state *player_npc = &npc_states[walkrun_state.npc_id];
		npc_change_type_maybe(player_npc, t->priv[14] & 0xFF);
		npc_turn(player_npc, player_npc->direction & 0xF);
		if (walkrun_state.bitfield & 8)
			sub_80DC4A4(player_npc->objid_surfing, 0, 0);
		player_obj->pos_2.x = 0;
		player_obj->pos_2.y = 0;
		sub_80F6F54(0, 1); // message box update message maybe?
		t->priv[1]++;
		return false;
	}
}

// 0805D7C0
/*sub_805D9C4 does some animation. Rod shaking maybe. 
  sub_8063500 gets some byte (anim id maybe) from a byte array depending on player facing dir
  (char *)0x841D169 - Not even a nibble */
bool fishingB(struct task_t *t) {
	struct obj *player_obj = &objects[walkrun_state.oamid];
	sub_805D9C4(player_obj);
	u8 anim_id = sub_8063500(player_get_direction_sp1AA(walkrun_state.oamid));
	obj_anim_image_start(player_obj, anim_id);
	rboxid_clear_pixels(0, 0x11);
	// box_related_one(unk, font, string, text_speed, unk, text_colour, text_bg_maybe, shadow);*/
	box_related_one(0, 2, (char *)0x841D169, 1, 0, 2, 1, 3);
	t->priv[0] = 13;
	return true;
}

// 0805D838
/*sub_805D9C4 does some animation. Rod shaking maybe.
  sub_8063500 gets some byte (anim id maybe) from a byte array depending on player facing dir
  (char *)0x841D17E - it got away */
bool fishingC(struct task_t *t) {
	struct obj *player_obj = &objects[walkrun_state.oamid];
	sub_805D9C4(player_obj);
	u8 anim_id = sub_8063500(player_get_direction_sp1AA(walkrun_state.oamid));
	obj_anim_image_start(player_obj, anim_id);
	// box_related_one(unk, font, string, text_speed, unk, text_colour, text_bg_maybe, shadow);*/
	box_related_one(0, 2, (char *)0x841D17E, 1, 0, 2, 1, 3);
	t->priv[0]++;
	return true;
}

// 0805D8AC
/*sub_805D9C4 does some animation. Rod shaking maybe.*/
bool fishingD(struct task_t *t) {
	sub_805D9C4(&objects[walkrun_state.oamid]);
	t->priv[0]++;
	return false;
}

// 0805D8D8
/*sub_805D9C4 does some animation. Rod shaking maybe.*/
bool fishingE(struct task_t *t) {
	struct obj *player_obj = &objects[walkrun_state.oamid];
	struct npc_state *player_npc = &npc_states[walkrun_state.npc_id];
	sub_805D9C4(player_obj);
	if (player_obj->bitfield & 0x10) {
		npc_change_type_maybe(player_npc, t->priv[14] & 0xFF);
		npc_turn(player_npc, player_npc->direction & 0xF);
		if (walkrun_state.bitfield & 8)
			sub_80DC4A4(player_npc.objid_surfing, 0, 0);
		player_obj->pos_2.x = 0;
		player_obj->pos_2.y = 0;
	}
}

// 0805D980
bool fishingF(struct task_t *t) {
	textbox_something();

	if (a_pressed_maybe(0))
		return false;

	walkrun_state.lock = false;
	script_env_2_disable();
	sub_08068A5C();
	sub_080F6F54(0, 1);
	u8 taskid = task_find_by_funcptr(&taskFF_fishing);
	task_del(taskid);
	return false;
}

#undef P
