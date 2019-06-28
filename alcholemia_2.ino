// Libraries
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int TIME_UNTIL_WARMUP = 2;
unsigned long time;

int TIME_UNTIL_MEASURE = 5;
unsigned long measurement_start;

int analogPin = A0;
int val = 0;

const int buzzerPin = 7;
const int buttonPin = 1;

bool measurement_mode = false;
bool measurement_done = false;

LiquidCrystal_I2C lcd(0x27,20,4);

void setup()   {  
  Serial.begin(115200);
  lcd.init(); 
  lcd.backlight();
}


void loop() {
  int button_state = !digitalRead(buttonPin);
  
  if (button_state && !measurement_mode) {
    measurement_mode = true;
    measurement_start = millis()/1000;
    measurement_done = false;
  }
  
  delay(100);
  time = millis()/1000;
  if(time<=TIME_UNTIL_WARMUP)
  {
    int progress_time = map(time, 0, TIME_UNTIL_WARMUP, 0, 100);
    printWarming(progress_time);
  }
  else
  { 
     if (!measurement_mode && !measurement_done) {    
       printPress();
     }
     if (measurement_mode && !measurement_done) {      
       printMeasure();
       tone(buzzerPin, 1000);
       val = readAlcohol();
     }
     if (measurement_mode && !measurement_done && ((time - measurement_start)> TIME_UNTIL_MEASURE)){ 
       noTone(buzzerPin);
       measurement_mode = false;
       measurement_done = true;
       lcd.clear();
     }
     if(measurement_done) { 
       printAlcohol(val);
       printAlcoholLevel(val);  
     }
  }

}

void printWarming(int progress)
{
  lcd.setCursor(0,0);
  lcd.print("Warming up: ");
  lcd.print(progress);
  lcd.print("%");
}

void printPress()
{
  lcd.setCursor(0,0);
  lcd.print("Press to start ...");
}

void printMeasure()
{
  lcd.setCursor(0,0);
  lcd.print("Breathe until the ");
  lcd.setCursor(0,1);
  lcd.print("sound stops ...");
}

void printAlcohol(int value)
{
  lcd.setCursor(0,0);
  lcd.print("Sensor: ");
  lcd.print(val);
}

void printAlcoholLevel(int value)
{
  lcd.setCursor(0,1);
  if(value<200)
  {
      lcd.print("You are sober.");
  }
  if (value>=200 && value<280)
  {
      lcd.print("You had a beer.");
  }
  if (value>=280 && value<350)
  {
      lcd.print("Two or more beers.");
  }
  if (value>=350 && value <450)
  {
      lcd.print("Too much ...");
  }
  if(value>450)
  {
     lcd.print("You are drunk!");
  }
 }
 
int readAlcohol() {
  int value = 0;
  int val1;
  int val2;
  int val3;

  val1 = analogRead(analogPin); 
  delay(10);
  val2 = analogRead(analogPin); 
  delay(10);
  val3 = analogRead(analogPin);
  
  value = (val1+val2+val3)/3;
  return value;
}
