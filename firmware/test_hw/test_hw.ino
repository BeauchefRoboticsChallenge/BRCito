#define DEBUG true


#include "DebugUtils.h"
#include <Adafruit_NeoPixel.h>
#include <QTRSensors.h>

#define LED_PIN 9
#define BUTTON_PIN 7
#define QTR_R 3
#define QTR_L 2

#define ML_IN1 13
#define ML_IN2 10
#define MR_IN1 11
#define MR_IN2 12
#define ML_PWM 6
#define MR_PWM 5

#define MD_STDBY 4


Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1,LED_PIN,NEO_GRB+NEO_KHZ800);
QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

void setup() {
  // serial
  Serial.begin(115200);
  // led
  pixel.begin();
  // boton
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // sensores
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){2,3,A0,A1,A2,A3,A4,A5}, 8);

  // motores
  pinMode(ML_IN1,OUTPUT);
  pinMode(ML_IN2,OUTPUT);
  pinMode(MR_IN1,OUTPUT);
  pinMode(MR_IN2,OUTPUT);
  pinMode(ML_PWM,OUTPUT);
  pinMode(MR_PWM,OUTPUT);
  pinMode(MD_STDBY,OUTPUT);
}

void loop() {
  //testButton();
  testLed();
  //testSensors();
  //testMotors();

  //delay(50);
}

void testMotors(){
  int boton = digitalRead(BUTTON_PIN);

  digitalWrite(ML_IN1,HIGH);
  digitalWrite(ML_IN2,LOW);
  digitalWrite(MR_IN1,LOW);
  digitalWrite(MR_IN2,HIGH);

  if(boton == 0){
    digitalWrite(MD_STDBY,HIGH);
  }
  else {
    digitalWrite(MD_STDBY,LOW);
  }
  analogWrite(ML_PWM,255);
  analogWrite(MR_PWM,255);

  delay(100);

}

void testButton(){

  int boton = digitalRead(BUTTON_PIN);
  DEBUG_PRINTLN(boton);

}

void testLed(){

/*
  uint32_t color = pixel.Color(255,0,255); // magenta
  pixel.setPixelColor(0,color);
  pixel.show();
*/
  PlasmaPulse(30);

}


void testSensors(){
  // read raw sensor values
  qtr.read(sensorValues);

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();

  delay(100);

}

void PlasmaPulse(uint8_t wait) {
  uint16_t i, j;
  uint8_t brightness = 10;

  for(i=0; i<pixel.numPixels(); i++) {
    pixel.setPixelColor(i, pixel.Color(255, 255, 255));
  }
  pixel.show();
  delay(wait);
  //Adjust 60 and 90 to the starting and ending colors you want to fade between.
  for(j=170; j>=135; --j) {
    for(i=0; i<pixel.numPixels(); i++) {
      pixel.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixel.show();
    brightness -= 6;
    pixel.setBrightness(brightness);
    delay(wait);
  }

  for(j=135; j<1170; j++) {
    for(i=0; i<pixel.numPixels(); i++) {
      pixel.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixel.show();
    brightness += 6;
    pixel.setBrightness(brightness);
    delay(wait);
  }

  for(i=0; i<pixel.numPixels(); i++) {
    pixel.setPixelColor(i, pixel.Color(255, 255, 255));
  }
  pixel.show();
  delay(wait);

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
