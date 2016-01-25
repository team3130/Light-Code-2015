#include <FastLED.h>

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
    CRGB &operator[](size_t i) { if(i >= m_size) return leds[m_size-1]; else return leds[i]; };
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
    virtual void Execute() = 0;
    virtual bool IsFinished() = 0;
    virtual void End();
  protected:
    Subsystem *my_strip;
    int cycleNumber = 0;
    int idleCycles = 0;
    int my_speed = 1;
};

