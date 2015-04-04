//DO NOT USE Port 13
//Code designed to run WS2811 LEDS and NEOPIXELs
//Attempts to multitask ceratin functions by breaking for loops and reentering them
//Based on non-for loop timers. This should cause both to update their assigend
// light strip once and then go and update the next strip.
#include <FastLED.h>


#define NUM_LEDS 32
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
#define PUSHERBACK '10'
#define PUSHERNEUTRAL '11'


#define DATA_PIN 2 //Using data pin one for primary light strip pin timer. Once again highly subject to change
#define DATA_PIN_LIFTER 3
//#define DATA_PIN_INPUT 3
//#define DATA_PIN_INPUT2 4
#define DELAY 20 //sets the delay between light updates to 15 milliseconds

CRGB leds[NUM_LEDS];
CRGB ledsLifter[NUM_LEDS_LIFTER];
  int testvariable1 = 0;
  int testvariable2 = 0;
  int forloopthingy1 = 0;
  int forloopthingy2 = 0;
  int forloopthingy3 = 0;
  bool chaseround1 = 0;
  bool chaseround2 = 0;
  bool chaseround3 = 0;
  int timer = 0;
  int timer1 = 0;
  int timer2 = 0;
  int timer3 = 0;
  int impartialTimeKeeper1 = 0;
  int impartialTimeKeeper2 = 0;
  int impartialTimeKeeper3 = 0;
  int impartialForLoopKeeper1 = 0;
  int impartialForLoopKeeper2 = 0;
  int impartialForLoopKeeper3 = 0;
  int lifterTaskState = 0;
  int intakeTaskState = 0;
  char serialValue = ' ';
  bool finished1 = false;
  bool finished2 = false;
  bool finished3 = false;
  
  
 ////////////////////
 ////////////////
 /////////
 /////
 ///
 //
 void resetTimers(){
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
   lifterTaskState = 0;
   intakeTaskState = 0;
    
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
 void updatePusherTask(){
  //serialValue = Serial.read();
    switch(serialValue){
       case PUSHERNEUTRAL:
       intakeTaskState = 2;
       finished1 = false;
       break;
       
       case PUSHERFORWARD:
        intakeTaskState = 1;
        finished1 = false;
       break;
       
       case PUSHERBACK:
        intakeTaskState = 3;
        finished1 = false;
       break;
       
       case NOTZEROED:
         intakeTaskState = 4;
         lifterTaskState = 4;
         //resetTimers();
       break;
       
       case TEAMCOLORRED:
       intakeTaskState = 5;
         lifterTaskState = 5;
       
       //resetTimers();
       break;
       
       case TEAMCOLORBLUE:
       intakeTaskState = 6;
         lifterTaskState = 6;
       //resetTimers();
       break;
       
       case TEAMCOLORUNKNOWN:
       intakeTaskState = 7;
       lifterTaskState = 7;
       //resetTimers();
       break;
       
       
       default:
       break;
     } 
 }
 
 void updateLifterTask(){
  //serialValue = Serial.read();
    switch(serialValue){
        case LIFTERUP:
       lifterTaskState = 1;
       finished3 = false;
       break;
       
       case LIFTERNEUTRAL:
        lifterTaskState = 2;
               finished3 = false;

       break;
       
       case LIFTERDOWN:
        lifterTaskState = 3;
               finished3 = false;

       break;
       
       case NOTZEROED:
         intakeTaskState = 4;
         lifterTaskState = 4;
         //resetTimers();
       break;
       
       case TEAMCOLORRED:
       intakeTaskState = 5;
         //lifterTaskState = 5;
       
       resetTimers();
       break;
       
       case TEAMCOLORBLUE:
       intakeTaskState = 6;
         lifterTaskState = 6;
       //resetTimers();
       break;
       
       case TEAMCOLORUNKNOWN:
       intakeTaskState = 7;
       lifterTaskState = 7;
       //resetTimers();
       break;
       
       default:
       break;
       
      
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
 void override(){
   if(serialValue == NOTZEROED or serialValue == TEAMCOLORRED or serialValue == TEAMCOLORBLUE or serialValue == TEAMCOLORUNKNOWN){
   
   switch(serialValue){
    case NOTZEROED:
    intakeTaskState = 4;
    lifterTaskState = 4;
    break;
    
    case TEAMCOLORRED:
    intakeTaskState = 5;
    lifterTaskState = 5;
    break;
    
    case TEAMCOLORBLUE:
    intakeTaskState = 6;
    lifterTaskState = 6;
    break;
    
    case TEAMCOLORUNKNOWN:
    intakeTaskState = 7;
    lifterTaskState = 7;
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
 void blind(){
   for(forloopthingy1 = 0; forloopthingy1 < 7; forloopthingy1++){
   for(timer = 0; timer < NUM_LEDS; timer++){
    leds[timer] = CRGB::White;
     ledsLifter[timer] = CRGB::White; 
   }
   FastLED.show();
   delay(100);
   for(timer = 0; timer < NUM_LEDS; timer++){
    leds[timer] = CRGB::Black;
     ledsLifter[timer] = CRGB::Black; 
   }
   FastLED.show();
   delay(100);
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
 
 void setColorUp(int ledAmount, CRGB ledArray[], int delayTime, int color, bool multitasking, int timerchoice, int impartialTimeKeeper, bool finishedkeeper){
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
   
   /*if(chaseround == 1){
    chaseround = 0; 
   }else{
    chaseround = 1; 
   }*/
   
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
  void test(){
   resetTimers(); //NEEDS TO BE MOVED AND/OR INTEGRATED INTO MULTITASK FUNCTIONS.  
  //chase(forloopthingy2, 32,  NUM_LEDS_LIFTER,  ledsLifter,  15,  CRGB::Yellow,  CRGB::Green, true, timer3,  impartialTimeKeeper3,  finished3);
  setColorUp(NUM_LEDS, leds,  15, CRGB::Red, true,  timer1,  impartialTimeKeeper1,  finished1); 
  }
  
  void useDuelStripUpDown(){
    twoStripChangerDown(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,15,CRGB::Orange);
  twoStripChangerDown(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,15,CRGB::Gold);
  twoStripChangerDown(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,15,CRGB::Brown);
  twoStripChangerDown(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,15,CRGB::White);
  twoStripChangerDown(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,15,CRGB::Blue);
  }
  
  //
  ///
  //////
  ////////
  ///////////
  ///////////////////
  /////////////////////////



void setup(){
pinMode(DATA_PIN,OUTPUT);
pinMode(DATA_PIN_LIFTER,OUTPUT);

 FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
 FastLED.addLeds<NEOPIXEL, DATA_PIN_LIFTER>(ledsLifter,NUM_LEDS_LIFTER);
 Serial.begin(9600);
 //Serial.setTimeout(500);

 for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
   ledsLifter[timer] = CRGB::Green;                      //Turns all of the LEDs off
   delay(DELAY);
   FastLED.show();
 }
 
  for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
   ledsLifter[timer] = CRGB::Red;                      //Turns all of the LEDs off
   delay(DELAY);
   FastLED.show();
 }
 
  for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
   ledsLifter[timer] = CRGB::Purple;                      //Turns all of the LEDs off
   delay(DELAY);
   FastLED.show();
 }
 
  for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
   ledsLifter[timer] = CRGB::Black;                      //Turns all of the LEDs off
   delay(DELAY);
   FastLED.show();
 }
 
  for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   
   ledsLifter[timer] = CRGB::Green;                      //Turns all of the LEDs off
   delay(DELAY);
   FastLED.show();
 }
 
 for(timer = 0; timer < NUM_LEDS_LIFTER; timer++){
   ledsLifter[timer] = CRGB::Black;                      //Turns all of the LEDs off
   delay(DELAY);
   FastLED.show();
 }
 //test();

}

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
  
  
  
  override();
  switch(lifterTaskState){
    case 1:          //Foward
          wave(forloopthingy3,  impartialForLoopKeeper3,  NUM_LEDS_LIFTER,  ledsLifter,  15,  true,  timer1,  impartialTimeKeeper3,  finished3);           
    break;
    
    case 2:    //Neutral
      chase(forloopthingy3, 32,  NUM_LEDS_LIFTER,  ledsLifter,  15,  CRGB::Yellow,  CRGB::Blue, true, timer3,  impartialTimeKeeper3,  finished3,chaseround3);
    break;
    
    case 3:    //Back
     multicolordown(forloopthingy3,  impartialForLoopKeeper3, NUM_LEDS_LIFTER,  ledsLifter,  15,  true,  timer3,  impartialTimeKeeper3,  finished3);
    break;
    
    case 4:
    blind();
        resetTimers();

    //updateLifterTask();
    break;
    
    case 5:
    twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,50,CRGB::Red);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,50,CRGB::Black);
   resetTimers();
  //updateLifterTask();
    break;
    
    case 6:
    twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,50,CRGB::Blue);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,50,CRGB::Black);
    resetTimers();
  //updateLifterTask();
    break;
    
    case 7:
    twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,50,CRGB::Yellow);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,50,CRGB::Black);
  resetTimers();
  //updateLifterTask();
  break;
    default:
    updateLifterTask();
     
     
    break;
    
    
  }
  
  //override();
  switch(intakeTaskState){
    case 1:                //Forward
         wave(forloopthingy1,  impartialForLoopKeeper1,  NUM_LEDS,  leds,  15,  true,  timer1,  impartialTimeKeeper1,  finished1);           
    break;
    
    case 2:    //Neutral
        chase(forloopthingy1, 32,  NUM_LEDS,  leds,  15,  CRGB::Yellow,  CRGB::Blue, true, timer1,  impartialTimeKeeper1,  finished1, chaseround1);
    break;
    
    case 3:      //Back
         multicolordown(forloopthingy1,  impartialForLoopKeeper1, NUM_LEDS,  leds,  15,  true,  timer1,  impartialTimeKeeper1,  finished1);

    break;
    
    case 4:
    blind();
        resetTimers();

    //updatePusherTask();
   break;
   
   case 5:
   twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,50,CRGB::Red);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,50,CRGB::Black);
  resetTimers();
  //updatePusherTask();
   break;
   
   case 6:
   twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,50,CRGB::Blue);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,50,CRGB::Black);
   resetTimers();
  //updatePusherTask();
   break;
   
   case 7:
   twoStripChangerUp(NUM_LEDS,NUM_LEDS_LIFTER,leds,ledsLifter,50,CRGB::Yellow);
  twoStripChangerUp(NUM_LEDS_LIFTER,NUM_LEDS,ledsLifter,leds,50,CRGB::Black);
   resetTimers();
  //updatePusherTask();
   break;
    
    default:
    /*for(testvariable1 =0; testvariable1 < NUM_LEDS; testvariable1++){
    switch(testvariable2){
     case 1:
     leds[testvariable1] = CRGB::Red;
     break;
    
    case 2:
    leds[testvariable1] = CRGB::Blue;
    break;
   
   case 3:
   leds[testvariable1] = CRGB::Green;
   break;
  
  case 4:
  leds[testvariable1] = CRGB::Purple;
  break;
 
 case 5: 
 leds[testvariable1] = CRGB::White;
 break;
 
 default:
if(testvariable2%2 == 0){ 
 leds[testvariable1] = CRGB::Yellow;
}else{
 leds[testvariable1] = CRGB::Blue; 
}
 break;
    }
    }
    delay(250);
    testvariable2++;
    */
   
                   updatePusherTask();
                   break;

     }
    
  
  FastLED.show();
  if(intakeTaskState > 0 or lifterTaskState > 0){
    delay(DELAY);
}

 
  }
  

 
