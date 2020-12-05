/*
 * Calculate Delay between two Beats. beat_type is the note length
 * Number   | Type
 * ---------|------
 * 0-25     | quarter note
 * 26-36    | quarter dotted
 * 37-72    | whole note
 * 73-89    | half note
 * 90-99    | half dotted
 * 100-136  | quarter triplets
 * 137-163  | eighth triplets
 * 164-189  | eighth
 * 190-200  | eighth dotted
 * 201-255  | sixteenth
 * 
 */
int calc_delay(uint8_t bpm, uint8_t beat_type) {
  int pure = (int) (60000 / bpm);
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
    return (int) (pure*4/3);
  }
  
  if(beat_type<=163){ //eight triplets
    return (int) (pure*4/6);
  }
  
  if(beat_type<=189){ //eight 
    return pure>>1;
  }

  if(beat_type <=200){ //eight dotted
    return (int) (pure*0.75);
  }
  else {  //sixteenth
    return pure>>2;
  }
}


/*
 * Analog pins are: A0=14, A1=15 etc. 
 * reads the analog inputs to calculate the DMX channel. Uses all 8 analog inputs + 1 digital input (Pin 8)
 */
uint16_t check_DMX_channel(){
  byte expo=0;
  float  sum=0;
  
  for (int i=21;i>=14;i--){    
    if (analogRead(i)>800){
      sum=sum + ((pow(2,expo)));
    }
    expo++;
  }
  if(digitalRead(DIG_PIN256)==LOW){
    sum+=256;
  }
  return (uint16_t) (0.1+sum);
}

/*
 * returns 3 byte values (RGB) given a strip num:
 * 0: analog 1;
 * 1: analog 2;
 * 2: digital primary
 * 3: digitl secondary
 */
uint32_t get_RGB_value(byte strip_num){
  return pixels.Color(DMXSerial.read(DMX_channel+3*strip_num),DMXSerial.read(DMX_channel+3*strip_num+1),DMXSerial.read(DMX_channel+3*strip_num+2));
}

uint8_t get_digital_effect(){
  return (uint8_t) (DMXSerial.read(DMX_channel+12));
}

bool still_valid(uint8_t lborder, uint8_t rborder){
  digital_effect=get_digital_effect();
  
  return (digital_effect>=lborder && digital_effect <=rborder);
}

/**
 * returns an intensity value between 0 and 10
 */
uint8_t get_intensity(){
  return (uint8_t)((DMXSerial.read(DMX_channel+15)/255)*10);
}

int check_bpm_delay(){
  if (DMXSerial.read(DMX_channel+13)==bpm && DMXSerial.read(DMX_channel+14)==beat_type){
    return bpm_delay;
  }
  else{
    return calc_delay(DMXSerial.read(DMX_channel+13),DMXSerial.read(DMX_channel+14));
  }
}
