//Written by Nick Koumaris
//info@educ8s.tv
//educ8s.tv
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int TIME_UNTIL_WARMUP = 10;
unsigned long time;

int analogPin = 0;
int val = 0;
LiquidCrystal_I2C lcd(0x3F,20,4);

void setup()   {        
  Serial.begin(115200);
  lcd.init(); 
  lcd.backlight();
}

void loop() {  
  delay(100);

  val = readAlcohol();
  //printTitle();
  //printWarming();

  time = millis()/1000;
  
  if(time<=TIME_UNTIL_WARMUP)
  {
    //time = map(time, 0, TIME_UNTIL_WARMUP, 0, 100);
    //lcd.drawRect(10, 50, 110, 10, WHITE); //Empty Bar
    //lcd.fillRect(10, 50, time,10,WHITE);
    lcd.setCursor(0,0);
    lcd.print("time: ");
    lcd.print(time);
  }else
  {
     printAlcohol(val);
     printAlcoholLevel(val);  
  }
  //lcd.lcd();
}

void printTitle()
{
  //lcd.clearlcd();
  //lcd.setTextSize(1);
  //lcd.setTextColor(WHITE);
  //lcd.setCursor(22,0);
  //lcd.println("Breath Analyzer");
}

void printWarming()
{
  //lcd.setTextSize(2);
  //lcd.setTextColor(WHITE);
  lcd.setCursor(0,0);
  lcd.print("Warming up");
}

void printAlcohol(int value)
{
  //lcd.setTextSize(2);
  //lcd.setTextColor(WHITE);
  lcd.setCursor(0,0);
  lcd.print(val);
}

void printAlcoholLevel(int value)
{
  //lcd.setTextSize(1);
  //lcd.setTextColor(WHITE);
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
      lcd.print("NivelPatoBulrich!");
  }
  if(value>450)
  {
     lcd.print("MODO CHANO");
  }
 }

 int readAlcohol() {
  int val = 0;
  int val1;
  int val2;
  int val3;

  lcd.clear();
  val1 = analogRead(analogPin); 
  delay(10);
  val2 = analogRead(analogPin); 
  delay(10);
  val3 = analogRead(analogPin);
  
  val = (val1+val2+val3)/3;
  return val;
 }
