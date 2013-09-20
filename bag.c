// 08108CFC
void coro_WIN0V_open_close_above(coro_id c) {
	typedef struct {
		u16 visible_lines;
		u16 direction;
	} CORO_PRIV;

	priv->visible_lines += priv->direction;
	lcd_io_set_and_sync_asap(WIN0V, min(priv->visible_lines, 160));

	if ((priv->direction ==  16 && priv->visible_lines == 160)
	 || (priv->direction == -16 && priv->visible_lines == 160))
		coro_del(c);
}