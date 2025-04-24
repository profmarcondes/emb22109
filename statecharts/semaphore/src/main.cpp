#include <Arduino.h>
#include "Semaforo.h"
#include "sc_timer_service.h"
#include "arduinoPins.h"
#include "SemaforoOCBs.h"

using namespace sc::timer;

#define MAX_TIMERS 10
TimerTask tasks[MAX_TIMERS];

Semaforo* arduino = new Semaforo();
TimerService* timer_sct = new TimerService(tasks, MAX_TIMERS);
SemaforoOCBs* operationCallback = new SemaforoOCBs(arduino);

int onOffState = 0;
int lastOnOffState = HIGH;
int pedRequestState = 0;
int lastPedRequestState = HIGH;

int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	Serial.println("Traffic Light Controller");
	pinMode(button_1_pin, INPUT_PULLUP);
	pinMode(button_2_pin, INPUT_PULLUP);

	//pushButton_1 = new PushButton(button_1_pin, button_1_changed);
	//pushButton_2 = new PushButton(button_2_pin, button_2_changed);

	pinMode(led_green_pin, OUTPUT);
	pinMode(led_yellow_pin, OUTPUT);
	pinMode(led_red_pin, OUTPUT);

	pinMode(ped_led_red_pin, OUTPUT);
	pinMode(ped_led_green_pin, OUTPUT);
	pinMode(ped_led_request_pin, OUTPUT);

	arduino->setTimerService(timer_sct);
	arduino->setOperationCallback(operationCallback);
	arduino->enter();
}

#define CYCLE_PERIOD (10)
static unsigned long cycle_count = 0L;
static unsigned long last_cycle_time = 0L;

void loop() {

	unsigned long current_millies = millis();

	int reading_1 = digitalRead(button_1_pin);
	if (reading_1 != lastPedRequestState) {
		lastDebounceTime = millis();
		lastPedRequestState = reading_1;
	}
	if ((millis() - lastDebounceTime) > debounceDelay) {
		if (reading_1 != pedRequestState) {
			pedRequestState = reading_1;
			if (pedRequestState == LOW) {
			  Serial.println("Button pressed");
			  arduino->raisePedButton();
			}
		}
	}

	reading_1 = digitalRead(button_2_pin);
	if (reading_1 != lastOnOffState) {
		lastDebounceTime = millis();
		lastOnOffState = reading_1;
	}
	if ((millis() - lastDebounceTime) > debounceDelay) {
		if (reading_1 != onOffState) {
			onOffState = reading_1;
			if (onOffState == LOW) {
			  Serial.println("Button pressed");
			  arduino->raiseOnOffButton();
			}
		}
	}

	if ( cycle_count == 0L || (current_millies >= last_cycle_time + CYCLE_PERIOD) ) {

		timer_sct->proceed(current_millies - last_cycle_time);
		arduino->runCycle();

		last_cycle_time = current_millies;
		cycle_count++;
	}


}