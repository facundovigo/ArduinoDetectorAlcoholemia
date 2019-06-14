#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int TIME_UNTIL_WARMUP = 10;
unsigned long time;

int TIME_UNTIL_MEASURE = 5;
unsigned long measurement_start;

int analogPin = A0;
int val = 0;

const int buzzerPin = 7;
const int buttonPin = 2;

bool measurement_mode;
bool measurement_done;

LiquidCrystal_I2C lcd(0x3F,20,4);

void setup() {
  Serial.begin(115200);
  lcd.init(); 
  lcd.backlight();
}

void loop() {
  int button_state = digitalRead(buttonPin);
  
  if (button_state && !measurement_mode) {
    lcd.clear();
    measurement_mode = true;
    measurement_start = millis()/1000;
    measurement_done = false;
  }
  
  delay(100);
  time = millis()/1000;
  
  if(time<=TIME_UNTIL_WARMUP) {
    int progress_time = map(time, 0, TIME_UNTIL_WARMUP, 0, 100);
    printWarming(progress_time);
  } else {
     if (measurement_mode == false && !measurement_done) {
       printPress();  
     }
     if (measurement_mode && !measurement_done) {      
       printMeasure();
       //tone(buzzerPin, 1000);
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
     }
  }

}

void printWarming(int progress) {
  lcd.setCursor(0,0);
  lcd.print("Calentando: ");
  lcd.print(progress);
  lcd.print("%");
}

void printPress() {
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Presiona");
  lcd.setCursor(2,1);
  lcd.print("para iniciar");
}

void printMeasure() {  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sopla hasta que");
  lcd.setCursor(0,1);
  lcd.print("pare el sonido");
}

void printAlcohol(int value) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reading ");
  lcd.print(value);
  
  lcd.setCursor(0,1);
  if(value<200){
      lcd.print("SuperSobrio");
  }
  if (value>=200 && value<280){
      lcd.print("Una birrita");
  }
  if (value>=280 && value<350){
      lcd.print("Un par de birras");
  }
  if (value>=350 && value <450){
      lcd.print("Nivel P.Bulrich!");
  }
  if(value>450){
     lcd.print("MODO CHANO!");
  }
}
 
//int readAlcohol() {
//  int nb_measurements = 5;
//  int measurements;
//  for (int i = 0; i < nb_measurements; i++) {
//    measurements = measurements + analogRead(analogPin); 
//  } 
//  measurements = measurements/nb_measurements;
//  return measurements;
//}

int readAlcohol() {
  int value = 0;
  int val1;
  int val2;
  int val3;

  lcd.clear();
  val1 = analogRead(analogPin); 
  delay(10);
  val2 = analogRead(analogPin); 
  delay(10);
  val3 = analogRead(analogPin);
  
  value = (val1+val2+val3)/3;
  return value;
}
