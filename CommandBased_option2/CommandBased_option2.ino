// DO NOT USE Port 13
// Code designed to run WS2811 LEDS and NEOPIXELs
// Use Subsystem class to describe a strip
// Use Command class to design an animation
// Command is similar to FIRST Command based robot programming:
//  There is Initialize that runs once every assignment to a Subsystem,
//  There is Execute that runs every cycle (with a tweak, see "speed"),
//  There is IsFinished that should return true when the Command is done.

#include <FastLED.h>

#define NUM_LEDS_PUSHER 32
#define NUM_LEDS_LIFTER 32

#define DATA_PIN_PUSHER 4
#define DATA_PIN_LIFTER 3

#define DELAY 20 //sets the delay between light updates in milliseconds


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


/*
   Under the hood stuff. TODO: move it to a library maybe
*/

class Command;

class Subsystem {
  public:
    CRGB* leds;
    Subsystem(int numberOfLeds);
    ~Subsystem();
    void SetDefaultCommand(Command *command);
    void SetCurrentCommand(Command *command);
    void execute();

    void Interlace(int size, CRGB *pattern);
    void SetColor(CRGB color);

  private:
    int actualNumberOfLeds;
    Command *default_command;
    Command *current_command;
};

class Command {
  public:
    Command(Subsystem *subsystem, int speed = 1);
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


// Subsystem class implementation

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
  else if (default_command != NULL) {
    if (!default_command->Run()) {
      default_command->Initialize();
    }
  }
}


void Subsystem::SetColor(CRGB color) {
  for (int t = 0; t < actualNumberOfLeds; t++) {
    leds[t] = color;
  }
}

void Subsystem::Interlace(int size, CRGB *pattern) {
  for (int t = 0; t < actualNumberOfLeds; t++) {
    leds[t] = pattern[t % size];
  }
}



// Command class implementation

Command::Command (Subsystem *subsystem, int speed)
  : my_strip(subsystem)
  , my_speed(speed)
{}

void Command::Initialize() {
  cycleNumber = 0;
  idleCycles = 0;
}

bool Command::Run() {
  if (idleCycles > 0) {
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

/*
   End of ""Under the hood section
*/




/*
   Command family classes represent different blinky styles.
   Just extend the Command class and be creative.
*/

// Chase - simple chasing lights like in a marquee sign.

class Chase : public Command {
    const static int CHASE_SIZE = 5;
    const static int CHASE_SPEED = 5;
  public:
    Chase(Subsystem *s): Command(s, CHASE_SPEED) {};
    void Execute();
    bool IsFinished();
  private:
    CRGB pattern[CHASE_SIZE];
};

void Chase::Execute() {
  for (int t = 0; t < CHASE_SIZE; t++) {
    if (
      (my_speed > 0 && (t + cycleNumber) % CHASE_SIZE == 0) ||
      (my_speed < 0 && (t - cycleNumber) % CHASE_SIZE == 0) ) {
      pattern[t] = CRGB::Yellow;
    }
    else pattern[t] = CRGB::Black;
  }
  if (my_strip != NULL) {
    my_strip->Interlace(CHASE_SIZE, pattern);
  }
}

bool Chase::IsFinished() {
  return (cycleNumber >= 2500);
}





/*

TODO: convert these into classes...

  void Subsystem::blind() {
  if(done) return;
  if(cycleNumber%2 == 0) {
    SetColor(CRGB::White);
  }
  else {
    SetColor(CRGB::Black);
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
  SetColor(calculated);
  cycleNumber++;
  }
*/


Subsystem *lifter;
Subsystem *pusher;
Command *default_lifter;
Command *default_pusher;

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing...\n");

  pinMode(DATA_PIN_PUSHER, OUTPUT);
  pinMode(DATA_PIN_LIFTER, OUTPUT);

  lifter = new Subsystem(NUM_LEDS_LIFTER);
  pusher = new Subsystem(NUM_LEDS_PUSHER);
  default_lifter = new Chase(lifter);
  default_pusher = new Chase(pusher);

  lifter->SetDefaultCommand(default_lifter);
  pusher->SetDefaultCommand(default_pusher);

  FastLED.addLeds<WS2811, DATA_PIN_PUSHER>(pusher->leds, NUM_LEDS_PUSHER);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_LIFTER>(lifter->leds, NUM_LEDS_LIFTER);

  lifter->SetColor(CRGB::Yellow);
  pusher->SetColor(CRGB::Yellow);
  FastLED.show();
  FastLED.delay(DELAY);
  lifter->SetColor(CRGB::Black);
  pusher->SetColor(CRGB::Black);
  FastLED.show();
  FastLED.delay(DELAY);
  Serial.print("Init is done. Starting...\n");
}


void loop() {
  //  dispatchInputs();
  lifter->execute();
  pusher->execute();
  FastLED.show();
  FastLED.delay(DELAY);
}

