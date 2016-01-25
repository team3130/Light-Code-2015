#include "Lights3130.h"

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
  for (size_t t = 0; t < m_size; t++) {
    leds[t] = color;
  }
}

void Subsystem::Interlace(int size, CRGB *pattern) {
  for (size_t t = 0; t < m_size; t++) {
    leds[t] = pattern[t % size];
  }
}

void Subsystem::ShiftForward(CRGB color) {
  for (size_t t = m_size-1; t > 0; t--) {
    leds[t] = leds[t-1];
  }
  leds[0] = color;
}

void Subsystem::ShiftBack(CRGB color) {
  for (size_t t = 0; t < m_size-1; t++) {
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



