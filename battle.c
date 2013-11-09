u8 b_buffer_A[4][0x200]; // 02022BC4
u8 b_buffer_B[4][0x200]; // 020233C4

// 0825089C
void (*b_tbl1_cmds[])() = { /* player */
	bt_00_1_exec_ch2,
	sub_80313B0,
	sub_8031438,
	sub_8031E8C,
	sub_8031F00,
	sub_8031F68,
	sub_8032160,
	bt_07_1_trainer_enter_field,
	bt_08_1_8032428,
	sub_8032590,
	sub_8032650,
	sub_803273C,
	sub_803275C,
	sub_80327B0,
	sub_8032810,
	sub_8032840,
	bt_10_1_message,
	bt_11_1_message_for_player_only,
	bt_12_1_battle_menu,
	bt_13_1_nullsub,
	bt_14_1_part_of_transition_to_move_menu,
	sub_8032CEC,
	sub_8032D50,
	sub_8032E28,
	bt_18_1_8032E4C,
	sub_8032F4C,
	sub_8032FE8,
	sub_8033060,
	sub_80330C8,
	bt_1D_1_nop,
	sub_8033140,
	sub_80331F4,
	bt_20_1_nop,
	sub_8033230,
	sub_8033244,
	sub_8033258,
	sub_803326C,
	sub_8033280,
	sub_803329C,
	sub_80332D4,
	sub_80332EC,
	bt_29_1_blink,
	bt_2A_1_nop,
	sub_8033390,
	bt_2C_1_fanfare,
	sub_8033404,
	bt_2E_1_battle_intro,
	bt_2F_1_pokemon_enter,
	sub_803376C,
	sub_8033878,
	sub_80338C8,
	bt_33_1_enemy_move,
	bt_34_1_move_anim,
	sub_80339B4,
	sub_8033A10,
	sub_8033A78,
	bt_38_1_nullsub
};

// 08250A34
void (*b_tbl2_cmds[])() = { /* opponent_trainer_probably */ 
	bt_00_2_exec_ch2,
	sub_8036D88,
	sub_8036E10,
	sub_8037788,
	sub_80377FC,
	sub_80379C8,
	sub_8037BAC,
	sub_8037CD0,
	bt_08_2_8037EA4,
	sub_8038078,
	sub_8038124,
	bt_0B_2_nop,
	bt_0C_2_nop,
	bt_0D_2_nop,
	bt_0E_2_nop,
	bt_0F_2_move_anim,
	bt_10_2_message,
	bt_11_2_nop,
	bt_12_2_nop,
	bt_13_2_nop,
	sub_80385B0,
	sub_8038718,
	sub_8038744,
	bt_17_2_nop,
	bt_18_2_8038844,
	bt_19_2_nop,
	sub_8038940,
	sub_80389B8,
	bt_1C_2_nop,
	bt_1D_2_nop,
	bt_1E_2_nop,
	bt_1F_2_nop,
	bt_20_2_nop,
	bt_21_2_nop,
	bt_22_2_nop,
	bt_23_2_nop,
	bt_24_2_nop,
	sub_8038A8C,
	sub_8038AA8,
	sub_8038AE0,
	sub_8038AF8,
	sub_8038B20,
	bt_2A_2_nop,
	sub_8038B9C,
	bt_2C_2_fanfare,
	sub_8038C10,
	bt_2E_2_battle_intro,
	sub_8038C80,
	bt_30_2_8038EA8,
	sub_8039004,
	bt_32_2_nop,
	sub_8039060,
	bt_34_2_move_anim,
	bt_35_2_nop,
	bt_36_2_nop,
	sub_8039140,
	bt_38_2_nullsub
};

// 084020F8
void (*b_tbl3_cmds[])() = { /* player */
	bt_00_3_exec_ch2,
	sub_80E9364,
	sub_80E9370,
	bt_03_3_nop,
	bt_04_3_nop,
	bt_05_3_nop,
	sub_80E9DE8,
	sub_80E9E6C,
	bt_08_3_80E9FB0,
	bt_09_3_nop,
	sub_80EA100,
	bt_0B_3_nop,
	sub_80EA1F8,
	sub_80EA24C,
	bt_0E_3_nop,
	sub_80EA2B8,
	bt_10_3_message,
	bt_11_3_message_for_player_only,
	bt_12_3_battle_menu,
	bt_13_3_nop,
	sub_80EA7DC,
	sub_80EA870,
	sub_80EA8D4,
	bt_17_3_nop,
	bt_18_3_80EA9B8,
	sub_80EAAB8,
	bt_1A_3_nop,
	bt_1B_3_nop,
	bt_1C_3_nop,
	bt_1D_3_nop,
	bt_1E_3_nop,
	bt_1F_3_nop,
	bt_20_3_nop,
	bt_21_3_nop,
	bt_22_3_nop,
	bt_23_3_nop,
	bt_24_3_nop,
	bt_25_3_nop,
	bt_26_3_nop,
	bt_27_3_nop,
	bt_28_3_nop,
	sub_80EAC08,
	bt_2A_3_nop,
	sub_80EAC84,
	bt_2C_3_fanfare,
	sub_80EACE4,
	bt_2E_3_battle_intro,
	bt_2F_3_080EAD54,
	bt_30_3_80EB11C,
	bt_31_3_nop,
	sub_80EB1AC,
	bt_33_3_nop,
	bt_34_3_move_anim,
	sub_80EB238,
	bt_36_3_nop,
	sub_80EB280,
	bt_38_3_nullsub
};

// 083FB134
void (*b_tbl4_cmds[])() = { /* player_no_menu */
	bt_00_4_exec_ch2,
	bt_01_4_nop,
	sub_80D52EC,
	sub_80D5D40,
	sub_80D5DB4,
	sub_80D5ED0,
	sub_80D60AC,
	sub_80D61C8,
	bt_08_4_nop,
	sub_80D6348,
	sub_80D63F4,
	bt_0B_4_nop,
	bt_0C_4_nop,
	bt_0D_4_nop,
	bt_0E_4_nop,
	sub_80D6510,
	bt_10_4_message,
	bt_11_4_nop,
	bt_12_4_nop,
	bt_13_4_nop,
	bt_14_4_nop,
	bt_15_4_nop,
	bt_16_4_nop,
	bt_17_4_nop,
	bt_18_4_80D6894,
	bt_19_4_nop,
	sub_80D6990,
	sub_80D6A08,
	bt_1C_4_nop,
	bt_1D_4_nop,
	bt_1E_4_nop,
	bt_1F_4_nop,
	bt_20_4_nop,
	bt_21_4_nop,
	bt_22_4_nop,
	bt_23_4_nop,
	bt_24_4_nop,
	sub_80D6ADC,
	sub_80D6AF8,
	sub_80D6B30,
	sub_80D6B48,
	sub_80D6B70,
	bt_2A_4_nop,
	sub_80D6BEC,
	bt_2C_4_fanfare,
	sub_80D6C60,
	bt_2E_4_battle_intro,
	sub_80D6CD4,
	sub_80D6FDC,
	sub_80D70E8,
	bt_32_4_nop,
	sub_80D7144,
	sub_80D71A4,
	bt_35_4_nop,
	bt_36_4_nop,
	sub_80D7224,
	bt_38_4_nullsub
};

// 08478F1C
void (*b_tbl5_cmds[])() = { /* either_side */
	bt_00_5_exec_ch2 ,
	sub_8157750,
	sub_815775C,
	bt_03_5_nop,
	sub_81580E0,
	sub_815823C,
	sub_81582C0,
	sub_8158344,
	sub_8158544,
	bt_09_5_nop,
	sub_815861C,
	bt_0B_5_nop,
	sub_8158754,
	sub_81587A8,
	bt_0E_5_nop,
	sub_8158814,
	bt_10_5_message,
	bt_11_5_message_for_player_only,
	bt_12_5_battle_menu,
	bt_13_5_nop,
	sub_8158C48,
	sub_8158C90,
	sub_8158CF4,
	bt_17_5_nop,
	sub_8158DD8,
	sub_8158ED0,
	sub_8158F6C,
	sub_8159014,
	sub_815907C,
	bt_1D_5_nop,
	bt_1E_5_nop,
	bt_1F_5_nop,
	bt_20_5_nop,
	bt_21_5_nop,
	bt_22_5_nop,
	bt_23_5_nop,
	bt_24_5_nop,
	bt_25_5_nop,
	bt_26_5_nop,
	bt_27_5_nop,
	bt_28_5_nop,
	sub_8159190,
	bt_2A_5_nop,
	sub_815920C,
	bt_2C_5_fanfare,
	sub_815926C,
	bt_2E_5_battle_intro,
	sub_8159304,
	sub_8159660,
	bt_31_5_nop,
	sub_81596F0,
	bt_33_5_nop,
	sub_8159720,
	sub_815977C,
	bt_36_5_nop,
	sub_81597C4,
	bt_38_5_nullsub
};

// 083FED00
void (*b_tbl6_cmds[])() = { /* safari_zone */
	bt_00_6_nop,
	bt_01_6_nop,
	bt_02_6_nop,
	bt_03_6_nop,
	bt_04_6_nop,
	bt_05_6_nop,
	bt_06_6_nop,
	bt_07_6_,
	bt_08_6_nop,
	bt_09_6_nop,
	bt_0A_6_nop,
	bt_0B_6_nop,
	bt_0C_6_,
	bt_0D_6_,
	bt_0E_6_nop,
	bt_0F_6_nop,
	bt_10_6_message,
	bt_11_6_message_for_player_only,
	bt_12_6_battle_menu,
	bt_13_6_nop,
	bt_14_6_nop,
	sub_80DDD04,
	bt_16_6_nop,
	bt_17_6_nop,
	bt_18_6_nop,
	bt_19_6_nop,
	sub_80DDD74,
	bt_1B_6_nop,
	bt_1C_6_nop,
	bt_1D_6_nop,
	bt_1E_6_nop,
	bt_1F_6_nop,
	bt_20_6_nop,
	bt_21_6_nop,
	bt_22_6_nop,
	bt_23_6_nop,
	bt_24_6_nop,
	bt_25_6_nop,
	bt_26_6_nop,
	bt_27_6_nop,
	bt_28_6_nop,
	bt_29_6_nop,
	bt_2A_6_nop,
	sub_80DDE70,
	bt_2C_6_fanfare,
	sub_80DDEE4,
	bt_2E_6_battle_intro,
	sub_80DDF54,
	bt_30_6_nop,
	bt_31_6_nop,
	bt_32_6_nop,
	bt_33_6_nop,
	sub_80DDFE0,
	bt_35_6_nop,
	bt_36_6_nop,
	sub_80DE054,
	bt_38_6_nullsub
};

// 08250B20
void (*b_tbl7_cmds[])() = { /* opponent_wild_probably */
	bt_00_7_exec_ch2,
	bt_01_7_nop,
	sub_803B9C8,
	sub_803C340,
	sub_803C3B4,
	sub_803C504,
	sub_803C6D4,
	sub_803C7F8,
	bt_08_7_nop,
	sub_803CB0C,
	sub_803CBB8,
	bt_0B_7_nop,
	bt_0C_7_nop,
	bt_0D_7_nop,
	bt_0E_7_nop,
	sub_803CC94,
	bt_10_7_message,
	bt_11_7_nop,
	bt_12_7_nop,
	bt_13_7_nop,
	bt_14_7_nop,
	bt_15_7_nop,
	bt_16_7_nop,
	bt_17_7_nop,
	sub_803D018,
	bt_19_7_nop,
	sub_803D114,
	sub_803D18C,
	bt_1C_7_nop,
	bt_1D_7_nop,
	bt_1E_7_nop,
	bt_1F_7_nop,
	bt_20_7_nop,
	bt_21_7_nop,
	bt_22_7_nop,
	bt_23_7_nop,
	bt_24_7_nop,
	sub_803D260,
	sub_803D27C,
	sub_803D2B4,
	sub_803D2CC,
	sub_803D2F4,
	bt_2A_7_nop,
	sub_803D370,
	bt_2C_7_fanfare,
	sub_803D3E4,
	bt_2E_7_battle_intro,
	sub_803D454,
	sub_803D67C,
	sub_803D7D8,
	bt_32_7_nop,
	sub_803D834,
	sub_803D894,
	bt_35_7_nop,
	bt_36_7_nop,
	sub_803D914,
	bt_38_7_nullsub
};

// 0802E3B4
void bk_exec_buffer_A_ch0_tbl1() {
	if ((b_bitfield & (1<<b_battle_side)) == 0) return;
	u8 cmd = b_buffer_A[b_battle_side][0];
	if (cmd >= 0x38) b_tbl1_exec_completed();
	else             b_tbl1_cmds[cmd]();
}

// 08035A98
void bk_exec_buffer_A_ch0_tbl2() {
	if ((b_bitfield & (1<<b_battle_side)) == 0) return;
	u8 cmd = b_buffer_A[b_battle_side][0];
	if (cmd >= 0x38) b_tbl2_exec_completed();
	else             b_tbl2_cmds[cmd]();
}

// 080E8A9C
void bk_exec_buffer_A_ch0_tbl3() {
	if ((b_bitfield & (1<<b_battle_side)) == 0) return;
	u8 cmd = b_buffer_A[b_battle_side][0];
	if (cmd >= 0x38) b_tbl3_exec_completed();
	else             b_tbl3_cmds[cmd]();
}

// 080D49E8
void bk_exec_buffer_A_ch0_tbl4() {
	if ((b_bitfield & (1<<b_battle_side)) == 0) return;
	u8 cmd = b_buffer_A[b_battle_side][0];
	if (cmd >= 0x38) b_tbl4_exec_completed();
	else             b_tbl4_cmds[cmd]();
}

// 081560DC
void bk_exec_buffer_A_ch0_tbl5() {
	if ((b_bitfield & (1<<b_battle_side)) == 0) return;
	u8 cmd = b_buffer_A[b_battle_side][0];
	if (cmd >= 0x38) b_tbl5_exec_completed();
	else             b_tbl5_cmds[cmd]();
}

// 080DD554
void bk_exec_buffer_A_ch0_tbl6() {
	if ((b_bitfield & (1<<b_battle_side)) == 0) return;
	u8 cmd = b_buffer_A[b_battle_side][0];
	if (cmd >= 0x38) b_tbl6_exec_completed();
	else             b_tbl6_cmds[cmd]();
}

// 0803A684
void bk_exec_buffer_A_ch0_tbl7() {
	if ((b_bitfield & (1<<b_battle_side)) == 0) return;
	u8 cmd = b_buffer_A[b_battle_side][0];
	if (cmd >= 0x38) b_tbl7_exec_completed();
	else             b_tbl7_cmds[cmd]();
}

// 08030610
void bk_t1_button_a() {
	if (button_a_pressed(0) == 0)
		b_tbl1_exec_completed();
}

// 080361E4
void bk_t2_button_a() {
	if (button_a_pressed(0) == 0)
		b_tbl2_exec_completed();
}

// 080E7918
void bk_t3_button_a() {
	if (button_a_pressed(0) == 0)
		b_tbl3_exec_completed();
}

// 080D4718
void bk_t4_button_a() {
	if (button_a_pressed(0) == 0)
		b_tbl4_exec_completed();
}

// 0815623C
void bk_t5_button_a() {
	if (button_a_pressed(0) == 0)
		b_tbl5_exec_completed();
}

// 080DD760
void bk_t6_button_a() {
	if (button_a_pressed(0) == 0)
		b_tbl6_exec_completed();
}

// 0803ADD8
void bk_t7_button_a() {
	if (button_a_pressed(0) == 0)
		b_tbl7_exec_completed();
}

// 0803A684
void bk_oak_inflicting_damage() {
	bk_text("OAK: Inflicting damage on the foe is the key to any battle.", 0x01);
}

// 080E8584
void bk_oak_lowering_the_foes_stats() {
	bk_text("OAK: Lowering the foe`s stats will put you at an advantage.", 0x40);
}

// 080E8598
void bk_oak_there_is_no_running_away() {
	bk_text("OAK: No! There`s no running away from a TRAINER POKeMON battle!", 0x01);
}

// 080E85AC
void bk_oak_hm_excellent() {
	bk_text("OAK: Hm! Excellent!\nIf you win, you earn prize money, ...", 0x40);
}

// 080E85C0
void bk_oak_how_disappointing() {
	bk_text("OAK: Hm. How disappointing.\nIf you win, you earn prize money, ...", 0x40);
}

// 080E85D4
void bk_text(const char *message, u8 _) {
	/* ... */
}