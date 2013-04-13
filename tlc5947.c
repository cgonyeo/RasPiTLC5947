#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include "tlc5947.h"

/*
 * Pin Mapping:
 * 
 * 0 -> SIN
 * 1 -> SCLK
 * 2 -> XLAT
 * 3 -> BLANK
 */

#define SIN 0
#define SCLK 1
#define XLAT 2
#define BLANK 3

int tlcleds[NUM_TLCS * 24];

void setLed(int ledIndex, int value)
{
	if(ledIndex >= 0 && ledIndex < NUM_TLCS * 16 && value >= 0 && value < 4096)
		tlcleds[ledIndex] = value;
}

void setAllLeds(int value)
{
	if(value >= 0 && value < 4096)
		for(int i = 0; i < NUM_TLCS * 16; i++)
			tlcleds[i] = value;
}

void clearLeds()
{
	setAllLeds(0);
}

int getLedValue(int index)
{
	if(index >= 0 && index < NUM_TLCS * 16)
		return index;
	return -1;
}

void pulsePin(int pin)
{
	digitalWrite(pin, 1);
	digitalWrite(pin, 0);
}

void tlc5940init()
{
	if(wiringPiSetup() == -1)
		printf("Wiring setup failed!\n");
	
	memset(&tlcleds, 0, sizeof(int));

	pinMode(SIN, OUTPUT);
	pinMode(SCLK, OUTPUT);
	pinMode(XLAT, OUTPUT);
	pinMode(BLANK, OUTPUT);
}

void tlc5940cleanup()
{
	for(int i =0; i < NUM_TLCS * 24; i++)
		free(tlcleds[i]);
}

void updateLeds()
{
	printf("Updating LEDs\n");
	for(int i = NUM_TLCS * 16 - 1; i >= 0; i--)
	{
		digitalWrite(SIN, tlcleds[i] & 2048);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 1024);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 512);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 256);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 128);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 64);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 32);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 16);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 8);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 4);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 2);
		pulsePin(SCLK);
		digitalWrite(SIN, tlcleds[i] & 1);
		pulsePin(SCLK);
	}
	
	digitalWrite(BLANK, 1);
	pulsePin(XLAT);
	digitalWrite(BLANK, 0);
}

//Example usage:
/* 
 * int main(void)
 * {
 * 	printf("Started\n");
 * 	printf("Num Tlcs: %i\n", NUM_TLCS);
 * 
 * 	tlc5940init();
 * 
 * 	for(int i = 0; i < 4096; i+= 1)
 * 	{
 * 		setAllLeds(i);
 * 		updateLeds();
 * 		usleep(1000);
 * 	}
 * 	setAllLeds(0);
 * 	updateLeds();
 * }
 */
