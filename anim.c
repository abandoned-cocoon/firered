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
			u8 duration : 6; // or goto-target
			u8 hflip    : 1;
			u8 vflip    : 1;
			u8 padding  : 8;
		};
	};
};

struct affine_frame {
	u16 scale_x_delta;
	u16 scale_y_delta;
	u8  rotation_delta;
	u8  field_5;
	u16 _padding;
};

struct affine_state {
	u8 index;
	u8 subindex;
	u16 _padding1;
	u16 scale_x_inv;
	u16 scale_y_inv;
	u16 rotation;
	u16 _padding2;
};

typedef struct animframe *animseq;

#define OBJ_FRAME(o) o->anim_table[o->anim_number][o->anim_frame]

// 08231D18
void (*anim_players_a[])(struct obj *) = {
	&anim_player_a_0,
	&anim_player_a_1
};

// 08231D20
void (*anim_players_b[])(struct obj *) = {
	&anim_player_b_0,
	&anim_player_b_1
};

// 08231D28
void (*animcmds[])(struct obj *) = {
	&animcmd_00_unknown,     // FFFD -3
	&animcmd_01_loop_point,  // FFFE -2
	&animcmd_02_stop,        // FFFF -1
	&animcmd_03_normal_frame // other
};

// 08007824
void obj_anim_step(struct obj *o) {
	bool playertype_a = !!(o->bitfield & OBJ_BIT1_PLAYERTYPE_A);
	bool playertype_b = !!(o->bitfield & OBJ_BIT1_PLAYERTYPE_B);

	anim_players_a[playertype_a](o);

	if (byte_02021CCC == 0)
		anim_players_b[playertype_b](o);
}

// 0800786C
void anim_player_a_1(struct obj *o) {
	o->anim_frame = 0;
	o->bitfield &= ~0x10;
	o->anim_unknown_counter = 0;

	struct animframe *frame = &OBJ_FRAME(o);
	if (frame->data == -1) // END
		return;

	animcmd_03_normal_frame(o);
}

// 0800795C
void anim_player_a_0(struct obj *o) {
	u8 adc = o->anim_delay_countdown;
	if (adc & 0x3F == 0) {
		if (adc & OBJ_ANIM_PAUSED == 0) {
			o->anim_frame++;
			i16 data = OBJ_FRAME(o).data;
			u16 cmd = (data < 0) ? data+3 : 3;
			animcmds[cmd](o);
		}
	} else {
		obj_anim_delay_progress(o);
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
	if (o->bitfield & OBJ_ANIM_PAUSED) {
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

// 08007AC0
void animcmd_01_loop_point(struct obj *o) {
	struct animframe *frame;

	frame = &OBJ_FRAME(o);
	o->anim_frame = frame.duration;

	animcmd_03_normal_frame(o);
}

// 08007B88
void animcmd_00_unknown(struct obj *o) {
	if (o->anim_unknown_counter)
		sub_08007BE0(o);
	else
		sub_08007BA8(o);
}

// 08007BA8
void sub_08007BA8(struct obj *o) {
	o->anim_unknown_counter = OBJ_FRAME(o).duration;
	obj_anim_rewind_to_cmd00(o);
	anim_player_a_0(o);
}

// 08007BE0
void sub_08007BE0(struct obj *o) {
	o->anim_unknown_counter--;
	obj_anim_rewind_to_cmd00(o);
	anim_player_a_0(o);
}

// 08007C00
void obj_anim_rewind_to_cmd00(struct obj *o) {
	if (o->anim_unknown_counter > 0) {
		animseq *seq = o->anim_table[o->anim_number];
		while (seq[--o->anim_frame].data != -3 && o->anim_frame >= 0);
	}
}

// 08007C74
void anim_player_b_1(struct obj *o) {
	if (o->sprite.attr0 & 0x100 == 0) // not rotscaled
		return;

	animseq *seq = o->affine_table[0];
	if (seq[0].data == 0x7FFF)
		return;

	struct affine_frame f;

	u8 rotscale_index = oam_get_rotscale_index_if_applicable(o);
	affine_reset(rotscale_index);
	affine_load_frame(rotscale_index, o, &f);
	o->bitfield &= ~0x08;
	o->bitfield &= ~0x20;
	sub_0800834C(rotscale_index, &f);
	unk_03000C68[rotscale_index].field_2 = &f[6]; // stride: 12

	if (o->bitfield & 0x80)
		obj_update_pos2(o, o->priv6, o->priv7);
}

// 08007D08
void anim_player_b_0(struct obj *o) {

}

// 08007DBC
void sub_08007DBC() {
	//
}

// ...

// 080081D8
void obj_anim_delay_progress(struct obj *o) {
	struct {
		u8 delay : 6;
		u8 flag0 : 1;
		u8 flag1 : 1;
	} *adc = &o->anim_delay_countdown;

	if (adc->flag0 == 0)
		adc->delay--;
}

// 03000C68
struct affine_state affine_states[32];

// 08008230
void sub_08008230(u8 rotscale_index, struct affine_frame *r) {
	struct affine_state *c = &affine_states[rotscale_index];
	u16 coeff[4];
	ObjAffineSet(&spec, coeff, 1, 2);
	rotscale_set_indirect(rotscale_index, coeff);
}

// 080082E0
void affine_load_frame(u8 rotscale_index, struct obj *o, struct affine_frame *r) {
	struct affine_state *c = &affine_states[rotscale_index];
	struct affine_frame *f = &o.affine_table[c->index][c->subindex];
	r->scale_x_delta  = f->scale_x_delta;
	r->scale_y_delta  = f->scale_y_delta;
	r->rotation_delta = f->rotation_delta;
	r->field_5        = f->field_5;
}
