#include "dialog.h"
#include "fbox.h"

u16 fbox0_func(struct dialog *rm) {
	struct dialogsub *rs = &rm->sub;
	if ( !(rs->field_1 & 0x80) ) {
		rs->ft_lo = 0;
		rs->field_1 |= 0x80u;
	}
	return font_render_b(rm);
}

u16 fbox1_func(struct dialog *rm) {
	struct dialogsub *rs = &rm->sub;
	if ( !(rs->field_1 & 0x80) ) {
		rs->ft_lo = 1;
		rs->field_1 |= 0x80u;
	}
	return font_render_b(rm);
}

u16 fbox2_func(struct dialog *rm) {
	struct dialogsub *rs = &rm->sub;
	if ( !(rs->field_1 & 0x80) ) {
		rs->ft_lo = 2;
		rs->field_1 |= 0x80u;
	}
	return font_render_b(rm);
}

u16 fbox3_func(struct dialog *rm) {
	struct dialogsub *rs = &rm->sub;
	if ( !(rs->field_1 & 0x80) ) {
		rs->ft_lo = 3;
		rs->field_1 |= 0x80u;
	}
	return font_render_b(rm);
}

u16 fbox4_func(struct dialog *rm) {
	struct dialogsub *rs = &rm->sub;
	if ( !(rs->field_1 & 0x80) ) {
		rs->ft_lo = 4;
		rs->field_1 |= 0x80u;
	}
	return font_render_b(rm);
}

u16 fbox5_func(struct dialog *rm) {
	struct dialogsub *rs = &rm->sub;
	if ( !(rs->field_1 & 0x80) ) {
		rs->ft_lo = 5;
		rs->field_1 |= 0x80u;
	}
	return font_render_b(rm);
}

