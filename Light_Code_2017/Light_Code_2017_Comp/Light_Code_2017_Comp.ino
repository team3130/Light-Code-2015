#include "FastLED.h"
#include "Lights.h"
FASTLED_USING_NAMESPACE
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN1    4                                                                                                                                             
#define DATA_PIN2    8
#define INPUT_PIN    9
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS1    64
#define NUM_LEDS2    64

int FPS = 120;
char rxChar = '1';
char preChar = '1';


Lights *lights1 = new Lights(NUM_LEDS1, 254);
Lights *lights2 = new Lights(NUM_LEDS2, 254);
void setup() {
  delay(3000); 
  Serial.begin(9600);
  Serial.setTimeout(40);
  lights1->help();

    FastLED.addLeds<LED_TYPE, DATA_PIN1, COLOR_ORDER>(lights1->leds, NUM_LEDS1).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(lights2->leds, NUM_LEDS2).setCorrection(TypicalLEDStrip);

}

void loop() {
  // put your main code here, to run repeatedly:
{
  if (Serial.available() > 0){
    rxChar = Serial.read();
    Serial.flush();
  }
    
    switch(rxChar){
      
    case '0':                          // If received a ?:
    lights1->help();                   // print the command list.
    rxChar = preChar;
    break;
    
    case '1':                 //Set the lights to red
    lights1->allRed(); 
    lights2->allRed();
    break;

    case '2':                 //Set the lights to Green
    lights1->allGreen();
    lights2->allGreen();
    break;
    
    case '3':                 //Set the lights to Blue
    lights1->allAsOne(CRGB::Blue);
    break;

    case '4':       
    lights1->allAsOne(CRGB::Indigo);      
    break;
    
    case '5':                 //Use sine for moving lights
    lights1->sineWave();
    lights2->sineWave();
    break;

    case '6':                 //BreathFunction
    lights1->breathRainbow();
    break;

    case '7':                 //Dim the lights
    lights1->dim();
    rxChar = preChar;
    break;

    case '8':                 //Brighten the lights
    lights1->brighten();
    rxChar = preChar;
    break;

    case '9':
    lights1->flashRainbow();
    break;
    
    case 'a':                 //Set to a single color change through list of colors
    //lights1->
    break;

    case 'A':                 //Flash Red, White, and Blue
    lights1->flashAmerican();
    break;
    
    case 'D':                 //Activate Death (Do not use if you have epilepsy(?))
    lights1->toggle();
    rxChar = preChar;
    break;

    case 'p':                 //Pause the lights
    lights1->pause();
    break;

    case 'F':                 //Firing lights
    lights1->firingMain();
    break;

    case 'R':                 //Ready to fire
    lights1->fireReadyMain();
    break;

    case 'S':                 //Sample through all the commands
    lights1->cycleCommandsMain();
    break;
    
    default:                     
      Serial.print("'");
      Serial.print((char)rxChar);
      Serial.println("' is not a command!");
      rxChar = preChar;
  }
      preChar = rxChar;

      FastLED.delay(1000 / FPS);
      // send the 'leds' array out to the actual LED strip
      FastLED.show();
      //Serial.println("Time Test");
      if(FPS != 120){
        FPS = 120;
      }
}
}
