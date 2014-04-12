// 0807F690
void task_add_01_battle_start(u8 priv1, u16 music_id) {
	u8 tid = task_add(&task01_battle_start, 0x01);
	tasks[tid].priv[1] = priv1;
	current_map_music_set__default_for_battle(music_id);
}

// 0807F868
void task_add_01_battle_start_with_music_and_stats() {
	task_add_01_battle_start(song_id_for_battle_alt(), 0);
	sav12_xor_increment(7);
	sav12_xor_increment(9);
}

/* see npc_cmds.c
	bool s5C_trainer_battle_configure_and_start(struct script_env *);
	bool s5D_trainer_battle_start(struct script_env *);
	bool s5E_jump_to_script_scheduled_after_battle(struct script_env *);
*/

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

	{&battle_after_script,     END}
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

	{&battle_after_script,     END}
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

	{&battle_after_script,     END}
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

	{&battle_after_script,     END}
};

// 083C6A20
struct battle_config_entry tb_format_9[] = {
	{&battle_type,             LOAD_8},
	{&battle_trainer_flag,     LOAD_16},
	{&battle_unknown,          LOAD_16}, // this is read by trainer_battle_start
	                                     // to add oak's comments (unknown&3) != 0

	{&battle_message_intro,    ZERO_32},
	{&battle_message_defeat,   LOAD_32},
	{&battle_message_2,        LOAD_32},
	{&battle_message_need_two, ZERO_32},
	{&battle_message_4,        ZERO_32},

	{&battle_after_script,     END}
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

	{&battle_after_script,     END}
};

// 08080228
char *battle_configure_by_script(char *cursor) {
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

// 08080168
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

// 08080424
bool trainer_flag_check(u16 trainer_id) {
	return flag_check(trainer_id+0x500);
}

// 0808043C
void trainer_flag_set(u16 trainer_id) {
	flag_set(trainer_id+0x500);
}

// 08080450
void trainer_flag_clear(u16 trainer_id) {
	flag_clear(trainer_id+0x500);
}

// 08080464
void trainer_battle_start() {
	battle_type_flags = BATTLE_TRAINER;

	if (trainerbattle_get_type == 9)
		if (trainerbattle_get_unknown() & 0x3)
			battle_type_flags |= BATTLE_OAK_COMMENTS;

	super.callback2backup = &c2_exit_battle_switch;
	task_add_01_battle_start_with_music_and_stats();
	script_env_2_context_set_ctx_paused();
}

// 080804AC
void c2_exit_battle_switch() {

	if (trainerbattle_battle_type == 9) {
		if ((var_800D = battle_exit_is_player_defeat(battle_exit_type))) {
			if (battle_unknown & 1 == 0) { // this is not a mock battle
				sp000_heal_pokemon();
			} else {
				set_callback2(&c2_whiteout);
				return;
			}
		}
	} else if (trainerbattle_flag_id == 0x400) {
		set_callback2(&c2_exit_to_overworld_1_continue_scripts_restart_music);
		return;
	}

	set_callback2(&c2_exit_to_overworld_1_continue_scripts_restart_music);
	sp18F_trainerflag_opponent_set();
	prev_quest_transcriber();
}

// 081C555B
u8 scr_default_after_battle_script[] = {
	// var0 = 0x081C558D
	// callstd 3
};

// 080805E8
u8 *battle_get_continuation_script() {
	u8 *scr = battle_next_script;
	return scr ? scr : scr_default_after_battle_script;
}

// 083E7CD4
void (*battle_intro_task_by_env[])() = {
	task_battle_intro_080BC47C,
	task_battle_intro_080BC47C,
	task_battle_intro_080BC6C8,
	task_battle_intro_080BC6C8,
	task_battle_intro_080BC6C8,
	task_battle_intro_080BC47C,
	task_battle_intro_080BC47C,
	task_battle_intro_080BC47C,
	task_battle_intro_080BC9D4,
	task_battle_intro_080BC9D4
};

// 080BC3A0
void battle_intro_launch(u8 environment_id) {
	void (*funcptr)(u8);
	if (battle_type_flags & BATTLE_WIRELESS)
		funcptr = task_battle_intro_wireless;
	else if ((battle_type_flags & 0x1000) && (byte_081E9F10 == 2)) {
		environment_id = 3;
		funcptr = task_battle_intro_080BC6C8;
	} else {
		funcptr = battle_intro_task_by_env[environment_id];
	}
	u8 tid = task_add(funcptr, 0);
	tasks[tid].priv[0] = 0;
	tasks[tid].priv[1] = environment_id;
	tasks[tid].priv[2] = 0;
	tasks[tid].priv[3] = 0;
	tasks[tid].priv[4] = 0;
	tasks[tid].priv[5] = 0;
	tasks[tid].priv[6] = 0;
}
