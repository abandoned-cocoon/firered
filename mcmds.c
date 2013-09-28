void mcmd00_0801D760() {

    if (byte_02023E8A) {
        dp01s_mode = 12; // 08016D70
        return;
    }

    struct battle_data *batk = battle_data_4x[battle_side_of_attacker_hl];

    uint16 b_28 = batk->field_28;

    if (!b_28 && (dword_02023DD0 & 0x200)==0) {
        dword_02023DD |= 0x80000;
        move_exec_cursor = &movescr_081D694E;
        return;
    }

    if (sub_080192D4())
        return;

    if (sub_08019F18(2, battle_side_of_defender_hl, 0, 0, 0))
        return;

    if (batk->field_24[byte_02023D48] == 0 &&
        move_to_execute != 0xA5            && // 165
        dword_02023DD0 & 0x00800200 == 0   &&
        batk->field_50 & 0x00001000 == 0) {

        move_exec_cursor = &movescr_081D8EA8;
        byte_02023DCC |= 1;
        return;
    }

    dword_02023DD0 &= ~0x00800000;

    uint x;

    if (dword_02023DD0 & 0x02000000 == 0 &&
        batk->field_50 & 0x00001000 == 0 &&
        (x = sub_0801D438())) {

        if (x == 2)
            dword_02023DD0 |= 0x02000000;
        else
            byte_02023DCC |= 1;
        return;
    }

    dword_02023DD0 |= 0x02000000;

    if ((int5)x_02023E8C[battle_side_of_defender_hl] < 0 &&
        move_data[move_to_execute].flags & MOVE_AFFECTED_BY_MAGIC_COAT) {
        sub_08016EC8(battle_side_of_attacker_hl,
                     battle_side_of_defender_hl,
                     0x115); // 277
        x_02023E8C[battle_side_of_defender_hl] &= ~0x11;
        sub_08017544();
        move_exec_cursor = &movescr_081D8FAA;
        return;
    }

    // TO BE CONTINUED
}

void mcmd60_08025B74() {
    if (battle_side_get_owner(battle_side_of_attacker_hl) == 0)
        sub_08054E90(move_exec_cursor[1]);
    move_exec_cursor += 2;
}

void mcmd62_08025C6C() {
    dp01_battle_side = get_battle_side_of(move_exec_cursor[1]);
    dp01_build_cmdbuf_x31_31_31_31(0);
    dp01_bitfield_set_flag_for_side(dp01_battle_side);
    move_exec_cursor += 2;
}

void mcmd63_continue_with_move() {
    move_to_execute = word_02023D4E;
    if (move_exec_cursor[1] == 0)
        word_02023D4C = word_02023D4E;
    move_exec_cursor = move_scripts[move_data[move_to_execute].move_script_id];
}

// 080267F0
void mcmd6E_state0_side_becomes_attacker() {
    battle_side_of_attacker_hl = battle_get_side_with_given_state(0);
    move_exec_cursor++;
}

// 080268B8 
void mcmd72_flee() {
    if (battle_flee()) {
        u32 target = (move_exec_cursor[1]<< 0) + (move_exec_cursor[3]<< 8)
                   + (move_exec_cursor[4]<<16) + (move_exec_cursor[5]<<24);
        move_exec_cursor = target;
    } else
        move_exec_cursor += 5;
}

uint8 get_battle_side_of(uint8 whom) {
    switch (whom) {
    case 0: return battle_side_of_defender_hl;
    case 1: return battle_side_of_attacker_hl;
    case 2: return byte_02023D6E;
    case 3:
    case 5: return byte_02023D6D;
    case 10: return byte_02023FC4[0x17];
    case 11: return sub_080751E8(0);
    case 12: return sub_080751E8(1);
    default: return 0;
    }
}