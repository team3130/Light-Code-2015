//DO NOT USE Port 13
//Code designed to run WS2811 LEDS and NEOPIXELs
//Attempts to multitask ceratin functions by breaking for loops and reentering them
//Based on non-for loop timers. This should cause both to update their assigend
// light strip once and then go and update the next strip.
#include <FastLED.h>

#define NUM_LEDS_PUSHER 32
#define NUM_LEDS_LIFTER 32

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

#define DATA_PIN_PUSHER 4 //Using data pin one for primary light strip pin timer. Once again highly subject to change
#define DATA_PIN_LIFTER 3
#define DELAY 20 //sets the delay between light updates in milliseconds


int BigTimer = 0;

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

class Command;

class Subsystem {
public:
	Subsystem(int numberOfLeds);
	~Subsystem();
	void interlace(int size, CRGB *pattern);
	void setColor(CRGB color);
  void SetDefaultCommand(Command *command);
  void SetCurrentCommand(Command *command);
	void execute();
 /*
	void chase(int speed);
	void wave();
	void breathe(CRGB color);
	void multiColorDown();
	void blind();
	void resetSubsystem();
	TaskStateVar taskState;
 */
	int actualNumberOfLeds;
	CRGB* leds;

private:
  Command *default_command;
  Command *current_command;
};

class Command {
public:
  Command(Subsystem *subsystem, int speed=1);
  ~Command();
  virtual void Initialize();
  bool Run();
  virtual void Execute();
  virtual bool IsFinished();
  virtual void End();
protected:
  Subsystem *my_strip;
  int cycleNumber = 0;
  int idleCycles = 0;
  int my_speed = 1;
};

Subsystem::Subsystem(int numberOfLeds) {
	leds = new CRGB[numberOfLeds];
	actualNumberOfLeds = numberOfLeds;
  default_command = NULL;
  current_command = NULL;
}

Subsystem::~Subsystem() {
	delete leds;
}

void Subsystem::SetDefaultCommand(Command *command) {
  default_command = command;
  default_command->Initialize();
}

void Subsystem::SetCurrentCommand(Command *command) {
  current_command = command;
  current_command->Initialize();
}

void Subsystem::execute() {
  if (current_command != NULL) {
    if (!current_command->Run()) {
      current_command = NULL;
      default_command->Initialize();
    }
  }
  else if(default_command != NULL) {
    if (!default_command->Run()) {
      default_command->Initialize();
    }
  }
}



void Subsystem::setColor(CRGB color){
	for (int t = 0; t < actualNumberOfLeds; t++) {
		leds[t] = color;
	}
//  Serial.print("Set color LEDs: ");
//  Serial.print((int)leds);
//  Serial.print("\n");
}

void Subsystem::interlace(int size, CRGB *pattern) {
	for (int t = 0; t < actualNumberOfLeds; t++) {
		leds[t] = pattern[t % size];
	}
/*  
  Serial.print("Interlace LEDs: ");
  Serial.print((int)leds);
  Serial.print(", size=");
  Serial.print(size);
  Serial.print(", R=");
  Serial.print(leds[0].r);
  Serial.print(", G=");
  Serial.print(leds[0].g);
  Serial.print(", B=");
  Serial.print(leds[0].b);
  Serial.print("\n");
*/
}





Command::Command (Subsystem *subsystem, int speed)
  : my_strip(subsystem)
  , my_speed(speed)
{}

void Command::Initialize() {
  cycleNumber = 0;
  idleCycles = 0;
}

bool Command::Run() {
  if(idleCycles > 0) {
    idleCycles--;
    return true;
  }
  idleCycles = abs(my_speed);
  cycleNumber++;
  Execute();
  if (IsFinished()) {
    End();
    return false;
  }
  return true;
}

void Command::End() {
  Serial.print("Command ");
  Serial.print((int)this);
  Serial.print(" has finished\n");
}

class Chase : public Command {
  const static int CHASE_SIZE = 5;
public:
  Chase(Subsystem *s): Command(s, 5) {};
  void Execute();
  bool IsFinished();
private:
  CRGB pattern[CHASE_SIZE];
};

void Chase::Execute() {
	for(int t=0; t < CHASE_SIZE; t++) {
		if(
				(my_speed > 0 && (t+cycleNumber) % CHASE_SIZE == 0) ||
				(my_speed < 0 && (t-cycleNumber) % CHASE_SIZE == 0) ) {
			pattern[t] = CRGB::Yellow;
		}
		else pattern[t] = CRGB::Black;
	}
  if (my_strip != NULL) {
  	my_strip->interlace(CHASE_SIZE, pattern);
  }
}

bool Chase::IsFinished() {
	return (cycleNumber >= 2500);
}





/*

void Subsystem::blind() {
	if(done) return;
	if(cycleNumber%2 == 0) {
		setColor(CRGB::White);
	}
	else {
		setColor(CRGB::Black);
	}
	idleCycles = 5;
	if(cycleNumber++ >= 10) {
		resetSubsystem();
		done = true;
	}
}

void Subsystem::breathe(CRGB color) {
	static const int upCycles = 50;
	static const int downCycles = 100;
	static const int totalCycles = 300;
	static const float maxPower = 0.5;
	CRGB calculated = CRGB::Black;
	float frac = 0.0;
	if(cycleNumber < upCycles) {
		frac = maxPower * cycleNumber / upCycles;
	}
	else if(cycleNumber < upCycles + downCycles)  {
		frac = maxPower - (maxPower*( cycleNumber - upCycles) / downCycles);
	}
	else if(cycleNumber < totalCycles) {
		frac = 0;
	}
	else {
		resetSubsystem();
		done = true;
		return;
	}
	calculated.setRGB(frac*color.r, frac*color.g, frac*color.b);
	setColor(calculated);
	cycleNumber++;
}
*/


Subsystem lifter(NUM_LEDS_LIFTER);
Subsystem pusher(NUM_LEDS_PUSHER);


///////////////////////////////////////////////////////////////////// Break of the code between
//////////////////////////////////////////////////////////////////// Intial startup and loop

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing...\n");

	pinMode(DATA_PIN_PUSHER, OUTPUT);
	pinMode(DATA_PIN_LIFTER, OUTPUT);

	FastLED.addLeds<WS2811, DATA_PIN_PUSHER>(pusher.leds, NUM_LEDS_PUSHER);
	//FastLED.addLeds<NEOPIXEL, DATA_PIN_PUSHER>(pusher.leds, NUM_LEDS_PUSHER);
	FastLED.addLeds<NEOPIXEL, DATA_PIN_LIFTER>(lifter.leds, NUM_LEDS_LIFTER);

  lifter.SetDefaultCommand(new Chase(&lifter));
  pusher.SetDefaultCommand(new Chase(&pusher));
  lifter.setColor(CRGB::Yellow);
  pusher.setColor(CRGB::Yellow);
	FastLED.show();
  delay(DELAY);
  lifter.setColor(CRGB::Black);
  pusher.setColor(CRGB::Black);
  FastLED.show();
  Serial.print("Init is done. Starting...\n");
}


void loop() {
//	dispatchInputs();
	lifter.execute();
	pusher.execute();
	FastLED.show();
	delay(DELAY);
}
