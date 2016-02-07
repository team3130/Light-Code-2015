#include "Lights3130.h"

// Subsystem class implementation

Subsystem::Subsystem(int numberOfLeds) {
  leds = new CRGB[numberOfLeds];
  m_size = numberOfLeds;
  m_first = true;
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
  if (current_command == NULL) current_command = default_command;

  if (current_command != NULL) {
    if (m_first) {
      current_command->Initialize();
      m_first = false;
    }
    if (false == current_command->Run()) {
      current_command = NULL;
      m_first = true;
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
}


// CommandGroup class implementation

CommandGroup::CommandGroup (Subsystem *subsystem, int speed)
  : Command(subsystem, speed)
  , m_root(NULL)
  , m_iterator(NULL)
  , m_first(true)
{
}

void CommandGroup::Initialize() {
  Command::Initialize();
  m_iterator = m_root;
  m_first = true;
}

bool CommandGroup::Run() {
  if (m_root == NULL) return false;
  if (m_iterator == NULL) m_iterator = m_root;

  if (Command::Run() == false) {
    if (m_iterator->command != NULL) m_iterator->command->End();
    return false;
  }

  if (m_iterator->command != NULL) {
    if (m_first) {
      m_first = false;
      m_iterator->command->Initialize();
    }
    if (m_iterator->command->Run()) return true;
  }

  // If we made it all the way down to here it means the current command has finished
  // It's time to move forward to the next command in the group
  m_iterator = m_iterator->next;
  if (m_iterator == NULL) {
    // This is the end. Run End() and retire.
    End();
    return false;
  }
}

void CommandGroup::End() {
}

void CommandGroup::AddSequential(Command *command) {
	CommandContainer **link = &m_root;
	while (*link != NULL) link = &((*link)->next);
	CommandContainer *cc = new CommandContainer;
	cc->command = command;
	cc->next = NULL;
	*link = cc;
}

CommandGroup::~CommandGroup() {
	CommandContainer *p = m_root;
	while (p != NULL) {
		CommandContainer *waste = p;
		p = p->next;
		delete waste;
	}
}
