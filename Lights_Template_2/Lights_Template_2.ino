//Fix sync... probably accept the input straight from the second subsystem.
//Scratch that thats impossible. Cannot refernce systems that don't yet exist in system commands
//Need CRGB array middeground floating somewhere
//Need to get synced back to working.
//Need to run sync everytime assuming subsystem is synced
//Make sure subsystems run in order so that the right things override each other
//need to make sure things desync properly


//DO NOT USE Port 13. Really. There are plenty of other ports. Don't do it.
//Code designed to run WS2811 LEDS and NEOPIXELs
//This runs on a custom built multitasker for light strips. It is designed
//to handle numerous sets of lights at the same time.
//Each of these sets of lights gets its own subsystem class
//Due to things the multitasker works on each function only doing one thing
//and then stopping.
#include <FastLED.h>  //Library we use because options. There are other things out there like the official neopixel library which are much easier to use. However they are overall much more limited and don't allow fancy things to be done. Should a time ever come where there is not enough time or people interested in learning lights to a serious degree just use one of these libraries.
#define NUM_LEDS_2 39
#define NUM_LEDS_1 32 //Number of Leds in strip 1
#define DATA_PIN_1 2  //Says which data pin to use for LED  strip 1
#define DATA_PIN_2 3

CRGB lame = CRGB(20,20,20);
CRGB blaple = CRGB(76,69,88); 
CRGB bluish = CRGB(10,50,250);
CRGB orangeFruit = CRGB(254,40,40);
int RNGTimer = 0;
CRGB randomColor(){      //Custom color option that gives a seemingly random color.
  switch(RNGTimer%6){
   case 0:
   return CRGB::Blue;
  break;
 
   case 1:
   return CRGB::Green;
   break;
  
  case 2:
  return CRGB::Red;
   break;
  
  case 3:
  return CRGB::White;
   break;
  
  case 4:
  return CRGB::Purple;
    break;
    
  case 5:
  return CRGB::Yellow;
    break;
  
  case 6:
    return CRGB::Orange;
    break;
    
    default:
    return CRGB::Black; //This should never run.
    break;
  
    
  }
  
}

class Subsystem  //Class that allows for multitasking lights. Made to be flexible.
{
  public:
  Subsystem(int numberOfLeds,int updateChooser, int serialLessUpdateChooser, int delayTime, bool serialBased, bool synced);
 
  
  
  void resetSubsystem(); //Wipes everything in the system to the starting settings.
  void resetTimers();//Wipes only the timers in the system.
  void runSystem(int runTimes, bool autoDelay);//Runs the system once.
  void updateTask();//Updates the systems task when a system is done running.
  int serialChangeTask(int currentTaskState, int changeTo);
  
  int getDelay();//Returns how much delay the subsystem should run.
  int getTaskState();//Returns the current task state of the subsystem.
  bool colorMatch(CRGB color, int low, int high);//Checks if the array is near a color.

  
  
  CRGB leds[64];       //Light Array. Set to 64 due to some oddities.
    short taskState = 0;
    int cycleSpeed = 25;      //Beta Overhaul
    short runProgress = 0;
    
    int returnLEDAmount();
    void desynced();
    
          bool returnSyncStatus();
          void synced();
          void sync(CRGB middleGround[], bool from);
          
     short syncTimer = 0;
      bool syncMiscCheck = false;


    
    
  private:
  
  void setColor(CRGB color);  //Sets the entire light strip to one color
  void invertColor();
  void setColorFrom(CRGB color, int low, int high); //Sets the a part of a light strip to one color
  void setColorUp(CRGB color, int low, int high); //Sets the light strip to one color going up from the bottom. Occurs over several system.run's
  void setColorDown(CRGB color, int low, int high);//Sets the light strip to one color going down from the top . Occurs over several system.runs's
  void moveUp(int moveTimes, int color); // Moves every LED bulb color up one.
  void moveDown(int moveTimes, int color); //Moves every LED bulb color down one.
  void moveUpOnly(int low, int high, int runTimes, int cycleSpeed, CRGB color, bool rain); //Moves up the LED bulb colors from a range up.
  void moveDownOnly(int low, int high,int runTimes, int cycleSpeed, CRGB color, bool rain); //Moves up the LED bulb colors from a ranage down.
  void alternateColor(CRGB color1, CRGB color2);// Makes the lights alternate between two colors
  void rippleCRGB(CRGB color, int rippleRate, int positionStart, int duration);//Makes the LEDs ripple from one LED to all others while also lowering each RGB value from one LED to the next.
  void rippleCHSV(CHSV color, int rippleRate, int positionStart, int duration, int HSV);//Makes the LEDs ripple from one LED to all others while also lowering each HSV of choice value from one LED to the next. (0=Hue, 1=Saturation, 2=value)
  void redFade(int runTimes);   //Adds red hue to all of a light strip
  void blueFade(int runTimes);  //Adds blue hue to all of a light strip
  void greenFade(int runTimes);  //Adds green hue to all of a light strip
  void brighten(CRGB color, int amount, int low, int high);
  void sparkle(CRGB color, int runTimes,bool replaceing);
  void doNothingFor(int stallTime);
  int tempRand(int randomNumber);
  
  bool isSerialBased;      //Boolean used to tell if the subsystem is serial based.
  bool setToTop = false;    //Used for some functions.
  bool done = false;  //Tells the subsystem whether or not to update the task. If it is done the task is updated.
  bool completelyDone = false;//Tells the subsystem if it is completey finished going thorugh evey step
  bool miscCheck = false;    //Random check used for various functions.
  int _updateChooser;        //SERIAL BASED: Used to distinguish between various serial based systems.
  int _serialLessUpdateChooser;
  int actualNumberOfLeds;    //Variable used to store how many LEDS are used in the system. Only needed due to oddities with the LED array and subsystem classes.
  int chaseround = 0;  //Variables
  int systemDelayTime = 0;//used to save the delay for the system
  int forloopthingy = 0;//used as a timer for strange for loop things
  int functionStep = 0;//used to keep track of where strange functions are
  int newcolorR = 0;//Sets new color for R in the function ripple equal to 0
  int newcolorG = 0;//Sets new color for G in the function ripple equal to 0
  int newcolorB = 0;//Sets new color for B in the function ripple equal to 0
  
  int timer = 0;
  int runTimer = 0;
  int miscTimer = 0;
  int ascentTimer = 1;
  int colorTimer = 0;
  int impartialTimeKeeper = 0;
  int impartialForLoopKeeper = 0;
  int cascadeProgress = 0;
  int temporaryRandomVariable = -1;
    
  int syncPoint = 0;
  bool syncedUp = false;
  bool syncedForNextStep =false;
  bool test = false;

  
  String serialValue;
  
  
  
  
};
  //Saves all of the subsystem inputs to more permenant variables for future uses.
 Subsystem::Subsystem(int numberOfLeds, int updateChooser, int serialLessUpdateChooser, int delayTime, bool serialBased, bool synced){
   
   actualNumberOfLeds = numberOfLeds;
   _updateChooser = updateChooser;
   _serialLessUpdateChooser = serialLessUpdateChooser;
   isSerialBased = serialBased;
   systemDelayTime = delayTime;
   syncedUp =synced;
   
   
  
}

/*void Subsystem::sync(CRGB middleGround[], bool from){
    if(!miscCheck){      //To avoid setting nulls = to nulls and crashing. boom.
      for(syncTimer = 0; syncTimer< actualNumberOfLeds; syncTimer++){
       middleGround[syncTimer] = CRGB::Black;
             leds[syncTimer] = CRGB::Black; 
 
      }
      miscCheck = true;
  }
  if(from){
  for(syncTimer = 0; syncTimer < actualNumberOfLeds; syncTimer++){
    leds[syncTimer] = middleGround[syncTimer];
    
  }
  }
  
  if(!from){
    for(syncTimer = 0; syncTimer < actualNumberOfLeds; syncTimer++){
    middleGround[syncTimer] = leds[syncTimer];
    
  }
  }
  
}
*/


int Subsystem::returnLEDAmount(){
 return actualNumberOfLeds; 
}



void Subsystem::desynced(){        //Say whether or not the system is synced with another system
 syncedForNextStep = false; 
}

void Subsystem::synced(){
 if(done){
  syncedForNextStep = true;
 } 
}



bool Subsystem::returnSyncStatus(){
 return syncedForNextStep; 
}

    int Subsystem::serialChangeTask(int currentTaskState, int changeTo){
     if(currentTaskState != changeTo){
      resetTimers();
     } 
       return changeTo;
    }
    
void Subsystem::doNothingFor(int stallTime){
     if(timer == stallTime){
      done = true;
      
     }else{
      timer++;
     } 
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
bool Subsystem::colorMatch(CRGB color,int low, int high){
   for(colorTimer = low; colorTimer < high; colorTimer++){
    if(leds[colorTimer].r == color.r || abs(leds[colorTimer].r-color.r) <= 10){
      if(leds[colorTimer].g == color.g || abs(leds[colorTimer].g-color.g) <= 10){
      if(leds[colorTimer].b == color.b || abs(leds[colorTimer].b-color.b) <= 10){
       
    }else{
      return false;
    }
    }else{
      return false;
    }
    }else{
      return false; 
    }
   }
 return true; 
}

void Subsystem::resetSubsystem(){
  cycleSpeed =1; 
  taskState = 0;                    //Just wipe the daylights out of everything
        resetTimers();
        completelyDone = false;

}

void Subsystem::resetTimers(){
  forloopthingy = 0;
   chaseround = 0;
   timer = 0;
   miscTimer = 0;   //When you want to get rid of all the timer values
   runTimer = 0;
   impartialTimeKeeper = 0;
   impartialForLoopKeeper = 0;
   temporaryRandomVariable = -1;
   done = false;
   miscCheck = false;
   
}


int Subsystem::tempRand(int randomNumber){
  if(temporaryRandomVariable == -1){
   temporaryRandomVariable = randomNumber; 
  }
  return temporaryRandomVariable;
}

//
//
//
//
//
//
//
//
void Subsystem::updateTask(){        //This function only runs when a task is completed. The logic is in other parts of code (Will reference sometime)
  
  
 if(isSerialBased){                //If the subsystem runs off of serial input
   if(Serial.available() >0){ 
        serialValue = Serial.readString(); 
      Serial.print(" Recieved: ");
      Serial.print(serialValue);  
   }
       if(_updateChooser == 1){
         //Serial.print(taskState);    //Determine what set of steps to run
       if(serialValue.startsWith("Chase")){
          taskState = serialChangeTask(taskState,1);
          Serial.print(" Running One...");
          Serial.print(runTimer);
          Serial.print("\n");
          test = true;
       } 
       if(serialValue.startsWith("Blind")){
          taskState = serialChangeTask(taskState,2);
          Serial.print(" Running Two...");
          Serial.print(runTimer);
          Serial.print("\n");
          
          test = true;
       } 
       if(serialValue.startsWith("Solid")){
          Serial.print(" Running Three...");
          Serial.print(runTimer);
          Serial.print("\n");
          taskState = serialChangeTask(taskState,3);
          
          if(isDigit(serialValue.charAt(6))){
           cycleSpeed =  serialValue.substring(6).toInt() + 8;
          }
          test = true;
         }
         if(!test){
        
         
         taskState = serialChangeTask(taskState,100); //Blow past all setting to trigger default.
        
       }
       test = false;
       //Serial.print(test);
       
      
       //Serial.print(" Task State Successfully Set to Default...\n");

         
       
       
   
       }else{
         taskState = 0;
       }
 
 
}else{                //If the subsystem is not serial based
                        //Consider serialess things run dumb they have no reason to break off. They just go up and down the steps.
if(completelyDone){    //If all tasks are done startover. 
 
 taskState = 0; //Anything below zero is one time start up stuff.
  resetSubsystem();
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
void Subsystem::runSystem(int runTimes, bool autoDelay){    //Basic function used to run the subsystem
  for(miscTimer = 0; miscTimer < runTimes; miscTimer++){    //Only important of you run a subsystem many times. Used generally for startup and weird things
  if(runProgress == cycleSpeed){ 
    runProgress = 0;
    
  if(isSerialBased){
      if(done){
       serialValue = "Done";
      taskState = 100;
      resetTimers();
      }    //if the function runs on serial make sure no emergency inputs are coming
    updateTask();
  }
  if(done && !isSerialBased){
    
    resetTimers();    //When the subsystem is done with its last function prepare timers for future use and get the next task
   updateTask(); 
  }
  
  if(isSerialBased){   //Here is where serial actually runs
  switch(taskState){
   case 1:
   cycleSpeed = 2;
   moveDownOnly(0,actualNumberOfLeds,500,5,CRGB(CHSV(random8(245),255,random8(245))),true);
   //CRGB(CHSV(random8(245),255,random8(245)))
   break;
 
 
   case 2:
   cycleSpeed = 2;
   moveUpOnly(0,actualNumberOfLeds,500,5,CRGB::Green,true);
   break;
   
   case 3:
   //Cycle speed is set in the function.
   moveDownOnly(0,actualNumberOfLeds,4,6,CRGB::White,true);
   


   break;
   
   default:
   cycleSpeed = 1;
   moveDownOnly(0,actualNumberOfLeds,1,5,CRGB(0,0,random16(150)),true);

   break; 
  }
  }else{
    if(_serialLessUpdateChooser ==1){    //Update choosing point for non-serial functions
   switch(taskState){
      case 0:
      cycleSpeed = 2;
         //rippleCRGB(CRGB::Red,25,random8(31),3);//Makes the LEDs ripple from one LED to all others while also lowering each RGB value from one LED to the next.

      sparkle(CRGB(0,random16(255),0),tempRand(random16(800)),false);
      //CRGB(CHSV(random8(245),255,random8(245)))
break;
       
     case 1:    
     cycleSpeed = 1;
         rippleCRGB(CRGB::Blue,30,tempRand(random8(31)),2);//Makes the LEDs ripple from one LED to all others while also lowering each RGB value from one LED to the next.
      break;
      
      case 2:
      cycleSpeed = 4;
       doNothingFor(tempRand(random8(30)));
break;
    
    case 3:
done = true;
completelyDone = true;
               //brighten(CRGB::Green,1,0,actualNumberOfLeds);

    
      
    break;
    
    case 4:
              //brighten(CRGB::Black,1,0,actualNumberOfLeds);
              done = true;
    completelyDone = true;

          
                              //Sync 1

    
    
    break;
    
    case 5:
    done = true;
    completelyDone = true;
                //Sync 2
    break;
   
    
    default:
    break;
   }
    } 
    if(_serialLessUpdateChooser==2){  //second set of commands to run in the event you want two serialess subsystems
      switch(taskState){
    
    case 0:
    cycleSpeed =1;
    //cycleSpeed = 150;
    //setColorFrom(CRGB::Red,actualNumberOfLeds-1,actualNumberOfLeds);
    setColor(CRGB::Yellow);
    break;
     
    case 1:
    setColorUp(CRGB::Yellow,0,actualNumberOfLeds);
         //cycleSpeed = 1;
    //setColorFrom(CRGB::White,4,5);
    //setColorFrom(CRGB::Red,3,4);
    //setColorFrom(CRGB(210,0,0),2,3);
    //setColorFrom(CRGB(100,0,0),1,2);
     //setColorFrom(CRGB(10,0,0),0,1);
    

    break;
     
    case 2:
    setColor(CRGB::White);
    
    
    
    //cycleSpeed = 15;
      //  moveUp(15,CRGB::Black);


    break;
    
    case 3:
     brighten(CRGB::Black,3,0,actualNumberOfLeds);

      //cycleSpeed = 15;
      //brighten(CRGB::Black,5,0,actualNumberOfLeds);

    break;
    
    case 4:
        done = true;
    completelyDone = true;
      //cycleSpeed = 19;
        //    rippleCHSV(CHSV(89,155,199),5,19,7, 0);


    
    break;
    
    case 5:
        //cycleSpeed = 20;
        //brighten(CRGB::Black,1,0,actualNumberOfLeds);

   
    break;
    
    case 6:  
       //setColor(CRGB::Black);
 
     
    break;
    
    case 7:
       // setColorFrom(CRGB(10,10,10),actualNumberOfLeds-2,actualNumberOfLeds);
    
    break;
    
    case 8:
    //cycleSpeed = 65;
      //  moveDownOnly(0,actualNumberOfLeds-2,5000,9,CRGB(CHSV(random8(245),255,random8(245))),true);
        
       // moveDown(25,CRGB::Blue);
    
    break;
    
    
    
    
    
    default:
    break;
   }
    }
    if(_serialLessUpdateChooser==3){
     switch(taskState){
       case 0:
       cycleSpeed = 1;
        setColor(CRGB::White);
        break;
        
      case 1:
      brighten(CRGB(0,0,20),1,0,actualNumberOfLeds);
break;
      
      case 2:
       done = true;
       completelyDone = true;
             
              //setColorUp(bluish,0,actualNumberOfLeds);

      break;
      
      case 3:        
              setColorUp(orangeFruit,0,actualNumberOfLeds);


    break;
    
    case 4:
               setColorUp(CRGB::Green,0,actualNumberOfLeds);



        
    break;
    
    case 5:
                  setColorUp(CRGB::Purple,0,actualNumberOfLeds);

    break;
    
    case 6:
                   brighten(CRGB::Black,1,0,actualNumberOfLeds);   
    break;
    
    case 7:
    done = true;
    completelyDone = true;
    break;
    
   default:
   break;
    
     } 
    }
  }
   
    
    if(autoDelay){
     delay(systemDelayTime); 
    }
    FastLED.show();      //Refreshes the lights of the subsystem to show their new values.

  }
  

runProgress++;
  }
}
///////
///////
///////

int Subsystem::getDelay(){
 return systemDelayTime;     //Function used to get delay of subsystem (no more obvious comments)
}

int Subsystem::getTaskState(){
 return taskState; 
}
///////
///////
//////////////Start light display basic functions
///////
///////
///////
///////
///////

void Subsystem::setColor(CRGB color){
  for(timer = 0; timer< actualNumberOfLeds; timer++){    //These functions names are fairly explanatory. Logic is....slightly less though
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
  
  
  void Subsystem::setColorUp(CRGB color, int low, int high){      //Sets color going from bot to top
    if(!miscCheck){
     timer = low;
    miscCheck = true; 
    }
    if(timer <=high){
     leds[timer] = color;
    timer++; 
    }else{
     done = true; 
    }
  }
  
  
  void Subsystem::setColorDown(CRGB color, int low, int high){    //Sets color going for top to bot
    if(!miscCheck){
     miscCheck = true;                          //Due to the nature of resetTimers() for functions that start at the top I need this one time run thingamajig to set the top as where to start and not zero
    timer = high; 
    }
    if(timer >= low){
     leds[timer] = color;
    timer--; 
    }else{
      done = true;
    }
  }
  
  
void Subsystem::moveUp(int moveTimes, int color){
    if(moveTimes > runTimer){
   for(timer = actualNumberOfLeds-1; timer >= 0; timer--){
     /*
     if(timer =! actualNumberOfLeds){
      leds[timer+1] = leds[timer];
       leds[timer] = CRGB::Black; 
     }*/
     leds[timer+1] = leds[timer];
     leds[timer] = color;
     
   }
  FastLED.show();
    runTimer++;
    }else{
          done = true; 
 
    }
  }
  
  void Subsystem::moveDown(int moveTimes,int color){
     if(moveTimes > runTimer){
    for(timer = 0; timer<actualNumberOfLeds; timer++){
     
      leds[timer-1] = leds[timer];
      leds[timer] = color;
      
    }
   FastLED.show();
     runTimer++;
     }else{
   done = true;
   
     }
  }
  
  
  /*void Subsystem::moveUpOnly(int low, int high){
    for(timer = high; timer>low; timer--){
      if(timer=!actualNumberOfLeds){
     leds[timer+1] = leds[timer];
    leds[timer] = CRGB::Black; 
      }
    }
    FastLED.show();
    done = true;
  }*/
  
  
  void Subsystem::moveUpOnly(int low, int high, int runTimes, int cycleSpeed, CRGB color, bool rain){
    if(runTimes > runTimer){
    for(timer = high; timer>low; timer--){
      
     leds[timer+1] = leds[timer];
     if(rain){
     if(random8(50) < 15){
             leds[timer] = color; 
     }else{
            leds[timer] = CRGB::Black; 
     }
     }else{
            leds[timer] = CRGB::Black; 
     }
     
      
    }
    
    runTimer++;
    
    }else{
     FastLED.show();
    done = true; 
    }
  }
  
  void Subsystem::moveDownOnly(int low, int high, int runTimes, int cycleSpeed, CRGB color, bool rain){
    if(runTimes > runTimer){
    for(timer = low; timer<high; timer++){
      
     leds[timer-1] = leds[timer];
     if(rain){
     if(random8(50) < 15){
             leds[timer] = color; 
     }else{
            leds[timer] = CRGB::Black; 
     }
     }else{
            leds[timer] = CRGB::Black; 
     }
     
      
    }
    
    runTimer++;
    
    }else{
     FastLED.show();
    done = true; 
    }
  }
  
  void Subsystem::alternateColor(CRGB color1,CRGB color2){
    for(timer = 0; timer<actualNumberOfLeds; timer++){
      if(timer%2==0){
       leds[timer] = color1; 
      }else{
        leds[timer] = color2;
      }
    }
    FastLED.show();
    done = true;
  }
  
 void Subsystem::rippleCRGB(CRGB color, int rippleRate, int positionStart, int duration){       //Definition for rippleCRGB command that takes in a CRGB color, an int for that rate at which the RGB values change, an int for which LED to start the rippleCRGB at, and an int for how many other LEDS(1 = 1 up and 1 down) the affect goes to.  It also sets the default incase no value is put in. 
    if(timer<=duration){                                             //Checks to see if the timer is still lower than the duration time passed.
      leds[positionStart] = color;                                    //Sets the color of the position put in to the color that was put in.      
      newcolorR = leds[positionStart+timer-1].r - rippleRate;         //Sets newcolorR equal to the r value of the LED before it minus the ripple rate of change passed.
      newcolorG = leds[positionStart+timer-1].g - rippleRate;         //Sets newcolorG equal to the G value of the LED before it minus the ripple rate of change passed.
      newcolorB = leds[positionStart+timer-1].b - rippleRate;         //Sets newcolorB equal to the B value of the LED before it minus the ripple rate of change passed.
      if(newcolorR<=0 or newcolorR>255){                             //Ifs used to make sure that the RGB values stay within 0 and 255 so some form of a color is displayed
        newcolorR = leds[positionStart+timer-1].r + rippleRate;       //And if the value goes outside this range, the RGB value gets reset to the value of the LED before it 
      }
      if(newcolorG<=0 or newcolorG>255){
        newcolorG = leds[positionStart+timer-1].g + rippleRate;       
      }                                                               
      if(newcolorB<=0 or newcolorB>255){                             
        newcolorB = leds[positionStart+timer-1].b + rippleRate;       
      }
                                                                     
      leds[positionStart+timer].r = newcolorR;                        //Sets the next set of LEDs (one above, one below) to the newcolor for its respective RGB values 
      leds[positionStart+timer].g = newcolorG;
      leds[positionStart+timer].b = newcolorB;
      leds[positionStart-timer].r = newcolorR;
      leds[positionStart-timer].g = newcolorG;
      leds[positionStart-timer].b = newcolorB;  
      timer++;                                                        //Increments the timer by one
    }
    else{                                                             //If the duration is done, then done is set to true ending the cycle. 
      done = true; 
    }
  
 }
  
  

 void Subsystem::rippleCHSV(CHSV color, int rippleRate, int positionStart, int duration, int HSV){
    if(timer<=duration){                                             //Checks to see if the timer is still lower than the duration time passed.
      leds[positionStart] = color;                                    //Sets the color of the position put in to the color that was put in.
      if(HSV==0){
         leds[positionStart+timer] = CHSV(color.hue - timer*rippleRate, color.sat, color.val);          //Sets the next set of LEDs (one above, one below) to the newcolor for its respective HSV values
         leds[positionStart-timer] = CHSV(color.hue - timer*rippleRate, color.sat, color.val);      
      }
      if(HSV==1){
         leds[positionStart+timer] = CHSV(color.hue, color.sat - timer*rippleRate, color.val);          //Sets the next set of LEDs (one above, one below) to the newcolor for its respective HSV values
         leds[positionStart-timer] = CHSV(color.hue, color.sat - timer*rippleRate, color.val);      
      }      
      if(HSV==2){
         leds[positionStart+timer] = CHSV(color.hue, color.sat, color.val - timer*rippleRate);          //Sets the next set of LEDs (one above, one below) to the newcolor for its respective HSV values
         leds[positionStart-timer] = CHSV(color.hue, color.sat, color.val - timer*rippleRate);      
      } 
      timer++;                                                        //Increments the timer by one
    }
    else{                                                             //If the duration is done, then done is set to true ending the cycle. 
      done = true; 
    }
  
 }

void Subsystem::redFade(int runTimes){
    if(timer< runTimes){
      for(miscTimer = 0; miscTimer < actualNumberOfLeds; miscTimer++){
       leds[miscTimer].r ++; 
      }
      timer++;
    }else{
     done = true; 
    }
  }
  
  void Subsystem::blueFade(int runTimes){
    if(timer< runTimes){
      for(miscTimer = 0; miscTimer < actualNumberOfLeds; miscTimer++){
       leds[miscTimer].b ++; 
      }
      timer++;
    }else{
     done = true; 
    }
  }
  
  void Subsystem::greenFade(int runTimes){
    if(timer< runTimes){
      for(miscTimer = 0; miscTimer < actualNumberOfLeds; miscTimer++){
       leds[miscTimer].g ++; 
      }
      timer++;
    }else{
     done = true; 
    }
  }
  
  void Subsystem::brighten(CRGB color, int amount, int low, int high){
    if(!colorMatch(color, low, high)){
      for(timer = low; timer < high; timer++){
        
        if(leds[timer].r != color.r){ 
      if(leds[timer].r < color.r){
        leds[timer].r += amount;
       }else{
        leds[timer].r -= amount;
       } 
        }
        
        if(leds[timer].g != color.g){
       if(leds[timer].g <= color.g){
        leds[timer].g += amount;
       }else{
        leds[timer].g -= amount;
       }
        }
      
      if(leds[timer].b != color.b){
      if(leds[timer].b <= color.b){
        leds[timer].b += amount;
       }else{
        leds[timer].b -= amount;
       } 
      }
      
      }
    }else{
     done = true; //WEEEE MADE ITTTTT
    }
  }

  void Subsystem::invertColor(){
   for(timer = 0; timer < actualNumberOfLeds; timer++){
    leds[timer] = -leds[timer]; 
   }
   done = true;
  }
  
  void Subsystem::sparkle(CRGB color,int runTimes, bool replaceing){
   if(runTimes > runTimer){
     if(replaceing){
       for(timer =0; timer< actualNumberOfLeds; timer++){
      leds[timer]= CRGB::Black;
       }
     }
    leds[random8(actualNumberOfLeds)] = color;
     
     runTimer++;
   }else{
    done = true;
   } 
  }
////
  ///
  ////
  ////
  ////
  ////
  ////
  ////
  //
  ////
  ////
  ////
  ////
  //
  ////
  ////
  ////
  ////
  
  class Synchronizer        //For real. Really.
  {
   public:
   Synchronizer(int ledAmount);
    CRGB leds[64];        //The middleground array...that sorceries
    void runSystem(CRGB lightArray1[], bool from);
   // void sync(Subsystem system1, Subsystem system2);
    bool miscCheck = false;
    int syncTimer = 0;
    
    private:
    int numberOfLeds;
  };
  
  Synchronizer::Synchronizer(int ledAmount){
    numberOfLeds = ledAmount;
  }
  
 void Synchronizer::runSystem(CRGB lightArray1[],bool from){
   if(!miscCheck){      //To avoid setting nulls = to nulls and crashing. boom.
      for(syncTimer = 0; syncTimer< numberOfLeds; syncTimer++){
       leds[syncTimer] = CRGB::Black;
             lightArray1[syncTimer] = CRGB::Black; 
 
      }
      miscCheck = true;
  }
  
  
  if(from){
  for(syncTimer = 0; syncTimer < numberOfLeds; syncTimer++){
    lightArray1[syncTimer] = leds[syncTimer];
    
  }
  }
  
  if(!from){
    for(syncTimer = 0; syncTimer < numberOfLeds; syncTimer++){
    leds[syncTimer] = lightArray1[syncTimer];
    
  }
  }
   
 }
 /*void Synchronizer::sync(Subsystem system1, Subsystem system2){
   system1.runSystem(1,false);    //Sync to mid
    runSystem(system1.leds,false);
    runSystem(system2.leds,true);
   system2.runSystem(1, false);    //Sync to mid
    runSystem(system2.leds,false);
    runSystem(system1.leds,true);
    

 }
 */
  
/*class Synchronizer
{
  public:
  Synchronizer(Subsystem system1, Subsystem system2);
  void execute();

  private:
  Subsystem* primarySystem;
  Subsystem* secondarySystem;
  
  void sync(Subsystem* sync1, Subsystem* sync2);
  void resetTimers();
  
  
  int syncTimer = 0;
  int miscCheck = false;
  
};

Synchronizer::Synchronizer(Subsystem system1, Subsystem system2){
  
  primarySystem = &system1;
  secondarySystem = &system2;
  
}

void Synchronizer::execute(){

  sync(primarySystem, secondarySystem);  //Set secondary equal to primary
    
     if(!(secondarySystem->returnSyncStatus())){ 

   secondarySystem->runSystem(1,false);   //Edit lights
   secondarySystem->upSync();
     }
     
   sync(secondarySystem, primarySystem); //Set primary back to modified
   
   if(!(primarySystem->returnSyncStatus())){
     primarySystem->runSystem(1,false);     //Edit lights
   primarySystem->upSync();
   }
   //primarySystem->runSystem(1,false);
   
   if((secondarySystem->returnSyncStatus())&&(primarySystem->returnSyncStatus())){
     secondarySystem->desynced();
     primarySystem->desynced();
   }
}



void Synchronizer::resetTimers(){
  syncTimer = 0;
}
*/


///////
///////
///////
///////
///////
//
  ////
  ////
  ////
  ////
  //
  ////
  ////
  ////
  ////


Subsystem lightSystem1(NUM_LEDS_1,1,1,1,false,true);      //Non-Serial
Subsystem lightSystem2(NUM_LEDS_2,1,2,5,true,true);      //Serial Based
Subsystem syncSystem1(NUM_LEDS_1,1,3,1,false,true);      //Bottom Secondary System
//Subsystem syncSystem2(NUM_LEDS_2,1,2,1,false,true);

Synchronizer lightSet1(NUM_LEDS_1);
//Synchronizer lightSet2(NUM_LEDS_2);



/////
////

 ////
 ///
 ///
 ///
 ///
 



void setup(){
pinMode(DATA_PIN_1,OUTPUT);

  FastLED.addLeds<WS2811, DATA_PIN_1, GRB>(lightSet1.leds,NUM_LEDS_1);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(lightSystem2.leds,NUM_LEDS_2);
 Serial.begin(57600);
 Serial.setTimeout(40);
 
  //lightSystem1.setColor(CRGB::Green); 
  //lightSystem1.taskState = -5;
  //lightSystem1.runSystem(5, true);
  
  

 

 


/*lightSystem1.resetSubsystem(); //Clears everything
lightSystem2.resetSubsystem();
lightSystem1.setColor(CRGB::Black);  //^^
lightSystem2.setColor(CRGB::Black);  //Yey....this should be part of ^^
*/
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
    RNGTimer++;
    lightSystem1.runSystem(1,false);
    lightSet1.runSystem(lightSystem1.leds,false);
    lightSet1.runSystem(syncSystem1.leds,true);
   syncSystem1.runSystem(1, false);
    lightSet1.runSystem(syncSystem1.leds,false);
    lightSet1.runSystem(lightSystem1.leds,true);
    
    lightSystem2.runSystem(1,false);
    //lightSet2.runSystem(lightSystem2.leds,false);
    //lightSet2.runSystem(syncSystem2.leds,true);
    //syncSystem2.runSystem(1,false);
    //lightSet2.runSystem(syncSystem2.leds,false);
    //lightSet2.runSystem(lightSystem2.leds,true);

     
   FastLED.show();
  if(lightSystem1.returnSyncStatus() && syncSystem1.returnSyncStatus()){
     lightSystem1.desynced();
    syncSystem1.desynced(); 
  }
  
  if(lightSystem1.getTaskState() > 0 or lightSystem2.getTaskState() > -100 or syncSystem1.getTaskState() > 0){   //Only delay if we have lights to show. No reason to delay if there is nothing there.
   
     delay((lightSystem1.getDelay() + lightSystem2.getDelay())/2); //Delay extra if running an override as most overrides shouldn't update fast.
    
}

  

 
  }
  

 
