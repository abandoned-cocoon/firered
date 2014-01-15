#define NUM_OBJS 0x40

struct proto_t {
	u16 field_0;
	u16 pal_num_maybe;
	gpu_sprite *sprite; // a.k.a. OAM
	struct animtable_t *anim_table_1;
	struct gfxentry_t  *gfx_table;
	struct animtable_t *anim_table_2;
	void *callback;
};

struct obj_t {
	gpu_sprite sprite; // a.k.a. OAM
	struct animtable_t *anim_table_1;
	struct gfxentry_t  *gfx_table;
	struct animtable_t *anim_table_2;
	struct proto_t *template;
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
		// 0x1  - horizontal flip
		// 0x2  - vertical flip
		// 0x40 - disable memory allocation (for tiles and palettes)
	u16 anim_data_offset;
	u8 field_42;
	u8 field_43;
};

// 0202063C
struct obj_t objects[NUM_OBJS];

// 08007094
u8 template_read(u8 oid, struct proto_t *proto, u16 x, u16 y, u8 arg4) {
	struct obj_t *o = objects[oid];
	obj_purge(o);
	o.bitfield2 |= OBJ_BIT2_IN_USE;
	// TODO
	if () {
		u16 size = proto->gfx_table[0].size;
		u16 tile_id = gpu_tiles_allocate(size/BYTES_PER_TILE);
		o->sprite.tile_id = tile_id;
	}
}

// 080071EC
u8 template_instanciate_and_run_once(struct proto_t *proto, u16 x, u16 y, u8 arg3) {

	for (u8 i = 0; i<NUM_OBJS; i++) {
		void *o = &objects[i];

		if (o->bitfield2 & OBJ_BIT2_IN_USE)
			continue;

		i = template_read(i, proto, x, y, arg3);

		if (i < NUM_OBJS) {
			o->callback(o);
			if (o->bitfield2 & OBJ_BIT2_IN_USE)
				sub_08007824(o);
		}

		return i;
	}
	return i;
}
