struct fbox {
    u16 (*function)(dialog*);
    char field_4;
    char x;
    char y;
    char field_7;
    char w;
    char h;
    u16 field_A;
};

u16 fbox0_func(dialog *rm) {
	dialogsub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 0;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox1_func(dialog *rm) {
	dialogsub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 1;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox2_func(dialog *rm) {
	dialogsub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 2;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox3_func(dialog *rm) {
	dialogsub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 3;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox4_func(dialog *rm) {
	dialogsub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 4;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

u16 fbox5_func(dialog *rm) {
	dialogsub *rs = &rm->sub;
    if ( !(rs->field_1 & 0x80) ) {
        rs->ft_lo = 5;
        rs->field_1 |= 0x80u;
    }
    return font_render_b(rm);
}

