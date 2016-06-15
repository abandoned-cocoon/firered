struct wild_pokemon_entry {
	u8 data0;
	u8 data1;
	u16 species;
};

struct wild_pokemon_distribution_domain {
	u8 num_entries;
	u8 padding[3];
	struct wild_pokemon_entry *entries;
};

struct wild_pokemon_distribution {
	u8 mapbank;
	u8 mapnr;
	u16 field_2;
	struct wild_pokemon_distribution_domain *grass;
	struct wild_pokemon_distribution_domain *water;
	struct wild_pokemon_distribution_domain *tree;
	struct wild_pokemon_distribution_domain *fishingrod;
};
