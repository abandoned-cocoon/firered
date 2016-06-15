#include "task.h"

// 08108CFC
void task_WIN0V_open_close_above(task_id c) {
	typedef struct {
		u16 visible_lines;
		u16 direction;
	} TASK_PRIV;

	priv->visible_lines += priv->direction;
	lcd_io_set_and_sync_asap(0x44 /*WIN0V*/, min(priv->visible_lines, 160));

	if ((priv->direction ==  16 && priv->visible_lines == 160)
	 || (priv->direction == -16 && priv->visible_lines == 0))
		task_del(c);
}
