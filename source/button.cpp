#include "button.h"


button::button(int posx, int posy, sftd_font *txtfont, unsigned int txtcolor, unsigned int txtsize, char * txt)
{
	x = posx;
	y = posy;
	font = txtfont;
	color = txtcolor;
	size = txtsize;
	text = txt;
}


button::~button()
{
	x = 0;
	y = 0;
	sftd_free_font(font);
	color = 0;
	size = 0;
	text = nullptr;
}

void button::update() // WARNING /!\ This has to be called in sf2d_start_frame(GFX_BOTTOM, GFX_LEFT); !!!
{
	sftd_draw_text(font, x, y, color, size, text);
}

bool button::isPressed()
{
	if (hidKeysHeld() & KEY_TOUCH) {
		hidTouchRead(&touch);
		touch_x = touch.px;
		touch_y = touch.py;
		if (x < touch_x &&
			x + sizeof(text)*size > touch_x &&
			y < touch_y &&
			size + y > touch_y) {
			return true;
		}
		else return false;
	}

	else return false;
}
