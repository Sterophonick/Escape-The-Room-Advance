#include <agb_lib.h>
#include "sheet1.h"
#include "start.h"
#include "backgrounds.h"

//clicked = 0x3003470

#define RGB16(r,g,b)  ((r)+(g<<5)+(b<<10)) 

#define POINTER_OFF 0
#define KEY_OFF          360
#define CROWB_OFF     632

int level,arpos;
int key;
int crow;
int cursorx;
int cursory;
int clicked;
u16 ee;
u16 ee2;
int time;
int safe, num1, num2, num3, rnum;
char* buf1[1];
char* buf2[1];
char* buf3[1];
int l1flower, l1code, l1crow, l1floor, l1key, l1flp, l1crowe, l1safee;

void playSound(int s) {
 REG_SOUNDCNT1_H = 0x0B04;                                                       //REG_SOUNDCNT_H = 0000 1011 0000 0100, volume = 100, sound goes to the left, sound goes to the right, timer 0 is used, FIFO buffer reset
 REG_SOUNDCNT1_X = 0x0080;                                                       //REG_SOUNDCNT_X = 0000 0000 1000 0000, enable the sound system, DMA 1
 REG_SD1SAD      = (unsigned long) sound[s].song;                                //REG_DM1SAD = NAME, address of DMA source is the digitized music sample
 REG_SD1DAD      = 0x040000A0;                                                   //REG_DM1DAD = REG_SGFIFOA, address of DMA destination is FIFO buffer for direct sound A
 REG_SD1CNT_H    = 0xB640;                                                       //REG_DM1CNT_H = 1011 0110 0100 0000, DMA destination is fixed, repeat transfer of 4 bytes when FIFO , buffer is empty, enable DMA 1 (number of DMA transfers is ignored), INTERRUPT
 REG_TM0SD       = 65536-(16777216/sound[s].frequency);                          //REG_TM0D = 65536-(16777216/frequency);, play sample every 16777216/frequency CPU cycles
 REG_TMSDCNT     = 0x00C0;  
}

int main()
{
	ee = "This is a Game Boy Advance Port of the Scratch Game called 'Escape the Room' by user SackBoylvr567. If you are interested in playing the original then I suggest checking out his page at http://scratch.mit.edu/users/SackBoylvr567";
	Initialize();
	fillscreen3((void*)0x0000);
	fillpal("bg", (void*)0x0000);
	SetMode(MODE_4|BG2_ENABLE|OBJ_ENABLE|OBJ_MAP_1D);
	memcpy(VRAM, discliamerBitmap, 38400);
	memcpy(BGPaletteMem, discliamerPalette, 512);
    FadeIn(2);
	while(!(time==255 | keyDown(KEY_A)))
	{
		time++;
		SetBGPalPoint(1, (void*)GetBGPalPoint(1)+1);
		SleepF(0.50);
		SetBGPalPoint(1, (void*)GetBGPalPoint(1)+1);
		SleepF(0.50);
		SetBGPalPoint(1, (void*)GetBGPalPoint(1)+1);
		SleepF(0.50);
	}
    FadeOut(2);
 	resetOffset();
	sprites[1].attribute0 = COLOR_256 | WIDE | 240;
	sprites[1].attribute1 = SIZE_64 | 160;
	sprites[1].attribute2 = 512 + 0; // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
	loadSpriteGraphics((void*)start, 1024);
	loadSpritePal((void*)startPalette);
	setbg2((void*)titleBitmap, (void*)titlePalette);
	MoveSprite(&sprites[1], 88, 85);
	CopyOAM();
	FadeIn(2);
	while(!(keyDown(KEY_START)));
	MoveSprite(&sprites[1], 240, 160);
		CopyOAM();
	FadeOut(2);
	setbg2((void*)l1sBitmap,(void*)l1sPalette);
	FadeIn(2);
	Sleep(1024);
	FadeOut(2);
	resetOffset();
	initSprite(0,SIZE_8,0);
	sprites[1].attribute0 = COLOR_256 | TALL | 240;
	sprites[1].attribute1 = SIZE_64 | 160;
	sprites[1].attribute2 = 512 +8; // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
	sprites[2].attribute0 = COLOR_256 | WIDE | 240;
	sprites[2].attribute1 = SIZE_64 | 160;
	sprites[2].attribute2 = 512 + CROWB_OFF; // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
	MoveSprite(&sprites[2], 0, 0);
	loadSpriteGraphics((void*)sheet1, 11136);
	loadSpritePal((void*)sheet1Palette);
	setbg2((void*)l1Bitmap, (void*)l1Palette);
	MoveSprite(&sprites[1], 25,0);
	FadeIn(2);
	level = 1;
	while(1)
	{
		WaitForVblank();
		CopyOAM();
		MoveSprite(&sprites[0], cursorx, cursory);
		if (clicked == 1)
		{
			if (level == 1)
			{
				if ((cursorx > 25)AND(cursory < 64)AND(cursorx < 25 + 32)AND(l1flp==0))
				{
					glideSpritetoPos(1, 25, 0, 5, 0, 5);
					l1flp = 1;
				}
				if ((cursorx > 93)AND(cursorx < 140)AND(cursory < 41)AND(l1safee==0))
				{
					initSprite(25, SIZE_8, 2);
					MoveSprite(&sprites[25], 0, 0);
					MoveSprite(&sprites[0], 240, 160);
					MoveSprite(&sprites[1], 240, 160);
					setbg2m3((void*)safeBitmap);
					SetMode(MODE_3 | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D);
					resetOffset();
					while ((!(keyDown(KEY_B)))AND(l1safee==0))
					{
						CopyOAM();
						Print(35, 67, buf1, 0x025F, BLACK);
						Print(77, 67, buf2, 0x025F, BLACK);
						Print(115, 67, buf3, 0x025F, BLACK);
						sprintf(buf1, "%d", num1);
						sprintf(buf2, "%d", num2);
						sprintf(buf3, "%d", num3);
						Sleep(10);
						if (arpos < 0)
						{
							arpos = 0;
						}
						if (arpos > 3)
						{
							arpos = 3;
						}
						if (keyDown(KEY_UP))
						{
							if (arpos == 0)
							{
								num1++;
							}
							if (arpos == 1)
							{
								num2++;
							}
							if (arpos == 2)
							{
								num3++;
							}
						}
						if (keyDown(KEY_DOWN))
						{
							if (arpos == 0)
							{
								num1--;
							}
							if (arpos == 1)
							{
								num2--;
							}
							if (arpos == 2)
							{
								num3--;
							}
						}
						if (num1 > 9)
						{
							num1 = 0;
						}
						if (num1 < 0)
						{
							num1 = 9;
						}
						if (num2 > 9)
						{
							num2 = 0;
						}
						if (num2 < 0)
						{
							num2 = 9;
						}
						if (num3 > 9)
						{
							num3 = 0;
						}
						if (num3 < 0)
						{
							num3 = 9;
						}
						if (keyDown(KEY_LEFT))
						{
							arpos--;
						}
						if (keyDown(KEY_RIGHT))
						{
							arpos++;
						}
						if (arpos == 0)
						{
							MoveSprite(&sprites[25], 24, 21);
						}
						if (arpos == 1)
						{
							MoveSprite(&sprites[25], 75, 21);
						}
						if (arpos == 2)
						{
							MoveSprite(&sprites[25], 114, 21);
						}
						rnum = (num1 * 100) + (num2 * 10) + num3;
						if (rnum == 339)
						{
							l1safee = 1;
						}
					}
					sprites[2].attribute0 = COLOR_256 | WIDE | 240;
					sprites[2].attribute1 = SIZE_64 | 160;
					sprites[2].attribute2 = 512 + CROWB_OFF; // NOTE: mode4 doesn't support the first tiles, so offset of 512 is requirerd
					SetMode(MODE_4 | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D);
					setbg2((void*)l1Bitmap, (void*)l1Palette);
					MoveSprite(&sprites[25],240, 160);
					if (l1flp == 1)
					{
						MoveSprite(&sprites[1], 5, 0);
					}
					else {
						MoveSprite(&sprites[1], 25, 0);
					}
					if (l1safee == 1)
					{
						MoveSprite(&sprites[2], 0, 0);
						loadSpriteGraphics((void*)sheet1, 11136);
						loadSpritePal((void*)sheet1Palette);
					}
				}
			}
		}
		if((keyDown(KEY_UP))AND(cursory>0))
		{
			cursory-=2;
		}
		if((keyDown(KEY_DOWN))AND(cursory<153))
		{
			cursory+=2;
		}
		if((keyDown(KEY_LEFT))AND(cursorx>0))
		{
			cursorx-=2;
		}
		if((keyDown(KEY_RIGHT))AND(cursorx<233))
		{
			cursorx+=2;
		}
		if (keyDown(KEY_A))
		{
			clicked = 1;
		}
		else {
			clicked = 0;
		}
	}
}