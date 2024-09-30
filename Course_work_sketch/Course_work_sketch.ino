#include <LiquidCrystal.h>
#define BEGIN A3
#define END A4
#define GREEN A5
#define YELLOW 13
#define RED A2

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int counted_minutes = 0; int counted_seconds = -1;
int minutes = 0; int seconds = 0;
int entered_minutes_FD = -3; 
int entered_minutes_SD = -3;
int entered_seconds_FD = -3; 
int entered_seconds_SD = -3;
bool correct = false; int state = -3;

bool condition = false;
enum Keypad
{
  K1 = A0, K2 = A1, K3 = 2, KA = 3, KB = 4, KC = 5, KD = 6
};
int keypad[4][3] 
{
  {1, 2, 3},
  {4, 5, 6},
  {7, 8, 9},
  {-1, 0, -2}
};
int rows[] {Keypad::KA, Keypad::KB, Keypad::KC, Keypad::KD};
int columns[] {Keypad::K1, Keypad::K2, Keypad::K3};
int scan_array()
{
  for(int col = 0; col < 3; col++)
  {
    digitalWrite(columns[col], LOW);
    for(int row = 0; row < 4; row++)
    {
      if(digitalRead(rows[row]) == LOW)
      {
        // Serial.print("keypad[][] = ");
        // Serial.println(keypad[row][col]);
        return keypad[row][col];
      }
    }
    digitalWrite(columns[col], HIGH);
  }
  // Serial.println(-3);
  return -3;
}
int enter(int digit, char choice, char c_digit) // c_digit(s/f) 
                                                // choice(s/m)
{
  correct = false;
  while(!(correct))
  {
    delay(300);
    lcd.clear();

    lcd.setCursor(0, 0);
    if (choice == 's')
      lcd.print("Seconds: ");
    else if (choice == 'm')
      lcd.print("Minutes: ");
    
    if (c_digit == 's')
    {
      lcd.setCursor(11, 0);
      if(choice == 's')
        lcd.print(entered_seconds_FD);
      if(choice == 'm')
        lcd.print(entered_minutes_FD);
    }
    while(digit == -3)
    {
      digit = scan_array();
      if (digit == -1 || digit == -2 || (c_digit == 's' && digit > 5))
        {digit = -3; delay(300);}
    }
    if(c_digit == 'f')
      lcd.setCursor(11, 0);
    if(c_digit == 's')
      lcd.setCursor(10, 0);
    lcd.print(digit);
    delay(300);
    while (state == -3)
      state = scan_array();
    if (state != -2)
      digit = -3;
    else 
      correct = true;
    state = -3;
  }
  return digit;
}
void setup() 
{
  Serial.begin(9600);
  //  Start/end buttons
  pinMode(END, INPUT);
  pinMode(BEGIN, INPUT);
  //  LEDs
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  //  Keypad
  pinMode(Keypad::K1, OUTPUT);
  pinMode(Keypad::K2, OUTPUT);
  pinMode(Keypad::K3, OUTPUT);
  pinMode(Keypad::KA, INPUT_PULLUP);
  pinMode(Keypad::KB, INPUT_PULLUP);
  pinMode(Keypad::KC, INPUT_PULLUP);
  pinMode(Keypad::KD, INPUT_PULLUP);
  
  // input a time from keypad
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter interval");
  entered_seconds_FD = enter(entered_seconds_FD, 's', 'f');
  entered_seconds_SD = enter(entered_seconds_SD, 's', 's');
  seconds = entered_seconds_FD + entered_seconds_SD * 10;
  entered_minutes_FD = enter(entered_minutes_FD, 'm', 'f');
  entered_minutes_SD = enter(entered_minutes_SD, 'm', 's');
  minutes = entered_minutes_FD + entered_minutes_SD * 10;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.setCursor(6, 0);
  lcd.print(minutes);
    if(!(minutes))
    {
      lcd.setCursor(7, 0);
      lcd.print(0);
    }
  lcd.setCursor(9, 0);
  lcd.print(":");
  lcd.setCursor(10, 0);
  lcd.print(seconds);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  
  while(digitalRead(END) != HIGH)
  {
    
    if(digitalRead(BEGIN) == HIGH)
    {
      while(digitalRead(END) != HIGH)
      {
        Serial.print("counted time: ");
        Serial.print(counted_minutes);
        Serial.print(":");
        Serial.println(counted_seconds);
        lcd.setCursor(7, 0);
        lcd.print(counted_minutes);
        lcd.setCursor(9, 0);
        lcd.print(":");
        lcd.setCursor(10, 0);
        lcd.print(++counted_seconds);
          if(counted_seconds > 59)
            {
              counted_minutes++;
              counted_seconds = 0;
            }
        delay(1000);     
      }
      break;
    }
  }
  lcd.clear();
  Serial.print("counted ");
  Serial.print(counted_minutes);
  Serial.print(":");
  Serial.println(counted_seconds);
  Serial.print("interval: ");
  Serial.print(minutes);
  Serial.print(":");
  Serial.print(seconds);
  if(counted_seconds == seconds && counted_minutes == minutes)
    digitalWrite(YELLOW, HIGH);
  else if(counted_minutes < minutes || (counted_seconds < seconds && counted_minutes == minutes))
    digitalWrite(GREEN, HIGH);
  else digitalWrite(RED, HIGH);
}

void loop() {} 