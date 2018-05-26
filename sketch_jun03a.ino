#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET A5
Adafruit_SSD1306 display(OLED_RESET);

//pin settings for arduino nano v3 
// 700nm led = + to 100ohm to 3, - to GND
// 470nm led = + to 4, - to GND
// phototransistor = + to 5V, - to A6 AND 100k resistor A6 to GND
// KY-040 rotary encoder = + to 5V, - to GND, CLK - 10, DT - 9, SW - 8
// SSD1306 OLED = VCC to 5V, GND - GND, SCL - A5, SDA - A4 

//AGE reader setup
const int L700nm = 3; 
const int L470nm = 4;
const int phototransistor = A6; 
//rotary encoder setup
int pinA = 10;//CLK
int pinB = 9; //DT
int pushbutton = 8; //SW
int encoderPosCount = 0;
int pinALast;
int aVal;
boolean bCW;
//phototransistor reading values
int reading1 = 0;
int reading2 = 0;

const unsigned char icon [] PROGMEM = {
0x00, 0x1F, 0xF0, 0x00, 0x00, 0xFF, 0xFE, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x07, 0xF8, 0x7F, 0x80,
0x0F, 0xC4, 0x67, 0xC0, 0x1F, 0xB2, 0x63, 0xE0, 0x1E, 0xF8, 0x20, 0xF0, 0x3D, 0xEF, 0xE0, 0x78,
0x7F, 0xDF, 0xF8, 0x78, 0x7B, 0xFC, 0x7C, 0x3C, 0x77, 0x71, 0x9E, 0x1C, 0xF7, 0xEF, 0xE6, 0x1C,
0xFE, 0xCF, 0xF7, 0x1C, 0xFF, 0xDF, 0xF3, 0x0E, 0xEF, 0xDF, 0xF8, 0x0E, 0xEF, 0x9F, 0xF8, 0x0E,
0xEF, 0x9E, 0x38, 0x0E, 0xFF, 0xDF, 0x33, 0x1E, 0xFE, 0xDF, 0xE7, 0x1E, 0xF7, 0xEF, 0xC6, 0x1C,
0x7F, 0x70, 0x0E, 0x1C, 0x7B, 0xB8, 0x1C, 0x3C, 0x7F, 0xDD, 0x78, 0x78, 0x3F, 0xF6, 0x60, 0x78,
0x1F, 0xFE, 0x00, 0xF0, 0x1F, 0xBF, 0x03, 0xE0, 0x0F, 0xEF, 0x0F, 0xC0, 0x07, 0xFC, 0x7F, 0x80,
0x01, 0xFF, 0xFF, 0x00, 0x00, 0x7F, 0xFC, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00
};




void setup() {
  // put your setup code here, to run once:
  pinMode(L700nm, OUTPUT);
  pinMode(L470nm, OUTPUT);
  pinMode(phototransistor,INPUT_PULLUP);
  digitalWrite(L700nm, LOW);
  digitalWrite(L470nm, LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); //0x3D
  display.clearDisplay();
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pushbutton,INPUT_PULLUP);
  pinALast = digitalRead(pinA);
  Serial.begin(9600);
}

void displaylogo(){
  display.drawBitmap(0, 16, icon, 32, 32, 1);
  display.display();
  delay(100);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(37,20);
  display.println("OICO AGE READER       www.oico.co.uk");
  display.display();
}

void readAgeStart(){
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,5);
  display.println("Enter patient age:");
  display.display();

  while(true){
  aVal = digitalRead(pinA);
  if(aVal!=pinALast){
    if(digitalRead(pinB)!=aVal){
      encoderPosCount ++;
      bCW = true;
      display.clearDisplay();
      display.display();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,5);
      display.println("Enter patient age:");
      display.display();
      display.setTextSize(1.5);
      display.setTextColor(WHITE);
      display.setCursor(30,30);
      display.println(encoderPosCount);
      display.display();
    } else {
      bCW = false;
      encoderPosCount--;
      display.clearDisplay();
      display.display();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,5);
      display.println("Enter patient age:");
      display.display();
      display.setTextSize(1.5);
      display.setTextColor(WHITE);
      display.setCursor(30,30);
      display.println(encoderPosCount);
      display.display();
    }
  }
    if(!digitalRead(pushbutton)){
      takeReading();
    }
}
}

void takeReading(){
  digitalWrite(L700nm, HIGH);
  delay(100);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("click to read AGE");
  display.display();
  while(true){
     if(!digitalRead(pushbutton)){
      digitalWrite(L700nm, LOW);
      delay(50);
      reading1 = analogRead(phototransistor);
      Serial.println(reading1);
      delay(50);
      digitalWrite(L470nm, HIGH);
      delay(50);
      reading2 = analogRead(phototransistor);
      Serial.println(reading2);
      delay(50);
      digitalWrite(L470nm, LOW);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0,20);
      display.println(reading1);
      display.println(reading2);
      display.display();
      delay(4000);
      if(digitalRead(pushbutton)){
        return;
      }
    }
  }

}


void loop() {
  // put your main code here, to run repeatedly:
  displaylogo();
  if(!digitalRead(pushbutton)){
    readAgeStart();
  }
  
}
