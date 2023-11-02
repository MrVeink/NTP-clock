#include <WiFi.h>
#include <FastLED.h>
#include <time.h>
#include "clock_display.h"

#define SSID "P30"
#define PASSWORD "00000000"

const char *NTP_POOL = "1.fi.pool.ntp.org";
const char *LOCAL_TZ = "EET-2EEST-3,M3.5.0/03:00:00,M10.5.0/04:00:00";

time_t currentTime = 0;
tm timeinfo;

void setup() {
	Serial.begin(115200);
	initDisplay();
	WiFi.begin(SSID, PASSWORD);

	Serial.print("Connecting to ");
	Serial.print(SSID);
	uint8_t counter = 0;
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(200);

		if (++counter == 100)
			ESP.restart();
	}
	Serial.println("\nConnected!");

	counter = 0;
	setenv("TZ", LOCAL_TZ, 1);
	configTime(0, 0, NTP_POOL);
	Serial.print("Synchronising time with ");
	Serial.print(NTP_POOL);
	do {
		time(&currentTime);
		localtime_r(&currentTime, &timeinfo);
		Serial.print(".");
		delay(50);

		if (++counter == 100)
			ESP.restart();
	} while (timeinfo.tm_year < 10);
}

void loop() {
	time(&currentTime);
	localtime_r(&currentTime, &timeinfo);
	displayTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}
