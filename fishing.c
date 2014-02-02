// 0805D2C0
void fishing_start(u8 rod_quality) {
	u8 taskid = task_add(&taskFF_fishing, 0xFF);
	tasks[taskid].priv[15] = rod_quality;
	taskFF_fishing(taskid);
	if (sub_08150474(2) == 1)
		sub_08112C9C();
}

// 0805D304
void taskFF_fishing(u8 taskid) {
	struct task_t *t = &tasks[taskid];
	while (fishing_states[t->priv[15]](t));
}

// 0805D33C
bool fishing0(struct task_t *t) {
	script_env_2_enable();
	walkrun_state.lock = true;
	t->priv[0]++;
	return false;
}

// 0805D35C
bool fishing1(struct task_t *t) {
	// TODO
}

// 0805D3F4
bool fishing2(struct task_t *t) {
	sub_0805D9C4(objects[walkrun_state.objid]);
	if ((++t->priv[1]) >= 60) // wait one second
		t->priv[0]++; // go to state 3
	return false;
}

// 0805D430
bool fishing3(struct task_t *t) {
	box_border_curve_load_tiles_and_pal();
	u16 *priv = t->priv;
	priv[0]++;
	priv[1] = 0;
	priv[2] = 0;
	priv[3] = (rand() % 10) + 1;
	if (priv[12] == 0)
		priv[3] += 3;
	if (priv[3] > 10)
		priv[3] = 10;
	return true;
}

// 0805D47C
bool fishing4(struct task_t *t) {
	// TODO
}

// 0805D508
bool fishing5(struct task_t *t) {
	// TODO
}

// 0805D574
bool fishing6(struct task_t *t) {
	t->priv[0] += 3; // go to state 9
	return false;
}

// 0805D580
bool fishing7(struct task_t *t) {
	// TODO
}

// 0805D5EC
bool fishing8(struct task_t *t) {
	// TODO
}

// 0805D66C
bool fishing9(struct task_t *t) {
	// TODO
}

// 0805D6C8
bool fishingA(struct task_t *t) {
	// TODO
}

// 0805D7C0
bool fishingB(struct task_t *t) {
	// TODO
}

// 0805D838
bool fishingC(struct task_t *t) {
	// TODO
}

// 0805D8AC
bool fishingD(struct task_t *t) {
	// TODO
}

// 0805D8D8
bool fishingE(struct task_t *t) {
	// TODO
}

// 0805D980
bool fishingF(struct task_t *t) {
	textbox_something();

	if (a_pressed_maybe(0))
		return false;

	walkrun_state.lock = false;
	script_env_2_disable();
	sub_08068A5C();
	sub_080F6F54(0, 1);
	u8 taskid = task_find_by_funcptr(&taskFF_fishing);
	task_del(taskid);
	return false;
}