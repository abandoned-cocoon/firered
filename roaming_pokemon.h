#pragma once
#include "types.h"

struct roaming_pokemon_info {
    u32 attribute_42;
    u32 id;
    u16 species;
    u16 hp;
    u8 level;
    u8 status_ailment;
    u8 coolness;
    u8 beauty;
    u8 cuteness;
    u8 smartness;
    u8 toughness;
    u8 roaming_pokemon_active;
    u8 rest[8];
};
