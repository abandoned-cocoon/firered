#pragma once
#include "types.h"

struct pokedude_rpc_hook {
	u8 match_b_rpcu8;
	u8 match_active_side;
	u16 field2;
	void *apply_bx;
};

struct pokedude8 {
	char subindex;
	char pdscript_index;
	char countup;
	char field_3;
	int field_4;
};

struct pokedude_instr {
	char field_0[4];
	char duration[4];
};
