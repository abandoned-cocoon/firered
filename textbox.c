// variable width font

// 0800486C
void rboxid_unpack_and_call(u8 rboxid, void *funcptr) {
	struct rbox_t *r = rboxes[rboxid]
	funcptr(rboxid, r->x, r->y, r->w, r->h, r->f5)
}

// 080044A8
void rboxid_vertical_scroll(u8 rboxid, u8 direction, u8 delta, u8 background_doublepixel) {

	// direction: 0=up 1=down

	u32 background_row = background_doublepixel * 0x01010101;

	rbox *rbx = &rboxes[rboxid];
	u32 *rrows = (u32*)rbx->pixels; // 4 bytes = 8 nibbles = 8 pixels at 4bpp
	u32 size = rbx->w*rbx->h * (8*8/2);
	u32 rbxw = rbx->w;

	#define SAFE_RROW(index) (((index)*sizeof(u32) >= size) ? background_row : rrows[index])
	#define REBASE(o) (((o)/8*rbxw)+((o)&7))

	if (reversed == 0) {
		for (u32 c=0; c<size/4; c+=8) {
			rrows[c+0] = SAFE_RROW(c+REBASE(delta+0));
			rrows[c+1] = SAFE_RROW(c+REBASE(delta+1));
			rrows[c+2] = SAFE_RROW(c+REBASE(delta+2));
			rrows[c+3] = SAFE_RROW(c+REBASE(delta+3));
			rrows[c+4] = SAFE_RROW(c+REBASE(delta+4));
			rrows[c+5] = SAFE_RROW(c+REBASE(delta+5));
			rrows[c+6] = SAFE_RROW(c+REBASE(delta+6));
			rrows[c+7] = SAFE_RROW(c+REBASE(delta+7));
		}
	} else if (reversed == 1) {
		// same as above, but with reverse memory access order
	}
}