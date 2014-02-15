// 03000FC8
u8 oe_active_list[32];


// 08083444
u32 oe_exec(u8 num) {
	u32 retval;
	oe_active_list_add(num);
	u8 *cursor = oe_table[num];
	while (oe_cmds[*cursor](&cursor, &retval));
	return retval;
}

// 0808348C
bool oec00_load_gfx(u8 **cursor, u32 *retval) {
	cursor++;
	oec00_load_gfx_impl(cursor);
	return true;
}

// 080834A0
bool oec01_load_pal(u8 **cursor, u32 *retval) {
	cursor++;
	oec01_load_pal_impl(cursor);
	return true;
}

// 080834B4
bool oec02_load_pal(u8 **cursor, u32 *retval) {
	cursor++;
	oec02_load_pal_impl(cursor);
	return true;
}

// 080834C8
bool oec03_call_asm(u8 **cursor, u32 *retval) {
	cursor++;
	oec03_call_asm_impl(cursor, retval);
	return true;
}

// 080834DC
bool oec04_end(u8 **cursor, u32 *retval) {
	return false;
}

// 080834E0
bool oec05_combined_00_01_03(u8 **cursor, u32 *retval) {
	cursor++;
	oec00_load_gfx_impl(cursor);
	oec01_load_pal_impl(cursor);
	oec03_call_asm_impl(cursor, retval);
	return true;
}

// 08083508
bool oec06_combined_00_03(u8 **cursor, u32 *retval) {
	cursor++;
	oec00_load_gfx_impl(cursor);
	oec03_call_asm_impl(cursor, retval);
	return true;
}

// 0808352C
bool oec07_combined_01_03(u8 **cursor, u32 *retval) {
	cursor++;
	oec01_load_pal_impl(cursor);
	oec03_call_asm_impl(cursor, retval);
	return true;
}

// 08083550
u32 oe_read_word(u8 **cursor) {
	u8 *src = *cursor;
	return (src[0]) + (src[1]<<8) + (src[2]<<16) + (src[3]<<24);
}

// 08083568
void oec00_load_gfx_impl(u8 **cursor) {
	struct blob_t *bl = oe_read_word(cursor);
	u16 unk1 = sub_08008804(bl->size);
	if (unk1 == 0xFFFF)
		sub_080086DC(bl);
	cursor += 4;
}

// 08083598
void sub_08083598() {
	// TODO
}

// 08083614
void oec01_load_pal_impl(u8 **cursor) {
	struct blob_t *bl = oe_read_word(cursor);
	u16 pal_tag = gpu_pal_tags_index_of(bl->size);
	load_palette_1(bl->data);
	if (pal_tag == 0xFF) {
		u16 pal_tag = gpu_pal_tags_index_of(bl->size);
		sub_08083598(unk2);
	}
	u16 unk3 = gpu_pal_tags_index_of(bl->size);
	sub_0807AA8C(unk3);
	cursor += 4;
}

// 0808365C
void oec02_load_pal_impl(u8 **cursor) {
	struct blob_t *bl = oe_read_word(cursor);
	u16 unk1 = gpu_pal_tags_index_of(bl->size);
	load_palette_1(bl->data);
	if (unk1 == 0xFF) {
		u16 unk2 = gpu_pal_tags_index_of(bl->size);
		sub_08083598(unk2);
	}
	cursor += 4;
}

// 08083698
void oec03_call_asm_impl(u8 **cursor, u32 *retval) {
	*retval = oe_read_word(cursor)();
	cursor += 4;
}

// 080836B4
void oe_stop2(...) {
	// TODO
}

// 080836D8
void oe_stop(struct obj_t *o, u8 num) {
	oe_stop2(o);
	oe_active_list_remove(num);
}

// 080836F0
void sub_080836F0(...) {
	// TODO
}

// 08083754
void sub_08083754(...) {
	// TODO
}

// 080837AC
void oe_active_list_clear() {
	for (u32 i=0; i<32; i++)
		oe_active_list[i] = 0xFF;
}

// 080837D0
void oe_active_list_add(u8 num) {
	for (u32 i=0; i<32; i++)
		if (oe_active_list[i] == 0xFF) {
			oe_active_list[i] = num;
			return;
		}

}

// 080837FC
void oe_active_list_remove(u8 num) {
	for (u32 i=0; i<32; i++)
		if (oe_active_list[i] == num) {
			oe_active_list[i] = 0xFF;
			return;
		}
}

// 0808382C
bool oe_active_list_contains(u8 num) {
	for (u32 i=0; i<32; i++)
		if (oe_active_list[i] == num)
			return true;
	return false;
}

// 083CBE30
bool (*oe_cmds)(u8 **, u32 *) = {
	&oec00_load_gfx,
	&oec01_load_pal,
	&oec02_load_pal,
	&oec03_call_asm,
	&oec04_end,
	&oec05_combined_00_01_03,
	&oec06_combined_00_03,
	&oec07_combined_01_03
};
