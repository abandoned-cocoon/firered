struct bg_config {
	uint active : 1;
	uint unknown0 : 1;
	uint unknown1 : 2;
	uint unknown2 : 2;
	uint unknown3 : 1;
	uint unknown4 : 1;
	uint charbase : 2;
	uint screenbase : 5;
	uint unknown5 : 1;
	u16 padding;
};

// 030008D0
struct bg_config gpu_bg_config[4];

// 080011E4
u8 gpu_bg_config_get_field(u8 bg_id, u8 field) {
	struct bg_config *bg = gpu_bg_config + bg_id;
	if (bg_id >= 4 || bg->active == 0) return 0xFF;
	switch (field) {
		case 1:  return bg->active;
		case 2:  return bg->charbase;
		case 3:  return bg->screenbase;
		case 4:  return bg->unknown1;
		case 5:  return bg->unknown5;
		case 6:  return bg->unknown2;
		case 7:  return bg->unknown3;
		case 8:  return bg->unknown4;
		default: return 0xFF;
	}
}