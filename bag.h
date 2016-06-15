#pragma once
#include "types.h"

// when should I use enums and when definitions

#define POCKET_INVALID 0
#define POCKET_GENERAL 1
#define POCKET2        2
#define POCKET_HM      3
#define POCKET_BERRY   4

#define ITEM_HM_CASE     0x16C
#define ITEM_BERRY_POUCH 0x16D

typedef u16 itemid;

itemid itemid_sanitize(itemid i);
struct item *item_by_id(itemid i);
u16 itemid_get_number(itemid i);
u16 itemid_get_market_price(itemid i);
u8 itemid_get_x12(itemid i);
u8 itemid_get_quality(itemid i);
const char *itemid_get_description(itemid i);
bool itemid_is_unique(itemid i);
u8 itemid_get_x19(itemid i);
u8 itemid_get_pocket_number(itemid i);
