#include "u08044.h"

// 08044898
void dex_flag_08044898(unsigned __int16 a1, unsigned __int8 a2, int pid) {
	if ( !(dex_flag_check_pokedex_index(a1, (-(a2 ^ 2) | a2 ^ 2u) >> 31) << 24) )
	{
		dex_flag_check_pokedex_index(a1, a2);
		u16 species = pokemon_species_by_national_dex_number(a1);
		if (species == poke_unown)  saveblock2_trainerdata->first_seen_unown_pid = pid;
		if (species == poke_spinda) saveblock2_trainerdata->first_seen_spinda_pid = pid;
	}
}
