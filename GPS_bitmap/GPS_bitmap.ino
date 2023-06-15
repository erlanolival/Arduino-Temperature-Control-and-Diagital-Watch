/*
 #01 GND -> GND
 #02 VCC -> VCC (5V)
 #03 SCL -> D13/SCK
 #04 SDA -> D11/MOSI
 #05 RES -> D8 or any digital
 #06 DC  -> D7 or any digital
 #07 BLK -> NC
*/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TFT_DC    7
#define TFT_RST   8 
#define SCR_WD   240
#define SCR_HT   240   // 320 - to allow access to full 240x320 frame buffer

Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);

#include "leadservice_logo.h"

uint16_t colorBar[50];

int upButton = 5; //pin 5 used to change menu upwards
int downButton = 6; //pin 6 used to change menu downwards
int selectButton = 12; //pin 12 used to select the option desired
int menu = 1;

#define ONE_WIRE_BUS 2
#define TEMP_X  20
#define TEMP_Y  85

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float previousTemp = -100.0;
float tempF = 0;
float minTemp = 200;
float maxTemp = 0;

// initial Time display is 12:59:45 PM
int h=12;
int m=59;
int s=45;
int flag=1; //PM

// Time Set Buttons
int button1;
int button2;
int button3;

// For accurate Time reading, use Arduino Real Time Clock and not just delay()
static uint32_t last_time, now = 0; // RTC

void setup() 
{
  Serial.begin(9600);
  lcd.init(SCR_WD, SCR_HT);
  lcd.fillScreen(BLACK);
  lcd.setCursor(60, 120);
  lcd.setTextColor(WHITE);
  lcd.setTextSize(3);
  lcd.println("Welcome");
  delay(3000);
  lcd.fillScreen(WHITE);
  int i,j;

  for(i=0;i<25;i++) {
    colorBar[i]    = RGBto565(i*256/25,0,i*256/25);
    colorBar[i+25] = RGBto565((24-i)*255/25,0,(24-i)*255/25);
  }
  for(i=0;i<240;i++) {
    lcd.drawImage(i,0,1,50,colorBar);
    lcd.drawImage(i,240-50,1,50,colorBar);
  }
  for(i=50;i<240-50;i++) {
    lcd.drawImage(0,i,50,1,colorBar);
    lcd.drawImage(240-50,i,50,1,colorBar);
  }
  delay(500);
  lcd.drawImageF(65,100,95,54,leadservice);
  delay(1000);

  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  now=millis(); // read RTC initial value
  updateMenu();
}

void loop() {
  if (!digitalRead(downButton)){
    menu++;
    Serial.println("Menu downButton value is:");
    Serial.println(menu);
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)){
    menu--;
    updateMenu();
    delay(100);
    while(!digitalRead(upButton));
  }
  if (!digitalRead(selectButton)){
    executeAction();
    updateMenu();
    delay(100);
    while (!digitalRead(selectButton));
  }
  Serial.println(menu);
}

void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      Serial.println("Case 0");
      break;
    case 1:
      Serial.println("Case 1");
      lcd.clearScreen();
      lcd.setTextColor(RED);
      lcd.setTextSize(3);
      lcd.setCursor(0, 20);
      lcd.print(">Temperature");
      lcd.setCursor(0, 50);
      lcd.print(" Watch");
      lcd.setCursor(0, 80);
      lcd.print(" Contacts");
      lcd.setCursor(0, 110);
      lcd.print(" Settings");
      break;
    case 2:
      lcd.clearScreen();
      lcd.setTextColor(RED);
      lcd.setTextSize(3);
      lcd.setCursor(0, 20);
      lcd.print(" Temperature");
      lcd.setCursor(0, 50);
      lcd.print(">Watch");
      lcd.setCursor(0, 80);
      lcd.print(" Contacts");
      lcd.setCursor(0, 110);
      lcd.print(" Settings");
      Serial.println("Case 2");
      break;
    case 3:
      lcd.clearScreen();
      lcd.setTextColor(RED);
      lcd.setTextSize(3);
      lcd.setCursor(0, 20);
      lcd.print(" Temperature");
      lcd.setCursor(0, 50);
      lcd.print(" Watch");
      lcd.setCursor(0, 80);
      lcd.print(">Contacts");
      lcd.setCursor(0, 110);
      lcd.print(" Settings");
      Serial.println("Case 3");
      break;
    case 4:
      lcd.clearScreen();
      lcd.setTextColor(RED);
      lcd.setTextSize(3);
      lcd.setCursor(0, 20);
      lcd.print(" Temperature");
      lcd.setCursor(0, 50);
      lcd.print(" Watch");
      lcd.setCursor(0, 80);
      lcd.print(" Contacts");
      lcd.setCursor(0, 110);
      lcd.print(">Settings");
      Serial.println("Case 4");
      break;
    case 5:
      menu = 4;
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 1:
      temperature();
      break;
    case 2:
      watch();
      break;
    case 3:
      contacts();
      break;
    case 4:
      settings();
      break;
  }
}

void temperature() {
  lcd.clearScreen();
  Serial.println("Starting up ...");
  sensors.begin();
  
  lcd.init(SCR_WD, SCR_HT);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0, 0);
  lcd.setTextColor(RED);  
  lcd.setTextSize(3);
  lcd.println(" Temperature");
  lcd.setCursor(160, TEMP_Y);
  lcd.setTextSize(5);
  lcd.setTextColor(WHITE);  
  lcd.println((char)247 );
  lcd.setCursor(200, TEMP_Y);
  lcd.println("F");

  lcd.setCursor(40, 180);
  lcd.setTextSize(2);
  lcd.setTextColor(CYAN); 
  lcd.println("MIN");
  printMinTempDegreesSymbol();
  printMaxTempDegreesSymbol();

  lcd.setCursor(170, 180);
  lcd.setTextSize(2);
  lcd.setTextColor(RED); 
  lcd.println("MAX");
  delay(1500);

  looptemp();
  delay(3000);
}
void watch() 
{
  lcd.clearScreen();
  for ( int j=0 ;j<100 ;j++)
  {
    lcd.setCursor(0,20);
    lcd.print("Time ");
    lcd.setCursor(0,50);
    if(h<10)lcd.print("0");// always 2 digits
    lcd.print(h);
    lcd.print(":");
  
    if(m<10)lcd.print("0");
    lcd.print(m);
    lcd.print(":");
 
    if(s<10)lcd.print("0");
    lcd.print(s);

    if(flag==0) lcd.print(" AM");
    if(flag==1) lcd.print(" PM");
 
    lcd.setCursor(0,80);// for Line 2
    lcd.print("LeadService");


    // improved replacement of delay(1000) 
    // Much better accuracy, no more dependant of loop execution time

    for ( int i=0 ;i<5 ;i++)// make 5 time 200ms loop, for faster Button response
    {

      while ((now-last_time)<200) //delay200ms
      { 
        now=millis();
      }
      // inner 200ms loop
      last_time=now; // prepare for next loop 

      // read Setting Buttons
      button1=digitalRead(upButton);// Read Buttons
      button2=digitalRead(downButton);
      button3=digitalRead(selectButton);
  
      if(  ((button1==0)|(button2==0)) & (button3==0)  )
      {
      // wait until Button released
        while ((button1==0)|(button2==0))
        {
          button1=digitalRead(upButton);// Read Buttons
          button2=digitalRead(downButton);
        }
      }
      else
      {
        if(button1==0)
        {
          h=h+1;
        }

        if(button2==0)
        {
          s=0;
          m=m+1;
        }

        /* ---- manage seconds, minutes, hours am/pm overflow ----*/
        if(s==60)
        {
          s=0;
          m=m+1;
        }
        if(m==60)
        {
          m=0;
          h=h+1;
        }
        if(h==13)
        {
          h=1;
          flag=flag+1;
          if(flag==2) flag=0;
        }


        if((button1==0)|(button2==0))// Update display if time set button pressed
        {
          // Update LCD Display
          // Print TIME in Hour, Min, Sec + AM/PM
          lcd.clearScreen();
          lcd.setCursor(0,20);
          lcd.print("Time ");
          lcd.setCursor(0,50);
          if(h<10)lcd.print("0");// always 2 digits
          lcd.print(h);
          lcd.print(":");
          if(m<10)lcd.print("0");
          lcd.print(m);
          lcd.print(":");
          if(s<10)lcd.print("0");
          lcd.print(s);

          if(flag==0) lcd.print(" AM");
          if(flag==1) lcd.print(" PM");
 
          lcd.setCursor(0,80);// for Line 2
          lcd.print("LeadService");
        }
      }
    }
    Serial.println(button3);
    if (button3==0) break; //if you hold the select button, it comes to the previous screen

  }
  
}
void contacts() 
{
  lcd.clearScreen();
  lcd.setTextColor(CYAN);
  lcd.setTextSize(3);
  lcd.setCursor(0, 20);
  lcd.print(" Carlos");
  lcd.setCursor(0, 50);
  lcd.print(" Flavia");
  lcd.setCursor(0, 80);
  lcd.print(" Marcio");
  lcd.setCursor(0, 110);
  lcd.print(" Ines");
  delay(1500);
}
void settings() {
  lcd.clearScreen();
  lcd.setCursor(0, 110);
  lcd.print(">Executing #4");
  delay(1500);
}

void looptemp() {

    delay(1000);
    sensors.requestTemperatures();
    Serial.print("Temperature for the device 1 (index 0) is: ");
    previousTemp = tempF;
    tempF = sensors.getTempFByIndex(0); 
    if(tempF<minTemp)
    {
      deleteMinTemp();
      minTemp = tempF; 
    }
    if(tempF>maxTemp)
    {
      deleteMaxTemp();
      maxTemp = tempF; 
    }
    if(previousTemp!=tempF)
    {
       deletePreviousTemp();
       printTemp();
       printMinTemp();
       printMaxTemp();
    }
}

void deletePreviousTemp()
{
  lcd.setCursor(TEMP_X, TEMP_Y);
  lcd.setTextSize(5);
  lcd.setTextColor(BLACK);
  lcd.println(previousTemp,1);
}

void printTemp()
{
  lcd.setCursor(TEMP_X, TEMP_Y);
  lcd.setTextSize(5);
  lcd.setTextColor(WHITE);
  lcd.println(tempF,1);
}

void printMinTemp()
{
  lcd.setCursor(10, 210);
  lcd.setTextSize(2);
  lcd.setTextColor(CYAN);
  lcd.println(minTemp,1);
}

void printMaxTemp()
{
  lcd.setCursor(150, 210);
  lcd.setTextSize(2);
  lcd.setTextColor(RED);
  lcd.println(maxTemp,1);
}

void deleteMaxTemp()
{
  lcd.setCursor(150, 210);
  lcd.setTextSize(2);
  lcd.setTextColor(BLACK);
  lcd.println(maxTemp,1);
}

void deleteMinTemp()
{
  lcd.setCursor(10, 210);
  lcd.setTextSize(2);
  lcd.setTextColor(BLACK);
  lcd.println(minTemp,1);
}

void printMinTempDegreesSymbol()
{
  lcd.setCursor(70, 210);
  lcd.setTextSize(2);
  lcd.setTextColor(CYAN);
  lcd.println((char)247 );
  lcd.setCursor(85, 210);
  lcd.println("F");
}

void printMaxTempDegreesSymbol()
{
  lcd.setCursor(210, 210);
  lcd.setTextSize(2);
  lcd.setTextColor(RED);
  lcd.println((char)247 );
  lcd.setCursor(225, 210);
  lcd.println("F");
}

