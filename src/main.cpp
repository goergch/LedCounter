#include <Arduino.h>
#include "FastLED.h"
#include <math.h>
#include <EasyButton.h>
#define NUM_LEDS 144
#define COUNTER_HEIGHT 36
CRGB leds[NUM_LEDS];
EasyButton pauseButton(D5);
EasyButton minusButton(D6);
EasyButton plusButton(D7);
EasyButton startButton(D0);

int progressDown = false;
int progressInverted = false;

void PauseButton(){
	progressDown = false;
	progressInverted = false;
}
void MinusButton(){
	progressDown = false;
	progressInverted = true;
}
void PlusButton(){
	progressDown = true;
	progressInverted = false;
}
void StartButton(){
	progressDown = true;
	progressInverted = true;
}

void drawProgress(float progress, bool down, bool inverted) {
	float progFl = COUNTER_HEIGHT * progress / 100.0;
	uint16 progressHeight = uint16(progFl);
	int dim = (progFl - float(progressHeight)) *255;


	for(int i=0;i<COUNTER_HEIGHT;i++){
		CRGB color;
		if (i == progressHeight+1){
			color.setHSV(0,0,inverted?255-dim:dim);
		} else if (i > progressHeight) {
			color = inverted?CRGB::White:CRGB::Black;
		}else {
			color = inverted?CRGB::Black:CRGB::White;
		}
		int index = down?COUNTER_HEIGHT-i-1:i;
		leds[index]=leds[COUNTER_HEIGHT*2-index-1]=leds[COUNTER_HEIGHT*2+index]=leds[COUNTER_HEIGHT*4-1-index]=color;
	}

}

void setup() { 
	Serial.begin(115200);
	FastLED.addLeds<NEOPIXEL, D4>(leds, NUM_LEDS);
	FastLED.setBrightness(10);
	startButton.onPressed(StartButton);
	minusButton.onPressed(MinusButton);
	plusButton.onPressed(PlusButton);
	pauseButton.onPressed(PauseButton);
	// attachInterrupt(digitalPinToInterrupt(D5),PauseButtonInterrupt,RISING);
	// attachInterrupt(digitalPinToInterrupt(D6),MinusButtonInterrupt,RISING);
	// attachInterrupt(digitalPinToInterrupt(D7),PlusButtonInterrupt,RISING);
	// attachInterrupt(digitalPinToInterrupt(D8),StartButtonInterrupt,RISING);
 }

void loop() {
	float t = fmod(millis(),100 * 250)/250;
	drawProgress(t,progressDown,progressInverted);
	FastLED.show();
	startButton.read();
	minusButton.read();
	plusButton.read();
	pauseButton.read();
}


