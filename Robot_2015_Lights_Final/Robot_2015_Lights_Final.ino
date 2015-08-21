//DO NOT USE Port 13
//Code designed to run WS2811 LEDS and NEOPIXELs
//Attempts to multitask ceratin functions by breaking for loops and reentering them
//Based on non-for loop timers. This should cause both to update their assigend
// light strip once and then go and update the next strip.
#include <FastLED.h>


#define NUM_LEDS 32
#define NUM_LEDS_LIFTER 39

#define NOTZEROED 'Z'
#define TEAMCOLORRED 'E'
#define TEAMCOLORBLUE 'B'
#define TEAMCOLORUNKNOWN 'U'
#define DRIVEFOWARD '3'
#define DRIVEBACK '4'
#define DRIVEIDLE '5'
#define LIFTERUP '6'
#define LIFTERDOWN '7'
#define LIFTERNEUTRAL '8'
#define PUSHERFORWARD '9'
#define PUSHERBACK '10'
#define PUSHERNEUTRAL '11'


#define DATA_PIN 2 //Using data pin one for primary light strip pin timer. Once again highly subject to change
#define DATA_PIN_LIFTER 3
//#define DATA_PIN_INPUT 3
//#define DATA_PIN_INPUT2 4
#define DELAY 50 //sets the delay between light updates to 15 milliseconds


  //CRGB leds[NUM_LEDS];
//CRGB ledsLifter[NUM_LEDS_LIFTER];
  
  int forloopthingy = 0;
  int dynamicDelay = 0;
  int testvariable1 = 0;
  int testvariable2 = 0;
  int forloopthingy1 = 0;
  int forloopthingy2 = 0;
  int forloopthingy3 = 0;
  bool chaseround1 = 0;
  bool chaseround2 = 0;
  bool chaseround3 = 0;
  int timer = 0;                        //Tons of outdated variables.
  int timer1 = 0;                       //To be deleted.
  int timer2 = 0;                      //Only still here so old functions don't throw errors
  int timer3 = 0;
  int impartialTimeKeeper1 = 0;
  int impartialTimeKeeper2 = 0;
  int impartialTimeKeeper3 = 0;
  int impartialForLoopKeeper1 = 0;
  int impartialForLoopKeeper2 = 0;
  int impartialForLoopKeeper3 = 0;
  //int lifterTaskState = 0;
  //int intakeTaskState = 0;
  char serialValue = ' ';
  bool finished1 = false;
  bool finished2 = false;
  bool finished3 = false;






///////
///////
///////
///////
///////
///////
//START Subsystem CLASS
///////
///////
///////
///////
///////
///////

void flushkinda(){
 for(timer3 = 0; timer3<100; timer++){
  Serial.read();
 } 
}

class Subsystem
{
  public:
  Subsystem(int numberOfLeds,int updateChooser);
  void chase();
  void wave();
  void multiColorDown();
  void blind();
  void setColor(CRGB color);
  void resetSubsystem();
  void resetTimers();
  void updateTask();
  int forloopthingy = 0;
  int chaseround = 0;
  int timer = 0;
  int impartialTimeKeeper = 0;
  int impartialForLoopKeeper = 0;
  int taskState = 0;
  int _updateChooser;
  int actualNumberOfLeds;
  bool setToTop =false;
  bool done = false;
  CRGB leds[64];
  
  private:
  
};

 Subsystem::Subsystem(int numberOfLeds, int updateChooser){
   
   actualNumberOfLeds = numberOfLeds;
//   CRGB leds[numberOfLeds];
   _updateChooser = updateChooser;
   
   
  
}

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
   
}

void Subsystem::setColor(CRGB color){
  for(timer = 0; timer< actualNumberOfLeds; timer++){
   leds[timer] = color; 
  }
  FastLED.show();
}

void Subsystem::chase(){
  if(!done){
    if(chaseround == 0){
     for(timer = 0; timer < actualNumberOfLeds; timer++){
        if(timer%2 == 0){
         leds[timer] = CRGB::Green; 
        }else{
         leds[timer] = CRGB::Red; 
        }
     } 
    }else{
      for(timer = 0; timer < actualNumberOfLeds; timer++){
        if(timer%2 == 0){
         leds[timer] = CRGB::Red; 
        }else{
         leds[timer] = CRGB::Green; 
        }
     }
      
    }
    if(chaseround == 1){
     chaseround = 0; 
    }else{
     chaseround = 1; 
    }
    impartialTimeKeeper++;
    if(impartialTimeKeeper == 100){
     done = true; 
    }

    }else{
     updateTask(); 
    }
}

void Subsystem::wave(){
  if(!done){
  switch(forloopthingy){
      case 1:
      leds[timer] = CRGB::Red;

      break;
      
      case 2:
      leds[timer] = CRGB::Green;

      break;
      
      case 3:
      leds[timer] = CRGB::Blue;

      break;
      
      case 4:
      leds[timer] = CRGB::Cyan;

      break;
      
      case 5:
      leds[timer] = CRGB::Pink;

      break;
      
      case 6:
      leds[timer] = CRGB::Yellow;

      break;
      
      case 7:
      leds[timer] = CRGB::Red;

      break;
      
      case 8:
      leds[timer] = CRGB::Yellow;

      break;
      
      case 9:
      leds[timer] = CRGB::Purple;

      break;
      
      case 10:
      leds[timer] = CRGB::Green;

      break;
      
      case 11:
      leds[timer] = CRGB::Cyan;

      break;
      
      case 12:
      leds[timer] = CRGB::Blue;

      break;
      
      case 13:
      leds[timer] = CRGB::Brown;

      break;
      
      case 14:
      leds[timer] = CRGB::Gold;

      break;
      
      case 15:
      leds[timer] = CRGB::Yellow;

      break;
      
      case 16:
      leds[timer] = CRGB::Red;

      break;
      
      case 17:
      leds[timer] = CRGB::White;

      break;
      
      case 18:
      leds[timer] = CRGB::Blue;

      break;
      
      case 19:
      leds[timer] = CRGB::Yellow;

      break;
      
      case 20:
      leds[timer] = CRGB::Green;

      break;
      
      case 21:
      leds[timer] = CRGB::Purple;

      
      break;
      
      case 23:
      leds[timer] = CRGB::Green;

      break;
      
      case 24:
      leds[timer] = CRGB::Blue;

      break;
      
      case 25:
      leds[timer] = CRGB::Green;

      break;
      
      case 26:
      leds[timer] = CRGB::Purple;

      break;
      
      case 27:
      leds[timer] = CRGB::Yellow;

      break;
      
      case 28:
      leds[timer] = CRGB::White;

      break;
      
      case 29:
      leds[timer] = CRGB::Brown;

      break;
      
      case 30:
      leds[timer] = CRGB::Yellow;

      break;
      
      case 31:
      leds[timer] = CRGB::White;

      break;
      
      case 32:
      leds[timer] = CRGB::Purple;

      break;
      
      
      
      default :
      leds[timer] = CRGB::Purple;

      
    }
    if(timer == forloopthingy){
     timer = 0;
    forloopthingy++; 
    }else{
     timer++; 
    }
 
 if(forloopthingy == actualNumberOfLeds -1){
  done = true; 
 }
    
}else{
 updateTask(); 
}
}

void Subsystem::multiColorDown(){
  if(!done){
    if(!setToTop){
     setToTop = true;
    timer = actualNumberOfLeds-1; 
    }
    switch(forloopthingy){
    case 1:
         leds[timer] = CRGB::Red;
         //FastLED.show();
         break;
        
        case 2:
         leds[timer] = CRGB::Blue;
         //FastLED.show();
        break;
       
       case 3:
        leds[timer] = CRGB::Green;
         //FastLED.show();
       break;
      
      case 4:
       leds[timer] = CRGB::Yellow;
         //FastLED.show();
      break;
     
     case 5:
      leds[timer] = CRGB::Cyan;
         //FastLED.show();
     break;
    
    default:
     leds[timer] = CRGB::White;
         //FastLED.show();
   break;
   
        }
        if(timer == 0){
         setToTop = false;
        forloopthingy++; 
        }else{
         timer--; 
        }
        if(forloopthingy == 6){
         done = true; 
        }
  }else{
   updateTask(); 
  }
}

void Subsystem::blind(){
  if(!done){
    if(timer%2 == 0){
      setColor(CRGB::White);
    }else{
      setColor(CRGB::Black);
    }
    timer++;
    if(timer == 16){
     done = true; 
    }
  }else{
   updateTask(); 
  }
}

void Subsystem::updateTask(){
 
  
       if(_updateChooser == 1){
    switch(serialValue){
      
     
        case LIFTERUP:
       taskState = 1;
       resetTimers();
       break;
       
       case LIFTERNEUTRAL:
        taskState = 2;
resetTimers();
       break;
       
       case LIFTERDOWN:
        taskState = 3;
resetTimers();
       break;
       
       case NOTZEROED:
         taskState = 4;
resetTimers();    
break;
       
       case TEAMCOLORRED:
       taskState = 5;
       resetTimers();
       break;
       
       case TEAMCOLORBLUE:
       taskState = 6;
resetTimers();      
break;
       
       case TEAMCOLORUNKNOWN:
       taskState = 7;
resetTimers();
break;
       
       default:
       break;
       
      
     }
       }else{
         switch(serialValue){
      
      case PUSHERNEUTRAL:
       taskState = 2;
resetTimers();
       break;
       
       case PUSHERFORWARD:
        taskState = 1;
resetTimers();
       break;
       
       case PUSHERBACK:
        taskState = 3;
resetTimers();
       break;
        
       
       case NOTZEROED:
         taskState = 4;
resetTimers();    
break;
       
       case TEAMCOLORRED:
       taskState = 5;
       resetTimers();
       break;
       
       case TEAMCOLORBLUE:
       taskState = 6;
resetTimers();      
break;
       
       case TEAMCOLORUNKNOWN:
       taskState = 7;
resetTimers();
break;
       
       default:
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

///////
///////
///////
///////
///////
///////



Subsystem lifter(NUM_LEDS_LIFTER,1);
Subsystem pusher(NUM_LEDS,0);

/////
////
void startUpLights(){
    dynamicDelay = 10000;
    for(timer = NUM_LEDS -1; timer >  -1; timer--){
   
     pusher.leds[timer] = CRGB::Red;      //Actually green. WS2811's are odd.
     FastLED.show();
      if(dynamicDelay > 20){
      dynamicDelay = dynamicDelay/2;
      }
      if(dynamicDelay < 10){
       dynamicDelay = 20; 
      }
      delay(dynamicDelay);
     
         
   
 }
 
 for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
     lifter.leds[timer] = CRGB::Green;
     FastLED.show();
      delay(dynamicDelay);    
   
 }
 for(forloopthingy = 0; forloopthingy < 10; forloopthingy++){
 for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
     lifter.leds[timer] = CRGB::White;
   
 }
 
  for(timer = 0; timer < NUM_LEDS; timer++){
   
     pusher.leds[timer] = CRGB::White;
   
 }
 FastLED.show();
 delay(100);
 
 for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
     lifter.leds[timer] = CRGB::Green;
   
 }
 
  for(timer = 0; timer < NUM_LEDS; timer++){
   
     pusher.leds[timer] = CRGB::Red;
   
 }
 FastLED.show();
 delay(200);
 } 
 for(timer = 0; timer < NUM_LEDS; timer++){
   
     pusher.leds[timer] = CRGB::Black;
   
 }
 
 for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
     lifter.leds[timer] = CRGB::Black;
   
 }
 FastLED.show();
 }
 ////
 ///
 ///
 ///
 ///
 



void setup(){
pinMode(DATA_PIN,OUTPUT);
pinMode(DATA_PIN_LIFTER,OUTPUT);

 FastLED.addLeds<WS2811, DATA_PIN>(pusher.leds, NUM_LEDS);
 FastLED.addLeds<NEOPIXEL, DATA_PIN_LIFTER>(lifter.leds,NUM_LEDS_LIFTER);
 Serial.begin(9600);
 //Serial.setTimeout(500);

startUpLights();
lifter.resetSubsystem();
pusher.resetSubsystem();
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
 void resetTimers(){
   
    pusher.resetSubsystem();
    lifter.resetSubsystem();
   
   
   
    forloopthingy1 = 0;
    forloopthingy2 = 0;
    forloopthingy3 = 0;
  timer = 0;
   timer1 = 0;
   timer2 = 0;
   timer3 = 0;
   impartialTimeKeeper1 = 0;
   impartialTimeKeeper2 = 0;
   impartialTimeKeeper3 = 0;
   impartialForLoopKeeper1 = 0;
   impartialForLoopKeeper2 = 0;
   impartialForLoopKeeper3 = 0;
   chaseround1 = 0;
   chaseround2 = 0;
   chaseround3 = 0;
   finished1 = false;
   finished2 = false;
   finished3 = false;
   lifter.taskState = 0;
   pusher.taskState = 0;
    
 }
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
 void override(){
   if(serialValue == NOTZEROED or serialValue == TEAMCOLORRED or serialValue == TEAMCOLORBLUE or serialValue == TEAMCOLORUNKNOWN){
   
   switch(serialValue){
    case NOTZEROED:
    pusher.taskState = 4;
    lifter.taskState = 4;
    break;
    
    case TEAMCOLORRED:
    pusher.taskState = 5;
    lifter.taskState = 5;
    break;
    
    case TEAMCOLORBLUE:
    pusher.taskState = 6;
    lifter.taskState = 6;
    break;
    
    case TEAMCOLORUNKNOWN:
    pusher.taskState = 7;
    lifter.taskState = 7;
    break;
     
   }
   }
 }
  //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 //\/\/\/\/\/\/\/\/\/
 void dispatchInputs(){
    override();
  switch(lifter.taskState){
    case 1:          //Foward
lifter.wave();    
break;
    
    case 2:    //Neutral
lifter.chase();
break;
    
    case 3:    //Back
lifter.multiColorDown();
break;
    
    case 4:
    lifter.blind();

    //updateLifterTask();
    break;
    
    case 5:
    twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,pusher.leds,lifter.leds,50,CRGB::Red);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,lifter.leds,pusher.leds,50,CRGB::Black);
flushkinda();
resetTimers();
  //updateLifterTask();
    break;
    
    case 6:
    twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,pusher.leds,lifter.leds,50,CRGB::Blue);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,lifter.leds,pusher.leds,50,CRGB::Black);
flushkinda();
resetTimers();
  //updateLifterTask();
    break;
    
    case 7:
    twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,pusher.leds,lifter.leds,50,CRGB::Yellow);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,lifter.leds,pusher.leds,50,CRGB::Black);
flushkinda();
resetTimers();
  //updateLifterTask();
  break;
    default:
    lifter.updateTask();
     
     
    break;
    
    
  }
  
  //override();
  switch(pusher.taskState){
    case 1:                //Forward
pusher.wave();
break;
    
    case 2:    //Neutral
pusher.chase();
break;
    
    case 3:      //Back
pusher.multiColorDown();

    break;
    
    case 4:
    pusher.blind();

   break;
   
   case 5:
   twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,pusher.leds,lifter.leds,50,CRGB::Red);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,lifter.leds,pusher.leds,50,CRGB::Black);
flushkinda();
resetTimers();
   break;
   
   case 6:
   twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,pusher.leds,lifter.leds,50,CRGB::Blue);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,lifter.leds,pusher.leds,50,CRGB::Black);
flushkinda();
resetTimers();
   break;
   
   case 7:
   twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,pusher.leds,lifter.leds,50,CRGB::Yellow);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,lifter.leds,pusher.leds,50,CRGB::Black);
flushkinda();
resetTimers();
   break;
    
    default:
  
   
                   pusher.updateTask();
                   break;

     }
 }
 ///////
///////
///////
///////
///////
///////
//END GLOBAL FUNCTIONS
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
//START OVERRIDE LIGHT FUNCTIONS
///////
///////
///////
///////
///////
///////
 void blind(){
   for(forloopthingy1 = 0; forloopthingy1 < 7; forloopthingy1++){
   for(timer = 0; timer < NUM_LEDS; timer++){
    pusher.leds[timer] = CRGB::White;
     lifter.leds[timer] = CRGB::White; 
   }
   FastLED.show();
   delay(100);
   for(timer = 0; timer < NUM_LEDS; timer++){
    pusher.leds[timer] = CRGB::Black;
     lifter.leds[timer] = CRGB::Black; 
   }
   FastLED.show();
   delay(100);
    }
    
  }
///////
///////
///////
///////
///////
///////
//END OVERRIDE LIGHT FUNCTIONS
///////
///////
///////
///////
///////
///////
 
/* void setColorUp(int ledAmount, CRGB ledArray[], int delayTime, int color, bool multitasking, int timerchoice, int impartialTimeKeeper, bool finishedkeeper){
    if(!finishedkeeper){
    for(timerchoice = impartialTimeKeeper; timerchoice < ledAmount; timerchoice++){
   
     ledArray[timerchoice] = color;
     FastLED.show();
     if(!multitasking){
      delay(delayTime);     
     } else{
       impartialTimeKeeper++;
       break;
     }
 }
     if(timerchoice == ledAmount -1){
      finishedkeeper = true;
     impartialTimeKeeper = 0; 
     }else{
      
     updatePusherTask();
       
       
       
     }
    }else if(ledAmount == NUM_LEDS_LIFTER){
         updateLifterTask();
    } 
    }
 
  
  ///////
  ///////
  ///////
  ///////
  ///////
  //////
  //////
  //////
  //////
  ///////
   void setColorDown(int ledAmount, CRGB ledArray[], int delayTime, int color, bool multitasking, int timerchoice, int impartialTimeKeeper, bool finishedKeeper){
    
    for(timer = ledAmount -1; timer > 0; timer--){
   
     ledArray[timer] = color;
     FastLED.show();
      delay(delayTime);     //Turns all of the LEDs off
   
 }
  }
  /////
  /////
  ////
  ///
  //
  ///
  //
  ///
  //
  //
  //
  //
  */
  void twoStripChangerUp(int ledAmountStripOne, int ledAmountStripTwo, CRGB ledArray1[], CRGB ledArray2[], int delayTime, int color){
    
    for(timer = ledAmountStripOne -1; timer >  -1; timer--){
   
     ledArray1[timer] = color;
     FastLED.show();
      delay(delayTime);     //Turns all of the LEDs off
   
 }
 
 for(timer = 0; timer < ledAmountStripTwo; timer++){
   
     ledArray2[timer] = color;
     FastLED.show();
      delay(delayTime);     //Turns all of the LEDs off
   
 }
  
  }
  
  /////////
  //////////////
   void twoStripChangerDown(int ledAmountStripOne, int ledAmountStripTwo, CRGB ledArray1[], CRGB ledArray2[], int delayTime, int color){
    
    for(timer = 0; timer < ledAmountStripOne; timer++){
   
     ledArray1[timer] = color;
     FastLED.show();
      delay(delayTime);     //Turns all of the LEDs off
   
 }
 
 for(timer = ledAmountStripTwo - 1; timer > 0; timer--){
   
     ledArray2[timer] = color;
     FastLED.show();
      delay(delayTime);     //Turns all of the LEDs off
   
 }
  
  }
  /*
  /////
  /////
  ////
  ////
  ///
  ///
  ///
  ///
  ///
  ///
  ///
  ///
  void chase(int forloopthingy, int repeats, int ledAmount, CRGB ledArray[], int delayTime, int color1, int color2, bool multitasking, int timerchase, int impartialTimeKeeper, bool finishedkeeper, int chaseround){
   if(!finishedkeeper){
    for(forloopthingy = impartialTimeKeeper; forloopthingy < repeats; forloopthingy= forloopthingy){
      if(chaseround == 0){
    for(timerchase = 0; timerchase < ledAmount-1; timerchase++){
      
    if(timerchase%2 == 0){
     ledArray[timerchase] = color1; 
    }else{
     ledArray[timerchase] = color2; 
    }
   } 
      
   //FastLED.show();
   
        
        goto chaseEscape;
      
      if(!multitasking){
   delay(delayTime);
      }      
    }
      if(chaseround == 1){
   for(timerchase = 0; timerchase < ledAmount-1; timerchase++){
    if(timerchase%2 == 0){
     ledArray[timerchase] = CRGB::Red; 
    }else{
     ledArray[timerchase] = CRGB::Green; 
    }
   } 
   //FastLED.show();
  
          goto chaseEscape;
      }
      if(!multitasking){
   delay(delayTime);
      }
   
   if(chaseround == 1){
    chaseround = 0; 
   }else{
    chaseround = 1; 
   }
   
   }
   chaseEscape:
   if(chaseround == 0){
    chaseround = 1;
        impartialTimeKeeper++; 
   }else{
    chaseround = 0;
     impartialTimeKeeper++; 
   }
   if(impartialTimeKeeper >= repeats-1){
    finishedkeeper = true;
   impartialTimeKeeper = 0; 
   }
   
   }else{
    if(ledAmount == NUM_LEDS){
     updatePusherTask();
    }else if(ledAmount == NUM_LEDS_LIFTER){
               updateLifterTask();

    }

    }
  }
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\////\/\/\/\/
  ///\/\/\/\/
  void wave(int forloopthingy, int impartialForLoopKeeper, int ledAmount, CRGB ledArray[], int delayTime, bool multitasking, int timerchoice, int impartialTimeKeeper, bool finishedkeeper){
   if(!finishedkeeper){
    for(forloopthingy = impartialForLoopKeeper; forloopthingy < ledAmount; forloopthingy++){
 
  for(timerchoice = impartialTimeKeeper; timerchoice < forloopthingy; timerchoice++){
  
    
    switch(forloopthingy){
      case 1:
      ledArray[timerchoice] = CRGB::Red;

      break;
      
      case 2:
      ledArray[timerchoice] = CRGB::Green;

      break;
      
      case 3:
      ledArray[timerchoice] = CRGB::Blue;

      break;
      
      case 4:
      ledArray[timerchoice] = CRGB::Cyan;

      break;
      
      case 5:
      ledArray[timerchoice] = CRGB::Pink;

      break;
      
      case 6:
      ledArray[timerchoice] = CRGB::Yellow;

      break;
      
      case 7:
      ledArray[timerchoice] = CRGB::Red;

      break;
      
      case 8:
      ledArray[timerchoice] = CRGB::Yellow;

      break;
      
      case 9:
      ledArray[timerchoice] = CRGB::Purple;

      break;
      
      case 10:
      ledArray[timerchoice] = CRGB::Green;

      break;
      
      case 11:
      ledArray[timerchoice] = CRGB::Cyan;

      break;
      
      case 12:
      ledArray[timerchoice] = CRGB::Blue;

      break;
      
      case 13:
      ledArray[timerchoice] = CRGB::Brown;

      break;
      
      case 14:
      ledArray[timerchoice] = CRGB::Gold;

      break;
      
      case 15:
      ledArray[timerchoice] = CRGB::Yellow;

      break;
      
      case 16:
      ledArray[timerchoice] = CRGB::Red;

      break;
      
      case 17:
      ledArray[timerchoice] = CRGB::White;

      break;
      
      case 18:
      ledArray[timerchoice] = CRGB::Blue;

      break;
      
      case 19:
      ledArray[timerchoice] = CRGB::Yellow;

      break;
      
      case 20:
      ledArray[timerchoice] = CRGB::Green;

      break;
      
      case 21:
      ledArray[timerchoice] = CRGB::Purple;

      
      break;
      
      case 23:
      ledArray[timerchoice] = CRGB::Green;

      break;
      
      case 24:
      ledArray[timerchoice] = CRGB::Blue;

      break;
      
      case 25:
      ledArray[timerchoice] = CRGB::Green;

      break;
      
      case 26:
      ledArray[timerchoice] = CRGB::Purple;

      break;
      
      case 27:
      ledArray[timerchoice] = CRGB::Yellow;

      break;
      
      case 28:
      ledArray[timerchoice] = CRGB::White;

      break;
      
      case 29:
      ledArray[timerchoice] = CRGB::Brown;

      break;
      
      case 30:
      ledArray[timerchoice] = CRGB::Yellow;

      break;
      
      case 31:
      ledArray[timerchoice] = CRGB::White;

      break;
      
      case 32:
      ledArray[timerchoice] = CRGB::Purple;

      break;
      
      
      
      default :
      ledArray[timerchoice] = CRGB::Purple;

      
    }
   
   

   //FastLED.show();
   if(!multitasking){
   delay(delayTime);
   }else{
    impartialTimeKeeper++;
    if(timerchoice == forloopthingy -1){
     impartialTimeKeeper = 0;
    impartialForLoopKeeper++; 
    }
   break; 
   }
   
 }
 if(forloopthingy ==impartialForLoopKeeper -1){
  finishedkeeper = true;
 impartialForLoopKeeper = 0; 
 }
 if(multitasking){
  break; 
 }
 } 
   }else{
     //serialValue = Serial.read();
    if(ledAmount == NUM_LEDS){
     updatePusherTask();
     
    }else if(ledAmount == NUM_LEDS_LIFTER){
                    updateLifterTask();

    } 
   }
  }
  
  
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\////\/\/\/\/
  ///\/\/\/\/
  void multicolordown(int forloopthingy, int impartialForLoopKeeper, int ledAmount, CRGB ledArray[], int delayTime, bool multitasking, int timerchoice, int impartialTimeKeeper, bool finishedkeeper){
    if(!finishedkeeper){
      for(forloopthingy = impartialForLoopKeeper; forloopthingy < 5; forloopthingy++){
      for(timerchoice = impartialTimeKeeper + ledAmount - 1; timerchoice > 0; timerchoice--){
        switch(forloopthingy){
         case 1:
         ledArray[timerchoice] = CRGB::Red;
         //FastLED.show();
         break;
        
        case 2:
         ledArray[timerchoice] = CRGB::Blue;
         //FastLED.show();
        break;
       
       case 3:
        ledArray[timerchoice] = CRGB::Green;
         //FastLED.show();
       break;
      
      case 4:
       ledArray[timerchoice] = CRGB::Yellow;
         //FastLED.show();
      break;
     
     case 5:
      ledArray[timerchoice] = CRGB::Cyan;
         //FastLED.show();
     break;
    
    default:
     ledArray[timerchoice] = CRGB::White;
         //FastLED.show();
   break;
   
        }
        
        if(!multitasking){
         delay(delayTime); 
        }else{
         impartialTimeKeeper++; 
         if(timerchoice == ledAmount){
          impartialTimeKeeper = 0;
         impartialForLoopKeeper++; 
         }
         break;
        }
        
      }
      
      if(forloopthingy == 5){
       finishedkeeper = true;
       impartialForLoopKeeper = 0;
      }
      if(multitasking){
       break; 
      }
     
      }
    }else{
     //serialValue = Serial.read();
    if(ledAmount == NUM_LEDS){
     updatePusherTask();
    }else if(ledAmount == NUM_LEDS_LIFTER){
     updateLifterTask();
    } 
   }
  }
   ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\/
  ///\/\/\/\////\/\/\/\/
  ///\/\/\/\/
  */
  void test(){
   resetTimers(); //NEEDS TO BE MOVED AND/OR INTEGRATED INTO MULTITASK FUNCTIONS.  
  //chase(forloopthingy2, 32,  NUM_LEDS_LIFTER,  ledsLifter,  15,  CRGB::Yellow,  CRGB::Green, true, timer3,  impartialTimeKeeper3,  finished3);
  //setColorUp(NUM_LEDS, leds,  15, CRGB::Red, true,  timer1,  impartialTimeKeeper1,  finished1); 
  }
  
  /*void useDuelStripUpDown(){
    twoStripChangerDown(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,15,CRGB::Orange);
  twoStripChangerDown(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,15,CRGB::Gold);
  twoStripChangerDown(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,15,CRGB::Brown);
  twoStripChangerDown(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,15,CRGB::White);
  twoStripChangerDown(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,15,CRGB::Blue);
  }*/
  
  //
  ///
  //////
  ////////
  ///////////
  ///////////////////
  /////////////////////////





///////////////////////////////////////////////////////////////////// Break of the code between
 //////////////////////////////////////////////////////////////////// Intial startup and loop
 /////////////////////
 ///////////////////
 /////////////////
 /////////////
 ///////
 ///
 //


void loop(){
  
  serialValue = Serial.read();
  dispatchInputs();
  FastLED.show();
  if(pusher.taskState > 0 or lifter.taskState > 0){   //Only delay if we have lights to show. No reason to delay if there is nothing there.
    if(pusher.taskState >= 4 and lifter.taskState >= 4){
     delay(250); //Delay extra if running an override as most overrides shouldn't update fast.
    }else{
      
    delay(DELAY);
    }
}

 
  }
  

 
