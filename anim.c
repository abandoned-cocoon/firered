struct gfxentry_t {
	void *data;
	u16 size;
	u16 padding;
};

struct animframe_t {
	u16 data;
	union {
		u16 _rd;
		struct {
			u6 duration;
			u1 hflip;
			u1 vflip;
			u8 padding;
		};
	};
};
typedef struct animframe_t *anim_t;
typedef struct anim_t *animtable_t;

#define OBJ_FRAME(o) o->anim_table[o->anim_number][o->anim_frame]

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

// 080073DC
void obj_delete(struct obj *o) {
	memcpy(o, obj_empty, sizeof(struct obj));
}

// 080073F0
void obj_center(struct obj *o, u8 shape, u8 size, u32 oamflags) {
	coords8 *c = &negative_half_oam_size[shape*4+size];
	if(oamflags & DOUBLESIZE) {
		o->pos_neg_center.x = c->x*2;
		o->pos_neg_center.y = c->y*2;
	} else {
		o->pos_neg_center.x = c->x;
		o->pos_neg_center.y = c->y;
	}
}

// 0800766C
void copy_queue_add_oamframe(u16 idx, u16 oam_attr2, struct gfxentry_t *gfx_table) {
	u8 i = copy_queue_cursor;
	if (i >= 0x40) return;
	copy_queue[i].src = gfx_table[idx]->data;
	copy_queue[i].dst = 0x06010000 + (oam_attr2<<5);
	copy_queue[i].len = gfx_table[idx]->size;
 }

// 0800786C
void anim_player_2(struct obj *o) {
	o->anim_frame = 0; // ??
	o->bitfield &= ~0x10;
	o->field_2D = 0;
	// todo
	struct animframe_t *frame = &OBJ_FRAME(o);
	if (frame->data == -1) // END
		return;

	animcmd_03_normal_frame(o);
}

// 0800795C
void anim_player_1(struct obj *o) {
	u8 adc = o->anim_delay_countdown;
	if (adc & 0x3F == 0) {
		if (adc & 0x40 == 0) {
			o->anim_frame++;
			i16 data = OBJ_FRAME(o).data;
			u16 cmd = (data < 0) ? data+3 : 3;
			animcmds[cmd](o);
		}
	} else {
		oam_anim_delay_progress(o);
		struct animframe_t *frame = &OBJ_FRAME(o);
		if (o->oam.attr0 & 0x100 == 0) { // no rotate/scale
			obj_set_horizonal_and_vertical_flip(o, frame->hflip, frame->vflip);
		}
	}
}

// 080079FC
void animcmd_03_normal_frame(struct obj *o) {
	struct animframe_t *frame = &OBJ_FRAME(o);
	u32 data 	= frame->data;
	u8 duration = frame->duration;

	if (duration) duration--;
	o->anim_delay_countdown &= 0xC0;
	o->anim_delay_countdown |= 0x3F & duration;

	if (o->oam.attr0 & 0x100 == 0) { // no rotate/scale
		obj_set_horizonal_and_vertical_flip(o, frame->hflip, frame->vflip);
	}
	if (o->bitfield & 0x40) {
		// set oam-start-tile to data + data_offset
		data += o->anim_data_offset;
		o->oam.attr2 &= 0xFC00;
		o->oam.attr2 |= 0x03FF & data;
	} else {
		copy_queue_add_oamframe(data, o->oam.attr2, o->gfx_table);
	}
}

// 08007AA8
void animcmd_02_stop(struct obj *o) {
	o->anim_frame--;
	o->bitfield |= 0x10;
}

// 08231D28
void (*animcmds[])(struct obj *) = {
	&animcmd_00_unknown,     // FFFD -3
	&animcmd_01_loop_point,  // FFFE -2
	&animcmd_02_stop,        // FFFF -1
	&animcmd_03_normal_frame // other
};