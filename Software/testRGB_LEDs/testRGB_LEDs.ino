#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 1

#define LED_DATA   13
#define LED_CLK   10
#define LED_LATCH   6
#define LED_OFF  -1  // set to -1 to not use the enable pin (its optional)
#define D7_LED 11

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, LED_CLK, LED_DATA, LED_LATCH);

// array holding RGB LED's pin mapping
// {D1_R, D1_G, D1_B, D2_R, D2_G, D2_B, ..., D6_B}
int LED_pins[18] = {5, 9, 8, 4, 11, 10, 3, 13, 12, 2, 15, 14, 1, 17, 16, 0, 19, 18};

int R_value = 127 * 4095 / 255;
int G_value = 0 * 4095 / 255;
int B_value = 255 * 4095 / 255;

double SCALAR = 4.15;

void setup() {
  Serial.begin(9600);
  
  Serial.println("TLC5974 test");
  tlc.begin();
  if (LED_OFF >= 0) {
    pinMode(LED_OFF, OUTPUT);
    digitalWrite(LED_OFF, LOW);
  }
  //setWhite();
  pinMode(D7_LED, OUTPUT);
}

void loop() {
  SCALAR = Serial.parseFloat();
  Serial.println(SCALAR);
  digitalWrite(D7_LED, LOW);
  setWhite();
  delay(1000);
  digitalWrite(D7_LED, HIGH);
  delay(1000);
}

void setWhite() {
  
/*  int R_scaled = R_value * scalar;
  int G_scaled = G_value * scalar;
  int B_scaled = B_value * scalar;*/

  double scalar = SCALAR;
  
  for(int i = 0; i < 18; i+=3) {
    int R_val = R_value*scalar;
    int G_val = G_value*scalar;
    int B_val = B_value*scalar;
    
    tlc.setPWM(LED_pins[i],  max(0, min(1000, R_val)));
    tlc.setPWM(LED_pins[i+1], max(0, min(1000, G_val)));
    tlc.setPWM(LED_pins[i+2], max(0, min(1000, B_val)));
    scalar--;
    
  }
  tlc.write();
}
