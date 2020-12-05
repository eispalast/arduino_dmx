/**
 * Range 0-10
 * Off
 */


void digital_off(){
  pixels.clear();
  pixels.show();
  update_analog();
}
/**
 * Range: 11-20
 * just acts like an analog strip
*/

void digital_normal(){
  pixels.fill(get_RGB_value(2),0,NUMPIXELS);
  pixels.show();
  update_analog();

}
/*
 * helper function for rising star: packs the whole trail into a big chunk which can easily be copied to the 
 * respective section in the pixels pointer
 */
uint8_t * trail_colors (uint32_t colors,byte total_intensity){
  uint8_t* trail_colors=malloc(3*total_intensity);
  uint8_t first=(uint8_t) (colors>>16)&255;
  uint8_t sec=(uint8_t) (colors>>8)&255;
  uint8_t third=(uint8_t) colors&255;
  for(int current_intensity=0;current_intensity<total_intensity;current_intensity++){
    update_analog();
    float divisor=((float)current_intensity)/((float)total_intensity);
    trail_colors[current_intensity*3]=(uint8_t) ((float)first*divisor);
    trail_colors[current_intensity*3+1]=(uint8_t) ((float)sec*divisor);
    trail_colors[current_intensity*3+2]=(uint8_t) ((float)third*divisor);
  }
  return trail_colors;
}
/*
 * Range: 21-30
 * shoots a star of primary color with a trail of INTENSITY length and secondary color
 */
void rising_star(){
  byte intensity= digital_intensity;
  if(DMX_on){
    primary_color=get_RGB_value(2);  //TODO: get colors from DMX signal
    secondary_color=get_RGB_value(3);    //TODO: get colors from DMX signal
    intensity=max(1,get_intensity()*2);         //TODO: get intensity from DMX signal
  }

  uint8_t* trail_cols=trail_colors(secondary_color,intensity);
  
  
  for(int i=0;i<NUMPIXELS+intensity;i++){   
    unsigned long StartTime = micros();
    update_analog();
    if(!still_valid(21,30) && DMX_on){
      break;
    }
    
    
    //the star itself:
    if(i<NUMPIXELS){
      pixels.setPixelColor(i, primary_color);  
    }
    //the trail:
    uint8_t underlap=abs(min(0,i-intensity));
    uint8_t overlap=max(0,i-NUMPIXELS);
    memcpy(pix+((i-intensity+underlap)*3),trail_cols+((underlap)*3),(intensity-overlap-underlap)*3);
    
    pixels.show(); //show star and trail
    
    unsigned long ElapsedTime = micros() - StartTime;
   
    delayMicroseconds(3000);
    //delayMicroseconds((unsigned long)(1000*((double)pause/(double)(NUMPIXELS+intensity)))-ElapsedTime-300);
    //delay(500);
    
  }
  
  free(trail_cols);
}

void blinky(){
  byte intensity= digital_intensity;
  if(DMX_on){
    primary_color=get_RGB_value(2);  //TODO: get colors from DMX signal
    intensity=max(1,get_intensity()*2);         //TODO: get intensity from DMX signal
  }
  uint8_t r=min(240,(uint8_t)(primary_color>>16)&255);
  uint8_t g=min(240,(uint8_t)(primary_color>>8)&255);
  uint8_t b=min(240,(uint8_t)(primary_color)&255);

  uint8_t star_position[intensity];
  int16_t current_brightness[intensity];
  bool still_up[intensity];
  int8_t variety[intensity];
  for (int i=0; i<intensity; i++){
    star_position[i]=random(3,NUMPIXELS-3);
    current_brightness[i]=random(-500,0);
    still_up[i]=true;
    variety[i]=random(-10,10);
  }
  while(/*still_valid(31,40) && DMX_on   TODO: Sinnvolle abbruchbedingung einfuegen*/true){
    
    for(int i=0;i<intensity;i++){
      if(current_brightness[i]>=0){
        float divisor =(float)((float)current_brightness[i])/((float)255);
        pixels.setPixelColor(star_position[i],pixels.Color((uint8_t)(r+variety[i])*divisor,(uint8_t)(b+variety[i])*divisor,(uint8_t)g*divisor));
        pixels.setPixelColor(star_position[i]-1,pixels.Color((uint8_t)(r+variety[i])*divisor*0.2,(uint8_t)(b+variety[i])*divisor*0.2,(uint8_t)g*divisor*0.2));
        pixels.setPixelColor(star_position[i]+1,pixels.Color((uint8_t)(r+variety[i])*divisor*0.2,(uint8_t)(b+variety[i])*divisor*0.2,(uint8_t)g*divisor*0.2));
      }
      if(still_up[i]){
        current_brightness[i]=min(current_brightness[i]+random(3,5),255);
        if(current_brightness[i]==255){
          still_up[i]=false;
        }
      }
      else{
        current_brightness[i]=max(6,current_brightness[i]-random(1,3));
        if(current_brightness[i]<=6){ //tausche gegen neuen stern
          pixels.setPixelColor(star_position[i],pixels.Color(0,0,0));
          pixels.setPixelColor(star_position[i]+1,pixels.Color(0,0,0));
          pixels.setPixelColor(star_position[i]-1,pixels.Color(0,0,0));
          bool found_pos=false;
          while(!found_pos){
            star_position[i]=random(3,NUMPIXELS-3);
            found_pos=true;
            for(int j=0;j<intensity;j++){
              if(j!=i && abs(star_position[i]-star_position[j])<3){
                found_pos=false;
                break;
              }
            }
          }
          current_brightness[i]=random(-500,0);
          variety[i]=random(-10,10);
          still_up[i]=true;
        }
      }
    }
    pixels.show();
    delay(20);
  }
}


void start_sequence(){
  for(int i=0; i<6;i++){

    analog_fade(i,0,200,10);
    
    analogWrite(analog_channels[i],0);
  }
  pixels.fill(pixels.Color(100,0,0),0,NUMPIXELS);
  pixels.show();
  delay(400);
  pixels.fill(pixels.Color(0,100,0),0,NUMPIXELS);
  pixels.show();
  delay(400);
  pixels.fill(pixels.Color(0,0,100),0,NUMPIXELS);
  pixels.show();
  delay(400);
  pixels.fill(pixels.Color(100,0,100),0,NUMPIXELS);
  pixels.show();
  delay(400);
  pixels.fill(pixels.Color(100,100,0),0,NUMPIXELS);
  pixels.show();
  delay(400);
  pixels.fill(pixels.Color(0,100,100),0,NUMPIXELS);
  pixels.show();
  delay(400);
  pixels.clear();
  pixels.show();
}

void analog_fade(uint8_t chan, uint8_t start_val, uint8_t end_val, int duration){
  uint8_t updater=1;
  if(end_val<start_val){
    updater=-1;
  }
  for(int i=start_val;i!=end_val;i=i+updater){
    analogWrite(analog_channels[chan],i);
    delay(duration);
  }
 
}
