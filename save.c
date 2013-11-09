// 0804C1F0
void save_serialize_pokemon() {
	struct pokemon *dst = sav1->party_pokemon;
	struct pokemon *src = party_pokemon;
	sav1->num_party_pokemon = num_party_pokemon;
	memcpy(dst, src, 6*sizeof(struct pokemon));
}

// 0804C230
void save_deserialize_pokemon() {
	struct pokemon *dst = party_pokemon;
	struct pokemon *src = sav1->party_pokemon;
	sav1->num_party_pokemon = num_party_pokemon;
	memcpy(dst, src, 6*sizeof(struct pokemon));
}

// 0804C270
void save_serialize_npcs() {
	struct npc_state *dst = sav1->npc_states;
	struct npc_state *src = npc_states;
	memcpy(dst, src, MAX_NPXS*sizeof(struct npc_state));
}

// 0804C2B8
void save_deserialize_npcs() {
	struct npc_state *dst = sav1->npc_states;
	struct npc_state *src = npc_states;
	memcpy(dst, src, MAX_NPXS*sizeof(struct npc_state));
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