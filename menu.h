enum menu_page {
	mp_main,
	mp_romsel,
	mp_loaderr,
	mp_controller,
	mp_loadstate,
	mp_savestate,
	mp_vidmode,
	mp_dmgpal,
};

void menu_init(void);
void menu_initpage(enum menu_page);
void menu_enter(void);
