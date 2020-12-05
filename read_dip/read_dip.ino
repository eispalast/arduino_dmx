
/*
 * Analog pins are: A0=14, A1=15 etc. 
 * reads the analog inputs to calculate the DMX channel. Uses all 8 analog inputs + 1 digital input (Pin 8)
 */

#define FUN_PIN 7
#define DIG_PIN256 12
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

void setup() {
  Serial.begin(9600);
  pinMode(DIG_PIN256, INPUT_PULLUP);
  pinMode(FUN_PIN, INPUT_PULLUP);
}

void loop() {
  uint16_t DMX_start_channel=check_DMX_channel();
  Serial.print(DMX_start_channel);
  if(digitalRead(FUN_PIN)==LOW){
    Serial.print("Fun on");
  }
  else{
    Serial.print("Fun off");
  }
  Serial.println("\n\n\n");// debug value

  delay(5000);
}
