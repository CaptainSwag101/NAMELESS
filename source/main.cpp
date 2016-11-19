#include <stdio.h>
#include <string.h>
#include <iostream>
#include <random>
#include <time.h>

#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>
#include <3ds.h>
#include "button.h"

#include "menu.h"

#include "PressStart_ttf.h" //Including the font
#include "player_down_png.h" //Including all the images
#include "player_left_png.h"
#include "player_right_png.h"
#include "player_up_png.h"
#include "background_png.h"
#define TEXT_COLOR RGBA8(255,255,255,255)
#define BACKGROUND_COLOR RGBA8(0, 0, 0, 255)
#define TEXT_SIZE 40
#define PLAYER_SPEED 430
int main()
{
    romfsInit();
	sf2d_init();
	sf2d_set_clear_color(BACKGROUND_COLOR);
	float x = 0;
	float y = 0;
	float delta = 0.0f;
	srand(time(NULL));
    sftd_init();
	sftd_font *font = sftd_load_font_mem(PressStart_ttf, PressStart_ttf_size);
	sf2d_texture *player_up = sfil_load_PNG_buffer(player_up_png, SF2D_PLACE_RAM);
	sf2d_texture *player_down = sfil_load_PNG_buffer(player_down_png, SF2D_PLACE_RAM);
	sf2d_texture *player_left = sfil_load_PNG_buffer(player_left_png, SF2D_PLACE_RAM);
	sf2d_texture *player_right = sfil_load_PNG_buffer(player_right_png, SF2D_PLACE_RAM);
	sf2d_texture *player = player_down; // STARTS LOOKING DOWN
	button btnquit(0, 240 - TEXT_SIZE+20, font, TEXT_COLOR, TEXT_SIZE-20, "QUIT");
	menu Menu(250, 0, 5, font, 18);
	Menu.charvector.push_back("OPTION1");
	Menu.charvector.push_back("OPTION2");
	Menu.charvector.push_back("OPTION3");
	Menu.charvector.push_back("QUIT");
	Menu.charvector.push_back("BACK");
	//sf2d_texture *background = sfil_load_PNG_buffer(background_png, SF2D_PLACE_RAM);
	while (aptMainLoop()) {
		hidScanInput();
		delta = osGetTime()* 0.000000000000001f;
		if (Menu.enabled == false) {
			if (hidKeysHeld() & KEY_UP) { y = y - (PLAYER_SPEED * delta); player = player_up; }
			if (hidKeysHeld() & KEY_DOWN) { y = y + (PLAYER_SPEED * delta); player = player_down; }
			if (hidKeysHeld() & KEY_LEFT) { x = x - (PLAYER_SPEED * delta); player = player_left; }
			if (hidKeysHeld() & KEY_RIGHT) { x = x + (PLAYER_SPEED * delta); player = player_right; }
		}
		sf2d_start_frame(GFX_TOP, GFX_LEFT);

		if (hidKeysDown() & KEY_START && Menu.enabled == false){ Menu.enable(1);}
		else if (hidKeysDown() & KEY_START && Menu.enabled == true) {Menu.enable(0); }
		else if (hidKeysDown() & KEY_B && Menu.enabled == true) { Menu.enable(0); }

		//sf2d_draw_texture(background, 0, 0);
		sf2d_draw_texture(player, int(x+0.5), int(y+0.5));
		Menu.update();
		switch (Menu.selected) {
		case 1:
			x = 0;
			y = 0;
			break;
		case 2:
			x = 320 / 2;
			y = 240 / 2;
			break;
		case 3:
			x = rand() % 320;
			y = rand() % 240;
			break;
		}

		sf2d_end_frame();

	    sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);

		btnquit.update();
		sftd_draw_text(font,0            , 10, RGBA8(255,  25,  38, 255), TEXT_SIZE, "N");
		sftd_draw_text(font,TEXT_SIZE    , 10, RGBA8(255, 127,   0, 255), TEXT_SIZE, "A");
		sftd_draw_text(font,TEXT_SIZE * 2, 10, RGBA8(242, 255,  25, 255), TEXT_SIZE, "M");
		sftd_draw_text(font,TEXT_SIZE * 3, 10, RGBA8( 89, 255,   0, 255), TEXT_SIZE, "E");
		sftd_draw_text(font,TEXT_SIZE * 4, 10, RGBA8( 77, 217, 255, 255), TEXT_SIZE, "L");
		sftd_draw_text(font,TEXT_SIZE * 5, 10, RGBA8( 59,  61, 255, 255), TEXT_SIZE, "E");
		sftd_draw_text(font,TEXT_SIZE * 6, 10, RGBA8( 72,   0, 255, 255), TEXT_SIZE, "S");
		sftd_draw_text(font,TEXT_SIZE * 7, 10, RGBA8(255,   0, 220, 255), TEXT_SIZE, "S");
		sftd_draw_text(font,320-(TEXT_SIZE-10)*4, TEXT_SIZE+10, RGBA8(255, 255, 255, 255), TEXT_SIZE-10, "DEMO");
		if (btnquit.isPressed() || Menu.selected == 4) break;

		sf2d_end_frame();

		sf2d_swapbuffers();
    }
	Menu.~menu();
    sftd_free_font(font);
    romfsExit();
    sftd_fini();
	sf2d_fini();
	return 0;
}