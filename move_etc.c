pokemon *party_of(int owner) {
    return owner ? party_opponent : party_player;
}

enum as_mode {
    // actions
    as_0 = 0x0,
    as_1 = 0x1,
    as_2 = 0x2,
    as_3 = 0x3,
    as_4 = 0x4,
    as_5 = 0x5,
    as_6 = 0x6,
    as_7 = 0x7,
    as_8 = 0x8,
    as_9 = 0x9,
    as_A = 0xA,
    as_B = 0xB,
    // queries
    as_find_in_other_team = 0xCu,
    as_find_in_own_team = 0xDu,
    as_find_in_own_team_excl_self = 0xF,
    as_count_other_team = 0x10,
    as_count_own_team = 0x11,
    as_count_excl_self = 0x12,
    as_find_in_alive_pokemon = 0x13
};

int ability_something(
    unsigned int mode,
    u8 battle_side_given,
    enum ability ability1,
    enum ability ability_fallback,
    unsigned short move_id
) {

    move_override = move_id;
    return_value = 0;

    if (b_attacker >= b_num_active_sides)
        b_attacker = battle_side_given;

    // Good question: What's the difference between:
    //  b_pokemon_team_id_by_side(x)
    // and
    //  battle_side_get_owner(x)

    pokemon *attacker = party_of(b_pokemon_team_id_by_side[b_attacker]);
    pokemon *defender = party_of(b_pokemon_team_id_by_side[b_defender]);

    u16 attacker_species = pokemon_getattr(attacker, req_species, v13);
    u32 attacker_iops    = pokemon_getattr(attacker, req_pokemon_id_or_personality, v17);
    u16 defender_species = pokemon_getattr(defender, req_species, v18);
    u32 defender_iops    = pokemon_getattr(defender, req_pokemon_id_or_personality, v19);

    if (battle_type_flags & battle_safari)
        return return_value;

    if (ability_fallback) {
        b_last_copied_ability = ability_fallback;
    }
    else {
        b_last_copied_ability = battle_data[battle_side_given].ability_id;
    }

    if (!move_override)
        move_override = move_to_execute_A;

    move_type = dp08_ptr->move_to_execute_type_override
              ? dp08_ptr->move_to_execute_type_override & 0x3F
              : move_data[move_override].type;

    enum ability ability = b_last_copied_ability

    if ((battle_type_flags & 0xA000) == battle_ghost) {
        if (ability == intimidate || ability == trace)
            return return_value;
    }

    #define for_each_side for (u32 i = 0; i < b_num_active_sides; ++i)
    #define find_side(cond) for_each_side { if(cond) { return_value=i+1; break; } }
    #define find_side_cabi(cond) for_each_side { if(cond) { b_last_copied_ability = ability1; return_value=i+1; break; } }

    switch (mode) {
        case 0u:
            if (b_attacker >= b_num_active_sides)
                b_attacker = battle_side_given;
            v23 = ability;
            if (v23 == sand_stream) {
                if (weather & 0x10)
                    goto def_801A8CE;
                weather = 24;
                b_push_move_exec(&unk_81D92BF);
                battle_side_unknown = battle_side_given;
                return_value = 1;
                goto def_801A8CE;
            } else if (v23 > sand_stream) { // 0x2D
                if (v23 == drought) {
                    if ((weather & 0x40) == 0) {
                        weather = 96;
                        b_push_move_exec(&unk_81D9379);
                        battle_side_unknown = battle_side_given;
                        return_value = 1;
                    }

                } else {
                    if (v23 <= drought) {
                        if (v23 != forecast)
                            goto def_801A8CE;
                        v28 = castform_switch(battle_side_given);
                        return_value = v28;
                        if (!v28)
                            return return_value;
                        b_push_move_exec(&castform_script);
                        battle_side_unknown = battle_side_given;
                        dp08_ptr->pading_7F[0] = return_value - 1;
                        goto def_801A8CE;
                    }
                    if (v23 == air_lock)
                        goto airlock_cloudnine;
                    if (v23 != 255)
                        goto def_801A8CE;
                    v24 = weather_get_current() - 3;
                    if (v24 <= 0xA) {
                        switch (v24) {
                            case 0:
                            case 2:
                            case 10:
                                if (!(weather & 7)) {
                                    weather = 5;
                                    byte_2023FD4 = 10;
                                    battle_side_unknown = battle_side_given;
                                    return_value = 1;
                                }
                                break;
                            case 5:
                                if (!(weather & 0x18)) {
                                    weather = 24;
                                    byte_2023FD4 = 12;
                                    battle_side_unknown = battle_side_given;
                                    return_value = 1;
                                }
                                break;
                            case 9:
                                if (!(weather & 0x60)) {
                                    weather = 96;
                                    byte_2023FD4 = 11;
                                    battle_side_unknown = battle_side_given;
                                    return_value = 1;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    if (return_value) {
                        byte_2023E87 = weather_get_current();
                        b_push_move_exec(&unk_81D8B1F);
                        goto def_801A8CE;
                    }
                    return return_value;
                }
            }
            if (v23 == cloud_nine) {
              airlock_cloudnine:
                for_each_side {
                    if ((return_value = castform_switch(i)))
                        break;
                }
                b_push_move_exec(&castform_script);
                battle_side_unknown = i+1;
                dp08_ptr->pading_7F[0] = return_value - 1;

            } else if (v23 > cloud_nine) {
                if (v23 == intimidate) {
                    v25 = &dp16_array[battle_side_given];
                    if (v25->field_0 & 8)
                        goto def_801A8CE;
                    b_effect_bitfield_pbs[battle_side_given] |= 0x80000u;
                    v26 = v25->field_0;
                    v27 = 8;
                    v25->field_0 = v26 | v27;
                }
                else if (v23 == trace)
                    v25 = &dp16_array[battle_side_given];
                    if ((v25->field_0 & 0x10) == 0) {
                        b_effect_bitfield_pbs[battle_side_given] |= 0x100000u;
                        v26 = v25->field_0;
                        v27 = 16;
                        v25->field_0 = v26 | v27;
                    }
                }

            } else if (v23 == drizzle && !(weather & 4)) {
                weather = 5;
                b_push_move_exec(&unk_81D927F);
                battle_side_unknown = battle_side_given;
                return_value = 1;
            }
        case 1u:
            v31 = &battle_data[battle_side_given];
            if (!v31->current_hp)
                break;
            b_attacker = battle_side_given;
            v32 = ability;
            if (v32 == rain_dish) {
                if (!ability_something(0x13u, 0, cloud_nine, 0, 0)
                  && !(ability_something(0x13u, 0, air_lock, 0, 0) << 24)
                  && weather & 7
                  && v31->max_hp > v31->current_hp) {
                    b_last_copied_ability = rain_dish;
                    b_push_move_exec(&unk_81D92AB);
                    battle_attack_damage = v31->max_hp >> 4;
                    if (!battle_attack_damage)
                        battle_attack_damage = 1;
                    battle_attack_damage = -battle_attack_damage;
                    return_value = 1;
                    goto def_801A8CE;
                }
            }
            else if (v32 > rain_dish) {
                if (v32 == truant) {
                    b_disable_data_pbs[b_attacker].field_18 = b_disable_data_pbs[b_attacker].field_18 & 0xFE | b_disable_data_pbs[b_attacker].field_18 & 1 ^ 1;
                    goto def_801A8CE;
                }
                if (v32 == shed_skin) {
                    v33 = &battle_data[battle_side_given].status1;
                    if (LOBYTE(battle_data[battle_side_given].status1)) {
                        if (!(rand() % 3u & 0xFFFF)) {
                            if (*v33 & 0x88)
                                strcpy_xFF_terminated(&battle_outcome, &unk_8250094);
                            if (*v33 & 7)
                                strcpy_xFF_terminated(&battle_outcome, &unk_825009C);
                            if (*v33 & 0x40)
                                strcpy_xFF_terminated(&battle_outcome, &unk_82500A4);
                            if (*v33 & 0x10)
                                strcpy_xFF_terminated(&battle_outcome, &unk_82500AC);
                            if (*v33 & 0x20)
                                strcpy_xFF_terminated(&battle_outcome, &unk_82500B4);
                            *v33 = 0;
                            battle_data[battle_side_given].status2 &= 0xF7FFFFFFu;
                            b_active_side = battle_side_given;
                            battle_side_unknown = battle_side_given;
                            b_push_move_exec(&unk_81D92D3);
                            dp01_build_cmdbuf_x02_a_b_varargs(0, 40, 0, 4);
                            b_active_side_mark_buffer_for_execution(b_active_side);
                            return_value = 1;
                            goto def_801A8CE;
                        }
                    }
                }
            }
            else if (v32 == speed_boost && v31->stat_buffs[3] <= 11 && b_disable_data_pbs[battle_side_given].battle_start != 2) {
                ++v31->stat_buffs[3];
                byte_2023FD4 = 17;
                byte_2023FD5 = 0;
                b_push_move_exec(&unk_81D9293);
                battle_side_unknown = battle_side_given;
                return_value = 1;
                goto def_801A8CE;
            }
            goto def_801A8CE;
        case 2u:
            if (ability != soundproof)
                goto def_801A8CE;
            v34 = 0;
            if (a_0gn[0] == 65535)
                goto def_801A8CE;
            if (a_0gn[0] == move_override)
                goto LABEL_110;
            v35 = "-";
            while (1) {
                v35 += 2;
                ++v34;
                v36 = *v35;
                if (v36 == 65535)
                    goto def_801A8CE;
                if (v36 == move_override) {
                  LABEL_110:
                    if (a_0gn[v34] == 65535)
                        goto def_801A8CE;
                    if (battle_data[b_attacker].status2 & 0x1000)
                        b_features_bitfield |= 0x800u;
                    b_move_cursor = &unk_81D946E;
                    return_value = 1;
                    goto def_801A8CE;
                }
            }
        case 3u:
            if (!move_override)
                goto def_801A8CE;
            if (ability == water_absorb) {
                if (move_type != 11 || !move_data[move_override].power)
                    goto LABEL_145;
                if (protect_structs[b_attacker].field_2 & 8) {
                    b_move_cursor = &byte_81D93BE;        // don't pp decrement
                }
                else {
                    b_move_cursor = &byte_81D93BD;        // do pp decrement
                }
                v42 = 1;
            }
            else {
                if (ability <= water_absorb) {
                    if (ability == volt_absorb) {
                        if (move_type == type_electric) {
                            if (move_data[move_override].power) {
                                if (protect_structs[b_attacker].field_2 & 8) {
                                    b_move_cursor = &byte_81D93BE;
                                } else {
                                    b_move_cursor = &byte_81D93BD;
                                }
                                return_value = 1;
                            }
                        }
                    }
                    LABEL_145:
                    if (return_value == 1) {
                        v47 = &battle_data[battle_side_given];
                        if (v47->max_hp == v47->current_hp) {
                            if (protect_structs[b_attacker].field_2 & 8)
                                b_move_cursor = &unk_81D93E1;
                            else
                                b_move_cursor = &unk_81D93E0;
                        }
                        else {
                            battle_attack_damage = v47->max_hp >> 2;
                            if (!battle_attack_damage)
                                battle_attack_damage = 1;
                            battle_attack_damage = -battle_attack_damage;
                        }
                    }
                    goto def_801A8CE;
                }
                if (ability != flash_fire || move_type != type_fire || battle_data[battle_side_given].status1 & 0x20)
                    goto LABEL_145;
                if (!(*(&battle_resource_bundle->field_4->field_0 + battle_side_given) & 1)) {
                    byte_2023E87 = 0;
                    if (protect_structs[b_attacker].field_2 & 8) {
                        b_move_cursor = &unk_81D93F7;
                    }
                    else {
                        b_move_cursor = &unk_81D93F6;
                    }
                    *(&battle_resource_bundle->field_4->field_0 + battle_side_given) |= 1u;
                    return_value = 2;
                    goto LABEL_145;
                }
                byte_2023E87 = 1;
                if (protect_structs[b_attacker].field_2 & 8) {
                    b_move_cursor = &unk_81D93F7;
                }
                else {
                    b_move_cursor = &unk_81D93F6;
                }
                v42 = 2;
            }
            return_value = v42;
            goto LABEL_145;
        case 4u:
            // ability effects for touching moves
            switch (ability) {
                case color_change:
                    if ((b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED)))
                        break;
                    if (move_override == mve_struggle)
                        break;
                    if (move_data[move_override].power) {
                        v49 = b_defender;
                        if (dp16_array[v49].field_8 || dp16_array[v49].field_C) {
                            v50 = &battle_data[battle_side_given];
                            v51 = &battle_data[battle_side_given].type1;
                            if (*v51 != move_type) {
                                if (v50->type2 != move_type && v50->current_hp) {
                                    *v51 = move_type;
                                    v50->type2 = move_type;
                                    battle_outcome = -3;
                                    byte_2022AB9 = 3;
                                    byte_2022ABA = move_type;
                                    byte_2022ABB = -1;
                                    b_movescr_stack_push_cursor();
                                    b_move_cursor = &unk_81D949C;
                                    return_value = 1;
                                    goto def_801A8CE;
                                }
                            }
                        }
                    }
                    break;
                case rough_skin:
                    if ((b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED)))
                        break;
                    if (battle_data[b_attacker].current_hp == 0)
                        break;
                    if ((protect_structs[b_attacker].protect_flags & 1))
                        break;
                    if (dp16_array[b_defender].field_8 || dp16_array[b_defender].field_C) {
                        if (move_data[move_override].flags & 1) {
                            u32 rough_skin_damage = battle_data[b_attacker]->max_hp / 16;
                            battle_attack_damage = min(1, rough_skin_damage);

                            b_movescr_stack_push_cursor();
                            b_move_cursor = &movescr_rough_skin;
                            return_value = 1;
                            break;
                        }
                    }
                    break;

                case effect_spore:
                    if ((b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED)))
                        break;
                    if (battle_data[b_attacker].current_hp == 0)
                        break;
                    if ((protect_structs[b_attacker].protect_flags & 1))
                        break;

                    if (dp16_array[b_defender].field_8 || dp16_array[b_defender].field_C) {
                        if (move_data[move_override].flags & 1 && !(rand() % 0xAu << 16)) {
                            do {
                                v56 = rand() & 3;
                                effect_to_apply = v56;
                            }
                            while (!v56);
                            if (effect_to_apply == 3)
                                effect_to_apply = 5;
                            effect_to_apply += 64;
                            b_movescr_stack_push_cursor();
                            b_move_cursor = &unk_81D94CB;
                            b_features_bitfield |= 0x2000u;
                            return_value = 1;
                            goto def_801A8CE;
                        }
                    }
                    break;

                case poison_point:
                    if ((b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED)))
                        break;
                    if (battle_data[b_attacker].current_hp == 0)
                        break;
                    if ((protect_structs[b_attacker].protect_flags & 1))
                        break;
                    if (dp16_array[b_defender].field_8 || dp16_array[b_defender].field_C) {
                        if (move_data[move_override].flags & 1 && !(rand() % 3u << 16)) {
                            effect_to_apply = 66;
                            b_movescr_stack_push_cursor();
                            b_move_cursor = &unk_81D94CB;
                            b_features_bitfield |= 0x2000u;
                            return_value = 1;
                            goto def_801A8CE;
                        }
                    }
                    break;
                case static_:
                    if ((b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED)))
                        break;
                    if (battle_data[b_attacker].current_hp == 0)
                        break;
                    if ((protect_structs[b_attacker].protect_flags & 1))
                        break;

                    if (dp16_array[b_defender].field_8 || dp16_array[b_defender].field_C) {
                        if (move_data[move_override].flags & 1 && !(rand() % 3u << 16)) {
                            effect_to_apply = 69;
                            b_movescr_stack_push_cursor();
                            b_move_cursor = &unk_81D94CB;
                            b_features_bitfield |= 0x2000u;
                            return_value = 1;
                            goto def_801A8CE;
                        }
                    }
                    break;

                case flame_body:
                    if ((b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED)))
                        break;
                    if (battle_data[b_attacker].current_hp == 0)
                        break;
                    if ((protect_structs[b_attacker].protect_flags & 1))
                        break;
                    if (move_data[move_override].flags & 1) {
                        if (dp16_array[b_defender].field_8 || dp16_array[b_defender].field_C) {
                            if (!(rand() % 3u << 16)) {
                                effect_to_apply = 67;
                                b_movescr_stack_push_cursor();
                                b_move_cursor = &unk_81D94CB;
                                b_features_bitfield |= 0x2000u;
                                return_value = 1;
                                goto def_801A8CE;
                            }
                        }
                    }
                    break;
                case cute_charm:
                    if ((b_attack_effectivity & (B_AE_FAILED | B_AE_NOT_AFFECTED | B_AE_MISSED)))
                        break;
                    if (battle_data[b_attacker].current_hp == 0)
                        break;
                    if ((protect_structs[b_attacker].protect_flags & 1))
                        break;
                    if ((move_data[move_override].flags & 1) == 0)
                        break;
                    if (!(dp16_array[b_defender].field_8 || dp16_array[b_defender].field_C))
                        break;
                    if (battle_data[b_defender].current_hp == 0)
                        break;
                    if ((rand() % 3u << 16))
                        break;
                    if (battle_data[b_attacker].ability_id == oblivious)
                        break;
                    if (pokemon_species_get_gender_info(attacker_species, attacker_iops) !=
                        pokemon_species_get_gender_info(defender_species, defender_iops))
                    {
                        if (!(battle_data[b_attacker].status2 & 0xF0000)) { // pre existing infatuation
                            if (pokemon_species_get_gender_info(
                                     attacker_species,
                                     attacker_iops) != 255
                              && pokemon_species_get_gender_info(
                                     defender_species,
                                     defender_iops) != 255) {
                                battle_data[b_attacker].status2 |= (bits[b_defender] << 16);
                                b_movescr_stack_push_cursor();
                                b_move_cursor = &unk_81D94BE;
                                return_value = 1;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        case 5u:
            battle_side_given = 0;
            if (b_num_active_sides <= 0u)
                goto def_801A8CE;
            while (1) {
                v66 = (battle_data[battle_side_given].ability_id - 7);
                if (v66+7 <= shell_armor) {
                    switch (battle_data[battle_side_given].ability_id) {
                        case immunity:
                            v66 = (battle_data[battle_side_given].status1 & 0xF88);
                            if (!v66)
                                break;
                            v67 = &battle_outcome;
                            v68 = &unk_8250094;
                            v66 = strcpy_xFF_terminated(v67, v68);
                            return_value = 1;
                            break;
                        case own_tempo:
                            v66 = (battle_data[battle_side_given].status2 & 7);
                            if (!v66)
                                break;
                            v66 = strcpy_xFF_terminated(&battle_outcome, &unk_82500BC);
                            return_value = 2;
                            break;
                        case limber:
                            v66 = (battle_data[battle_side_given].status1 & 0x40);
                            if (!v66)
                                break;
                            v67 = &battle_outcome;
                            v68 = &unk_82500A4;
                            v66 = strcpy_xFF_terminated(v67, v68);
                            return_value = 1;
                            break;
                        case insomnia:
                        case vital_spirit:
                            v66 = (battle_data[battle_side_given].status1 & 7);
                            if (!v66)
                                break;
                            battle_data[battle_side_given].status2 &= 0xF7FFFFFFu;
                            strcpy_xFF_terminated(&battle_outcome, &unk_825009C);
                            v66 = 1;
                            return_value = 1;
                            break;
                        case water_veil:
                            v66 = (battle_data[battle_side_given].status1 & 0x10);
                            if (!v66)
                                break;
                            v66 = strcpy_xFF_terminated(&battle_outcome, &unk_82500AC);
                            return_value = 1;
                            break;
                        case magma_armor:
                            v66 = (battle_data[battle_side_given].status1 & 0x20);
                            if (!v66)
                                break;
                            v67 = &battle_outcome;
                            v68 = &unk_82500B4;
                            v66 = strcpy_xFF_terminated(v67, v68);
                            return_value = 1;
                            break;
                        case oblivious:
                            v69 = battle_data[battle_side_given].status2;
                            v66 = (v69 & attraction_infatuation);
                            if (v66) {
                                v66 = strcpy_xFF_terminated(&battle_outcome, &unk_82500C4);
                                return_value = 3;
                            }
                            break;
                        default:
                            break;
                    }
                }
                if (return_value)
                    break;
                battle_side_given = (battle_side_given + 1) & 0xFF;
                if (battle_side_given >= b_num_active_sides)
                    goto def_801A8CE;
            }
          LABEL_241:
            if (return_value == 2) {
                v70 = &battle_data[battle_side_given].status2;
                v71 = *v70;
                v72 = -8;
                v66 = (v71 & v72);
            } else {
                if (return_value <= 2) {
                    if (return_value != 1)
                        goto LABEL_252;
                    v70 = &battle_data[battle_side_given].status1;
                    v66 = 0;
                    goto LABEL_251:
                }
                v66 = return_value;
                if (return_value != 3)
                    goto LABEL_252;
                v70 = &battle_data[battle_side_given].status2;
                v71 = *v70;
                v72 = defense_curl_used|next_attack_will_hit|cursed|nightmares|prevent_escape|destiny_bond|substitute|atk_up_if_hit|needs_recharge|transformed|focus_critical|multi_turn_counter|multi_turn_flag|bide|uproar|flinching|confusion_mask|0x80000080;
                v66 = (v71 & v72);
            }
          LABEL_251:
            *v70 = v66;
          LABEL_252:
            b_movescr_stack_push_cursor();
            b_move_cursor = &unk_81D94D1;
            battle_side_unknown = battle_side_given;
            b_active_side = battle_side_given;
            dp01_build_cmdbuf_x02_a_b_varargs(0, 40, 0, 4);
            b_active_side_mark_buffer_for_execution(b_active_side);
            return return_value;
        case 6u:
            for_each_side {
                if (battle_data[i].ability_id == forecast) {
                    return_value = castform_switch(i);
                    if (return_value) {
                        b_push_move_exec(&castform_script);
                        battle_side_unknown = i;
                        dp08_ptr->pading_7F[0] = return_value - 1;
                        return return_value;
                    }
                }
            }
        case 7u:
            if (ability != synchronize || !(b_features_bitfield & 0x4000))
                goto def_801A8CE;
            b_features_bitfield &= ~0x00004000;
            dp08_ptr->pading_AD[7] &= 0x3Fu;
            if (dp08_ptr->pading_AD[7] == 6)
                dp08_ptr->pading_AD[7] = 2;
            effect_to_apply = dp08_ptr->pading_AD[7] + 64;
            battle_side_unknown = b_defender;
            b_movescr_stack_push_cursor();
            b_move_cursor = &unk_81D94CE;
            b_features_bitfield |= 0x2000u;
            return_value = 1;
            goto def_801A8CE;
        case 8u:
            if (ability != synchronize || !(b_features_bitfield & 0x4000))
                goto def_801A8CE;
            b_features_bitfield &= ~0x00004000;
            dp08_ptr->pading_AD[7] &= 0x3Fu;
            v73 = &dp08_ptr->pading_AD[7];
            if (dp08_ptr->pading_AD[7] == 6)
                *v73 = 2;
            effect_to_apply = dp08_ptr->pading_AD[7];
            battle_side_unknown = b_attacker;
            b_movescr_stack_push_cursor();
            b_move_cursor = &unk_81D94CE;
            b_features_bitfield |= 0x2000u;
            return_value = 1;
            goto def_801A8CE;
        case 9u:
            v74 = 0;
            v75 = b_num_active_sides;
            if (b_num_active_sides <= 0)
                goto def_801A8CE;
            v76 = b_effect_bitfield_pbs;
            v77 = &battle_data[0].ability_id;
            while (*v77 != intimidate || !(*v76 & 0x80000)) {
                v76 += 4;
                v77 += 88;
                ++v74;
                if (v74 >= v75)
                    goto def_801A8CE;
            }
            b_last_copied_ability = intimidate;
            *v76 &= 0xFFF7FFFFu;
            b_push_move_exec(&movescr_081D9307);
            dp08_ptr->pading_C0[24] = v74;
            return_value = 1;
            goto def_801A8CE;
        case 0xBu:
            v78 = 0;
            if (b_num_active_sides <= 0)
                goto def_801A8CE;
            v131 = &battle_data[0].ability_id;
            v132 = 0;
            while (2) {
                if (*v131 != 36)
                    goto LABEL_301;
                v79 = b_effect_bitfield_pbs[v132];
                if (!(v79 & 0x100000))
                    goto LABEL_301;
                v80 = (battle_get_per_side_status(v78) ^ 1) & 1;
                v81 = battle_get_side_with_given_state(v80);
                v82 = battle_get_side_with_given_state(v80 + 2);
                if (battle_type_flags & 1) {
                    v83 = &battle_data[v81];
                    if (battle_data[v81].ability_id) {
                        if (v83->current_hp) {
                            v84 = &battle_data[v82];
                            if (battle_data[v82].ability_id) {
                                if (v84->current_hp) {
                                    v85 = rand();
                                    b_active_side = battle_get_side_with_given_state(v80 | 2 * (v85 & 1));
                                    *v131 = battle_data[b_active_side].ability_id;
                                    b_last_copied_ability = battle_data[b_active_side].ability_id;
                                  LABEL_298:
                                    return_value = (return_value + 1) & 0xFF;
                                    goto LABEL_299;
                                }
                            }
                        }
                        v86 = &battle_data[v81];
                        if (battle_data[v81].ability_id && v86->current_hp) {
                            b_active_side = v81;
                            battle_data[v78].ability_id = battle_data[v81].ability_id;
                            b_last_copied_ability = battle_data[b_active_side].ability_id;
                            goto LABEL_298;
                        }
                    }
                    v87 = &battle_data[v82];
                    if (battle_data[v82].ability_id && v87->current_hp) {
                        b_active_side = v82;
                        battle_data[v78].ability_id = battle_data[v82].ability_id;
                        v88 = battle_data[b_active_side].ability_id;
                      LABEL_297:
                        b_last_copied_ability = v88;
                        goto LABEL_298;
                    }
                }
                else {
                    b_active_side = v81;
                    v89 = &battle_data[v81];
                    if (v89->ability_id && v89->current_hp) {
                        *v131 = v89->ability_id;
                        v88 = v89->ability_id;
                        goto LABEL_297;
                    }
                }
              LABEL_299:
                if (!return_value) {
                  LABEL_301:
                    v131 += 88;
                    ++v132;
                    ++v78;
                    if (v78 >= b_num_active_sides)
                        goto def_801A8CE;
                    continue;
                }
                break;
            }
            b_push_move_exec(&unk_81D92A1);
            b_effect_bitfield_pbs[v132] &= 0xFFEFFFFFu;
            battle_side_unknown = v78;
            battle_outcome = -3;
            byte_2022AB9 = 4;
            byte_2022ABA = b_active_side;
            byte_2022ABB = b_pokemon_team_id_by_side[b_active_side];
            byte_2022ABC = -1;
            byte_2022AC8 = -3;
            byte_2022AC9 = 9;
            byte_2022ACA = b_last_copied_ability;
            byte_2022ACB = -1;
            goto def_801A8CE;

        case 0xAu:
            for_each_side {
                if (battle_data[i].ability_id == intimidate && (b_effect_bitfield_pbs[i] & 0x80000)) {
                    b_last_copied_ability = intimidate;
                    v120 = b_effect_bitfield_pbs[i] & 0xFFF7FFFF;
                    b_effect_bitfield_pbs[i] = v120;
                    b_movescr_stack_push_cursor();
                    b_move_cursor = &movescr_abilities_081D9310;
                    dp08_ptr->pading_C0[24] = i;
                    return_value = 1;
                    break;
                }
            }

        case 0xCu:
            // find ability in opponents
            u8 given_owner = battle_side_get_owner(battle_side_given);
            find_side_cabi (
                battle_side_get_owner(i) != given_owner &&
                battle_data[i].ability_id == ability1);
            break;

        case 0xDu:
            // find ability in own team
            unsigned owner = battle_side_get_owner(battle_side_given);
            for_each_side {
                if (battle_side_get_owner(i) == owner) {
                    if (battle_data[i].ability_id == ability1) {
                        b_last_copied_ability = ability1;
                        return_value = i+1;
                    }
                }
            }

        case 0xEu:
            if (ability == 0xFD) {
                find_side (b_effect_bitfield_pbs[i] & 0x10000);
            }
            else if (ability == 0xFE) {
                find_side (b_effect_bitfield_pbs[i] & 0x20000);
            }
            else {
                for_each_side {
                    if (battle_data[i].ability_id == ability1) {
                        b_last_copied_ability = ability1;
                        return_value = i+1;
                    }
                    ++v100;
                }
            }
            break;

        case 0x13u:
            // find ability in alive pokemon
            for_each_side {
                if (battle_data[i].ability_id == ability1) {
                    if (battle_data[i].current_hp) {
                        b_last_copied_ability = ability1;
                        return_value = i+1;
                    }
                }
            }
            break;

        case 0xFu:
            // find ability in opponents then own team, excluding user
            unsigned given_owner = battle_side_get_owner(battle_side_given);

            for_each_side {
                if (battle_data[i].ability_id == ability1 && battle_side_get_owner(v113) != given_owner) {
                    b_last_copied_ability = ability1;
                    return_value = i+1;
                }
            }

            if (return_value)
                break;

            for_each_side {
                if (battle_data[i].ability_id == ability1 && battle_side_get_owner(v114) == given_owner) {
                    if (i != battle_side_given) {
                        b_last_copied_ability = ability1;
                        return_value = i+1;
                    }
                }
            }
            break;

        case 0x10: {
            // count pokemon of other team with ability X
            u32 owner = battle_side_get_owner(battle_side_given);
            for_each_side {
                if (battle_side_get_owner(i) != owner) {
                    if (battle_data[i].ability_id == ability1) {
                        b_last_copied_ability = ability1;
                        return_value = return_value + 1) & 0xFF;
                    }
                }
            }
            break;
        }

        case 0x11: {
            // count pokemon of own team with ability X
            u32 owner = battle_side_get_owner(battle_side_given);
            for_each_side {
                if (battle_side_get_owner(j) == owner) {
                    if (battle_data[j].ability_id == ability1) {
                        b_last_copied_ability = ability1;
                        return_value++;
                    }
                }
            }
            break;
        }

        case 0x12: {
            // count pokemon except self with ability X
            for_each_side {
                if (battle_data[j].ability_id == ability1) {
                    if (i != battle_side_given) {
                        b_last_copied_ability = ability1;
                        return_value++;
                    }
                }
            }
            break;
        }

        default:
            break;
    }
    if (return_value) {
        if (mode <= 0xB && b_last_copied_ability != 255)
            brb_movehistory__record_ability_usage_of_player(battle_side_given, b_last_copied_ability);
    }
    return return_value;

}





// 0801EAB8
int b_wonderguard_and_levitate() {
    if (move_to_execute_A == mve_struggle)
        return;

    move *move_to_execute = &move_data[move_to_execute_A];
    if (move_to_execute->power == 0)
        return;

    int move_to_execute_type;
    if (dp08_ptr->move_to_execute_type_override)
        move_to_execute_type = dp08_ptr->move_to_execute_type_override & 0x3F;
    else
        move_to_execute_type = move_to_execute->type;

    if (battle_data[b_defender].ability_id == abl_levitate && move_to_execute_type != t_ground) {
        b_last_copied_ability = abl_levitate;
        battle_message_to_show_indexed_a = 4;// avoided the attack
        brb_movehistory__record_ability_usage_of_player(b_defender, abl_levitate);
        return;
    }

    int bits=0;
    int i=0;

    while (true) {
        type_effectiveness *te = type_effectiveness_table[i++];
        type tet1 = te->type1;
        type tet2 = te->type2;
        enum {
            ZERO =  0
            LESS =  5
            MORE = 20
        } effect = te->effect;

        if (tet1 == 0xFF)
            break;

        if (tet1 == 0xFE && (battle_data[b_defender].status2 & next_attack_will_hit))
            break;

        if (tet1 != move_to_execute_type)
            continue;

        if (tet2 != battle_data[b_defender].type1 &&
            tet2 != battle_data[b_defender].type2)
            continue;

        if (effect == ZERO) {
            b_attack_effectivity |= 8u;
            protect_structs[b_attacker].protect_flags |= 2u;
        }

        if (effect == MORE)
            bits |= 1;

        if (effect == LESS)
            bits |= 2;
    }

    if (battle_data[b_defender].ability_id == abl_wonder_guard
     && move_weather_interaction(b_attacker, move_to_execute_A) == 2
     && bits != 1
     && move_data[move_to_execute_A].power
   ) {
        b_last_copied_ability = abl_wonder_guard;
        battle_message_to_show_indexed_a = 3;// avoided damage with
        brb_movehistory__record_ability_usage_of_player(b_defender, abl_wonder_guard);
    }
}
