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

// 0800E030
void bt_03_emit(u8 target, u8 a, u8 len, u8 *data) {
	b_codegen_buffer[0] = 0x02;
	b_codegen_buffer[1] = a;
	b_codegen_buffer[2] = len;
	for (u32 i=0; i<len; i++)
		b_codegen_buffer[3+i] = data[i];
	b_prepare_buffer(target, b_codegen_buffer, 3+len);
}

// 0800E070
void bt_04_emit(u8 target) {
	b_codegen_buffer[0] = 0x04;
	b_codegen_buffer[1] = 0x04;
	b_codegen_buffer[2] = 0x04;
	b_codegen_buffer[3] = 0x04;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800E090
void bt_05_emit(u8 target, u8 a, u8 b, u8 c) {
	b_codegen_buffer[0] = 0x05;
	b_codegen_buffer[1] = a;
	b_codegen_buffer[2] = b;
	b_codegen_buffer[3] = c;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800E0B4
void bt_06_emit(u8 target, u8 a) {
	b_codegen_buffer[0] = 0x06;
	b_codegen_buffer[1] = a;
	b_prepare_buffer(target, b_codegen_buffer, 2);

}

// 0800E0D4
void bt_07_emit(u8 target) {
	b_codegen_buffer[0] = 0x07;
	b_codegen_buffer[1] = 0x07;
	b_codegen_buffer[2] = 0x07;
	b_codegen_buffer[3] = 0x07;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800E0F4
void bt_08_emit(u8 target) {
	b_codegen_buffer[0] = 0x08;
	b_codegen_buffer[1] = 0x08;
	b_codegen_buffer[2] = 0x08;
	b_codegen_buffer[3] = 0x08;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800E114
void bt_09_emit(u8 target) {
	b_codegen_buffer[0] = 0x09;
	b_codegen_buffer[1] = 0x09;
	b_codegen_buffer[2] = 0x09;
	b_codegen_buffer[3] = 0x09;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800E134
void bt_0A_emit(u8 target) {
	b_codegen_buffer[0] = 0x0A;
	b_codegen_buffer[1] = 0x0A;
	b_codegen_buffer[2] = 0x0A;
	b_codegen_buffer[3] = 0x0A;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800E154
void bt_0B_emit(u8 target) {
	b_codegen_buffer[0] = 0x0B;
	b_codegen_buffer[1] = 0x0B;
	b_codegen_buffer[2] = 0x0B;
	b_codegen_buffer[3] = 0x0B;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800E174
void bt_0C_emit(u8 target) {
	b_codegen_buffer[0] = 0x0C;
	b_codegen_buffer[1] = 0x0C;
	b_codegen_buffer[2] = 0x0C;
	b_codegen_buffer[3] = 0x0C;
	b_prepare_buffer(target, b_codegen_buffer, 4);
}

// 0800E194
void bt_0D_emit(u8 target, u8 a) {
	b_codegen_buffer[0] = 0x0D;
	b_codegen_buffer[1] = a;
	b_prepare_buffer(target, b_codegen_buffer, 2);

}

// 0800E1B4
void bt_0E_emit(u8 target, ...) {
	// TODO
}