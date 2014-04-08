#define NUM_OBJS 0x40
#define NUM_ROTSCALE_OBJS 0x20
#define NUM_COPY_QUEUE_ENTRIES 0x40

struct proto_t {
	u16 tile_tag;
	u16 pal_tag;
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

// from here on everything up to 08007434 and except for
// the parts marked with TODO is continuously documented
	
// 08006B5C
void objc_exec() {
	for (u8 i = 0; i<NUM_OBJS; i++) {
		obj *o = &objects[i];

		if (o->bitfield2 & OBJ_BIT2_IN_USE) o->callback(o);
		if (o->bitfield2 & OBJ_BIT2_IN_USE) obj_anim_step(o);
	}
}

// 08006BA8
void obj_sync_something() {
	write_oam_coords();
	sub_08006CB8();
	sub_08006CF8();

	u8 i = super.field_439 & 1;
	super.field_439 |= 1;

	sub_08006F04();
	sub_08006EB8();

	super.field_439 &= ~1;
	super.field_439 |= i;

	byte_02021840 = 1;
}

// 08006BF4
void write_oam_coords() {
	// TODO
}

// 08006CB8
void sub_08006CB8() {
	// TODO
}

// 08006CF8
void sub_08006CF8() {
	// TODO (big)
}

// 08006EB8
void write_rotscale_coefficients() {
	for (u8 i = 0; i<NUM_ROTSCALE_OBJS; i++) {
		obj *o = &objects[i];
		super.oam[i*4+0].rotscale = rotscale_coefficients[i].a;
		super.oam[i*4+1].rotscale = rotscale_coefficients[i].b;
		super.oam[i*4+2].rotscale = rotscale_coefficients[i].c;
		super.oam[i*4+3].rotscale = rotscale_coefficients[i].d;
	}
}

// 08006F04
void super_sprites_fill() {
	u8 super_index = 0;

	// transfer all objects into super.oam

	for (u8 i = 0; i<NUM_OBJS; i++) {
		obj *o = &objects[obj_ids_to_display[i]];

		if (o->bitfield2 & (OBJ_BIT2_IN_USE|OBJ_BIT2_INVISIBLE) == OBJ_BIT2_IN_USE) {
			if (super_sprite_add(o, &super_index) != 0)
				return;
		}
	}

	// pad the rest of super.oam with dummy entries

	while (super_index < last_super_index)
		super.oam[super_index++] = dummy_oam;
}

// 08006F8C
u8 template_instanciate_forward_search(struct proto_t *proto, u16 x, u16 y, u8 f43) {
	for (u8 i = 0; i<NUM_OBJS; i++) {
		obj *o = &objects[i];
		
		if (o->bitfield2 & OBJ_BIT2_IN_USE == 0)
			return template_read(i, proto, x, y, f43);
	}
}

// 08006FE0
u8 template_instanciate_reverse_search(struct proto_t *proto, u16 x, u16 y, u8 f43) {
	for (u8 i=NUM_OBJS-1; i>=0; i--) {
		obj *o = &objects[i];
		
		if (o->bitfield2 & OBJ_BIT2_IN_USE == 0)
			return template_read(i, proto, x, y, f43);
	}
}

// 0800704C
u8 obj_instanciate_empty_with_callback(void (*func)(u8)) {
	u8 objid = template_instanciate_forward_search(proto_empty, 0, 0, 0x1F);
	if (objid == NUM_OBJS)
		return objid;

	obj *o = &objects[i];
	o->bitfield2 |= OBJ_BIT2_INVISIBLE;
	o->callback = func;
}

// 08007094
u8 template_read(u8 oid, struct proto_t *proto, u16 x, u16 y, u8 arg4) {
	struct obj_t *o = objects[oid];
	obj_delete(o);
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
		obj_t *o = &objects[i];

		if (o->bitfield2 & OBJ_BIT2_IN_USE)
			continue;

		i = template_read(i, proto, x, y, arg3);

		// see objc_exec
		if (i < NUM_OBJS) {
			o->callback(o);
			if (o->bitfield2 & OBJ_BIT2_IN_USE)
				obj_anim_step(o);
		}

		return i;
	}
	return i;
}

// 08007280 
void obj_delete_and_free_vram(struct obj_t *o) {
	if (o->bitfield2 & OBJ_BIT2_IN_USE == 0)
		return;

	if (o->bitfield2 & OBJ_BIT2_SHARES_RESOURCES == 0) {
		// in this case NOW is the time to release
		// these resources that are owned by the
		// object (as opposed to 'shared resources')

		// TODO
		// clear bits in 'gpu_tile_allocation_map_obj'
	}

	obj_delete(o);
}

// 080072E8
void super_sprites_delete_all(u8 start, u8 end) {
	for (u8 i=start; i<end; i++)
		super.oam[i] = oam_dummy;
}

// 08007320
void gpu_sprites_upload() {
	if (super.gpu_sprites_upload_skip & 1)
		return;

	CpuSet(super.oam, 0x07000000, 0x4000000 | (sizeof(super.oam)>>2));
}

// 08007350
void copy_queue_clear() {
	struct copy_queue_entry default_element = {0, 0, 0, 0};

	byte_02021840 = 0;
	copy_queue_cursor = 0;

	for (u8 i=0; i < NUM_COPY_QUEUE_ENTRIES; i++)
		copy_queue[i] = default_element;
}

// 08007390
void rotscale_coefficients_reset() {
	struct rotscale default_element = { 0x100, 0, 0, 0x100 };

	for (u8 i = 0; i<NUM_ROTSCALE_OBJS; i++)
		rotscale_coefficients[i] = default_element;
}

// 080073BC
void rotscale_set(u8 i, u16 a, u16 b, u16 c, u16 d) {
	rotscale_coefficients[i] = (struct rotscale){a, b, c, d};
}

// 080073DC
void obj_delete(struct obj_t *o) {
	memcpy(o, obj_empty, sizeof(struct obj));
}

// 080073F0
void obj_center(struct obj_t *o, u8 shape, u8 size, u32 oamflags) {
	coords8 *c = &negative_half_oam_size[shape*4+size];
	if(oamflags & DOUBLESIZE) {
		o->pos_neg_center.x = c->x*2;
		o->pos_neg_center.y = c->y*2;
	} else {
		o->pos_neg_center.x = c->x;
		o->pos_neg_center.y = c->y;
	}
}

// 08007434
// gpu_tile_obj_alloc

// TODO

// 0800766C
void copy_queue_add_oamframe(u16 idx, u16 oam_attr2, struct gfxentry_t *gfx_table) {
	u8 i = copy_queue_cursor;
	if (i >= 0x40) return;
	copy_queue[i].src = gfx_table[idx]->data;
	copy_queue[i].dst = 0x06010000 + (oam_attr2<<5);
	copy_queue[i].len = gfx_table[idx]->size;
}
