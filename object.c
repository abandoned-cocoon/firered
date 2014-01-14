#define NUM_OBJS 0x40

struct obj_t {
	gpu_sprite sprite; // a.k.a. OAM
	struct animtable_t *anim_table;
	struct gfxentry_t  *gfx_table;
	void *field_10;
	void *template;
	void *field_18;
	void *callback;
	coords16 pos_1;
	coords16 pos_2;
	coords8  pos_neg_center;
	u8 anim_number;
	u8 anim_frame;
	u8 anim_delay_countdown; // top two bits used as flags
	u8 priv_1;
	u16 priv2;
	u16 priv3;
	u16 priv4;
	u16 priv5;
	u16 priv6;
	u16 priv7;
	u16 priv8;
	u16 priv9;
	u8 bitfield2;
		// 0x1 - in use
		// 0x4 - invisible
	u8 bitfield;
	u16 anim_data_offset;
	u8 field_42;
	u8 field_43;
};

struct obj_t objects[NUM_OBJS];


// 080071EC
u8 template_instanciate(void *template, u16 x, u16 y, u8 arg3) {

	for (u8 i = 0; i<NUM_OBJS; i++) {
		void *o = &objects[i];

		if (o->bitfield2 & OBJ_BIT2_IN_USE)
			continue;

		i = template_read(i, template, x, y, arg3);

		if (i < NUM_OBJS) {
			o->callback(o);
			if (o->bitfield2 & OBJ_BIT2_IN_USE)
				sub_08007824(o);
		}

		return i;
	}
	return i;
}
