//DO NOT USE Port 13
//Code designed to run WS2811 LEDS and NEOPIXELs
//Attempts to multitask ceratin functions by breaking for loops and reentering them
//Based on non-for loop timers. This should cause both to update their assigend
// light strip once and then go and update the next strip.
#include <FastLED.h>

#define NUM_LEDS_PUSHER 32
#define NUM_LEDS_LIFTER 39

#define NOTZEROED 'Z'
#define TEAMCOLORRED 'R'
#define TEAMCOLORBLUE 'B'
#define TEAMCOLORUNKNOWN 'U'
#define DRIVEFOWARD '3'
#define DRIVEBACK '4'
#define DRIVEIDLE '5'
#define LIFTERUP '6'
#define LIFTERDOWN '7'
#define LIFTERNEUTRAL '8'
#define PUSHERFORWARD '9'
#define PUSHERBACK '1'
#define PUSHERNEUTRAL '0'

#define DATA_PIN_PUSHER 2 //Using data pin one for primary light strip pin timer. Once again highly subject to change
#define DATA_PIN_LIFTER 3
#define DELAY 20 //sets the delay between light updates to 15 milliseconds

char serialValue = 0;

enum TaskStateVar {
	kInit,
	kZeroMe,
	kBreatheRed,
	kBreatheBlue,
	kBreatheUnknown,
	kWave,
	kChaseUp,
	kChaseDown,
	kRainbow
};

class Subsystem {
public:
	Subsystem(int numberOfLeds);
	void interlace(int size, CRGB *pattern);
	void setColor(CRGB color);
	void execute();
	void chase(int speed);
	void wave();
	void breathe(CRGB color);
	void multiColorDown();
	void blind();
	void resetSubsystem();
	int cycleNumber = 0;
	int sequenceStep = 0;
	int idleCycles = 0;
	TaskStateVar taskState;
	int actualNumberOfLeds;
	bool setToTop = false;
	bool done = false;
	CRGB leds[];

private:

};

Subsystem::Subsystem(int numberOfLeds) {
	taskState = kInit;
	actualNumberOfLeds = numberOfLeds;
	CRGB leds[numberOfLeds];
	resetSubsystem();
}

void Subsystem::resetSubsystem() {
	done = false;
	cycleNumber = 0;
	sequenceStep = 0;
	idleCycles = 0;
	setColor(CRGB::Black);
}

void Subsystem::execute() {
	if(done) return;
	if(idleCycles > 0) return;
	else idleCycles--;

	switch (taskState) {
	case kWave:
		wave();
		break;

	case kChaseUp:
		chase(5);
		break;
	case kChaseDown:
		chase(-5);
		break;

	case kRainbow:
		multiColorDown();
		break;

	case kZeroMe:
		blind();
		break;

	case kBreatheBlue:
		breathe(CRGB::Blue);
		break;
	case kBreatheRed:
		breathe(CRGB::Red);
		break;
	case kBreatheUnknown:
		breathe(CRGB::Violet);
		break;

	default:
		breathe(CRGB::Yellow);
	}
}

void Subsystem::setColor(CRGB color){
	for (int t = 0; t < actualNumberOfLeds; t++) {
		leds[t] = color;
	}
}

void Subsystem::interlace(int size, CRGB *pattern) {
	for (int t = 0; t < actualNumberOfLeds; t++) {
		leds[t] = pattern[t % size];
	}

}

void Subsystem::chase(int speed) {
	CRGB pattern[3];
	for(int t=0; t < sizeof(pattern); t++) {
		if(
				(speed > 0 && cycleNumber % sizeof(pattern) == 0) ||
				(speed < 0 && -cycleNumber % sizeof(pattern) == 0) ) {
			pattern[t] = CRGB::Yellow;
		}
		else pattern[t] = CRGB::Black;
	}
	interlace(sizeof(pattern), pattern);
	idleCycles = abs(speed);
	if (cycleNumber++ >= 250) {
		done = true;
	}
}

void Subsystem::blind() {
	if(cycleNumber%2 == 0) {
		setColor(CRGB::White);
	}
	else {
		setColor(CRGB::Black);
	}
	idleCycles = 5;
	if(cycleNumber++ >= 10) {
		done = true;
	}
}

void Subsystem::breathe(CRGB color) {
	static const int upCycles = 50;
	static const int downCycles = 100;
	CRGB calculated = CRGB::Black;
	float frac = 0.0;
	if(cycleNumber < upCycles) {
		frac = 1.0 * cycleNumber / upCycles;
	}
	else {
		frac = 1.0 - ((upCycles - cycleNumber) / downCycles);
	}
	calculated.setRGB(frac*color.r, frac*color.g, frac*color.b);
	setColor(calculated);
	if(cycleNumber > upCycles + downCycles) {
		done = true;
	}
	else {
		cycleNumber++;
	}
}

void Subsystem::wave() {
	if (!done) {
		switch (sequenceStep) {
		case 1:
			leds[cycleNumber] = CRGB::Red;

			break;

		case 2:
			leds[cycleNumber] = CRGB::Green;

			break;

		case 3:
			leds[cycleNumber] = CRGB::Blue;

			break;

		case 4:
			leds[cycleNumber] = CRGB::Cyan;

			break;

		case 5:
			leds[cycleNumber] = CRGB::Pink;

			break;

		case 6:
			leds[cycleNumber] = CRGB::Yellow;

			break;

		case 7:
			leds[cycleNumber] = CRGB::Red;

			break;

		case 8:
			leds[cycleNumber] = CRGB::Yellow;

			break;

		case 9:
			leds[cycleNumber] = CRGB::Purple;

			break;

		case 10:
			leds[cycleNumber] = CRGB::Green;

			break;

		case 11:
			leds[cycleNumber] = CRGB::Cyan;

			break;

		case 12:
			leds[cycleNumber] = CRGB::Blue;

			break;

		case 13:
			leds[cycleNumber] = CRGB::Brown;

			break;

		case 14:
			leds[cycleNumber] = CRGB::Gold;

			break;

		case 15:
			leds[cycleNumber] = CRGB::Yellow;

			break;

		case 16:
			leds[cycleNumber] = CRGB::Red;

			break;

		case 17:
			leds[cycleNumber] = CRGB::White;

			break;

		case 18:
			leds[cycleNumber] = CRGB::Blue;

			break;

		case 19:
			leds[cycleNumber] = CRGB::Yellow;

			break;

		case 20:
			leds[cycleNumber] = CRGB::Green;

			break;

		case 21:
			leds[cycleNumber] = CRGB::Purple;

			break;

		case 23:
			leds[cycleNumber] = CRGB::Green;

			break;

		case 24:
			leds[cycleNumber] = CRGB::Blue;

			break;

		case 25:
			leds[cycleNumber] = CRGB::Green;

			break;

		case 26:
			leds[cycleNumber] = CRGB::Purple;

			break;

		case 27:
			leds[cycleNumber] = CRGB::Yellow;

			break;

		case 28:
			leds[cycleNumber] = CRGB::White;

			break;

		case 29:
			leds[cycleNumber] = CRGB::Brown;

			break;

		case 30:
			leds[cycleNumber] = CRGB::Yellow;

			break;

		case 31:
			leds[cycleNumber] = CRGB::White;

			break;

		case 32:
			leds[cycleNumber] = CRGB::Purple;

			break;

		default:
			leds[cycleNumber] = CRGB::Purple;

		}
		if (cycleNumber == sequenceStep) {
			cycleNumber = 0;
			sequenceStep++;
		} else {
			cycleNumber++;
		}

		if (sequenceStep == actualNumberOfLeds) {
			done = true;
		}

	}
}

void Subsystem::multiColorDown() {
	if (!done) {
		int t = actualNumberOfLeds - cycleNumber - 1;
		if (t <= 0) {
			cycleNumber = 0;
			sequenceStep++;
		} else {
			cycleNumber++;
		}
		if (sequenceStep > 5) {
			done = true;
		}
		switch (sequenceStep) {
		case 1:
			leds[t] = CRGB::Red;
			break;

		case 2:
			leds[t] = CRGB::Blue;
			break;

		case 3:
			leds[t] = CRGB::Green;
			break;

		case 4:
			leds[t] = CRGB::Yellow;
			break;

		case 5:
			leds[t] = CRGB::Cyan;
			break;

		default:
			leds[t] = CRGB::White;
			break;

		}
	}
}

///////
///////
///////
///////
///////
///////
//END LIFTER CLASS
///////
///////
///////
///////
///////
///////
///////
///////
///////
///////
///////
///////

Subsystem lifter(NUM_LEDS_LIFTER);
Subsystem pusher(NUM_LEDS_PUSHER);

///////
///////
///////
///////
///////
///////
//START GLOBAL FUNCTIONS
///////
///////
///////
///////
///////
///////
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
//\/\/\/\/\/\/\/\/\/
void dispatchInputs() {
	while(Serial.available() > 0) {
		serialValue = Serial.read();

		switch (serialValue) {
		case NOTZEROED:
			pusher.resetSubsystem();
			lifter.resetSubsystem();
			pusher.taskState = kZeroMe;
			lifter.taskState = kZeroMe;
			break;

		case TEAMCOLORRED:
			pusher.resetSubsystem();
			lifter.resetSubsystem();
			pusher.taskState = kBreatheRed;
			lifter.taskState = kBreatheRed;
			break;

		case TEAMCOLORBLUE:
			pusher.resetSubsystem();
			lifter.resetSubsystem();
			pusher.taskState = kBreatheBlue;
			lifter.taskState = kBreatheBlue;
			break;

		case TEAMCOLORUNKNOWN:
			pusher.resetSubsystem();
			lifter.resetSubsystem();
			pusher.taskState = kBreatheUnknown;
			lifter.taskState = kBreatheUnknown;
			break;

		case LIFTERUP:
			lifter.resetSubsystem();
			lifter.taskState = kChaseUp;
			break;

		case LIFTERNEUTRAL:
			lifter.resetSubsystem();
			lifter.taskState = kWave;
			break;

		case LIFTERDOWN:
			lifter.resetSubsystem();
			lifter.taskState = kChaseDown;
			break;


		case PUSHERNEUTRAL:
			pusher.resetSubsystem();
			pusher.taskState = kWave;
			break;

		case PUSHERFORWARD:
			pusher.resetSubsystem();
			pusher.taskState = kChaseDown;
			break;

		case PUSHERBACK:
			pusher.resetSubsystem();
			pusher.taskState = kChaseUp;
			break;

		default:
			break;
		}
	}
}


void startUpLights() {
	int dynamicDelay = 10000;
	for (int timer = NUM_LEDS_PUSHER - 1; timer > -1; timer--) {

		pusher.leds[timer] = CRGB::Red;      //Actually green. WS2811's are odd.
		FastLED.show();
		if (dynamicDelay > 20) {
			dynamicDelay = dynamicDelay / 2;
		}
		if (dynamicDelay < 10) {
			dynamicDelay = 20;
		}
		delay(dynamicDelay);

	}

	for (int timer = 0; timer < NUM_LEDS_LIFTER; timer++) {

		lifter.leds[timer] = CRGB::Green;
		FastLED.show();
		delay(dynamicDelay);

	}
	for (int forloopthingy = 0; forloopthingy < 10; forloopthingy++) {
		for (int timer = 0; timer < NUM_LEDS_LIFTER; timer++) {

			lifter.leds[timer] = CRGB::White;

		}

		for (int timer = 0; timer < NUM_LEDS_PUSHER; timer++) {

			pusher.leds[timer] = CRGB::White;

		}
		FastLED.show();
		delay(100);

		for (int timer = 0; timer < NUM_LEDS_LIFTER; timer++) {

			lifter.leds[timer] = CRGB::Green;

		}

		for (int timer = 0; timer < NUM_LEDS_PUSHER; timer++) {

			pusher.leds[timer] = CRGB::Red;

		}
		FastLED.show();
		delay(100);
	}
	for (int timer = 0; timer < NUM_LEDS_PUSHER; timer++) {

		pusher.leds[timer] = CRGB::Black;

	}

	for (int timer = 0; timer < NUM_LEDS_LIFTER; timer++) {

		lifter.leds[timer] = CRGB::Black;

	}
	FastLED.show();
}


///////////////////////////////////////////////////////////////////// Break of the code between
//////////////////////////////////////////////////////////////////// Intial startup and loop
/////////////////////

void setup() {
	pinMode(DATA_PIN_PUSHER, OUTPUT);
	pinMode(DATA_PIN_LIFTER, OUTPUT);

	FastLED.addLeds<WS2811, DATA_PIN_PUSHER>(pusher.leds, NUM_LEDS_PUSHER);
	FastLED.addLeds<NEOPIXEL, DATA_PIN_LIFTER>(lifter.leds, NUM_LEDS_LIFTER);
	startUpLights();

	lifter.resetSubsystem();
	pusher.resetSubsystem();
	FastLED.show();
	Serial.begin(9600);
}


void loop() {
	dispatchInputs();
	lifter.execute();
	pusher.execute();
	FastLED.show();
	delay(DELAY);
}

