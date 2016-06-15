// 0804C1F0
void save_serialize_pokemon() {
	struct pokemon_extended *dst = sav1i->party_pokemon;
	struct pokemon_extended *src = party_player;
	sav1i->num_party_pokemon = party_player_count;
	memcpy(dst, src, 6*sizeof(struct pokemon_extended));
}

// 0804C230
void save_deserialize_pokemon() {
	struct pokemon_extended *dst = party_player;
	struct pokemon_extended *src = sav1i->party_pokemon;
	sav1i->num_party_pokemon = party_player_count;
	memcpy(dst, src, 6*sizeof(struct pokemon_extended));
}

// 0804C270
void save_serialize_npcs() {
	struct npc_state *dst = sav1i->npc_states;
	struct npc_state *src = npc_states;
	memcpy(dst, src, MAX_NPCS*sizeof(struct npc_state));
}

// 0804C2B8
void save_deserialize_npcs() {
	struct npc_state *dst = sav1i->npc_states;
	struct npc_state *src = npc_states;
	memcpy(dst, src, MAX_NPCS*sizeof(struct npc_state));
}

// 0804C300
void save_serialize_game() {
	save_serialize_pokemon();
	save_serialize_npcs();
}

// 0804C310
void save_deserialize_game() {
	save_deserialize_pokemon();
	save_deserialize_npcs();
}

// 080DA248
void save_game(u8 mode) {
	// mode:
	//   0 normal save
	//   4 save newly started game
	void *vblank_backup = super.vblank_callback;
	super.vblank_callback = NULL;
	sub_080DA1D4();
	switch (mode) {
		case 1:
			save_serialize_game();
			for (u8 i=0; i<4; i++)
				save_write_to_flash(i, &dword_030053B0);
			break;
		case 2:
			save_serialize_game();
			save_write_to_flash(0, &dword_030053B0);
			break;
		case 4:
			for (u8 i=0x1C; i<0x20; i++)
				(*flash_c_erase_block)(i);
			save_serialize_game();
			save_write_to_flash(0xFFFF, &dword_030053B0);
			break;
		case 5:
			for (u8 i=0x1C; i<0x20; i++)
				(*flash_c_erase_block)(i);
		case 3:
			if (sub_08054EC4(0xA) <= 0x3E6)
				sub_08054E90(0xA);
		default:
			save_serialize_game();
			save_write_to_flash(0xFFFF, &dword_030053B0);
			break;
	}
	super.vblank_callback = vblank_backup;
}
