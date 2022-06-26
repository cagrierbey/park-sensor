#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#define TRIGGER_PIN 7
#define ECHO_PIN 6
#define LED 2
#define buzzer 4

LiquidCrystal_I2C lcd(0x27,16,2);

unsigned long old_time;
unsigned long new_time;

unsigned long old_time_2;
unsigned long new_time_2;

unsigned long old_time_lcd;
unsigned long new_time_lcd;

// Maximum distance we want to ping for (in centimeters).
int MAX_DISTANCE = 50;
int MIN_DISTANCE = 10;

int MAX_DISTANCE_LB = 500;

int Buzzer_LED_Freq_Max = 1000;
int Buzzer_LED_Freq_Min = 10;

int Current_Dist = 0;
float Current_Freq = 0;

int Update_Freq = 500;
int Update_LCD = 200;

boolean led_state = true;

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE_LB);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // put your main code here, to run repeatedly:
  // Send ping, get distance in cm
  old_time = millis();
  old_time_2 = millis();
  old_time_lcd = millis();
  Current_Dist = sonar.ping_cm();
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);//camelcase**

  // initialize the lcd 
  lcd.init();
  // set up the LCD's number of columns and rows:
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("DISTANCE :");
  lcd.setCursor(0,1);
  lcd.print("FREQUENCY:");
  Current_Freq = calculate_current_freq(Current_Dist);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer,LOW);
}

void loop() {
  
  new_time = millis();
  new_time_lcd = millis();
  
  if((new_time - old_time) >= Update_Freq)
  {
    
    Current_Dist = sonar.ping_cm();
    old_time = new_time;
    //Serial.println(Current_Dist);
  }
  
  if((new_time_lcd - old_time_lcd) >= Update_LCD)
  {
    if(Current_Dist < 10)
    {
      lcd.setCursor(10,0);
      lcd.print("     ");
      lcd.print((int)Current_Dist);
    }
    else if(Current_Dist >= 10 && Current_Dist < 100)
    {
      lcd.setCursor(10,0);
      lcd.print("    ");
      lcd.print((int)Current_Dist);
    }
    else
    {
      lcd.setCursor(10,0);
      lcd.print("   ");
      lcd.print((int)Current_Dist);
    }
    
    if(Current_Freq < 10)
    {
      lcd.setCursor(10,1);
      lcd.print("     ");
      lcd.print((int)Current_Freq);
    }

    else if(Current_Freq >= 10 && Current_Freq < 100)
    {
      lcd.setCursor(10,1);
      lcd.print("    ");
      lcd.print((int)Current_Freq);
    }
    else
    {
      lcd.setCursor(10,1);
      lcd.print("   ");
      lcd.print((int)Current_Freq);
    }
    old_time_lcd = new_time_lcd;
  } 
  
  // Send results to Serial Monitor
  //Serial.print("Mesafe = ");
  
  if (Current_Dist >= MAX_DISTANCE || Current_Dist <= MIN_DISTANCE) 
  {
    //Serial.println("Out of range");
    if(Current_Dist >= MAX_DISTANCE)
    {
      digitalWrite(LED,LOW);
      digitalWrite(buzzer,LOW);
      //Serial.println(Current_Dist);
    }
    else if(Current_Dist <= MIN_DISTANCE)
    {
      digitalWrite(LED,HIGH);
      digitalWrite(buzzer,HIGH);
      //Serial.println(Current_Dist);
    }
      
  }
  else 
  {
    if(led_state)
    {
      Current_Freq = calculate_current_freq(Current_Dist);
    }
    
    
    //Serial.println(Current_Freq);
    //Serial.println(" cm");
    new_time_2 = millis();
    //Current_Dist = sonar.ping_cm();
    if((new_time_2 - old_time_2) >= Current_Freq)
    {
      
      if(led_state)
      {
        digitalWrite(LED,HIGH);
        digitalWrite(buzzer,HIGH);
        led_state = false;
        //Serial.println("yandım");
      }
      else
      {
        digitalWrite(LED,LOW);
        digitalWrite(buzzer,LOW);
        led_state = true;
        //Serial.println("söndüm");
      }
      old_time_2 = new_time_2;
    }
    
  }
}

float calculate_current_freq(int Current_Dist)
{
  if((MAX_DISTANCE - MIN_DISTANCE) != 0)
  {
    return (((float)(Current_Dist - MIN_DISTANCE)/ (float)(MAX_DISTANCE - MIN_DISTANCE))*(Buzzer_LED_Freq_Max - Buzzer_LED_Freq_Min))+Buzzer_LED_Freq_Min;
  }
  else
  {
    Serial.print("denominator is zero!!");
    return -1;
  }
  
}
