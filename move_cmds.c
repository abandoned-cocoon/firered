/* 
03: pp_decrement
04: critical_calculator
05: base_damage_calculator
06: type_damage_calculator
0B: decrese_health_bar
0C: decrease_health_value
0D: determine_critical
10: message_and_critical_hit_maybe
21: goto_if_any_bit_of_mask_set
28: goto
29: compare_jump_8
2A: compare_jump_16
2D: memcmp
2E: ptr_set
2F: ptr_add
30: ptr_sub
31: memcpy
32: memcpy_with_offset
33: ptr_bit_set_8
34: ptr_bit_set_16
35: ptr_bit_set_32
36: ptr_bit_clear_8
37: ptr_bit_clear_16
38: ptr_bit_clear_32
39: wait
3A: wait_for_dp01_bitfield_to_be_zero
43: ability_checking_maybe
45: animation
55: fanfare
63: goto_move
6B: end_battle_80261CC
6E: state0_side_becomes_attacker
72: flee
73: hp_tresholds
74: hp_tresholds_2
79: set_hp_0
7B: refill_half_hp_jump_if_fully_healed
93: ko_move
E3: if_dead
E7: castform_switch
EF: item_handler_maybe
F0: catch_pokemon
F4: subtract_damage_from_attacker_hp
F5: reset_attacker_bd_x4C
*/

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

// 08022650
void mcmd28_goto() {
    u32 target = (move_exec_cursor[1]<< 0) + (move_exec_cursor[2]<< 8)
               + (move_exec_cursor[3]<<16) + (move_exec_cursor[4]<<24);
    move_exec_cursor = target;
}

static bool compare(u8 mode, u32 a, u32 b) {
    switch (mode) {
        case 0: return a == b;
        case 1: return a != b;
        case 2: return a > b;
        case 3: return a < b;
        case 4: return a & b;
    }
}

// 08022670
// [29][mode:1][ptr_a:4][b:1][goto_target:4]
void mcmd29_compare_jump_8() {
    u8 mode = move_exec_cursor[1];
    u8 b    = move_exec_cursor[6];
    u8 *ptr_a  = (move_exec_cursor[2]<< 0) + (move_exec_cursor[3]<< 8)
               + (move_exec_cursor[4]<<16) + (move_exec_cursor[5]<<24);
    u32 target = (move_exec_cursor[7]<< 0) + (move_exec_cursor[ 8]<< 8)
               + (move_exec_cursor[9]<<16) + (move_exec_cursor[10]<<24);
    if (compare_jump(mode, *ptr_a, b);
        move_exec_cursor = target;
    else
        move_exec_cursor += 11;
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
        u32 target = (move_exec_cursor[1]<< 0) + (move_exec_cursor[2]<< 8)
                   + (move_exec_cursor[3]<<16) + (move_exec_cursor[4]<<24);
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
