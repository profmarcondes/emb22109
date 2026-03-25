#include <Arduino.h>
#include "arduinoPins.h"
#include "sc_timer_service.h"

// TODO: Include the header files for the state machine
// and the operation callback class.

using namespace sc::timer;

#define MAX_TIMERS 10
TimerTask tasks[MAX_TIMERS];
TimerService* timer_sct = new TimerService(tasks, MAX_TIMERS);

// TODO: Create an instance of the state machine and the operation callback class. 
// The state machine instance must be globally accessible for 
// the operation callback class.

int onOffState = 0;
int lastOnOffState = HIGH;
int pedRequestState = 0;
int lastPedRequestState = HIGH;

int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


//The setup function is called once at startup of the sketch
void setup() {

	Serial.begin(9600);
	Serial.println("Code generation example");

	pinMode(button_1_pin, INPUT_PULLUP);
	pinMode(button_2_pin, INPUT_PULLUP);

	pinMode(led_green_pin, OUTPUT);
	pinMode(led_yellow_pin, OUTPUT);
	pinMode(led_red_pin, OUTPUT);

	pinMode(ped_led_red_pin, OUTPUT);
	pinMode(ped_led_green_pin, OUTPUT);
	pinMode(ped_led_request_pin, OUTPUT);

	// TODO: Set the timer service for the state machine and the 
	// operation callback for the state machine, then enter the state machine.
	
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
			  Serial.println("Button 01 pressed");

			  // TODO: Raise the event for the pedestrian request

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
			  Serial.println("Button 02 pressed");

			  // TODO:Raise the event for the on/off button

			}
		}
	}

	if ( cycle_count == 0L || (current_millies >= last_cycle_time + CYCLE_PERIOD) ) {

		// TODO: Call the timer service to proceed with the elapsed time since 
		// the last cycle and then call the runCycle method of the state machine.


		
		last_cycle_time = current_millies;
		cycle_count++;
	}

}