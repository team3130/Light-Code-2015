// DO NOT USE Port 13
// Code designed to run WS2811 LEDS and NEOPIXELs
// Use Subsystem class to describe a strip
// Use Command class to design an animation
// Command is similar to FIRST Command based robot programming:
//  There is Initialize that runs once every assignment to a Subsystem,
//  There is Execute that runs every cycle (with a tweak, see "speed"),
//  There is IsFinished that should return true when the Command is done.

#include <FastLED.h>

#define NUM_LEDS_PUSHER 64
#define NUM_LEDS_LIFTER 64

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
    Subsystem(int numberOfLeds);
    ~Subsystem();
    void SetDefaultCommand(Command *command);
    void SetCurrentCommand(Command *command);
    void execute();

    void Interlace(int size, CRGB *pattern);
    void SetColor(CRGB color);
    void ShiftForward(CRGB color=0);
    void ShiftBack(CRGB color=0);
    CRGB &operator[](size_t i) { if(i<0) return leds[0]; else if(i>=m_size) return leds[m_size-1]; else return leds[i]; };
    size_t size() {return m_size;};
  protected:
    CRGB* leds;
  private:
    size_t m_size;
    Command *default_command;
    Command *current_command;
  friend void setup();
};

class Command {
  public:
    Command(Subsystem *subsystem, int speed = 1);
    ~Command();
    bool Run();
    void SetSpeed(int speed) {my_speed=speed;};
    virtual void Initialize();
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
  m_size = numberOfLeds;
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
  for (int t = 0; t < m_size; t++) {
    leds[t] = color;
  }
}

void Subsystem::Interlace(int size, CRGB *pattern) {
  for (int t = 0; t < m_size; t++) {
    leds[t] = pattern[t % size];
  }
}

void Subsystem::ShiftForward(CRGB color) {
  for (int t = m_size-1; t > 0; t--) {
    leds[t] = leds[t-1];
  }
  leds[0] = color;
}

void Subsystem::ShiftBack(CRGB color) {
  for (int t = 0; t < m_size-1; t++) {
    leds[t] = leds[t+1];
  }
  leds[m_size-1] = color;
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
  return (cycleNumber >= 600);
}


// Blind - quick strob effect in intent to blind all around

class Blind : public Command {
    const static int SPEED = 1;
  public:
    Blind(Subsystem *s): Command(s, SPEED) {};

    void Execute() {
      if (my_strip != NULL) {
        my_strip->SetColor(cycleNumber%2 ? CRGB::White : 0);
      }
    };

    bool IsFinished() { return (cycleNumber >= 20); };
};


// Breathe - slowly oscillate up and down
//
class Breathe : public Command {
    const static int SPEED = 2;
    CRGB m_color;
  public:
    Breathe(Subsystem *s, CRGB color=CRGB::Green): Command(s, SPEED), m_color(color) {};

    void Execute() {
      if (my_strip != NULL) {
        CRGB color = m_color;
        if(cycleNumber < 256) {
          uint8_t val = triwave8(cycleNumber);
          //Serial.print("Breathe: "); Serial.print(val); Serial.print("\n");
          color %= (val/2);
        }
        else color = 0;
        my_strip->SetColor(color);
      }
    };

    bool IsFinished() { return (cycleNumber >= 300); };
};


// RainbowBreathe - slowly oscillate back and forth
//
class RainbowBreathe : public Command {
    const static int SPEED = 2;
    uint8_t m_hue;
    int step;
  public:
    RainbowBreathe(Subsystem *s, uint8_t hue=0): Command(s, SPEED), m_hue(hue) {step=256/s->size();};

    void Execute() {
      if (my_strip != NULL) {
        CHSV color;
        if(cycleNumber < 256) {
          uint8_t val = triwave8(cycleNumber);
          //Serial.print("Breathe: "); Serial.print(val); Serial.print("\n");
	  color.h = (m_hue + cycleNumber*step)%256;
	  color.s = 255;
          color.v = val/2;
        }
        else color.v = 0;
        my_strip->ShiftForward(color);
      }
    };

    bool IsFinished() { return (cycleNumber >= 300); };
};



/*
 * Here and below is the execution part.
 * As usual in Arduino: first setup() once and then loop() repeatedly.
 */

Subsystem *lifter;
Subsystem *pusher;
Command *default_lifter;
Command *default_pusher;
Command *chaser_p;
Command *blinder_p;

void dispatchInputs() {
  while(Serial.available() > 0) {
    String input = Serial.readString();
    Serial.print("Serial received: "); Serial.print(input);
    if(input.startsWith("Chase")) {
      pusher->SetCurrentCommand(chaser_p);
      if(isDigit(input.charAt(6)) or input.charAt(6)=='-') {
        int speed = input.substring(6).toInt();
        chaser_p->SetSpeed(speed);
      }
    }
    else if(input.startsWith("Blind")) {
      pusher->SetCurrentCommand(blinder_p);
    }
  }
}


void setup() {
  Serial.begin(9600);
  Serial.print("Initializing...\n");

  pinMode(DATA_PIN_PUSHER, OUTPUT);
  pinMode(DATA_PIN_LIFTER, OUTPUT);

  lifter = new Subsystem(NUM_LEDS_LIFTER);
  pusher = new Subsystem(NUM_LEDS_PUSHER);
  default_lifter = new Chase(lifter);
  default_pusher = new RainbowBreathe(pusher);

  chaser_p = new Chase(pusher);
  blinder_p = new Blind(pusher);

  lifter->SetDefaultCommand(default_lifter);
  pusher->SetDefaultCommand(default_pusher);

  FastLED.addLeds<WS2811, DATA_PIN_PUSHER>(pusher->leds, NUM_LEDS_PUSHER);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_LIFTER>(lifter->leds, NUM_LEDS_LIFTER);

  lifter->SetColor(CRGB::Yellow);
  pusher->SetColor(CRGB::Yellow);
  FastLED.show();
  FastLED.delay(500);
  lifter->SetColor(CRGB::Black);
  pusher->SetColor(CRGB::Black);
  FastLED.show();
  FastLED.delay(1000);
  Serial.print("Init is done. Starting...\n");
}


void loop() {
  dispatchInputs();
  lifter->execute();
  pusher->execute();
  FastLED.show();
  FastLED.delay(DELAY);
}

