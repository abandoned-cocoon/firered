#include "bag.h"
#include "flags.h"

struct bag_item {
	u16 itemid;
	u16 qty;
};

struct bag_pocket {
	struct bag_item *items;
	u32 num_items;
};

struct item {
	u8 name[14];
	u16 index;
	u16 market_price;
	u8 field_12;
	u8 quality;
	u8 *description;
	u8 is_unique;
	u8 field_19;
	u8 pocket_number;
	u8 type;
	void *overworld_routine;
	u8 usage;
	u8 field_21;
	u16 field_22;
	void *battle_routine;
	u32 field_28;
};

#define NUM_ITEMS 375

// 0203988C
extern struct bag_pocket bag_pockets[5];
// 083DB028
extern struct item items[NUM_ITEMS];

// 0809A084
bool bag_add_item(itemid it, u16 qty) {
	if (!(itemid_get_pocket_number(it) << 24))
		return 0;

	u8 pocket = itemid_get_pocket_number(it) - 1;
	struct bag_item *entry = bag_pockets[pocket].items;

	for (uint i = 0; i < bag_pockets[pocket].num_items; i++, entry++) {
		if (entry->itemid == it) {
			// increase qty of existing entry
			u16 new_qty = sav2_read16_xor(&entry->qty) + qty;
			if (new_qty > 999)
				return 0;
			sav2_write16_xor(&entry->qty, new_qty);
			return 1;
		}
	}
	// if necessary give the player a HM case
	if (pocket == POCKET_HM && check_item_2(ITEM_HM_CASE, 1u)) {
		int slot = bag_find_space(1);
		if (slot == -1)
			return 0;
		bag_pockets[1].items[slot].itemid = 364;
		sav2_write16_xor(&bag_pockets[1].items[slot].qty, 1);
	}
	// if necessary give the player a berry pouch
	if (pocket == POCKET_BERRY && check_item_2(ITEM_BERRY_POUCH, 1u)) {
		int slot = bag_find_space(1);
		if (slot == -1)
			return 0;
		bag_pockets[1].items[slot].itemid = ITEM_BERRY_POUCH;
		sav2_write16_xor(&bag_pockets[1].items[slot].qty, 1);
		flag_set(FLAG_HAS_BERRY_POUCH);
	}
	if (it == ITEM_BERRY_POUCH)
		flag_set(FLAG_HAS_BERRY_POUCH);

	int slot = bag_find_space(pocket);
	if (slot == -1)
		return 0;

	struct bag_pocket *p = &bag_pockets[pocket];
	entry = &p->items[slot];
	entry->itemid = it;
	sav2_write16_xor(&entry->qty, qty);
	return 1;
}

// 0809A8A4
itemid itemid_sanitize(itemid i) {
	if (i >= NUM_ITEMS) return 0;
	return i;
}

// 0809A8BC
struct item *item_by_id(itemid i) {
	i = itemid_sanitize(i);
	return &items[i];
}

// 0809A8DC
u16 itemid_get_index(itemid i) {
	i = itemid_sanitize(i);
	return items[i].index;
}

// 0809A900
u16 itemid_get_market_price(itemid i) {
	i = itemid_sanitize(i);
	return items[i].market_price;
}

// 0809A924
u8 itemid_get_x12(itemid i) {
	i = itemid_sanitize(i);
	return items[i].field_12;
}

// 0809A948
u8 itemid_get_quality(itemid i) {
	i = itemid_sanitize(i);
	return items[i].quality;
}

// 0809A96C
const char *itemid_get_description(itemid i) {
	i = itemid_sanitize(i);
	return items[i].description;
}

// 0809A990
bool itemid_is_unique(itemid i) {
	i = itemid_sanitize(i);
	return items[i].is_unique;
}

// 0809A9B4
u8 itemid_get_x19(itemid i) {
	i = itemid_sanitize(i);
	return items[i].field_19;
}

// 0809A9D8
u8 itemid_get_pocket_number(itemid i) {
	i = itemid_sanitize(i);
	return items[i].pocket_number;
}

// 0809A9FC itemid_get_type
// 0809AA20 itemid_get_overworld_function
// 0809AA44 itemid_get_usage
// 0809AA68 itemid_get_inbattle_function
// 0809AA8C itemid_get_x28
