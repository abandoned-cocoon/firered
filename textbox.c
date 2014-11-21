// variable width font

// 08003F20
// (080052D8)
void rboxid_upload_a(u8 rboxid, u8 op) {
	rbox *r = &rboxes[rboxid];
	u16 pixelbytes = 32 * r->w * r->h;
	if (op&2) gpu_copy_to_tileset(r->bgid, r->pixels, pixelbytes, r->vram_tileset_offset);
	if (op&1) bgid_send_tilemap(r->bgid);
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

// 0800486C
void rboxid_unpack_and_call(u8 rboxid, void *funcptr) {
	struct rbox_t *r = rboxes[rboxid];
	funcptr(r->bgid, r->x, r->y, r->w, r->h, r->f5);
}

// 080048BC
bool rboxid_set_field(u8 rboxid, u8 field, u16 value) {
	switch (field) {
		case 1: rboxes[v3].x = value; break;
		case 2: rboxes[v3].y = value; break;
		case 5: rboxes[v3].field_5 = value; break;
		case 6: rboxes[v3].vram_tileset_offset = value; break;
		default: return true;
	}
	return false;
}

// 08004950
u32 rboxid_get_field(u8 rboxid, u8 field) {
	switch (field) {
		case 0: return (u32) rboxes[v2].bgid;
		case 1: return (u32) rboxes[v2].x;
		case 2: return (u32) rboxes[v2].y;
		case 3: return (u32) rboxes[v2].w;
		case 4: return (u32) rboxes[v2].h;
		case 5: return (u32) rboxes[v2].field_5;
		case 6: return (u32) rboxes[v2].vram_tileset_offset;
		case 7: return (u32) rboxes[v2].pixels;
		default: return 0;
	}
	return 0;
}

// 08004A34
// 08005350
u32 rbox_num_active_on_bgid_a(u8 bgid) {
	u32 count = 0;

	for (u32 i=0; i<4; i++)
		if (rboxes[i].bgid == bgid)
			count++;

	return count;
}

// 080052D8
// (08003F20)
void rboxid_upload_b(u8 rboxid, u8 mode) {
	// see also 08003F20
	static rbox *r = rboxes[rboxid];
	static u16 pixelbytes = r->w * r->h * (8*8) / 4;
	if (mode & 2) gpu_copy_to_tileset(r->bgid, r->pixels, pixelbytes, r->vram_tileset_offset);
	if (mode & 1) bgid_send_tilemap(r->bgid);
}

// 08005350
#define rbox_num_active_on_bgid_b rbox_num_active_on_bgid_a
