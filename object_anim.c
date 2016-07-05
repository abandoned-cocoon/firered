#include "object.h"
#include "object_anim.h"

// 03000C68
struct rotscale_state rotscale_states[32];

#define IMAGE_FRAME(o)       o->image_anims   [o->anim_number][o->anim_frame]
#define ROTSCALE_FRAME(o, c) o->rotscale_anims[c->index]      [c->subindex]

// 08231D18
void (*obj_anim_image_funcs[])(struct obj *) = {
	&obj_anim_image_continue,
	&obj_anim_image_begin
};

// 08231D20
void (*obj_anim_rotscale_funcs[])(struct obj *) = {
	&obj_anim_rotscale_continue,
	&obj_anim_rotscale_begin
};

// 08231D28
void (*anim_image_cmds[])(struct obj *) = {
	&anim_image_0_unknown,     // FFFD -3
	&anim_image_1_goto,        // FFFE -2
	&anim_image_2_stop,        // FFFF -1
	&anim_image_3_normal_frame // other
};

// 08231D38
void (*anim_rotscale_cmds[])(struct obj *) = {
	&anim_rotscale_0_unknown,     // FFFD -3
	&anim_rotscale_1_goto,        // FFFE -2
	&anim_rotscale_2_stop,        // FFFF -1
	&anim_rotscale_3_normal_frame // other
};

// 08007824
void obj_anim_step(struct obj *o) {
	bool image_begin = !!(o->bitfield & OBJ_BIT1_ANIM_IMAGE_BEGIN);
	bool rotscale_begin = !!(o->bitfield & OBJ_BIT1_ANIM_ROTSCALE_BEGIN);

	obj_anim_image_funcs   [image_begin]   (o);  if (byte_02021CCC != 0) return;
	obj_anim_rotscale_funcs[rotscale_begin](o);
}

// 0800786C
void obj_anim_image_begin(struct obj *o) {
	o->anim_frame = 0;
	o->bitfield &= ~0x10;
	o->anim_unknown_counter = 0;

	struct image_frame *frame = &IMAGE_FRAME(o);
	if (frame->data == -1) // END
		return;

	anim_image_3_normal_frame(o);
}

// 0800795C
void obj_anim_image_continue(struct obj *o) {
	u8 adc = o->anim_delay_countdown;
	if (adc & 0x3F == 0) {
		if (adc & OBJ_ANIM_IMAGE_PAUSED == 0) {
			o->anim_frame++;
			i16 data = IMAGE_FRAME(o).data;
			u16 cmd = (data < 0) ? data+3 : 3;
			(*animcmds[cmd])9g(o);
		}
	} else {
		obj_anim_delay_progress(o);
		struct image_frame *frame = &IMAGE_FRAME(o);
		if (o->oam.attr0 & 0x100 == 0) { // no rotate/scale
			obj_set_horizonal_and_vertical_flip(o, frame->hflip, frame->vflip);
		}
	}
}

// 080079FC
void anim_image_3_normal_frame(struct obj *o) {
	struct image_frame *frame = &IMAGE_FRAME(o);
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
void anim_image_2_stop(struct obj *o) {
	o->anim_frame--;
	o->bitfield |= 0x10;
}

// 08007AC0
void anim_image_1_goto(struct obj *o) {
	struct image_frame *frame;

	frame = &IMAGE_FRAME(o);
	o->anim_frame = frame->duration;

	anim_image_3_normal_frame(o);
}

// 08007B88
void anim_image_0_unknown(struct obj *o) {
	if (o->anim_unknown_counter)
		sub_08007BE0(o);
	else
		sub_08007BA8(o);
}

// 08007BA8
void sub_08007BA8(struct obj *o) {
	o->anim_unknown_counter = IMAGE_FRAME(o).duration;
	obj_anim_image_rewind_to_cmd00(o);
	obj_anim_image_continue(o);
}

// 08007BE0
void sub_08007BE0(struct obj *o) {
	o->anim_unknown_counter--;
	obj_anim_image_rewind_to_cmd00(o);
	obj_anim_image_continue(o);
}

// 08007C00
void obj_anim_image_rewind_to_cmd00(struct obj *o) {
	if (o->anim_unknown_counter > 0) {
		animseq *seq = o->anim_table[o->anim_number];
		while (seq[--o->anim_frame].data != -3 && o->anim_frame >= 0);
	}
}

// 08007C74
void obj_anim_rotscale_begin(struct obj *o) {
	if (o->sprite.attr0 & 0x100 == 0) // not rotscaled
		return;

	animseq *seq = o->rotscale_table[0];
	if (seq[0].data == 0x7FFF)
		return;

	struct rotscale_frame f;

	u8 affidx = oam_get_affidx_if_applicable(o);
	rotscale_reset(affidx);
	rotscale_load_frame(affidx, o, &f);
	o->bitfield &= ~0x08;
	o->bitfield &= ~0x20;
	sub_0800834C(affidx, &f);
	unk_03000C68[affidx].field_2 = &f[6]; // stride: 12

	if (o->bitfield & 0x80)
		obj_update_pos2(o, o->priv6, o->priv7);
}

// 08007D08
void obj_anim_rotscale_continue(struct obj *o) {
	if (o->sprite.attr0 & 0x100 == 0) // not rotscaled
		return;

	// TODO
}

// 08007DBC
void sub_08007DBC(u8 affidx, struct obj *o) {
	if (obj_anim_rotscale_delay_progress(o, affidx)) // if paused
		return;

	struct rotscale_frame frame;
	rotscale_load_frame(affidx, obj, &frame);
	rotscale_frame_apply_relative_and_sync(affidx, &frame);
}

// 08007DF0
void anim_rotscale_0_unknown(u8 affidx, struct obj *o) {
	if (rotscale_states[affidx].field_3)
		sub_08007E60(affidx, o);
	else
		sub_08007E24(affidx, o);
}

// 08007E60
void sub_08007E60(u8 affidx, struct obj *o) {
	rotscale_states[affidx].field_3--;
	obj_anim_rotscale_rewind_to_cmd00_maybe(affidx, o);
	obj_anim_rotscale_continue(affidx, o);
}

// 08007E90
// obj_rotscale_anim_rewind_to_cmd00

// 08007EFC
void anim_rotscale_1_goto(u8 affidx, struct obj *o) {
	struct rotscale_frame *frame;

	struct rotscale_state *c = &rotscale_states[affidx];
	frame = &ROTSCALE_FRAME(o, c);
	c->subindex = frame->scale_y_delta;

	anim_rotscale_3_normal_frame(affidx, o);
}

// 08007F48
void anim_rotscale_2_stop(u8 affidx, struct obj *o) {
//	TODO
// 	rotscale_frame_apply_relative_and_sync
}

// 08007F84
void anim_rotscale_3_normal_frame(u8 affidx, struct obj *o) {
//	TODO
//	rotscale_load_frame
//	sub_0800834C
//		(rotscale_frame_apply_absolute)
//		rotscale_frame_apply_relative_and_sync
}

// 08007FBC
// affine_set_indirect

// 08007FDC
// sprite_get_affidx_if_applicable

// 08007FFC
// sub_08007FFC

// 0800800C
// sub_0800800C

// 08008038
// obj_update_pos2

// 080080D4
// obj_set_horizonal_and_vertical_flip

// 08008148
// rotscale_reset_half

// 08008164
// rotscale_reset_full_1

// 0800818C
// rotscale_reset_full_2

// 080081B4
// rotscale_frame_apply_absolute

// 080081D8
void obj_anim_image_delay_progress(struct obj *o) {
	// see also: 08008200 obj_anim_rotscale_delay_progress
	struct {
		u8 delay : 6;
		u8 flag0 : 1; // don't progress in image frames
		u8 flag1 : 1; // don't progress in rotscale frames
	} *adc = &o->anim_delay_countdown;

	if (adc->flag0 == 0)
		adc->delay--;
}

// 08008200
// obj_anim_rotscale_delay_progress

// 08008230
void rotscale_frame_apply_relative_and_sync(u8 affidx, struct rotscale_frame *f) {
	struct rotscale_state *c = &rotscale_states[affidx];
	u16 coeff[4];

	c->scale_x  += f->scale_x_delta;
	c->scale_y  += f->scale_y_delta;
	c->rotation += f->rotation_delta << 8;
	c->rotation &= 0xFF00;

	struct {
		i16 scale_x;
		i16 scale_y;
		u16 rotation;
	} spec = {
		div_x10000_by(c->scale_x),
		div_x10000_by(c->scale_y),
		c->rotation
	};

	ObjAffineSet(&spec, coeff, 1, 2);
	affine_set_indirect(affidx, coeff);
}

// 080082E0
void rotscale_load_frame(u8 affidx, struct obj *o, struct rotscale_frame *f) {
	struct rotscale_state *c = &rotscale_states[affidx];
	struct rotscale_frame *f = &o.rotscale_table[c->index][c->subindex];
	r->scale_x_delta  = f->scale_x_delta;
	r->scale_y_delta  = f->scale_y_delta;
	r->rotation_delta = f->rotation_delta;
	r->duration       = f->duration;
}

// 0800834C
void sub_0800834C(struct obj *o, struct rotscale_frame *f) {

	if (f->duration > 0) {
		f->duration--;
		rotscale_frame_apply_relative_and_sync(o, f);
	} else {
		rotscale_frame_apply_absolute(o, f);
		rotscale_frame_apply_relative_and_sync(o, (struct rotscale_frame){0, 0, 0, 0, 0});
	}
}

// 0800838C
void obj_anim_image_start(struct obj *o, u8 anim_number) {
	o->anim_number = anim_number;
	o->bitfield |= OBJ_BIT1_ANIM_IMAGE_BEGIN;
}

// 080083A4
void obj_anim_image_start_if_different(struct obj *o, u8 anim_number) {
	if (o->anim_number == anim_number)
		return;
	o->anim_number = anim_number;
	o->bitfield |= OBJ_BIT1_ANIM_IMAGE_BEGIN;
}

// 080083C0
void obj_anim_image_seek(struct obj *obj, u8 frame) {
	u8 old_image_pause_flag = obj.anim_delay_countdown & OBJ_ANIM_IMAGE_PAUSED;

	obj->anim_frame = frame-1;

	obj->anim_delay_countdown &= ~OBJ_ANIM_ANY_PAUSED;

	obj->bitfield &= ~0x10;
	obj->bitfield &= ~OBJ_BIT1_ANIM_IMAGE_BEGIN;
	obj_anim_image_continue(obj);

	u8 countdown = obj->anim_delay_countdown & 0x3F;
	if (countdown) {
		obj->anim_delay_countdown &= OBJ_ANIM_ANY_PAUSED;
		obj->anim_delay_countdown |= ++countdown;
	}

	obj->anim_delay_countdown &= ~OBJ_ANIM_IMAGE_PAUSED;
	obj->anim_delay_countdown |= old_image_pause_flag;
}
