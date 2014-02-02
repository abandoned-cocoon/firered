struct flash_chip_metrics {
	u32 num_bytes;
	u32 block_size;
	u8 block_shift;
	u8 __unused1;
	u16 num_blocks;
	u32 __unused2;
	u16 waitcnt_or;
	u16 unknown1;
	u16 flash_chip_id;
	u16 unknown2;
};

struct flash_chip_interface {
	void (*unknown_func)();
	void (*write_block)();
	void (*erase_all)();
	void (*erase_block)();
	void (*verify)();
	void *unknown;
	struct flash_chip_metrics metrics;
};

// 081DE820
void flash_bank_switch(u8 bank) {
	*(u8*)0x0E005555 = 0xAA;
	*(u8*)0x0E002AAA = 0x55;
	*(u8*)0x0E005555 = 0xB0;
	*(u8*)0x0E000000 = bank;
}

// 081DE844
void flash_get_manufacturer_and_device_type() {
	// GBATEK#gbacartbackupflashrom
	// > Also, reading anything (data or status/busy information) can be done <only> by opcodes
	// > executed in WRAM (not from opcodes in ROM) (there's no such restriction for writing).
	u16 buffer[0x22];
	u8 (*flash_read_byte)(void*) = (u8(*)(void*))buffer;
	flash_read_byte_to_buffer(buffer);

	*(u8*)0x0E005555 = 0xAA; // enter chip identification mode
	*(u8*)0x0E002AAA = 0x55;
	*(u8*)0x0E005555 = 0x90;
	{ volatile u16 cycles = 20000; while (cycles--); } // wait for chip

	u8 device       = flash_read_byte(0xE000001);
	u8 manufacturer = flash_read_byte(0xE000000);
	u16 chip_id = (device<<8) + manufacturer;

	*(u8*)0x0E005555 = 0xAA; // exit chip identification mode
	*(u8*)0x0E002AAA = 0x55;
	*(u8*)0x0E005555 = 0xF0;
	{ volatile u16 cycles = 20000; while (cycles--); } // wait for chip

	return chip_id;
}

// 086FBF24
struct flash_chip_interface flash_chip_list[] = {
	&flash_chip_macronix_128k,
	&flash_chip_sanyo_128k,
	&flash_chip_fallback
};

// 086FBF70
struct flash_chip_interface flash_chip_macronix_128k = {
	sub_81DEFA4,
	flash_write_block,
	flash_erase_all,
	flash_erase_block,
	flash_verify_write_or_timeout,
	&unk_86FBF58, {
		0x20000,
		0x1000,
		0xC, 0,
		0x20, 0,
		3, 1,
		0x09C2, 0 // chip id
	}
};

// 086FBFA0
struct flash_chip_interface flash_chip_fallback = {
	sub_81DEFA4,
	flash_write_block,
	flash_erase_all,
	flash_erase_block,
	flash_verify_write_or_timeout,
	&unk_86FBF58, {
		0x20000,
		0x1000,
		0xC, 0,
		0x20, 0,
		3, 1,
		0x0000, 0 // chip id
	}
};

// 086FC00C
struct flash_chip_interface flash_chip_sanyo_128k = {
	sub_81DEFA4,
	flash_write_block,
	flash_erase_all,
	flash_erase_block,
	flash_verify_write_or_timeout,
	&unk_86FBFF4, {
		0x20000,
		0x1000,
		0xC, 0,
		0x20, 0,
		3, 1,
		0x1362, 0 // chip id
	}
};
