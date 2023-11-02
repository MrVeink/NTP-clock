#include <FastLED.h>
#include <stdint.h>
#include "clock_display.h"

#define FONT_WIDTH 4
#define FONT_MASK 0x80
#define COLON_WIDTH 2
#define COLON_MASK (1 << (COLON_WIDTH - 1))
#define START_OFFSET 0

CRGB leds[NUM_LEDS];

void initDisplay(void) {
	pinMode(DISPLAY_PIN, OUTPUT);
	FastLED.addLeds<WS2812, DISPLAY_PIN, GRB>(leds, NUM_LEDS);
	FastLED.setBrightness(7);
	FastLED.clear();
}

uint16_t translateXY(uint8_t x, uint8_t y) {
	uint8_t oddColumn = x&1;
	return ((DISPLAY_WIDTH - (x + 1 - oddColumn))*DISPLAY_HEIGHT - oddColumn) + y*(1 - 2*oddColumn);
}

void displayDigit(uint8_t digit, uint8_t xOffset, uint8_t yOffset) {
	for (uint8_t x = 0; x < FONT_WIDTH; x++) {
		for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++) {
			uint8_t on = (NUMBER_FONT[digit][y] & (FONT_MASK >> x)) > 0;
			leds[translateXY(x + xOffset, y + yOffset)] = on*CRGB::Red + CRGB::Black;
		}
	}
}

void displayColon(uint8_t xOffset, uint8_t yOffset) {
	for (uint8_t x = 0; x < COLON_WIDTH; x++) {
		for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++) {
			uint8_t on = (COLON[y] & (COLON_MASK >> x)) > 0;
			leds[translateXY(x + xOffset, y + yOffset)] = on*CRGB::Red + CRGB::Black;
		}
	}
}

void displayTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
	FastLED.clear();

	displayDigit(hours/10, START_OFFSET, 0);
	displayDigit(hours%10, START_OFFSET + FONT_WIDTH, 0);

	if (seconds&1)
		displayColon(START_OFFSET + FONT_WIDTH*2, 0);

	displayDigit(minutes/10, START_OFFSET + FONT_WIDTH*2 + COLON_WIDTH, 0);
	displayDigit(minutes%10, START_OFFSET + FONT_WIDTH*3 + COLON_WIDTH, 0);
	FastLED.show();
}
