// 080391EC
int ai_080391EC() {
	if (b_type_flags & 1)
		return 0;

	u8 plside = battle_get_side_with_given_state(0);

	struct battle_data *plpoke = &b_data[plside];

	if (plpoke->ability_id == ab_wonder_guard)
		return 0;

	// when the players pokemom has a certain move, stop right here

	for (u32 i=0; i<4; i++) {
		u16 move = plpoke->move_ids[i];
		if (move && (ai_rate_move(move, plpoke->species, plpoke->ability_id) & 2))
			return 0;
	}

	// opponent considers all his pokemon
	// except for the one, he currently has on the field

	for (u32 i=0; i<6; i++) {
		struct pokemon *oppoke = pokemon_opponent[i];
		if (pokemon_getattr(oppoke, req_current_hp) == 0) // dead pokemon can't battle
			continue;
		if (pokemon_getattr(oppoke, req_species2) == 0) // species 0 can't battle
			continue;
		if (pokemon_getattr(oppoke, req_species2) == 412) // eggs can't battle
			continue;
		if (b_pokemon_team_id_by_side[b_active_side] == i)
			continue;

		pokemon_getattr(oppoke, req_species); // discard result
		pokemon_getattr(oppoke, req_ability_bit); // discard result
		u8 plside2 = battle_get_side_with_given_state(0),

		for (u32 j=0; j<4; j++) {
			u16 move = pokemon_getattr(oppoke, req_move1 + j);
			if (move && (ai_rate_move(move, oppoke->species, oppoke->ability_id) & 2) && rand()%3 == 0) {
				u8 status = battle_get_per_side_status(b_active_side);
				dword_02023FE8.field_92[status] = j;
				dp01_build_cmdbuf_x21_a_bb(TARGET_BUFFER_B, 2, 0);
				return 1;
			}
		}
	}
	return 0;
}