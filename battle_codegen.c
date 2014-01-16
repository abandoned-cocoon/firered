// from battle_interp.c
// u8 b_buffer_A[4][0x200]; // 02022BC4
// u8 b_buffer_B[4][0x200]; // 020233C4

u8 b_codegen_buffer[0x100];

// 0800D8B0
void b_prepare_buffer(u8 dst_id, u8 *src, u16 length) {
	if (battle_type_bits & BATTLE_WIRELESS)
		return b_prepare_buffer_wireless(dst_id, src, length);

	if (dst_id >= 2) return;
	u8 (*dst_buffer)[0x200] = dst_id ? b_buffer_B : b_buffer_A;
	u8 *dst = &dst[b_active_side];
	memcpy(dst, src, length);
}

// 0800DFA4
void bt_00_emit(u8 target, u8 a, u8 b) {
	b_codegen_buffer[0] = 0x00;
	b_codegen_buffer[1] = a;
	b_codegen_buffer[2] = b;
	b_codegen_buffer[3] = 0x00;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800DFC8
void bt_01_emit(u8 target, u8 a, u8 b) {
	b_codegen_buffer[0] = 0x01;
	b_codegen_buffer[1] = a;
	b_codegen_buffer[2] = b;
	b_codegen_buffer[3] = 0x00;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800DFF0
void bt_02_emit(u8 target, u8 a, u8 b, u8 len, u8 *data) {
	b_codegen_buffer[0] = 0x02;
	b_codegen_buffer[1] = a;
	b_codegen_buffer[2] = b;
	for (u32 i=0; i<len; i++)
		b_codegen_buffer[3+i] = data[i];
	b_prepare_buffer(target, b_codegen_buffer, 3+len);
}

