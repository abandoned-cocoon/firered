#pragma once
#include "types.h"
#include "object.h"

// 02021CCC
extern u8 byte_02021CCC;

void obj_anim_step(struct obj *o);
void obj_anim_image_begin(struct obj *o);
void obj_anim_image_continue(struct obj *o);
void anim_image_3_normal_frame(struct obj *o);
void anim_image_2_stop(struct obj *o);
void anim_image_1_goto(struct obj *o);
void anim_image_0_unknown(struct obj *o);
void sub_08007BA8(struct obj *o);
void sub_08007BE0(struct obj *o);
void obj_anim_image_rewind_to_cmd00(struct obj *o);
void obj_anim_rotscale_begin(struct obj *o);
void obj_anim_rotscale_continue(struct obj *o);
void sub_08007DBC(u8 affidx, struct obj *o);
void anim_rotscale_0_unknown(u8 affidx, struct obj *o);
void sub_08007E60(u8 affidx, struct obj *o);
void anim_rotscale_1_goto(u8 affidx, struct obj *o);
void anim_rotscale_2_stop(u8 affidx, struct obj *o);
void anim_rotscale_3_normal_frame(u8 affidx, struct obj *o);
void obj_anim_image_delay_progress(struct obj *o);
void rotscale_frame_apply_relative_and_sync(u8 affidx, struct rotscale_frame *r);
void rotscale_load_frame(u8 affidx, struct obj *o, struct rotscale_frame *r);
void sub_0800834C(u8 affidx, struct rotscale_frame *f);
void obj_anim_image_start(struct obj *o, u8 anim_number);
void obj_anim_image_start_if_different(struct obj *o, u8 anim_number);
void obj_anim_image_seek(struct obj *obj, u8 frame);
