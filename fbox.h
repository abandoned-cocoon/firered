#pragma once
#include "types.h"

struct dialog;
struct fbox {
	u16 (*function)(struct dialog*);
	char field_4;
	char x;
	char y;
	char field_7;
	char w;
	char h;
	u16 field_A;
};

// extern struct fbox fboxes[]; // 0841F444
extern struct fbox *fboxes_ptr; // 03003D90
