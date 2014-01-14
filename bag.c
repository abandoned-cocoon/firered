// 0809A084
??? bag_add_item(itemit i, u16 qty) {
	// TODO
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
void itemid_get_number(itemid i) {
	i = itemid_sanitize(i);
	return items[i].number;
}

// 0809A900
u16 itemid_get_market_price(itemid i) {
	i = itemid_sanitize(i);
	return items[i].marked_price;
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
bool itemid_can_not_be_held(itemid i) {
	i = itemid_sanitize(i);
	return items[i].can_not_be_held;
}

// 0809A9B4
u8 itemid_get_x19(itemid i) {
	i = itemid_sanitize(i);
	return items[i].field_19;
}

// 0809A9D8
void itemid_get_pocket_number(itemid i) {
	i = itemid_sanitize(i);
	return items[i].get_pocket_number;
}

// 0809A9FC itemid_get_type
// 0809AA20 itemid_get_overworld_function
// 0809AA44 itemid_get_usage
// 0809AA68 itemid_get_inbattle_function
// 0809AA8C itemid_get_x28

// 08108CFC
void task_WIN0V_open_close_above(task_id c) {
	typedef struct {
		u16 visible_lines;
		u16 direction;
	} TASK_PRIV;

	priv->visible_lines += priv->direction;
	lcd_io_set_and_sync_asap(WIN0V, min(priv->visible_lines, 160));

	if ((priv->direction ==  16 && priv->visible_lines == 160)
	 || (priv->direction == -16 && priv->visible_lines == 0))
		task_del(c);
}