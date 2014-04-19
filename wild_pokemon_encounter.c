// 08082CBC
bool is_it_battle_time_2(blockinfo bi, u16 role);

// 080830B8
bool repel_per_step() {
    if (in_trade_center())
        return 0;
    if (byte_203ADFA == 2)
        return 0;

    u16 remaining_steps = var_load(VAR_REPEL_STEPS);
    if (remaining_steps == 0)
        return 0;

    var_set(VAR_REPEL_STEPS, remaining_steps-1)
    if (remainings_steps > 0)
        return 0;

    script_env_12_start_and_stuff(scr_repel_wore_off);
    return 1;
}

// 0808310C
bool sub_808310C(u8 limit) {
    if (var_load(VAR_REPEL_STEPS) == 0)
        return true;

    for (u32 i=0; i<6; i++) {
        struct pokemon *pk = &party_player[i];
        if (pokemon_getattr(pk, req_current_hp) == 0)
            continue;
        if (pokemon_getattr(pk, req_is_egg) == true)
            continue;

        return pokemon_getattr(pk, req_level) <= limit;
    }
    return false;
}

// 080832E4
bool is_it_battle_time_1(blockinfo bi);

// 080833B0
bool is_it_battle_time_3(blockinfo bi) {
    u16 role = block_get_field(bi, BLOCK_FIELD_ROLE);

    if (is_it_battle_time_1(bi)                        == false ||
        is_it_battle_time_2(bi, struc_20386D0.field_4) == false)
    {
        struc_20386D0.field_4 = role;
        return false;
    } else {
        struc_20386D0.field_4 = role;
        struc_20386D0.field_6 = 0;
        struc_20386D0.field_8 = 0;
        return true;
    }
}