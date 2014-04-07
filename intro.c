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
