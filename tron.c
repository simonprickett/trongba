/*
 * Tron Lightcycle like game for Gameboy Advance.
 *
 * Author: Simon Prickett
 *
 * Borrows from tutorial material at:
 *      http://www.loirak.com/gameboy/gbatutor.php
 *
 */

typedef unsigned short u16;
typedef unsigned long u32;

// Definitions for keypad register and addresses
#include "keypad.h"

// Encoded background image data
#include "modus.h"

#define RGB16(r,g,b) ((r)+(g<<5)+(b<<10))

// Video mode 4
#define REG_DISPCNT *(u32*)0x4000000
#define MODE_4 0x4
#define BG2_ENABLE 0x400

// Keep track of which direction user is moving
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

// Addresses for palette and video buffer
u16* paletteMem = (u16*)0x5000000;
u16* videoBuffer = (u16*)0x6000000;

// Draw a pixel of color c at location x, y
void plotPixel(int x, int y, u16 c) {
	videoBuffer[(y) *120 + (x)] = (c);
}

// Return color of pixel at location x,y
u16 getPixel(int x, int y) {
	return videoBuffer[(y) * 120 + (x)];
}

// Entry point
int main(void) {
	int x, y, blockX, blockY, loop, dir;
	u16 whitePixel = RGB16(31, 31, 31);

	// Set display mode 4
	REG_DISPCNT = MODE_4 | BG2_ENABLE;

	// Load the background image palette
	for(loop = 0; loop < 256; loop++) {
		paletteMem[loop] = modusPalette[loop];
	}

	// Infinite game loop
	while(1) {
		// Draw the background image
		for(y = 0; y < 160; y++) {
			for(x = 0; x < 120; x++) {
				plotPixel(x,y,modusData[y*120+x]);
			}
		}

		// Draw the border around the playfield - vertical lines
		for (y = 10; y < 150; y++) {
			plotPixel(10, y, whitePixel);
			plotPixel(110, y, whitePixel);
		}

		// And horizontal lines
		for (x = 10; x < 111; x++) {
			plotPixel(x, 10, whitePixel);
			plotPixel(x, 150, whitePixel);
		}

		// Wait for game to start
		while (1) {
			if (!((*KEYS) & KEY_START)) {
				break;
			}
		}

		loop = 0;

		// Initial random spawn point
		blockX = 10 + (rand() % 100);
		blockY = 10 + (rand() % 70);

		// Initial random movement direction
		dir = rand() % 3;

		// In game loop
		while (1)
		{
			// Only sample keys every 4000 iterations to slow game down
			if (loop == 4000) {
				if (!((*KEYS) & KEY_UP)) {
					dir = DIR_UP;
				}
				if (!((*KEYS) & KEY_DOWN)) {
					dir = DIR_DOWN;
				}
				if (!((*KEYS) & KEY_LEFT)) {
					dir = DIR_LEFT;
				}
				if (!((*KEYS) & KEY_RIGHT)) {
					dir = DIR_RIGHT;
				}

				// This is here rather than in block above so even if
				// user isn't pressing D pad they still keep moving in
				// last direction of travel
				switch(dir) {
					case DIR_UP:
						blockY--;
						break;
					case DIR_DOWN:
						blockY++;
						break;
					case DIR_LEFT:
						blockX--;
						break;
					case DIR_RIGHT:
						blockX++;
						break;
				}

				// Collision detection, if user hit a pixel that's
				// already white, game ends
				if (getPixel(blockX, blockY) == whitePixel) {
					break;
				}

				plotPixel(blockX, blockY, whitePixel);
				loop = 0;
			} else {
				loop++;
			}
		}
	}

	// This is unreachable but keeps compiler happy as it sees one
	// path through main that could potentially return an int
	return 0;
}