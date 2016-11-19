#include <stdio.h>
#include <string.h>
#include <iostream>
#include <random>
#include <time.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>


#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>
#include <3ds.h>

#include "button.h"
#include "menu.h"

#define TEXT_COLOR RGBA8(255,255,255,255)
#define BACKGROUND_COLOR RGBA8(0, 0, 0, 255)

//#define 3DS_TOPSCREEN_WIDTH 400
//#define 3DS_BOTTOMSCREEN_WIDTH 240
//#define 3DS_TOPSCREEN_HEIGHT 320
//#define 3DS_BOTTOMSCREEN_HEIGHT 240

#define TEXT_SIZE 40
#define PLAYER_SPEED 50 //Around 50pxls/second
bool run = true;
ndspWaveBuf waveBuf;
u8* data = NULL;
using namespace std;
void mainMenu(sftd_font *font);
int playWav(string path, int channel = 1, bool toloop = true);
int fadeout(int color);
int main()
{
	sf2d_init();
	ndspInit();
	sf2d_set_clear_color(BACKGROUND_COLOR);
	float x = 0;
	float y = 0;
	float delta = 0.0f;
	srand(time(NULL));
    sftd_init();
	sftd_font *font = sftd_load_font_file("sdmc:/NAMELESS/font.ttf");
	mainMenu(font);
	if(!run){sftd_free_font(font); sftd_fini(); sf2d_fini(); ndspExit(); return 0;}
	sf2d_texture *player_up = sfil_load_PNG_file("sdmc:/NAMELESS/player/player_up.png", SF2D_PLACE_RAM);
	sf2d_texture *player_down = sfil_load_PNG_file("sdmc:/NAMELESS/player/player_down.png", SF2D_PLACE_RAM);
	sf2d_texture *player_left = sfil_load_PNG_file("sdmc:/NAMELESS/player/player_left.png", SF2D_PLACE_RAM);
	sf2d_texture *player_right = sfil_load_PNG_file("sdmc:/NAMELESS/player/player_right.png", SF2D_PLACE_RAM);
	sf2d_texture *player = player_down; // STARTS LOOKING DOWN
	button btnquit(0, 240 - TEXT_SIZE+20, font, TEXT_COLOR, TEXT_SIZE-20, "QUIT");
	menu Menu(250, 0, 5, font, 18);
	Menu.charvector.push_back("DEFAULT");
	Menu.charvector.push_back("RANDOM");
	Menu.charvector.push_back("SOUND");
	Menu.charvector.push_back("QUIT");
	Menu.charvector.push_back("BACK");
	int lookpos = 0;
	ndspChnWaveBufClear(1);
	playWav("sdmc:/NAMELESS/bgm.wav");
	//sf2d_texture *background = sfil_load_PNG_buffer(background_png, SF2D_PLACE_RAM);
	while (run && aptMainLoop()) {
		hidScanInput();
		delta = osGetTime()* 0.00000000000001f;
		if (Menu.enabled == false) {
			if (hidKeysHeld() & KEY_UP) { y = y - (PLAYER_SPEED * delta); if(lookpos != 0){player = player_up; lookpos = 0;}}
			if (hidKeysHeld() & KEY_DOWN) { y = y + (PLAYER_SPEED * delta); if(lookpos != 1){player = player_down; lookpos = 1;} }
			if (hidKeysHeld() & KEY_LEFT) { x = x - (PLAYER_SPEED * delta); if(lookpos != 2){player = player_left; lookpos = 2;} }
			if (hidKeysHeld() & KEY_RIGHT) { x = x + (PLAYER_SPEED * delta); if(lookpos != 3){player = player_right; lookpos = 3;} }
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
			x = rand() % 320;
			y = rand() % 240;
			break;
		case 3:
			ndspChnWaveBufClear(2);
		    playWav("sdmc:/NAMELESS/sfx.wav",2,false);
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
		if (btnquit.isPressed() || Menu.selected == 4) run = false;

		sf2d_end_frame();

		sf2d_swapbuffers();
    }
    sftd_free_font(font);
    sftd_fini();
	sf2d_fini();
	ndspExit();
	return 0;
}
int playWav(string path, int channel, bool toloop) {
    u32 sampleRate;
    u32 dataSize;
    u16 channels;
    u16 bitsPerSample;
  
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    ndspSetOutputCount(2); // Num of buffers
  
    // Reading wav file
    FILE* fp = fopen(path.c_str(), "rb");
  
    if(!fp)
    {
        printf("Could not open the example.wav file.\n");
        return -1;
    }
  
    char signature[4];
  
    fread(signature, 1, 4, fp);
  
    if( signature[0] != 'R' &&
        signature[1] != 'I' &&
        signature[2] != 'F' &&
        signature[3] != 'F')
    {
        printf("Wrong file format.\n");
        fclose(fp);
        return -1;
    }
  
    fseek(fp,0,SEEK_END);
	dataSize = ftell(fp);
    fseek(fp, 22, SEEK_SET);
    fread(&channels, 2, 1, fp);
    fseek(fp, 24, SEEK_SET);
    fread(&sampleRate, 4, 1, fp);
    fseek(fp, 34, SEEK_SET);
    fread(&bitsPerSample, 2, 1, fp);
  
    if(dataSize == 0 || (channels != 1 && channels != 2) ||
        (bitsPerSample != 8 && bitsPerSample != 16))
    {
        printf("Corrupted wav file.\n");
        fclose(fp);
        return -1;
    }
 
    // Allocating and reading samples
    data = static_cast<u8*>(linearAlloc(dataSize));
    fseek(fp, 44, SEEK_SET);
    fread(data, 1, dataSize, fp);
    fclose(fp);
  
    fseek(fp, 44, SEEK_SET);
    fread(data, 1, dataSize, fp);
    fclose(fp);
    dataSize/=2;
    // Find the right format
    u16 ndspFormat;
  
    if(bitsPerSample == 8)
    {
        ndspFormat = (channels == 1) ?
            NDSP_FORMAT_MONO_PCM8 :
            NDSP_FORMAT_STEREO_PCM8;
    }
    else
    {
        ndspFormat = (channels == 1) ?
            NDSP_FORMAT_MONO_PCM16 :
            NDSP_FORMAT_STEREO_PCM16;
    }
  
    ndspChnReset(channel);
    ndspChnSetInterp(channel, NDSP_INTERP_NONE);
    ndspChnSetRate(channel, float(sampleRate));
    ndspChnSetFormat(channel, ndspFormat);
  
    // Create and play a wav buffer
    std::memset(&waveBuf, 0, sizeof(waveBuf));
  
    waveBuf.data_vaddr = reinterpret_cast<u32*>(data);
    waveBuf.nsamples = dataSize / (bitsPerSample >> 3);
    waveBuf.looping = toloop;
    waveBuf.status = NDSP_WBUF_FREE;
  
    DSP_FlushDataCache(data, dataSize);
  
    ndspChnWaveBufAdd(channel, &waveBuf);
  
    return ((dataSize / (bitsPerSample >> 3)) / sampleRate); // Return duration in seconds, for debugging purposes
  
}

void mainMenu(sftd_font *font){
	bool menurun =true;
	int txtsiz = 50;
	int fadout = 0;
	bool fadoutact = false;
	button bplay(105, 50, font, TEXT_COLOR, 30, "PLAY");
	button bquit(105, 150, font, TEXT_COLOR, 30, "QUIT");
	ndspChnWaveBufClear(1);
	playWav("sdmc:/NAMELESS/menu.wav");
	while(menurun && aptMainLoop()){
	hidScanInput();
	sf2d_start_frame(GFX_TOP, GFX_LEFT);

		sftd_draw_text(font,0+2            , txtsiz, RGBA8(255,  25,  38, 255), txtsiz, "N");
		sftd_draw_text(font,txtsiz    +2, txtsiz, RGBA8(255, 127,   0, 255), txtsiz, "A");
		sftd_draw_text(font,txtsiz * 2+2, txtsiz, RGBA8(242, 255,  25, 255), txtsiz, "M");
		sftd_draw_text(font,txtsiz * 3+2, txtsiz, RGBA8( 89, 255,   0, 255), txtsiz, "E");
		sftd_draw_text(font,txtsiz * 4+2, txtsiz, RGBA8( 77, 217, 255, 255), txtsiz, "L");
		sftd_draw_text(font,txtsiz * 5+2, txtsiz, RGBA8( 59,  61, 255, 255), txtsiz, "E");
		sftd_draw_text(font,txtsiz * 6+2, txtsiz, RGBA8( 72,   0, 255, 255), txtsiz, "S");
		sftd_draw_text(font,txtsiz * 7+2, txtsiz, RGBA8(255,   0, 220, 255), txtsiz, "S");
		sftd_draw_text(font,130, txtsiz*2+30, RGBA8(255, 255, 255, 255), txtsiz-10, "DEMO");
	if(fadoutact){
		fadout = fadeout(fadout);
		sf2d_draw_rectangle(0, 0, 400, 400, RGBA8(0,0,0,fadout));
	}

	sf2d_end_frame();
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		bplay.update();
		bquit.update();
	if(fadoutact){
		sf2d_draw_rectangle(0, 0, 400, 400, RGBA8(0,0,0,fadout));
	}
	sf2d_end_frame();
	sf2d_swapbuffers();
	if (bplay.isPressed()) fadoutact = true;
	if (bquit.isPressed()){run = false; fadoutact = true;}

	if(fadout>=255){
	menurun = false;	
	}
	
	}
}
int fadeout(int color){
	color+=4;
	if(color > 255) color = 255;
	return color;
}