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

struct flash_chip_interface flash_chip_sanyo_128k = {
	sub_81DEFA4,
	flash_write_block,
	flash_erase_all,
	flash_erase_block,
	flash_verify_write_or_timeout,
	&unk_86FBFF4, {
		0x20000,
		0x1000,
		0xC,
		 0,
		0x20,
		 0,
		3, 1,
		0x1362,
		0
	}
};