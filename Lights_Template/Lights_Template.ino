//DO NOT USE Port 13
//Code designed to run WS2811 LEDS and NEOPIXELs
//Attempts to multitask ceratin functions by breaking for loops and reentering them
//Based on non-for loop timers. This should cause both to update their assigend
// light strip once and then go and update the next strip.
#include <FastLED.h>  //Library we use because of its options
#define NUM_LEDS_1 30 //Number of Leds in strip 1
#define DATA_PIN_1 3  //Says which data pin to use for LED  strip 1



class Subsystem  //Class that allows for multitasking lights. Made to be flexible.
{
  public:
  Subsystem(int numberOfLeds,int updateChooser, int delayTime, bool serialBased);
 
  void setColor(CRGB color);  //Sets the entire light strip to one color
  void setColorFrom(CRGB color, int low, int high); //Sets the a part of a light strip to one color
  void setColorUp(CRGB color); //Sets the light strip to one color going up from the bottom. Occurs over several system.run's
  void setColorDown(CRGB color);//Sets the light strip to one color going down from the top . Occurs over several system.runs's
  void moveUp(); // Moves every LED bulb color up one.
  void moveDown(); //Moves every LED bulb color down one.
  void moveUpOnly(int low, int high); //Moves up the LED bulb colors from a range up.
  void moveDownOnly(int low, int high); //Moves up the LED bulb colors from a ranage down.
  
  
  
  void resetSubsystem(); //Wipes everything in the system to the starting settings.
  void resetTimers();//Wipes only the timers in the system.
  void override();//SERIAL ONLY: Checks if any serial functions need to occur that override the current function.
  void runSystem();//Runs the system once.
  void updateTask();//Updates the systems task when a system is done running.
  
  
  int getDelay();//Returns how much delay the subsystem should run.
  int getTaskState();//Returns the current task state of the subsystem.
  
  
  CRGB leds[64];              //Light Array. Set to 64 due to some oddities.
  
  private:
  
  bool isSerialBased;      //Boolean used to tell if the subsystem is serial based.
  bool setToTop = false;    //Used for some functions.
  bool done = false;  //Tells the subsystem whether or not to update the task.
  bool completelyDone = false;
  bool miscCheck = false;    //Random check used for various functions.
  int _updateChooser;        //SERIAL BASED: Used to distinguish between various serial based systems.
  int actualNumberOfLeds;    //Variable used to store how many LEDS are used in the system. Only needed due to oddities with the LED array and subsystem classes.
  int chaseround = 0;  //Variables
  int systemDelayTime = 0;
  int forloopthingy = 0;
  int functionStep = 0;
  
  int timer = 0;
  int impartialTimeKeeper = 0;
  int impartialForLoopKeeper = 0;
  int taskState = 0;
  
  char serialValue = Serial.read();
  
  
};

 Subsystem::Subsystem(int numberOfLeds, int updateChooser,int delayTime, bool serialBased){
   
   actualNumberOfLeds = numberOfLeds;
   _updateChooser = updateChooser;
   isSerialBased = serialBased;
   systemDelayTime = delayTime;
   
   
   
  
}
///////
///////
///////
///////
///////
//////////////Start system functions
///////
///////
///////
///////
///////


void Subsystem::resetSubsystem(){
   taskState = 0;
    setColor(CRGB::Black);
        resetTimers();

}

void Subsystem::resetTimers(){
  forloopthingy = 0;
   chaseround = 0;
   timer = 0;
   impartialTimeKeeper = 0;
   impartialForLoopKeeper = 0;
   done = false;
   miscCheck = false;
   
}

void Subsystem::override(){
  
}
//
//
//
//
//
//
//
//
void Subsystem::updateTask(){        //This function only runs when a task is completed.
  
  
 if(isSerialBased){
   serialValue = Serial.read();
  
       if(_updateChooser == 1){
    switch(serialValue){
      
     
        case 'R':
       taskState = 1;
       resetTimers();
       break;
       
       
       
       default:
       break;
       
      
     }
       }else{
         switch(serialValue){
      
      case 'R':
       taskState = 2;
resetTimers();
       break;
       
       
       default:
       break;
       }
 
 
}
}else{                //If the subsystem is not serial based

if(completelyDone){    //If all tasks are done startover. 10,000 is a random number
 taskState = 0; 
}
taskState++; //Move onto the next task for the system


  
}
}
////
///
///
///
//
///

///


///

///

///
void Subsystem::runSystem(){
  
  if(isSerialBased){
    override();
    
  }
  if(done){
    resetTimers();
   updateTask(); 
  }
  
  if(serialBased){
  switch(taskState){
   case 1:
                       //run light function here
   break;
 
   default:
   break; 
  }
  }else{
   switch(taskState){
    case 1:
    
    break;
    
    default:
    break;
   } 
  }
  
  
  FastLED.show();      //Refreshes the lights of the subsystem to show their new values.
  
  
}
///////
///////
///////

int Subsystem::getDelay(){
 return systemDelayTime;
}

int Subsystem::getTaskState(){
 return taskState; 
}
///////
///////
//////////////Start light display basics
///////
///////
///////
///////
///////

void Subsystem::setColor(CRGB color){
  for(timer = 0; timer< actualNumberOfLeds; timer++){
   leds[timer] = color; 
  }
  FastLED.show();
  done = true;
}

 
  void Subsystem::setColorFrom(CRGB color, int low, int high){
    for(timer = low; timer<high; timer++){
     leds[timer] = color; 
    }
    
    FastLED.show();
    done = true;
  }
  
  
  void Subsystem::setColorUp(CRGB color){
    
    if(timer <=actualNumberOfLeds){
     leds[timer] = color;
    timer++; 
    }else{
     done = true; 
    }
  }
  
  
  void Subsystem::setColorDown(CRGB color){
    if(!miscCheck){
     miscCheck = true;
    timer = actualNumberOfLeds; 
    }
    if(timer >= 0){
     leds[timer] = color;
    timer--; 
    }else{
      done = true;
    }
  }
  
  
  void Subsystem::moveUp(){
    
   for(timer= actualNumberOfLeds; timer >= 0; timer--){
     if(timer =! actualNumberOfLeds){
      leds[timer+1] = leds[timer];
       leds[timer] = CRGB::Black; 
     }
     
   }
  FastLED.show();
 done = true; 
  }
  
  
  void Subsystem::moveDown(){
    
    for(timer = 0; timer<actualNumberOfLeds; timer++){
     if(timer=! 0){
      leds[timer-1] = leds[timer];
      leds[timer] = CRGB::Black;
     } 
    }
   FastLED.show();
   done = true;
  }
  
  
  void Subsystem::moveUpOnly(int low, int high){
    for(timer = high; timer>low; timer--){
      if(timer=!actualNumberOfLeds){
     leds[timer+1] = leds[timer];
    leds[timer] = CRGB::Black; 
      }
    }
    FastLED.show();
    done = true;
  }
  
  
  void Subsystem::moveDownOnly(int low, int high){
    for(timer = low; timer<high; timer++){
      if(timer=!0){
     leds[timer-1] = leds[timer];
      leds[timer] = CRGB::Black; 
      }
    }
    FastLED.show();
    done = true;
  }
  
 
  
  



///////
///////
///////
///////
///////



Subsystem exampleSubsystem(NUM_LEDS_1,1,30,false);


/////
////

 ////
 ///
 ///
 ///
 ///
 



void setup(){
pinMode(DATA_PIN_1,OUTPUT);



 FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(exampleSubsystem.leds,NUM_LEDS_1);
 Serial.begin(9600);
 


exampleSubsystem.resetSubsystem(); //Clears everything

}



  
  
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
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/

void loop(){
  
   exampleSubsystem.runSystem();
  FastLED.show();
  if(exampleSubsystem.getTaskState() > 0){   //Only delay if we have lights to show. No reason to delay if there is nothing there.
   
     delay(exampleSubsystem.getDelay()); //Delay extra if running an override as most overrides shouldn't update fast.
    
}

 
  }
  

 
