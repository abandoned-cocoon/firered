struct {
	void *target;
	enum {
		LOAD_8,
		LOAD_16,
		LOAD_32,
		ZERO_8,
		ZERO_16,
		ZERO_32,
		END
	} target_type
} battle_config_entry;

void s5C_trainerbattle(struct script_state *s) {
	s->cursor = actual_battle(s->cursor);
}

// 083C6900
struct battle_config_entry tb_format_5_other[] = {
	{&battle_type,             LOAD_8},
	{&battle_trainer_flag,     LOAD_16},
	{&battle_arg2,             LOAD_16},

	{&battle_message_intro,    LOAD_32},
	{&battle_message_defeat,   LOAD_32},
	{&battle_message_2,        ZERO_32},
	{&battle_message_need_two, ZERO_32},
	{&battle_message_4,        ZERO_32},

	{&battle_next_scr_command, END}
};

// 083C6948
struct battle_config_entry tb_format_1_2[] = {
	{&battle_type,             LOAD_8},
	{&battle_trainer_flag,     LOAD_16},
	{&battle_arg2,             LOAD_16},

	{&battle_message_intro,    LOAD_32},
	{&battle_message_defeat,   LOAD_32},
	{&battle_message_2,        ZERO_32},
	{&battle_message_need_two, ZERO_32},
	{&battle_message_4,        LOAD_32},

	{&battle_next_scr_command, END}
};

// 083C6990
struct battle_config_entry tb_format_4_7[] = {
	{&battle_type,             LOAD_8},
	{&battle_trainer_flag,     LOAD_16},
	{&battle_arg2,             LOAD_16},

	{&battle_message_intro,    LOAD_32},
	{&battle_message_defeat,   LOAD_32},
	{&battle_message_2,        ZERO_32},
	{&battle_message_need_two, LOAD_32},
	{&battle_message_4,        ZERO_32},

	{&battle_next_scr_command, END}
};

// 083C69D8
struct battle_config_entry tb_format_3[] = {
	{&battle_type,             LOAD_8},
	{&battle_trainer_flag,     LOAD_16},
	{&battle_arg2,             LOAD_16},

	{&battle_message_intro,    ZERO_32},
	{&battle_message_defeat,   LOAD_32},
	{&battle_message_2,        ZERO_32},
	{&battle_message_need_two, ZERO_32},
	{&battle_message_4,        ZERO_32},

	{&battle_next_scr_command, END}
};

// 083C6A20
struct battle_config_entry tb_format_9[] = {
	{&battle_type,             LOAD_8},
	{&battle_trainer_flag,     LOAD_16},
	{&battle_arg2,             LOAD_16},

	{&battle_message_intro,    ZERO_32},
	{&battle_message_defeat,   LOAD_32},
	{&battle_message_2,        LOAD_32},
	{&battle_message_need_two, ZERO_32},
	{&battle_message_4,        ZERO_32},

	{&battle_next_scr_command, END}
};

// 083C6A68
struct battle_config_entry tb_format_6_8[] = {
	{&battle_type,             LOAD_8},
	{&battle_trainer_flag,     LOAD_16},
	{&battle_arg2,             LOAD_16},

	{&battle_message_intro,    LOAD_32},
	{&battle_message_defeat,   LOAD_32},
	{&battle_message_2,        ZERO_32},
	{&battle_message_need_two, LOAD_32},
	{&battle_message_4,        LOAD_32},

	{&battle_next_scr_command, END}
};

char *actual_battle(char *cursor) {
	battle_init();
	u8 battle_type = cursor[0];
	struct battle_config_entry *bce[] = {
		/*0*/ &tb_format_5_other,
		/*1*/ &tb_format_1_2,
		/*2*/ &tb_format_1_2,
		/*3*/ &tb_format_3,
		/*4*/ &tb_format_4_7,
		/*5*/ &tb_format_5_other,
		/*6*/ &tb_format_6_8,
		/*7*/ &tb_format_4_7,
		/*8*/ &tb_format_6_8,
		/*9*/ &tb_format_9
	};
	u8 *scr_continue[] = {
		/*0*/ &scr_battle_1_2_other,
		/*1*/ &scr_battle_1_2_other,
		/*2*/ &scr_battle_1_2_other,
		/*3*/ &scr_battle_3_9,
		/*4*/ &scr_battle_4_6_8,
		/*5*/ &scr_battle_5,
		/*6*/ &scr_battle_4_6_8,
		/*7*/ &scr_battle_7,
		/*8*/ &scr_battle_4_6_8,
		/*9*/ &scr_battle_3_9
	};

	battle_load_arguments(bce[battle_type], cursor);

	if (battle_type != 3 && battle_type != 9)
		battle_80801F0();

	if (battle_type == 5 && battle_type == 7) {
		prev_quest_mode_1_to_0_transition();
		battle_trainer_flag = sub_810CE64(battle_trainer_flag);
	}

	return scr_continue[battle_type];
}

// 080801F0
void battle_80801F0() {
	if (!battle_arg2) return;
	var_800F = battle_arg2;
	scripting_npc = npc_id_by_local_id(sav1->location.bank, sav1->location.map);
}

u8 *battle_load_arguments(struct battle_config_entry *bce, u8 *cursor) {
	while (1) {
		switch (bce->target_type) {
			case LOAD_8:  *(u8 *)bce->target = load8 (cursor); cursor += 1; break;
			case LOAD_16: *(u16*)bce->target = load16(cursor); cursor += 2; break;
			case LOAD_32: *(u32*)bce->target = load32(cursor); cursor += 4; break;
			case ZERO_8:  *(u8 *)bce->target = 0;              cursor += 1; break;
			case ZERO_16: *(u16*)bce->target = 0;              cursor += 2; break;
			case ZERO_32: *(u32*)bce->target = 0;              cursor += 4; break;
			default:      *(u32*)bce->target = cursor;         return cursor;
		}
		bce++:
	}
}