struct fbox {
  void *function;
  char field_4;
  char x;
  char y;
  char field_7;
  char w;
  char h;
  u16 field_A;
};

struct remosub {
	union {
		char font_type;
		struct {
			u8 ft_lo : 4;
			u8 ft_hi : 4;
		};
	}
	char field_1;
	u16 frames_visible_counter;
	int field_4;
};

struct remo {
	void *char_cursor;
	u8 rbox_id;
	u8 fbox_id;
	u8 menu_cursor_1a;
	u8 menu_cursor_1b;
	u8 menu_cursor_2a;
	u8 menu_cursor_2b;
	u8 field_A;
	u8 field_B;
	u8 field_C;
	u8 field_D;
	u8 gap_E[2];
	u32 field_10;
	remosub sub;
	u8 mode;
	u8 text_speed;
	u8 field_1E;
	u8 field_1F;
	u8 field_20;
	u8 japanese;
};

u16 fbox0_func(remo *rm) {
	remosub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 0;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox1_func(remo *rm) {
	remosub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 1;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox2_func(remo *rm) {
	remosub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 2;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox3_func(remo *rm) {
	remosub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 3;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox4_func(remo *rm) {
	remosub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 4;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox5_func(remo *rm) {
	remosub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 5;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

