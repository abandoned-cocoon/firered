struct gfxentry {
	void *data;
	u16 size;
	u16 tag;
};

struct animframe {
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

typedef struct animframe *animtable;

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

// 0800786C
void anim_player_2(struct obj *o) {
	o->anim_frame = 0; // ??
	o->bitfield &= ~0x10;
	o->field_2D = 0;
	// todo
	struct animframe *frame = &OBJ_FRAME(o);
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
		struct animframe *frame = &OBJ_FRAME(o);
		if (o->oam.attr0 & 0x100 == 0) { // no rotate/scale
			obj_set_horizonal_and_vertical_flip(o, frame->hflip, frame->vflip);
		}
	}
}

// 080079FC
void animcmd_03_normal_frame(struct obj *o) {
	struct animframe *frame = &OBJ_FRAME(o);
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