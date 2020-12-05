#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 2 // Hier wird angegeben, an welchem digitalen Pin die WS2812 LEDs bzw. NeoPixel angeschlossen sind
#define NUMPIXELS 100 // Hier wird die Anzahl der angeschlossenen WS2812 LEDs bzw. NeoPixel angegeben
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); //TODO: figure out the correct permutation
uint16_t pause;
int th=0;



uint32_t primary_color;
uint32_t secondary_color;
uint8_t *pixls;

uint16_t calc_delay(uint8_t bpm, uint8_t beat_type) {
  uint16_t pure = (uint16_t) (60000 / bpm);
  if(beat_type<=25){ //quarter
    return pure;
  }
  
  if(beat_type<=36){ //quarter dotted
    return (int)(pure*1.5);
  }
  
  if (beat_type<=72){ //whole
    return pure*4;
  }
  
  if (beat_type<=89){ //half
    return pure*2;
  }

  if(beat_type<=99){ //half dotted
    return pure*3;
  }

  if (beat_type<=136){ //quarter triplets
    return (uint16_t) (pure*4/3);
  }
  
  if(beat_type<=163){ //eight triplets
    return (uint16_t) (pure*4/6);
  }
  
  if(beat_type<=189){ //eight 
    return pure>>1;
  }

  if(beat_type <=200){ //eight dotted
    return (uint16_t) (pure*0.75);
  }
  else {  //sixteenth
    return pure>>2;
  }
}

uint8_t * trail_colors (uint32_t colors,byte total_intensity){
  uint8_t* trail_colors=malloc(3*total_intensity);
  uint8_t first=(uint8_t) (colors>>16)&255;
  uint8_t sec=(uint8_t) (colors>>8)&255;
  uint8_t third=(uint8_t) colors&255;
  for(int current_intensity=0;current_intensity<total_intensity;current_intensity++){
    float divisor=((float)current_intensity)/((float)total_intensity);
    trail_colors[current_intensity*3]=(uint8_t) ((float)first*divisor);
    trail_colors[current_intensity*3+1]=(uint8_t) ((float)sec*divisor);
    trail_colors[current_intensity*3+2]=(uint8_t) ((float)third*divisor);
  }
  return trail_colors;
}

void setup() 
{
  pixels.begin(); // Initialisierung der NeoPixel
  pixls=pixels.getPixels();
  Serial.begin(9600);
  pause=calc_delay(120,1);
  Serial.println(pause);
  Serial.println((unsigned long)(1000*((double)pause/(double)(NUMPIXELS+5))));
}

void the_comet(){
  primary_color=pixels.Color(255,255,255);  //TODO: get colors from DMX signal
  secondary_color=pixels.Color(0,75,th);    //TODO: get colors from DMX signal
  byte intensity=20;                        //TODO: get intensity from DMX signal
  uint8_t* trail_cols=trail_colors(secondary_color,intensity);
  
  
  for(int i=0;i<NUMPIXELS+intensity;i++){   //TODO: check if this program is still to be shown (DMX value for program still the same)
    //TODO (in final project): Check for other DMX-Signals ins between
    unsigned long StartTime = micros();
    
    //the star itself:
    if(i<NUMPIXELS){
      pixels.setPixelColor(i, primary_color);  
    }
    //the trail:
    uint8_t underlap=abs(min(0,i-intensity));
    uint8_t overlap=max(0,i-NUMPIXELS);
    memcpy(pixls+((i-intensity+underlap)*3),trail_cols+((underlap)*3),(intensity-overlap-underlap)*3);
    
    pixels.show(); //show star and trail
    
    unsigned long ElapsedTime = micros() - StartTime;
   
    delayMicroseconds(3000);
    //delayMicroseconds((unsigned long)(1000*((double)pause/(double)(NUMPIXELS+intensity)))-ElapsedTime-300);
    //delay(500);
    
  }
  
  free(trail_cols);
}

void loop() 
{
  the_comet();

  th+=5;
  if(th>=127){
    th=0;
  }
//delay (calc_delay(100,1)); // Pause, die LEDs bleiben in dieser Zeit aus

}
