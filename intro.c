#include "memory.h"
#include "object.h"
#include "task.h"
#include "u08078_intro.h"

u16 word_0203AB00; // 0203AB00
u16 word_0203AB02; // 0203AB02
u16 word_0203AB04; // 0203AB04
u16 word_0203AB06; // 0203AB06
u16 word_0203AB08; // 0203AB08
u16 word_0203AB0A; // 0203AB0A
u16 word_0203AB0C; // 0203AB0C
u16 word_0203AB0E; // 0203AB0E
u16 word_0203AB10; // 0203AB10
u16 word_0203AB12; // 0203AB12
u16 word_0203AB14; // 0203AB14
u16 word_0203AB16; // 0203AB16
u16 word_0203AB18; // 0203AB18
u16 word_0203AB1A; // 0203AB1A
u16 word_0203AB1C; // 0203AB1C
u16 word_0203AB1E; // 0203AB1E
u16 word_0203AB20; // 0203AB20
u16 spark_scaling_x; // 0203AB22
u16 spark_scaling_y; // 0203AB24

struct intro {
	void (*funcptr)(struct intro*);
	u8 field4;
	u8 tid;
	u8 padding6[0x18-0x6];
	struct obj *field18;
	// and 0x28BC total bytes
};

// TODO: Move to header
void intro_sys_init();
void intro_set_cb(struct intro *in, void (*funcptr)(struct intro*));
void task03_intro_sys(u8 tid);
void intro_1_load_stars(struct intro *in);
void intro_end(struct intro *in);
void objc_080EE580(struct obj *o);
void sub_80EEA94(struct intro *in);
void objc_080EEB08(struct obj *o);


// 080ECA70
void intro_sys_init() {
	struct intro *in = mem_alloc(sizeof(struct intro));

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
	struct intro *in = (struct intro*) task_get_priv_u32(tid, 0);

	if (super.buttons_held_remapped & (KEYPAD_A | KEYPAD_START | KEYPAD_SELECT))
		if (in->funcptr != &intro_end)
			intro_set_cb(in, &intro_end);

	(*in->funcptr)(in);
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
// sub_80ED118 (intro 6 sub)

// 080ED140
// task00_intro_zoom_in

// 080ED188
// intro_7_scene_2

// 080ED40C
// task00_intro_7_080ED40C

// 080ED428
// task00_intro_7_080ED428

// 080ED444
// sub_80ED444 (intro 7 sub)

// 080ED4A0
// sub_80ED4A0 (intro 7 sub)

// 080ED4C0
// intro_8_scene_3

// 080ED69C
// task00_080ED69C

// 080ED6D8
// sub_80ED6D8

// 080ED6EC
// sub_80ED6EC

// 080ED714
// sub_80ED714

// 080ED760
// sub_80ED760 (intro 9 sub)

// 080ED788
// sub_80ED788 (intro 9 sub)

// 080ED7B0
// sub_80ED7B0 (intro 9 sub)

// 080ED714
// sub_80ED714 (intro 8 sub)

// 080ED760
// sub_80ED760 (intro 9 sub)

// 080ED788
// sub_80ED788 (intro 9 sub)

// 080ED7B0
// sub_80ED7B0 (intro 9 sub)

// 080ED7D4
// sub_80ED7D4 (intro 8 sub)

// 080ED818
// objc_080ED818 (intro 8 object)

// 080ED898
// intro_9_scene_4

// 080EDAD8
// TBC

// 080EDAF0
// sub_80EDAF0 (intro 9 sub)

// 080EDB70
// sub_80EDB70 (intro 9 sub)

// 080EDBAC
// sub_80EDBAC (intro 9 sub)

// 080EDBE8
void intro_end(struct intro *in) {
	if (in->field4 == 0) {
		sub_8070424(0, 0, 0x400);
		in->field4 = 1;
	} else if (in->field4 == 1) {
		if (sub_80F682C() == 0) {
			task_del(in->tid);
			memory_free(in);
			sub_8000B94(2);
			hblank_handler_set(0);
			set_callback2(&c2_intro_2);
		}
	}
}

// 080EDC40
// spawn_intro_star (intro 3 sub)

// 080EDD28
// sub_80EDD28 (for star)

// 080EDDF0
// sub_80EDDF0 (intro 3 sub)

// 080EDE04
// task01_intro_star_out

// 080EDED8
// sub_80EDED8 (intro 4 sub)

// 080EDEEC
// task02_080EDEEC (launched by 080EDED8)

// 080EDF68
// sub_80EDF68 (intro 5 sub)

// 080EDF94
// sub_80EDF94 (intro 9 sub)

// 080EDFD8
// sub_80EDFD8 (intro 4 sub)

// 080EE024
// task04_080EE024 (launched by 080EDFD8)

// 080EE160
// sub_80EE160 (task04_080EE024 sub)

// 080EE1C4
// objc_080EE1C4 (sub_80EE160 instanciated object callback)

// 080EE200
// sub_80EE200 (intro 8)

// 080EE29C
// objc_star

// 080EE350
// objc_sparks_1

// 080EE40C
// objc_080EE40C (used by task01_intro_star_out)

// 080EE4DC
// objc_sparks_2

// 080EE4F8
// sub_80EE4F8 (intro 8 sub)

// 080EE528
// sub_80EE528 (intro 8 sub)

// 080EE580
void objc_080EE580(struct obj *o) { // for sub_80EE528

	// move with linear speed
	// but from frame 40 on decrease the speed to 1
	// stop when having reached priv3

	if (o->priv4++ >= 40 && o->priv1 > 1)
			o->priv1--;
	o->priv0 += o->priv1;

	o->pos_1.x = o->priv0 / 16;

	if (o->pos_1.x >= o->priv3) {
		o->pos_1.x = o->priv3;
		o->callback = &objc_nullsub;
	}
}

// 080EE5C8
// sub_80EE5C8 (intro 8 sub)

// 080EE5E4
// sub_80EE5E4 (intro 9 sub)

// 080EE610
// objc_080EE610 (for sub_80EE5E4)

// 080EE6A4
// sub_80EE6A4 (intro 9 sub)

// 080EE704
// objc_080EE704 (for sub_80EE6A4)

// 080EE850
// sub_80EE850 (intro 9 sub)

// 080EE864
// sub_80EE864 (objc_080EE704 sub)

// 080EE8E4
// objc_080EE8E4 (for sub_80EE864)

// 080EE970
// sub_80EE970 (intro 9 sub)

// 080EE9D4
// objc_080EE9D4 (for sub_80EE970)

// 080EEA94
void sub_80EEA94(struct intro *in) { // intro 9 sub
	struct obj *o = in->field18;

	o->priv0 = 0;
	o->priv1 = 0;
	o->priv2 = 0;
	o->priv3 = 0;
	o->priv4 = 0;
	o->priv5 = 0;

	o->pos_1.x += o->pos_2.x;
	o->pos_2.x = 0;

	word_0203AB0C = 0x24;
	word_0203AB06 = 0x28;
	word_0203AB04 = 0x03;
	word_0203AB08 = 0x04;

	o->priv7 = 0x24;

	obj_anim_image_start(o, 2);
	o->callback = &objc_080EEB08;
}

// 080EEB08
// objc_080EEB08 (for sub_80EEA94)

// 080EEBE4
// sub_80EEBE4 (intro 6 sub)

// 080EEC0C
// END
