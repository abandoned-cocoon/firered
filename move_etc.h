#pragma once
#include "pokemon.h"
#include "types.h"

enum as_mode {
    // actions
    as_pre_turn = 0x0,
    as_post_turn = 0x1,
    as_move_veto_soundproof = 0x2,
    as_move_veto = 0x3,
    as_post_move = 0x4,
    as_pre_ailment = 0x5,
    as_forecast = 0x6,
    as_post_ailment_1 = 0x7,
    as_post_ailment_2 = 0x8,
    as_enter_battle_maybe_1 = 0x9,
    as_enter_battle_maybe_2 = 0xA,
    as_trace = 0xB,
    // queries
    as_find_in_other_team = 0xC,
    as_find_in_own_team = 0xD,
    as_update_bits_for_mud_and_water_sport = 0xE,
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
);
