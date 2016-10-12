#pragma once
#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>
#include <3ds.h>

class button
{
private:


public:
	button(int posx, int posy, sftd_font *txtfont, unsigned int txtcolor, unsigned int txtsize, char * txt);
	~button();
	int x; int y; sftd_font *font; unsigned int color; unsigned int size; char * text;
	void update();
	bool isPressed();
	touchPosition touch;
	u16 touch_x = 320 / 2;
	u16 touch_y = 240 / 2;
	u32 held;
};

