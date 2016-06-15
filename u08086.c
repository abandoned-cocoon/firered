#include "gpu.h"
#include "task.h"
#include "uncategorized.h"

struct obj;

typedef void (*vblank_cb)(void);

// TODO
void ve_ll4_f_0(struct task_t*); // 08086498
void ve_ll4_f_1(struct task_t*); // 080864D4
void ve_ll4_f_2(struct task_t*); // 08086554
void ve_ll4_f_3(struct task_t*); // 080865C0
void ve_ll4_f_4(struct task_t*); // 080865F0
void ve_ll4_f_5(struct task_t*); // 08086630
void ve_ll4_f_6(struct task_t*); // 08086650
void sub_8086728(struct task_t* t); // 08086728
void sub_80868C0(struct obj *); // 080868C0
extern u8 pal_83CBB10[];

// 083CC138
void (*ve_ll4_funcs[])(struct task_t*) = {
	&ve_ll4_f_0,
	&ve_ll4_f_1,
	&ve_ll4_f_2,
	&ve_ll4_f_3,
	&ve_ll4_f_4,
	&ve_ll4_f_5,
	&ve_ll4_f_6,
};

// 08086468
void taskFF_ve_ll4(u8 tid) {
	(*ve_ll4_funcs[task[tid].priv[0]])(&task[tid]);
}

// 080866E0
void vblank_80866E0() {
	vblank_cb *vbcb;
	struct task_t *t = &task[task_find_id_by_funcptr(taskFF_ve_ll4)];

	read_unaligned_16(&t->priv[13], &vbcb);
	(*vbcb)();

	lcd_io_set(16 /*BG0HOFS*/, t->priv[1]);
	lcd_io_set(18 /*BG0VOFS*/, t->priv[2]);
}

// 08086498
void ve_ll4_f_0(struct task_t *t) {
	lcd_io_set(16 /*BG0HOFS*/, t->priv[1]);
	lcd_io_set(18 /*BG0VOFS*/, t->priv[2]);
	write_unaligned_16(&t->priv[13], super.vblank_callback);
	vblank_handler_set(vblank_80866E0);
	t->priv[0]++;
}

// 080864D4
void ve_ll4_f_1(struct task_t *t) {
	u32 zero = 0;

	u16 bg0cnt = lcd_io_get(0);
	u32 character_base = ((bg0cnt >> 2) &  3) << 14;
	u32 screen_base    = ((bg0cnt >> 8) & 31) << 11;

	t->priv[12] = screen_base;

	CpuSet((u8*)0x083CBA90, (u8*)0x06000000 + character_base, 0x40 | CPU_SET_HALF);
	CpuSet(&zero,           (u8*)0x06000000 + screen_base,   0x200 | CPU_SET_WORD | CPU_SET_FILL);
	gpu_pal_apply(pal_83CBB10, 240, 32);

	t->priv[0]++;
}

// 08086554
void ve_ll4_f_2(struct task_t *t) {

	if ( sub_8086738(t) << 24 ) {
		u16 winin = t->priv[5] = lcd_io_get(0x48);

		// enable WIN1 for BG0 and OBJ
		lcd_io_set(0x48, winin | 0x1100);

		lcd_io_set(0x42 /* WIN1H */ , 0x00F0); //  0 - 240
		lcd_io_set(0x46 /* WIN1V */ , 0x2878); // 40 - 120

		objects[t->priv[15]].callback = &sub_80868C0;

		++t->priv[0];
	}
	sub_8086728(t);
}

void ve_ll4_f_3(struct task_t *t) {
}

void ve_ll4_f_4(struct task_t *t) {
}

void ve_ll4_f_5(struct task_t *t) {
}

// 08086650
void ve_ll4_f_6(struct task_t *t) {
	vblank_cb *vbcb_old;

	read_unaligned_16(&t->priv[13], &vbcb_old);
	vblank_handler_set(vbcb_old);

	bgid_mod_x_offset(0, 0, 0);
	bgid_mod_y_offset(0, 0, 0);

	sub_80F77B8();
	sub_8083A5C(&objects[t->priv[15]], t->priv[15] & 0xFF);

	oe_active_list_remove(6);

	task_del(task_find_id_by_funcptr(&taskFF_ve_ll4));
}

// 08086728
void sub_8086728(struct task_t* t) {
	t->priv[1] -= 16;
	t->priv[3] += 16;
}

// 080868C0
void sub_80868C0(struct obj *t) {
	// TODO
}
