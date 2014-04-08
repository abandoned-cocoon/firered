struct intro {
	void (*funcptr)(struct intro*);
	u8 field4;
	u8 tid;
	// and 0x28BC total bytes
};

// 080ECA70
void intro_sys_init() {
	struct intro *in = malloc(sizeof(struct intro));

	intro_set_cb(in, &intro_1_load_stars);

	in->tid = task_add(&task03_intro_sys, 3);
	taskid_set_priv_u32(in->tid, 0, in);
}

// 080ECAA8
void intro_set_cb(struct intro *in, void (*funcptr)(struct intro*)) {
	in->funcptr = funcptr;
	in->field4 = 0;
}

// 080ECAB0
void task03_intro_sys(u8 tid) {
	struct intro *in = (struct in*) task_get_priv_u32(tid, 0);

	if (super.button_held_remapped & (KEYPAD_A | KEYPAD_START | KEYPAD_SELECT))
		if (in->funcptr != &intro_end)
			intro_set_cb(in, &intro_end);

	*in->funcptr(in);
}

// 080ECAF0
// intro_1_load_stars

// 080ECB98
// intro_2_dark_blue_stripe

// 080ECC3C
// intro_3_star_animation

// 080ECCA8
// intro_4_fadein_gamefreak_name

// 080ECD60
// intro_5_fadein_logo_then_fadeout_all

// 080ECEA4
// intro_6_scene_1

// 080ED0AC
// task00_080ED0AC

// 080ED118
// sub_080ED118 (intro 6 sub)

// 080ED140
// task00_intro_zoom_in

// 080ED188
// intro_7_scene_2

// 080ED40C
// task00_intro_7_080ED40C

// 080ED428
// task00_intro_7_080ED428

// 080ED444
// sub_080ED444 (intro 7 sub)

// 080ED4A0
// sub_080ED4A0 (intro 7 sub)

// 080ED4C0
// intro_8_scene_3

// 080ED69C
// task00_080ED69C

// 080ED6D8
// sub_080ED6D8

// 080ED6EC
// sub_080ED6EC

// 080ED714
// sub_080ED714

// 080ED760
// sub_080ED760 (intro 9 sub)

// 080ED788
// sub_080ED788 (intro 9 sub)

// 080ED7B0
// sub_080ED7B0 (intro 9 sub)

// 080ED714
// sub_080ED714 (intro 8 sub)

// 080ED760
// sub_080ED760 (intro 9 sub)

// 080ED788
// sub_080ED788 (intro 9 sub)

// 080ED7B0
// sub_080ED7B0 (intro 9 sub)

// 080ED7D4
// sub_080ED7D4 (intro 8 sub)

// 080ED818
// objc_080ED818 (intro 8 object)

// 080ED898
// intro_9_scene_4

// 080EDAD8
// TBC

// 080EDAF0
// sub_080EDAF0 (intro 9 sub)

// 080EDB70
// sub_080EDB70 (intro 9 sub)

// 080EDBAC
// sub_080EDBAC (intro 9 sub)

// 080EDBE8
void intro_end(struct intro *in) {
	if (in->field4 == 0) {
		sub_08070424(0, 0, 0x400);
		in->field4 = 1;
	} else if (in->field4 == 1) {
		if (sub_080F682C() == 0) {
			task_del(in->tid);
			free(in);
			sub_08000B94(2);
			hblank_handler_set(0);
			set_callback2(&c2_intro_2);
		}
	}
}

// 080EDC40
// spawn_intro_star (intro 3 sub)

// 080EDD28
// sub_080EDD28 (for star)

// 080EDDF0
// sub_080EDDF0 (intro 3 sub)

// 080EDE04
// task01_intro_star_out

// 080EDED8
// sub_080EDED8 (intro 4 sub)

// 080EDEEC
// task02_080EDEEC (launched by 080EDED8)

// 080EDF68
// sub_080EDF68 (intro 5 sub)

// 080EDF94
// sub_080EDF94 (intro 9 sub)

// 080EDFD8
// sub_080EDFD8 (intro 4 sub)

// 080EE024
// task04_080EE024 (launched by 080EDFD8)

// 080EE160
// sub_080EE160 (task04_080EE024 sub)

// 080EE1C4
// objc_080EE1C4 (sub_080EE160 instanciated object callback)

// 080EE200
// sub_080EE200 (intro 8)

// 080EE29C
// objc_star

// 080EE350
// objc_sparks_1

// 080EE40C
// objc_080EE40C (used by task01_intro_star_out)

// 080EE4DC
// objc_sparks_2

// 080EE4F8
// sub_080EE4F8 (intro 8 sub)

// 080EE528
// sub_080EE528 (intro 8 sub)

// 080EE580
void objc_080EE580(obj_t *o) { // for sub_080EE528

	// move with linear speed
	// but from frame 40 on decrease the speed to 1
	// stop when having reached priv5

	if (o->priv6++ >= 40 && o->priv3 > 1)
			o->priv3--;
	o->priv2 += o->priv3;

	o->pos1.x = o->priv2 / 16;

	if (o->pos1.x >= o->priv5) {
		o->pos1.x = o->priv5;
		o->callback = &objc_nothing;
	}
}

// 080EE5C8
// sub_080EE5C8 (intro 8 sub)

// 080EE5E4
// sub_080EE5E4 (intro 9 sub)

// 080EE610
// objc_080EE610 (for sub_080EE5E4)

// 080EE6A4
// sub_080EE6A4 (intro 9 sub)

// 080EE704
// objc_080EE704 (for sub_080EE6A4)

// 080EE850
// sub_080EE850 (intro 9 sub)

// 080EE864
// sub_080EE864 (objc_080EE704 sub)

// 080EE8E4
// objc_080EE8E4 (for sub_080EE864)

// 080EE970
// sub_080EE970 (intro 9 sub)

// 080EE9D4
// objc_080EE9D4 (for sub_080EE970)

// 080EEA94
void sub_080EEA94(struct intro *in) { // intro 9 sub
	obj_t *o = in->field18;

	o->priv2 = 0;
	o->priv3 = 0;
	o->priv4 = 0;
	o->priv5 = 0;
	o->priv6 = 0;
	o->priv7 = 0;

	o->pos1.x += o->pos2.x;
	o->pos2.x = 0;

	word_0203AB0C = 0x24;
	word_0203AB06 = 0x28;
	word_0203AB04 = 0x03;
	word_0203AB08 = 0x04;

	o->field_3C = 0x24;

	obj_set_anim_and_bitfield_xxx0_x1xx(o, 2);
	o->callback = &objc_080EEB08;
}

// 080EEB08
// objc_080EEB08 (for sub_080EEA94)

// 080EEBE4
// sub_080EEBE4 (intro 6 sub)

// 080EEC0C
// END
