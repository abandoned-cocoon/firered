#pragma once
#include "types.h"

void  mem_init();
void *mem_alloc(u32);
void *mem_alloc_cleared(u32);
void  mem_free(void*);
