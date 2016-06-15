#pragma once

#define TARGET_BUFFER_A 0
#define TARGET_BUFFER_B 1

void b_prepare_buffer(u8 dst_id, u8 *src, u16 length);
void bt_00_emit(u8 target, u8 a, u8 b);
void bt_01_emit(u8 target, u8 a, u8 b);
void bt_02_emit(u8 target, u8 a, u8 b, u8 len, u8 *data);
void bt_03_emit(u8 target, u8 a, u8 len, u8 *data);
void bt_04_emit(u8 target);
void bt_05_emit(u8 target, u8 a, u8 b, u8 c);
void bt_06_emit(u8 target, u8 a);
void bt_07_emit(u8 target);
void bt_08_emit(u8 target);
void bt_09_emit(u8 target);
void bt_0A_emit(u8 target);
void bt_0B_emit(u8 target);
void bt_0C_emit(u8 target);
void bt_0D_emit(u8 target, u8 a);
void bt_0E_emit(u8 target, ...);
void bt_0F_emit(u8 target, u16 moveid, u8 b, u16 c, u32 damage, u8 happiness, struct disable *d);
void bt_10_emit(u8 target, ...);
void bt_11_emit(u8 target, ...);
void bt_12_emit(u8 target, u8 a, u16 b);
void bt_13_emit(u8 target, u8 a);
void bt_14_emit(u8 target, u8 a, u8 b, void *c);
void bt_15_emit(u8 target, u8 *a);
void bt_16_emit(u8 target, u8 a, u8 b, u8 c, u8 *d);
void bt_17_emit(u8 target);
void bt_18_emit(u8 target, u8 a);
void bt_19_emit(u8 target, u8 a, u16 b);
void bt_1A_emit(u8 target, u32 a, u32 b);
void bt_1B_emit(u8 target, u32 a, u8 b);
void bt_1C_emit(u8 target, u8 a);
void bt_25_emit(u8 target);
void bt_27_emit(u8 target);
void bt_28_emit(u8 target);
void bt_29_emit(u8 target);
void bt_2A_emit(u8 target);
void bt_2D_emit(u8 target);
void bt_2E_emit(u8 target, u8 environment_index);
void bt_2F_emit(u8 target);
void bt_30_emit(u8 target, ...);
void bt_31_emit(u8 target);
void bt_32_emit(u8 target);
void bt_33_emit(u8 target, u8 a);
void bt_34_emit(u8 target, u8 a, u16 bb);
void bt_35_emit(u8 target, u8 a);
void bt_37_emit(u8 target, u8 a);
void bt_38_emit(u8 target, u8 a);