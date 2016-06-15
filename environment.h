#pragma once
#include "types.h"

struct envdata {
	u8 mapname;
	u8 field_1;
	u16 field_2;
	void *envirogfx;
	void *field_8;
	struct palette *palette;
};

extern struct envdata environments[];
