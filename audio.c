enum audio {
	sound_RS_berry_picking_maybe = 0x1,
	sound_PC_go_back = 0x3,
	sound_PC_open = 0x4,
	sound_generic_clink = 0x5,
	sound_wallrun = 0x7,
	sound_door_open_maybe = 0x8,
	sound_bike_clink = 0xB,
	sound_not_very_effective = 0xC,
	sound_normal_damage = 0xD,
	sound_super_effective = 0xE,
	sound_pokemon_enter_leave = 0xF,
	sound_got_away_safely = 0x11,
	sound_telephone_maybe = 0x12,
	sound_fly_maybe = 0x14,
	sound_exclamation_mark = 0x15,
	sound_RS_contest_not_you = 0x16,
	sound_RS_contest_1 = 0x18,
	sound_RS_contest_2 = 0x19,
	sound_RS_contest_3 = 0x1A,
	sound_exp_gain = 0x1B,
	sound_RSE_berry_mix_click = 0x22,
	sound_fall_through_ice = 0x23,
	sound_damage_ice_floor = 0x24,
	sound_RSE_roulette_ball_fall = 0x25,
	sound_RSE_roulette_ball_land = 0x26,
	sound_some_beep = 0x28,
	sound_game_saved = 0x30,
	sound_countinous_raining_1 = 0x4A,
	sound_rain_end = 0x4B,
	sound_countinous_raining_2 = 0x4C,
	sound_level_up = 0x58,
	sound_sunlight = 0xC3,
	soubd_boulder_bump = 0xCF,
	sound_yawn_maybe = 0xDC,
	sound_run_the_terminator = 0xE3,
	sound_some_air_move_1 = 0xE4,
	sound_sweet_scent = 0xE5,
	sound_yawn_probably = 0xE6,
	sound_some_air_move_2 = 0xE7,
	sound_stat_increase = 0xE8,
	sound_some_fire_move = 0xE9,
	sound_heavenly = 0xEC,
	sound_woah = 0xED,
	sound_stat_decrease = 0xEE,
	sound_what = 0xEF,
	sound_door_open_maybe_2 = 0xF1,
	sound_hmm = 0xF2,
	sound_trainer_card = 0xF4,
	sound_open_start_menu_maybe = 0xF5,
	sound_scratching = 0xF7,
	sound_the_store_is_open = 0xF8,
	sound_someone_order_a_ship = 0xF9,
	sound_opening_help_menu = 0xFA,
	sound_closing_help_menu = 0xFB,
	sound_cant_open_help_menu = 0xFC,
	sound_triangle_quiz = 0xFD,
	sound_healing = 0xFE,
	song_healing_RS = 0x105,
	song_level_up = 0x106,
	song_evolution_start = 0x107,
	song_evolution_safari = 0x108,
	song_battle_1 = 0x109,
	song_battle_2 = 0x10A,
	song_unknown_10B = 0x10B,
	song_fanfare_1 = 0x10C,
	song_fanfare_2 = 0x10D,
	song_you_fail_it = 0x10E,
	song_you_fail_it_again = 0x10F,
	song_follow_me = 0x110,
	song_game_corner = 0x111,
	song_evil_lurks = 0x112,
	song_gym = 0x113,
	song_jigglypuffs_song = 0x114,
	song_introduction = 0x115,
	song_pokemon_theme = 0x116,
	song_cinnabar_island = 0x117,
	song_lavender_town = 0x118,
	song_healing = 0x119,
	song_bicycle = 0x11A,
	song_encounter_1 = 0x11B,
	song_encounter_2 = 0x11C,
	song_encounter_3 = 0x11D,
	song_youre_in_the_hall_of_fame = 0x11E,
	song_viridian_forest = 0x11F,
	song_mount_moon = 0x120,
	song_abandoned_place = 0x121,
	song_end_credits = 0x122,
	song_route_theme_1 = 0x123,
	song_route_theme_2 = 0x124,
	song_route_theme_3 = 0x125,
	song_route_theme_4 = 0x126,
	song_indigo_plateau = 0x127,
	song_battle_3 = 0x128,
	song_battle_4 = 0x129,
	song_battle_5 = 0x12A,
	song_battle_6 = 0x12B,
	song_pallet_town = 0x12C,
	song_oaks_lab = 0x12D,
	song_oaks_theme = 0x12E,
	song_pokemon_center = 0x12F,
	song_ss_anne = 0x130,
	song_surfs_up = 0x131,
	song_pokemon_tower = 0x132,
	song_silph_co = 0x133,
	song_cerulean_city = 0x134,
	song_celadon_city = 0x135,
	song_victory_1 = 0x136,
	song_victory_2 = 0x137,
	song_victory_3 = 0x138,
	song_vermillon_city = 0x139,
	song_viridian_city_and_pewter_city = 0x13A,
	song_garys_theme = 0x13B,
	song_garys_theme_bis = 0x13C,
	song_fanfare_3 = 0x13D,
	song_fanfare_4 = 0x13E,
	song_you_caught_a_pokemon = 0x13F,
	song_trainer_card_photo = 0x140,
	song_game_freak = 0x141,
	song_victory_2_bis = 0x142,
	song_intro_message_1 = 0x143,
	song_intro_message_2 = 0x144,
	song_intro_message_3 = 0x145,
	song_game_corner_1 = 0x146,
	song_game_corner_2 = 0x147,
	song_net_corner = 0x148,
	song_mystery_connection = 0x149,
	song_game_corner_3 = 0x14A,
	song_mount_ember = 0x14B,
	song_follow_me_alt = 0x14C,
	song_water_labyrinth = 0x14D,
	song_tanoby_ruins = 0x14E,
	song_islands_1_2_3 = 0x14F,
	song_islands_4_5 = 0x150,
	song_islands_6_7 = 0x151,
	song_poke_flute = 0x152,
	song_battle_deoxys = 0x153,
	song_battle_5_1 = 0x154,
	song_battle_5_2 = 0x155,
	song_encounter_4 = 0x156,
	song_deoxys_encounter = 0x157,
	song_trainer_tower = 0x158,
	song_pallet_town_remix = 0x159,
	song_teachy_tv = 0x15A,
};
