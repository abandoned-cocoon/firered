#include "battle.h"
#include "engine_scripts.h"
#include "move_etc.h"
#include "move_interp.h"
#include "moves.h"

extern u8 bits[];
u8 get_battle_side_of(u8 whom);

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
54* battle cmd 2B
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

// 0801D760
void mcmd00_0801D760() {

    if (bc_bs_followup_bc_func_index /*battle_exit_type*/) {
        bs_mode = 12; // 08016D70
        return;
    }

    struct battle_data *batk = &b_data[b_attacker];

    if (batk->current_hp == 0 && (b_features_bitfield & 0x200)==0) {
        b_features_bitfield |= 0x80000;
        b_move_cursor = movescr_081D694E_common;
        return;
    }

    if (dp08_B7_effects_80192D4())
        return;

    if (ability_something(as_move_veto_soundproof, b_defender, 0, 0, 0))
        return;

    if (batk->current_pp[b_moveset_index] == 0 &&
        b_move_to_execute_A != mve_struggle &&
        b_features_bitfield & 0x00800200 == 0 &&
        batk->status2 & 0x00001000 == 0) {

        b_move_cursor = movescr_no_pp_left;
        b_attack_effectivity |= 1;
        return;
    }

    b_features_bitfield &= ~0x00800000;

    uint x;

    if (b_features_bitfield & 0x02000000 == 0 &&
        batk->status2 & 0x00001000 == 0 &&
        (x = sub_0801D438())) {

        if (x == 2)
            b_features_bitfield |= 0x02000000;
        else
            b_attack_effectivity |= 1;
        return;
    }

    b_features_bitfield |= 0x02000000;

    if (protect_structs[b_defender].flags & B_PR1_MAGIC_COAT &&
        move_data[b_move_to_execute_A].flags & MOVE_FLAG_AFFECTED_BY_MAGIC_COAT) {
        sub_08016EC8(b_attacker,
                     b_defender,
                     0x115); // 277
        // strip magic coat
        protect_structs[b_defender].flags &= ~B_PR1_MAGIC_COAT;
        b_movescr_stack_push_cursor();
        b_move_cursor = movescr_081D8FAA;
        return;
    }

    // TO BE CONTINUED
}

// 0801E1D8
void mcmd02_display_x_used_y_message() {
    char v0 = b_buffers_awaiting_execution_bitfield;

    if (v0) {
        if (b_features_bitfield & 0x600 == 0) {
            // X used Y!
            b_std_message(4, b_attacker);
            b_features_bitfield |= 0x400;
        }

        b_buffers_awaiting_execution_bitfield__copied_after_displaying_x_used_y_message = v0;
        b_move_cursor++;
    }
}

// 0801E22C
void mcmd03_pp_decrement() {

    int pp_deduction = 1;

    if (b_buffers_awaiting_execution_bitfield)
        return;

    if ( !(dp16_array[b_attacker].flags << 26 >> 31) )
    {
        u8 affected = move_data[b_move_to_execute_A].target;
        if ( affected == 0x20 )                   // everyone affected
        {
            pp_deduction = ability_something(
                               as_count_excl_self,
                               b_attacker,
                               ab_pressure,
                               0,
                               dp16_array[b_attacker].flags << 26 >> 31)
                         + 1;
        }
        else
        {
            if ( affected > 32 )
            {
                if ( affected != 64 )
                {
                  LABEL_11:
                    if ( b_attacker != b_defender )
                    {
                        if ( b_data[b_defender].ability_id == ab_pressure )
                            pp_deduction = 2;
                    }
                    goto LABEL_14;
                }
            }
            else if ( affected != 8 )
            {
                goto LABEL_11;
            }
            pp_deduction = ability_something(as_count_other_team, b_attacker, ab_pressure, 0, 0) + 1;
        }
    }
  LABEL_14:
    if (b_features_bitfield & (B_F_PP_DEDUCTION_OUTSTANDING | B_F_INHIBIT_X_USED_Y_MESSAGE) == 0) {

        if ( b_data[b_attacker].current_pp[b_moveset_index] )
        {
            protect_structs[b_attacker].field_2 |= 8u;
            u8 *p_pp = &b_data[b_attacker].current_pp[b_moveset_index];
            u8 pp = *p_pp;
            if ( pp <= pp_deduction )
                *p_pp = 0;
            else
                *p_pp = pp - pp_deduction;
            if ( !(b_data[b_attacker].status2 & B_S2_TRANSFORMED) )
            {
                if ( !((b_disable_data_pbs[b_attacker].field_18 /*flags_mmmmuuuu__m_moves*/ >> 4) & bits[b_moveset_index]) )
                {
                    b_active_side = b_attacker;
                    bt_02_emit(TARGET_BUFFER_A, (b_moveset_index + 9) & 0xFF, 0, 1,
                        &b_data[b_attacker].current_pp[b_moveset_index]);
                    b_active_side_mark_buffer_for_execution(b_attacker);
                }
            }
        }
    }
    b_features_bitfield &= 0xFFFFF7FFu;
    ++b_move_cursor;
}

// 0801FA7C
void mcmd0D_critical_print_message() {
    // wait for low-level system to be idle
    if (b_buffers_awaiting_execution_bitfield)
        return;

    if (b_critical_multiplier != 2)
        return;

    if (b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED))
        return;

    b_std_message(/*A critical hit*/0xD9, b_attacker);
    //b_message_shown_maybe = 1;
    b_buffers_awaiting_execution_bitfield__copied_after_displaying_x_used_y_message = 1;
}

// 08022650
void mcmd28_goto() {
    u32 target = (b_move_cursor[1]<< 0) + (b_move_cursor[2]<< 8)
               + (b_move_cursor[3]<<16) + (b_move_cursor[4]<<24);
    b_move_cursor = (u8*)target;
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
    u8 mode = b_move_cursor[1];
    u8 b    = b_move_cursor[6];
    u32 addr_a = (b_move_cursor[2]<< 0) + (b_move_cursor[3]<< 8)
               + (b_move_cursor[4]<<16) + (b_move_cursor[5]<<24);
    u32 target = (b_move_cursor[7]<< 0) + (b_move_cursor[ 8]<< 8)
               + (b_move_cursor[9]<<16) + (b_move_cursor[10]<<24);
    if (compare_jump(mode, *(u8*)addr_a, b))
        b_move_cursor = (u8*)target;
    else
        b_move_cursor += 11;
}

// 08023210
void mcmd49() {
    u32 move_id = b_move_to_execute_B == 0xFFFF ? 0 : b_move_to_execute_B;
    /* ... */
    while (1) {
        u8 modo;
        switch (modo) {
        case 0: {
            struct battle_data *defender = &b_data[b_defender];
            if ((defender->status2 & B_S2_ATK_UP_IF_HIT) &&
                (defender->current_hp != 0) &&
                (b_attacker != b_defender) &&
                (battle_side_get_owner(b_attacker) != battle_side_get_owner(b_defender)) &&
                (b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED) == 0) &&
                (dp16_array[b_defender].some_damage_amount__phys_damage_only_maybe != 0 ||
                 dp16_array[b_defender].field_C != 0) &&
                (move_data[b_move_to_execute_A].power != 0) &&
                (defender->stat_buffs[STAT_ATK] < 12)
            ) {
                defender->stat_buffs[STAT_ATK]++;
                b_stack_push_move_cursor();
                b_move_cursor = movescr_after_atk_up_if_hit;
            }
            /* ... */
        }
        }
        /* ... */
    }
    /* ... */
}

// 08025B74
void mcmd60_08025B74() {
    if (battle_side_get_owner(b_attacker) == 0)
        sub_08054E90(b_move_cursor[1]);
    b_move_cursor += 2;
}

void mcmd62_08025C6C() {
    b_active_side = get_battle_side_of(b_move_cursor[1]);
    bt_31_emit(TARGET_BUFFER_A);
    dp01_bitfield_set_flag_for_side(b_active_side);
    b_move_cursor += 2;
}

// 08025C9C
void mcmd63_continue_with_move() {
    b_move_to_execute_A = b_move_to_execute_C;
    if (b_move_cursor[1] == 0)
        b_move_to_execute_B = b_move_to_execute_C;
    b_move_cursor = move_scripts[move_data[b_move_to_execute_A].move_script_id];
}

// 080267F0
void mcmd6E_state0_side_becomes_attacker() {
    b_attacker = battle_get_side_with_given_state(0);
    b_move_cursor++;
}

// 080268B8 
void mcmd72_flee() {
    if (battle_flee()) {
        u32 target = (b_move_cursor[1]<< 0) + (b_move_cursor[2]<< 8)
                   + (b_move_cursor[3]<<16) + (b_move_cursor[4]<<24);
        b_move_cursor = (u8*)target;
    } else
        b_move_cursor += 5;
}

void atkD3_copy_ability() {
    struct battle_data *attacker = &b_data[b_attacker];
    struct battle_data *defender = &b_data[b_defender];

    if (defender->ability_id != ab_none &&
        defender->ability_id != ab_wonder_guard )
    {
        attacker->ability_id = b_last_copied_ability = defender->ability_id;
        b_move_cursor += 5;
    } else {
        u32 target = (b_move_cursor[1]<< 0) + (b_move_cursor[2]<< 8)
                   + (b_move_cursor[3]<<16) + (b_move_cursor[4]<<24);
        b_move_cursor = (u8*)target;
    }
}

u8 get_battle_side_of(u8 whom) {
    switch (whom) {
    case 0: return b_defender;
    case 1: return b_attacker;
    case 2: return b_defender_partner;
    case 3:
    case 5: return b_defender_partner;
    case 10: return b_side_unknown; // = byte_02023FC4[0x17];
    case 11: return sub_080751E8(0);
    case 12: return sub_080751E8(1);
    default: return 0;
    }
}
