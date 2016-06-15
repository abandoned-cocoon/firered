#include "rbox.h"
#include "dialog.h"

struct rbox rboxes[32]; // 020204B4

// variable width font

// 08003F20
// (080052D8)
void rboxid_upload_a(u8 rboxid, u8 op) {
	struct rbox *r = &rboxes[rboxid];
	u16 pixelbytes = 32 * r->w * r->h;
	if (op&2) gpu_copy_to_tileset(r->bg_id, r->pixels, pixelbytes, r->vram_tileset_offset);
	if (op&1) bgid_send_tilemap(r->bg_id);
}

// 080044A8
void rboxid_vertical_scroll(u8 rboxid, u8 direction, u8 delta, u8 background_doublepixel) {

	// direction: 0=up 1=down

	u32 background_row = background_doublepixel * 0x01010101;

	struct rbox *rbx = &rboxes[rboxid];
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
void rboxid_unpack_and_call(u8 rboxid, void (*funcptr)(u8, u8, u8, u8, u8, u8)) {
	struct rbox *r = &rboxes[rboxid];
	(*funcptr)(r->bg_id, r->x, r->y, r->w, r->h, r->palnum);
}

// 080048BC
bool rboxid_set_field(u8 rboxid, u8 field, u16 value) {
	switch (field) {
		case 1: rboxes[rboxid].x = value; break;
		case 2: rboxes[rboxid].y = value; break;
		case 5: rboxes[rboxid].palnum = value; break;
		case 6: rboxes[rboxid].vram_tileset_offset = value; break;
		default: return true;
	}
	return false;
}

// 08004950
u32 rboxid_get_field(u8 rboxid, u8 field) {
	switch (field) {
		case 0: return (u32) rboxes[rboxid].bg_id;
		case 1: return (u32) rboxes[rboxid].x;
		case 2: return (u32) rboxes[rboxid].y;
		case 3: return (u32) rboxes[rboxid].w;
		case 4: return (u32) rboxes[rboxid].h;
		case 5: return (u32) rboxes[rboxid].palnum;
		case 6: return (u32) rboxes[rboxid].vram_tileset_offset;
		case 7: return (u32) rboxes[rboxid].pixels;
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
	struct rbox *r = rboxes[rboxid];
	static u16 pixelbytes = r->w * r->h * (8*8) / 4;
	if (mode & 2) gpu_copy_to_tileset(r->bg_id, r->pixels, pixelbytes, r->vram_tileset_offset);
	if (mode & 1) bgid_send_tilemap(r->bg_id);
}

// 08005350
#define rbox_num_active_on_bgid_b rbox_num_active_on_bgid_a
