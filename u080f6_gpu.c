// 0203AB58
extern u8 bgmaps_that_need_syncing[4];

// 080F6790
void gpu_tilemaps_sync_none() {
	bgmaps_that_need_syncing[0] = false;
	bgmaps_that_need_syncing[1] = false;
	bgmaps_that_need_syncing[2] = false;
	bgmaps_that_need_syncing[3] = false;
}

// 080F67A4
void bgid_mark_for_sync(u8 bgid) {
	bgmaps_that_need_syncing[bgid] = true;
}

// 080F67B8
void gpu_tilemaps_sync() {
	u8 *t = bgmaps_that_need_syncing;
	if (t[0]) { bgid_send_tilemap(0); t[0] = false; }
	if (t[1]) { bgid_send_tilemap(1); t[1] = false; }
	if (t[2]) { bgid_send_tilemap(2); t[2] = false; }
	if (t[3]) { bgid_send_tilemap(3); t[3] = false; }
}
