#include "main.h" // for sprite
#include "object.h"
#include "uncategorized.h"

#ifndef NO_RAM
// 0202063C
struct obj objects[NUM_OBJS];
#endif


// TODO
extern struct sprite sprite_empty;

// 08231D00
extern struct proto_t proto_empty;

// 08231CA0
extern struct obj obj_empty;

// 02021800
extern u8 obj_ids_to_display[0x40];
// 02021840
extern u8 byte_02021840;
// 02021841
extern u8 copy_queue_cursor;
// 02021844
extern struct copy_queue_entry {
	void *src;
	void *dst;
	u16 len;
	u16 padding;
} copy_queue[64];
// 02021B44
extern u8 last_sprite_index;
// 02021B46
extern u16 gpu_tile_allocation_lower_boundary_obj;



// from here on everything up to 08007434 and except for
// the parts marked with TODO is continuously documented

// 08006B10
void obj_and_aux_reset_all() {
	super_sprites_delete_all(0x00, 0x80);
	obj_delete_all();
	copy_queue_clear();
	rotscale_reset_all();
	gpu_tile_obj_tags_reset();
	last_sprite_index = 0x40;
	gpu_tile_allocation_lower_boundary_obj = 0;
	gpu_tile_obj_alloc(0); // clears all allocations
	global_sprite_displace.x = 0;
	global_sprite_displace.y = 0;
}

// 08006B5C
void objc_exec() {
	for (u8 i = 0; i<NUM_OBJS; i++) {
		struct obj *o = &objects[i];

		if (o->bitfield2 & OBJ_BIT2_IN_USE) o->callback(o);
		if (o->bitfield2 & OBJ_BIT2_IN_USE) obj_anim_step(o);
	}
}

// 08006BA8
void obj_sync_something() {
	write_oam_coords();
	sub_08006CB8();
	sub_08006CF8();

	u8 i = super.gpu_sprites_upload_skip & 1;
	super.gpu_sprites_upload_skip |= 1;

	sub_08006F04();
	sub_08006EB8();

	super.gpu_sprites_upload_skip &= ~1;
	super.gpu_sprites_upload_skip |= i;

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
		struct obj *o = &objects[i];
		super.sprites[i*4+0].rotscaleinfo = rotscale_coefficients[i].a;
		super.sprites[i*4+1].rotscaleinfo = rotscale_coefficients[i].b;
		super.sprites[i*4+2].rotscaleinfo = rotscale_coefficients[i].c;
		super.sprites[i*4+3].rotscaleinfo = rotscale_coefficients[i].d;
	}
}

// 08006F04
void super_sprites_fill() {
	u8 sprite_index = 0;

	// transfer all objects into super.sprites

	for (u8 i = 0; i<NUM_OBJS; i++) {
		struct obj *o = &objects[obj_ids_to_display[i]];

		if (o->bitfield2 & (OBJ_BIT2_IN_USE|OBJ_BIT2_INVISIBLE) == OBJ_BIT2_IN_USE) {
			if (super_sprite_add(o, &sprite_index) != 0)
				return;
		}
	}

	// pad the rest of super.sprites with dummy entries

	while (sprite_index < last_sprite_index)
		super.sprites[sprite_index++] = sprite_empty;
}

// 08006F8C
u8 template_instanciate_forward_search(struct proto_t *proto, u16 x, u16 y, u8 f43) {
	for (u8 i = 0; i<NUM_OBJS; i++) {
		struct obj *o = &objects[i];
		
		if (o->bitfield2 & OBJ_BIT2_IN_USE == 0)
			return template_read(i, proto, x, y, f43);
	}
}

// 08006FE0
u8 template_instanciate_reverse_search(struct proto_t *proto, u16 x, u16 y, u8 f43) {
	for (u8 i=NUM_OBJS-1; i>=0; i--) {
		struct obj *o = &objects[i];
		
		if (o->bitfield2 & OBJ_BIT2_IN_USE == 0)
			return template_read(i, proto, x, y, f43);
	}
}

// 0800704C
u8 obj_instanciate_empty_with_callback(void (*func)(struct obj*)) {
	u8 objid = template_instanciate_forward_search(&proto_empty, 0, 0, 0x1F);
	if (objid == NUM_OBJS)
		return objid;

	struct obj *o = &objects[objid];
	o->bitfield2 |= OBJ_BIT2_INVISIBLE;
	o->callback = func;
	return objid;
}

#define BYTES_PER_TILE (8*8/2)

// 08007094
u8 template_read(u8 oid, struct proto_t *proto, u16 x, u16 y, u8 arg4) {
	struct obj *o = &objects[oid];
	obj_delete(o);
	o->bitfield2 |= OBJ_BIT2_IN_USE;
	/* TODO
	if (false) {
		u16 size = proto->gfx_table[0].size;
		u16 tile_id = gpu_tiles_allocate(size/BYTES_PER_TILE);
		o->sprite.tile_id = tile_id;
	}*/
}

// 080071EC
u8 template_instanciate_and_run_once(struct proto_t *proto, u16 x, u16 y, u8 arg3) {
	u8 i;
	for (i = 0; i<NUM_OBJS; i++) {
		struct obj *o = &objects[i];

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
void obj_delete_and_free_tiles(struct obj *o) {
	if (o->bitfield2 & OBJ_BIT2_IN_USE == 0)
		return;

	if (o->bitfield & OBJ_BIT1_SHARES_RESOURCES == 0) {
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
		super.sprites[i] = sprite_empty;
}

// 08007320
void gpu_sprites_upload() {
	if (super.gpu_sprites_upload_skip & 1)
		return;

	CpuSet(super.sprites, (void*)0x07000000, 0x4000000 | (sizeof(super.sprites)>>2));
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
void rotscale_reset_all() {
	struct rotscale_coeff default_element = { 0x100, 0, 0, 0x100 };

	for (u8 i = 0; i<NUM_ROTSCALE_OBJS; i++)
		rotscale_coefficients[i] = default_element;
}

// 080073BC
void rotscale_set(u8 i, u16 a, u16 b, u16 c, u16 d) {
	rotscale_coefficients[i] = (struct rotscale_coeff){a, b, c, d};
}

// 080073DC
void obj_delete(struct obj *o) {
	memcpy(o, &obj_empty, sizeof(struct obj));
}

// 08231C85
coords8 negative_half_oam_size[] = {
	{ -4,  -4},
	{ -8,  -8},
	{-16, -16},
	{-32, -32},

	{ -8,  -4},
	{-16,  -4},
	{-16,  -8},
	{-32, -16},

	{ -4,  -8},
	{ -4, -16},
	{ -8, -16},
	{-16, -32}
};

// 080073F0
void obj_center(struct obj *o, u8 shape, u8 size, u32 oamflags) {
	coords8 *c = &negative_half_oam_size[shape*4+size];
	if(oamflags & 2 /*DOUBLESIZE*/) {
		o->pos_neg_center.x = c->x*2;
		o->pos_neg_center.y = c->y*2;
	} else {
		o->pos_neg_center.x = c->x;
		o->pos_neg_center.y = c->y;
	}
}

// 08007434
// gpu_tile_obj_alloc

// 08007550
// gpu_tile_allocation_obj_edit

// 080075C0
// gpu_tile_obj_free_by_range_probably

// 0800760C
// objc_nullsub

// 08007610
// copy_queue_process

// 0800766C
void copy_queue_add_oamframe(u16 idx, u16 oam_attr2, struct gfxentry_t *gfx_table) {
	u8 i = copy_queue_cursor;
	if (i >= 0x40) return;
	copy_queue[i].src = gfx_table[idx].data;
	copy_queue[i].dst = ((u8*)0x06010000) + (oam_attr2<<5);
	copy_queue[i].len = gfx_table[idx].size;
}

// 080076D0
// copy_queue_add

// 08007720
// copy_oamt_states_to_R0 (unused)

// 08007748
// obj_delete_all_bytewise

// 08007770
// obj_delete_all

// 080077AC
// obj_free_tiles_by_tag

// 080077C8
// obj_free_pal_by_tag

// 080077D8
// obj_free_rotscale_entry

// 08007804
void obj_delete_and_free_associated_resources(struct obj *o) {
	obj_free_tiles_by_tag(o);
	obj_free_pal_by_tag(o);
	obj_free_rotscale_entry(o);
	obj_delete_and_free_tiles(o);
}
