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

#define OAM_FRAME(o) o->anim_table[o->anim_number][o->anim_frame]

// 0800795C
void anim_player_1(struct oamt *o) {
	if (o->field_2C & o->bitfield == 0) {
		if (o->field_2C & 0x40 == 0) {
			o->anim_frame++;
			i16 data = OAM_FRAME(o).data;
			u16 cmd = (data < 0) ? (data-0xFFFD) : 3;
			animcmds[cmd](o);
		}
	} else {
		sub_80081D8(o);
		struct animframe_t *frame = &OAM_FRAME(o);
		if (o->oam.attr0 & 0x100 == 0) { // no rotate/scale
			oamt_set_horizonal_and_vertical_flip(o, frame->hflip, frame->vflip);
		}
	}
}

// 08007AA8
void animcmd_02_stop(struct oamt *o) {
	o->anim_frame--;
	o->bitfield |= 0x10;
}

void animcmd_03_normal_frame(struct oamt *o) {
	struct animframe_t *frame = &OAM_FRAME(o);
	u32 data 	= frame->data;
	u8 duration = frame->duration;

	if (duration) duration--;
	o->field_2C &= 0xC0;
	o->field_2C |= 0x3F & duration;
	
	if (o->oam.attr0 & 0x100 == 0) { // no rotate/scale
		oamt_set_horizonal_and_vertical_flip(o, frame->hflip, frame->vflip);
	}
	if (o->bitfield & 0x40) {
		data += o->anime_data_offset;
		o->oam.attr2 &= 0xFC00;
		o->oam.attr2 |= 0x03FF & data;
	} else {
		sub_800766C(data);
	}
}

// 08231D28
void (*animcmds[])(struct oamt *) = {
	&animcmd_00_unknown,
	&animcmd_01_loop_point,
	&animcmd_02_stop,
	&animcmd_03_normal_frame
};