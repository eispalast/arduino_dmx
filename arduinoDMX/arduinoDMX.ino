#include <DMXSerial.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif



/**
 * DEFINE VARIABLES
 */

#define FUN_PIN 7
#define DIG_PIN256 12

//All the analog outputs
#define CHANNEL1 11
#define CHANNEL2 10
#define CHANNEL3 9
#define CHANNEL4 6
#define CHANNEL5 3
#define CHANNEL6 5
uint8_t analog_channels[]={11,10,9,6,3,5};

//digital LED strip
#define WS28_pin  2 
#define NUMPIXELS 100 // Hier wird die Anzahl der angeschlossenen WS2812 LEDs bzw. NeoPixel angegeben
uint32_t primary_color; //RGBValue of prim color
uint32_t secondary_color; ///RGB value of sec color
uint8_t digital_effect=0;
uint8_t digital_intensity;
uint8_t* pix;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WS28_pin, NEO_GRB + NEO_KHZ800);

uint16_t DMX_channel=1;
uint8_t bpm=0;
uint8_t beat_type=0;
uint8_t bpm_delay;
int this_delay = 0;

bool DMX_on=true;
bool fun_mode=false;




/***************
 * LIGHT EFFECTS
 * **************/
void update_analog(){
  for (int i=0; i<6;i++){
    analogWrite(analog_channels[i],DMXSerial.read(DMX_channel+i));
  }
}



void setup() {
  // put your setup code here, to run once:
  pinMode(FUN_PIN, INPUT_PULLUP);
  pinMode(DIG_PIN256, INPUT_PULLUP);
  pixels.begin(); // Initialisierung der NeoPixel
  DMXSerial.init(DMXReceiver);
  pix=pixels.getPixels();
  //Check if DMX or Function mode
  if(digitalRead(FUN_PIN)==LOW){
    DMX_on=false;
    fun_mode=true;
  }
  DMX_channel=check_DMX_channel();
  start_sequence();
  

}

void loop() {
  if(fun_mode){ //so function mode is on
    DMX_channel=check_DMX_channel(); //is used to determine which function to show
    if(DMX_channel <=10){
      start_sequence();
    }
    if(DMX_channel >10 && DMX_channel<=20){
      pixels.fill(pixels.Color(100,0,0),0,100);
      pixels.show();
      delay(500);
    }
    if(DMX_channel>20 && DMX_channel<=30){
      primary_color=pixels.Color(200,200,200);
      secondary_color=pixels.Color(100,30,30);
      digital_intensity=20;
      rising_star();
    }
    if(DMX_channel>30 && DMX_channel<=255){
      primary_color=pixels.Color(10,150,200);
      digital_intensity=12;
      blinky();
    }
  }
  else{
    while(1){
      digital_effect=get_digital_effect();
      if(digital_effect<=10){
        digital_off();
      }
      if(digital_effect>10 && digital_effect<=20){
        digital_normal();
      }
      if(digital_effect>20 && digital_effect<=30){
        rising_star();
      }
    }
  }
  // put your main code here, to run repeatedly:
  //int this_delay=calc_delay(120);
  

}
