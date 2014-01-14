u8 b_buffer_A[4][0x200]; // 02022BC4
u8 b_buffer_B[4][0x200]; // 020233C4

// 0825089C
void (*bt1_cmds[])() = { /* player */
	bt1_00_exec_ch2,
	sub_80313B0,
	sub_8031438,
	sub_8031E8C,
	sub_8031F00,
	sub_8031F68,
	sub_8032160,
	bt1_07_trainer_enter_field,
	bt1_08_8032428,
	sub_8032590,
	sub_8032650,
	sub_803273C,
	sub_803275C,
	sub_80327B0,
	sub_8032810,
	sub_8032840,
	bt1_10_message,
	bt1_11_message_for_player_only,
	bt1_12_battle_menu,
	bt1_13_nullsub,
	bt1_14_part_of_transition_to_move_menu,
	sub_8032CEC,
	sub_8032D50,
	sub_8032E28,
	bt1_18_8032E4C,
	sub_8032F4C,
	sub_8032FE8,
	sub_8033060,
	sub_80330C8,
	bt1_1D_nop,
	sub_8033140,
	sub_80331F4,
	bt1_20_nop,
	sub_8033230,
	sub_8033244,
	sub_8033258,
	sub_803326C,
	sub_8033280,
	sub_803329C,
	sub_80332D4,
	sub_80332EC,
	bt1_29_blink,
	bt1_2A_nop,
	sub_8033390,
	bt1_2C_fanfare,
	sub_8033404,
	bt1_2E_battle_intro,
	bt1_2F_pokemon_enter,
	sub_803376C,
	sub_8033878,
	sub_80338C8,
	bt1_33_enemy_move,
	bt1_34_move_anim,
	sub_80339B4,
	sub_8033A10,
	sub_8033A78,
	bt1_38_nullsub
};

// 08250A34
void (*bt2_cmds[])() = { /* opponent_trainer_probably */ 
	bt2_00_exec_ch2,
	sub_8036D88,
	sub_8036E10,
	sub_8037788,
	sub_80377FC,
	sub_80379C8,
	sub_8037BAC,
	sub_8037CD0,
	bt2_08_8037EA4,
	sub_8038078,
	sub_8038124,
	bt2_0B_nop,
	bt2_0C_nop,
	bt2_0D_nop,
	bt2_0E_nop,
	bt2_0F_move_anim,
	bt2_10_message,
	bt2_11_nop,
	bt2_12_nop,
	bt2_13_nop,
	sub_80385B0,
	sub_8038718,
	sub_8038744,
	bt2_17_nop,
	bt2_18_8038844,
	bt2_19_nop,
	sub_8038940,
	sub_80389B8,
	bt2_1C_nop,
	bt2_1D_nop,
	bt2_1E_nop,
	bt2_1F_nop,
	bt2_20_nop,
	bt2_21_nop,
	bt2_22_nop,
	bt2_23_nop,
	bt2_24_nop,
	sub_8038A8C,
	sub_8038AA8,
	sub_8038AE0,
	sub_8038AF8,
	sub_8038B20,
	bt2_2A_nop,
	sub_8038B9C,
	bt2_2C_fanfare,
	sub_8038C10,
	bt2_2E_battle_intro,
	sub_8038C80,
	bt2_30_8038EA8,
	sub_8039004,
	bt2_32_nop,
	sub_8039060,
	bt2_34_move_anim,
	bt2_35_nop,
	bt2_36_nop,
	sub_8039140,
	bt2_38_nullsub
};

// 084020F8
void (*bt3_cmds[])() = { /* player */
	bt3_00_exec_ch2,
	sub_80E9364,
	sub_80E9370,
	bt3_03_nop,
	bt3_04_nop,
	bt3_05_nop,
	sub_80E9DE8,
	sub_80E9E6C,
	bt3_08_80E9FB0,
	bt3_09_nop,
	sub_80EA100,
	bt3_0B_nop,
	sub_80EA1F8,
	sub_80EA24C,
	bt3_0E_nop,
	sub_80EA2B8,
	bt3_10_message,
	bt3_11_message_for_player_only,
	bt3_12_battle_menu,
	bt3_13_nop,
	sub_80EA7DC,
	sub_80EA870,
	sub_80EA8D4,
	bt3_17_nop,
	bt3_18_80EA9B8,
	sub_80EAAB8,
	bt3_1A_nop,
	bt3_1B_nop,
	bt3_1C_nop,
	bt3_1D_nop,
	bt3_1E_nop,
	bt3_1F_nop,
	bt3_20_nop,
	bt3_21_nop,
	bt3_22_nop,
	bt3_23_nop,
	bt3_24_nop,
	bt3_25_nop,
	bt3_26_nop,
	bt3_27_nop,
	bt3_28_nop,
	sub_80EAC08,
	bt3_2A_nop,
	sub_80EAC84,
	bt3_2C_fanfare,
	sub_80EACE4,
	bt3_2E_battle_intro,
	bt3_2F_080EAD54,
	bt3_30_80EB11C,
	bt3_31_nop,
	sub_80EB1AC,
	bt3_33_nop,
	bt3_34_move_anim,
	sub_80EB238,
	bt3_36_nop,
	sub_80EB280,
	bt3_38_nullsub
};

// 083FB134
void (*bt4_cmds[])() = { /* player_no_menu */
	bt4_00_exec_ch2,
	bt4_01_nop,
	sub_80D52EC,
	sub_80D5D40,
	sub_80D5DB4,
	sub_80D5ED0,
	sub_80D60AC,
	sub_80D61C8,
	bt4_08_nop,
	sub_80D6348,
	sub_80D63F4,
	bt4_0B_nop,
	bt4_0C_nop,
	bt4_0D_nop,
	bt4_0E_nop,
	sub_80D6510,
	bt4_10_message,
	bt4_11_nop,
	bt4_12_nop,
	bt4_13_nop,
	bt4_14_nop,
	bt4_15_nop,
	bt4_16_nop,
	bt4_17_nop,
	bt4_18_80D6894,
	bt4_19_nop,
	sub_80D6990,
	sub_80D6A08,
	bt4_1C_nop,
	bt4_1D_nop,
	bt4_1E_nop,
	bt4_1F_nop,
	bt4_20_nop,
	bt4_21_nop,
	bt4_22_nop,
	bt4_23_nop,
	bt4_24_nop,
	sub_80D6ADC,
	sub_80D6AF8,
	sub_80D6B30,
	sub_80D6B48,
	sub_80D6B70,
	bt4_2A_nop,
	sub_80D6BEC,
	bt4_2C_fanfare,
	sub_80D6C60,
	bt4_2E_battle_intro,
	sub_80D6CD4,
	sub_80D6FDC,
	sub_80D70E8,
	bt4_32_nop,
	sub_80D7144,
	sub_80D71A4,
	bt4_35_nop,
	bt4_36_nop,
	sub_80D7224,
	bt4_38_nullsub
};

// 08478F1C
void (*bt5_cmds[])() = { /* either_side */
	bt5_00_exec_ch2 ,
	sub_8157750,
	sub_815775C,
	bt5_03_nop,
	sub_81580E0,
	sub_815823C,
	sub_81582C0,
	sub_8158344,
	sub_8158544,
	bt5_09_nop,
	sub_815861C,
	bt5_0B_nop,
	sub_8158754,
	sub_81587A8,
	bt5_0E_nop,
	sub_8158814,
	bt5_10_message,
	bt5_11_message_for_player_only,
	bt5_12_battle_menu,
	bt5_13_nop,
	sub_8158C48,
	sub_8158C90,
	sub_8158CF4,
	bt5_17_nop,
	sub_8158DD8,
	sub_8158ED0,
	sub_8158F6C,
	sub_8159014,
	sub_815907C,
	bt5_1D_nop,
	bt5_1E_nop,
	bt5_1F_nop,
	bt5_20_nop,
	bt5_21_nop,
	bt5_22_nop,
	bt5_23_nop,
	bt5_24_nop,
	bt5_25_nop,
	bt5_26_nop,
	bt5_27_nop,
	bt5_28_nop,
	sub_8159190,
	bt5_2A_nop,
	sub_815920C,
	bt5_2C_fanfare,
	sub_815926C,
	bt5_2E_battle_intro,
	sub_8159304,
	sub_8159660,
	bt5_31_nop,
	sub_81596F0,
	bt5_33_nop,
	sub_8159720,
	sub_815977C,
	bt5_36_nop,
	sub_81597C4,
	bt5_38_nullsub
};

// 083FED00
void (*bt6_cmds[])() = { /* safari_zone */
	bt6_00_nop,
	bt6_01_nop,
	bt6_02_nop,
	bt6_03_nop,
	bt6_04_nop,
	bt6_05_nop,
	bt6_06_nop,
	bt6_07_,
	bt6_08_nop,
	bt6_09_nop,
	bt6_0A_nop,
	bt6_0B_nop,
	bt6_0C_,
	bt6_0D_,
	bt6_0E_nop,
	bt6_0F_nop,
	bt6_10_message,
	bt6_11_message_for_player_only,
	bt6_12_battle_menu,
	bt6_13_nop,
	bt6_14_nop,
	sub_80DDD04,
	bt6_16_nop,
	bt6_17_nop,
	bt6_18_nop,
	bt6_19_nop,
	sub_80DDD74,
	bt6_1B_nop,
	bt6_1C_nop,
	bt6_1D_nop,
	bt6_1E_nop,
	bt6_1F_nop,
	bt6_20_nop,
	bt6_21_nop,
	bt6_22_nop,
	bt6_23_nop,
	bt6_24_nop,
	bt6_25_nop,
	bt6_26_nop,
	bt6_27_nop,
	bt6_28_nop,
	bt6_29_nop,
	bt6_2A_nop,
	sub_80DDE70,
	bt6_2C_fanfare,
	sub_80DDEE4,
	bt6_2E_battle_intro,
	sub_80DDF54,
	bt6_30_nop,
	bt6_31_nop,
	bt6_32_nop,
	bt6_33_nop,
	sub_80DDFE0,
	bt6_35_nop,
	bt6_36_nop,
	sub_80DE054,
	bt6_38_nullsub
};

// 08250B20
void (*bt7_cmds[])() = { /* opponent_wild_probably */
	bt7_00_exec_ch2,
	bt7_01_nop,
	sub_803B9C8,
	sub_803C340,
	sub_803C3B4,
	sub_803C504,
	sub_803C6D4,
	sub_803C7F8,
	bt7_08_nop,
	sub_803CB0C,
	sub_803CBB8,
	bt7_0B_nop,
	bt7_0C_nop,
	bt7_0D_nop,
	bt7_0E_nop,
	sub_803CC94,
	bt7_10_message,
	bt7_11_nop,
	bt7_12_nop,
	bt7_13_nop,
	bt7_14_nop,
	bt7_15_nop,
	bt7_16_nop,
	bt7_17_nop,
	sub_803D018,
	bt7_19_nop,
	sub_803D114,
	sub_803D18C,
	bt7_1C_nop,
	bt7_1D_nop,
	bt7_1E_nop,
	bt7_1F_nop,
	bt7_20_nop,
	bt7_21_nop,
	bt7_22_nop,
	bt7_23_nop,
	bt7_24_nop,
	sub_803D260,
	sub_803D27C,
	sub_803D2B4,
	sub_803D2CC,
	sub_803D2F4,
	bt7_2A_nop,
	sub_803D370,
	bt7_2C_fanfare,
	sub_803D3E4,
	bt7_2E_battle_intro,
	sub_803D454,
	sub_803D67C,
	sub_803D7D8,
	bt7_32_nop,
	sub_803D834,
	sub_803D894,
	bt7_35_nop,
	bt7_36_nop,
	sub_803D914,
	bt7_38_nullsub
};

u8 b_active_side;
u8 b_oamid[NUM_BATTLE_SIDES];

void bt1_29_blink_for_damage() {
	u8 oamid = b_oamid[b_active_side];
	struct obj *o = &objects[oamid];
	if (o->bitfield2 & 0x7) {
		bx_busy = true;
		o->priv3 = 0;
		sub_0804BE70(b_active_side);
		bx_ptr[b_active_side] = &bx1_blink_for_damage;
	} else {
		bt1_done();
	}
}

// bx1: 080307B4
// bx2: 080361FC
// bx3: 080E8948
// bx4: 080D4730
// bx5: 08156F00
// bx7: 0803ADF0

void bx1_blink_for_damage() {
	u8 oamid = b_oamid[b_active_side];
	struct obj *o = &objects[oamid];
	u8 t = o->priv3;  // time that passed
	if (t >= 0x20) {  // for 20 frames
		if (t%4 == 0) // every fourth frame
			o->bitfield2 ^= OBJ_BIT2_INVISIBLE;
		o->priv3++;
	} else {
		o->priv3 = 0;
		o->bitfield2 &= ~OBJ_BIT2_INVISIBLE;
		bx_busy = false;
		bx1_done();
}

// bx1: 08030778
// bx6: 080DD7F0
void bx1_wait() {
	// TODO
}

// bx3: 080E7970
// bx5: 08156484
void bx3_wait() {
	if (!bx_busy)
		bx3_done();
}

// bx1: 0802E3B4
// bx2: 08035A98
// bx3: 080E8A9C
// bx4: 080D49E8
// bx5: 081560DC
// bx6: 080DD554
// bx7: 0803A684
void bx1_exec_buffer_A_ch0() {
	if ((b_bitfield & (1<<b_battle_side)) == 0) return;
	u8 cmd = b_buffer_A[b_battle_side][0];
	if (cmd >= 0x38) bx1_done();
	else             bt1_cmds[cmd]();
}

// bx1: 08030610
// bx2: 080361E4
// bx3: 080E7918
// bx4: 080D4718
// bx5: 0815623C
// bx6: 080DD760
// bx7: 0803ADD8
void bx1_button_a() {
	if (button_a_pressed(0) == 0)
		bx1_done();
}

// 0803A684
void bx3_oak_inflicting_damage() {
	bx_text("OAK: Inflicting damage on the foe is the key to any battle.", 0x01);
}

// 080E8584
void bx3_oak_lowering_the_foes_stats() {
	bx_text("OAK: Lowering the foe`s stats will put you at an advantage.", 0x40);
}

// 080E8598
void bx3_oak_there_is_no_running_away() {
	bx_text("OAK: No! There`s no running away from a TRAINER POKeMON battle!", 0x01);
}

// 080E85AC
void bx3_oak_hm_excellent() {
	bx_text("OAK: Hm! Excellent!\nIf you win, you earn prize money, ...", 0x40);
}

// 080E85C0
void bx3_oak_how_disappointing() {
	bx_text("OAK: Hm. How disappointing.\nIf you win, you earn prize money, ...", 0x40);
}

// 080E85D4
void bx3_text(const char *message, u8 _) {
	/* ... */
}