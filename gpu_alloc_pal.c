#define TAG_FREE 0xFFFF

struct pal_spec {
	u16 *data;
	u16 tag;
	u16 padding;
};

u16 gpu_pal_tags[16]; // 03000DE8
u8 gpu_pal_tag_search_lower_boundary; // 03003E58

u8 gpu_pal_tags_index_of(u16 tag); // TODO

// 08008928
u8 gpu_pal_alloc_and_load(struct pal_spec* ps) {
	u8 index;
	if ((index = gpu_pal_tags_index_of(ps->tag)) == 0xFF) { // not loaded yet
		if ((index = gpu_pal_tags_index_of(TAG_FREE)) != 0xFF) { // free slots available
			gpu_pal_tags[index] = ps->tag;
			gpu_pal_load(ps->data, index);
		}
	}
	return index;
}

// 08008974
u8 gpu_pal_alloc_and_load_multiple(struct pal_spec* ps) {
	for (;ps->data; ps++)
		if (gpu_pal_alloc_and_load(ps) == 0xFF)
			return 0xFF;
	return 0;
}

// 080089B8
u8 gpu_pal_alloc_new(u16 tag) {
	u8 index = gpu_pal_tags_index_of(TAG_FREE);
	if (index != 0xFF)
		gpu_pal_tags[index] = tag;
}

// 080089E8
u8 gpu_pal_tags_index_of(u16 tag) {
	for (u8 index = gpu_pal_tag_search_lower_boundary; index < 16; index++)
		if (gpu_pal_tags[index] == tag)
			return index;
	return 0xFF;
}

// 08008A20
u16 gpu_pal_tag_by_index(u8 index) {
	return gpu_pal_tags[index];
}

// 08008A30
void gpu_pal_free_by_tag(u16 tag) {
	u8 index = gpu_pal_tags_index_of(tag);
	if (index != 0xFF)
		gpu_pal_tags[index] = 0xFFFF;
}
