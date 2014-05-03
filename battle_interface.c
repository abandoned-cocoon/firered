// 08010508
void c2_pre_battle_2_single() {
	task_exec();
	objc_exec();
	obj_sync_something();
	u8 mid = link_get_multiplayer_id();
	dword_2023FE8->fieldB5 = mid;

	switch (battle_preparation_phase) {

	case 0:
		if (dma_08001960() == 0) {
			gpu_sync_bg_show(0);
			gpu_sync_bg_show(1);
			gpu_sync_bg_show(2);
			gpu_sync_bg_show(3);
			sub_80357C8();
			battle_preparation_phase = 1;
		}
		if (link_mode_is_wireless)
			sub_80FCEA8();
		break;

	case 1:
		if (battle_type_flags & BATTLE_LINK) {
			if (byte_3003F64 == 0)
				break;
			if (sub_800A4BC()) {
				// TODO
				battle_preparation_phase = 2;
			}
			if (link_mode_is_wireless)
				sub_80FCD74(0, 0);
		} else {
			battle_type_flags |= BATTLE_WILD;
			battle_preparation_phase = 15;
			berry_0801017C();
		}
		break;

	// TODO

	case 15:
		b_setup_bx();
		battle_preparation_phase++;
		battle_preparation_subphase = 0;
		battle_preparation_subsubphase = 0;
		break;

	case 16:
		battle_preparation_subphase++;
		if (!battle_load_something())
			break;

		b_callback1_backup = super.callback1;
		super.callback1 = &c1_battle_exec_bc_and_bx;
		set_callback2(&c2_battle);

		if (battle_type_flags & BATTLE_LINK)
			battle_type_flags |= 0x20;
		break;

	}
}
