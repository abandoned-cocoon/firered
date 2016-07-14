#include "object.h"
#include "battle_config.h"

void bt1_29_blink_for_damage() {
	u8 oamid = b_oamid[b_active_side];
	struct obj *o = &objects[oamid];
	if (o->bitfield2 & 0x7) {
		bx_busy = true;
		o->priv1 = 0;
		sub_804BE70(b_active_side);
		bx[b_active_side] = &bx1_blink_for_damage;
	} else {
		bt1_done();
	}
}

// 08033444
void bt1_2E_battle_intro() {
	u8 environment_id = b_buffer_A[b_active_side][1];
	battle_intro_launch(environment_id);
	word_02023F4C |= 1;
	bt1_done();
}
