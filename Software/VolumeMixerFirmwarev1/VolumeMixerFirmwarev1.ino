#include "Adafruit_TLC5947.h"

// pin definitions for Class D Amp (TPA2017)
#define AMP_ENABLE    2
#define AMP_CONFIG_1  7
#define AMP_CONFIG_2  5

// pin definitions for RGB LED Controller (TLC5947)
#define NUM_TLC5974   1
#define LED_DATA      13
#define LED_CLK       10
#define LED_LATCH     6
#define LED_OFF       12  

// pin definition for on-board LED
#define D7_LED        11

/********** VOLUME MIXER DEFINITIONS ************/

/* Pot 1 - Windows Master Volume
 * Pot 2 - Google Chrome
 * Pot 3 - Spotify & Discord
 * Pot 4 - Games
 */

// pin definitions for Volume Level Potentiometers
#define VOL_1         A10
#define VOL_2         A11
#define VOL_3         A7
#define VOL_4         A8

// floats for storing analog read value of potentiometers
// to detect change in volumes
float Vol_1_val = 0.0;
float Vol_2_val = 0.0;
float Vol_3_val = 0.0;
float Vol_4_val = 0.0;

// color definitions for each application [0 - 255]
#define R_VOL_1  220
#define G_VOL_1  185
#define B_VOL_1  0

#define R_VOL_2  76
#define G_VOL_2  139
#define B_VOL_2  245

#define R_VOL_3  30
#define G_VOL_3  215
#define B_VOL_3  30

#define R_VOL_4  184
#define G_VOL_4  58
#define B_VOL_4  15

// var for keeping track of LED timeout
unsigned long delayStart = 0;


// create LED controller object
Adafruit_TLC5947 LED_Controller = Adafruit_TLC5947(NUM_TLC5974, LED_CLK, LED_DATA, LED_LATCH);

/* array holding RGB LED's channel mapping
 * {D1_R, D1_G, D1_B, D2_R, D2_G, D2_B, ..., D6_B} */
int LED_pins[18] = {8, 9, 5, 10, 11, 4, 12, 13, 3, 14, 15, 2, 16, 17, 1, 18, 19, 0};

void setup() {

  // delay at beginning so we can reupload code in case of loop or Serial in use errors
  delay(1000);
  
  Serial.begin(9600);

  // begin LED_Controller & set LED's off initially
  LED_Controller.begin();
  if (LED_OFF >= 0) {
    pinMode(LED_OFF, OUTPUT);
    digitalWrite(LED_OFF, HIGH);
  }

  // set pin mode for on-board LED
  pinMode(D7_LED, OUTPUT);

  // set pin mode for Class D amp pins
  pinMode(AMP_ENABLE, OUTPUT);
  pinMode(AMP_CONFIG_1, OUTPUT);
  pinMode(AMP_CONFIG_2, OUTPUT);

  // mute amp to start and enable all features (limiter, compression, & noise gate)
  digitalWrite(AMP_ENABLE, LOW);
  digitalWrite(AMP_CONFIG_1, LOW);
  digitalWrite(AMP_CONFIG_2, LOW);

  // set volume potentiometer pins to inputs
  pinMode(VOL_1, INPUT);
  pinMode(VOL_2, INPUT);
  pinMode(VOL_3, INPUT);
  pinMode(VOL_4, INPUT);
}

void loop() {

  // print analog read values to Serial for Deej application
  printVolumeValues();

  /* if you detect change on pots, turn on LED light bar
   * and begin timer for LED timeout  */
  switch (detectAnalogChange()) {
    case 1:
      setLEDs(analogRead(VOL_1), R_VOL_1, G_VOL_1, B_VOL_1);
      delayStart = millis();
      break;
    case 2:
      setLEDs(analogRead(VOL_2), R_VOL_2, G_VOL_2, B_VOL_2);
      delayStart = millis();
      break;
    case 3:
      setLEDs(analogRead(VOL_3), R_VOL_3, G_VOL_3, B_VOL_3);
      delayStart = millis();
      break;
    case 4:
      setLEDs(analogRead(VOL_4), R_VOL_4, G_VOL_4, B_VOL_4);
      delayStart = millis();
      break;
    default:
      break;
  }

  // if LEDs have been on 2 sec, turn them off
  if (millis() - delayStart > 2000) {
    digitalWrite(LED_OFF, HIGH);
  }

  // delay for 0.1 sec before checking again
  delay(100);
}

/* if the read analog value has changed by more than 5 (ie 0.5%)
 * then return the number of the pot that has changed for proper LED control
 * & update the read value so we know if it has changed again for next time
 */
int detectAnalogChange() {

  if (abs(analogRead(VOL_1) - Vol_1_val) >= 5) {
    Vol_1_val = analogRead(VOL_1);
    return 1;
  } else if (abs(analogRead(VOL_2) - Vol_2_val) >= 5) {
    Vol_2_val = analogRead(VOL_2);
    return 2;
  } else if (abs(analogRead(VOL_3) - Vol_3_val) >= 5) {
    Vol_3_val = analogRead(VOL_3);
    return 3;
  } else if (abs(analogRead(VOL_4) - Vol_4_val) >= 5) {
    Vol_4_val = analogRead(VOL_4);
    return 4;
  }
  return 0;
}

/* sets the LEDs brightness proportionally to the pot's current
 * reading, provided the proper RGB colors (0 - 255)
 * saturates the brightness level at the defined MAX_PWM (0 - 4095)
 * 
 * using analog_read value, it sets a scalar between 0 and 6 (0 for
 * no LEDs on & 6 for all on at max level. Then, for each LED it sets,
 * it subtracts one from the scalar, so that an LED brightens to full as
 * the volume is turned up and then the next LED begins brightening, and so on.
 */
void setLEDs(float analog_read, int R_val, int G_val, int B_val) {

  // define max brightness [0 - 4095]
  #define MAX_PWM 200.0

  // scalar value calculated for LED light bar effect
  float scalar = (float) analog_read * 5.99 / 1023.0;

  // convert RGB values to PWM [0 - 255] --> [0 - MAX_PWM]
  R_val = (float) R_val * MAX_PWM / 255.0;
  G_val = (float) G_val * MAX_PWM / 255.0;
  B_val = (float) B_val * MAX_PWM / 255.0;

  // go through array and set each LED
  for(int i = 0; i < 18; i+=3) {   
    if (scalar > 1) {
      // if the scalar is greater than 1, LED should be at full brightness
      LED_Controller.setPWM(LED_pins[i], max(0, R_val));
      LED_Controller.setPWM(LED_pins[i+1], max(0, G_val));
      LED_Controller.setPWM(LED_pins[i+2], max(0, B_val));
    } else {
      // otherwise, scale the PWM value down by our scalar value
      float R_intensity = (float) R_val * scalar;
      float G_intensity = (float) G_val * scalar;
      float B_intensity = (float) B_val * scalar;
    
      LED_Controller.setPWM(LED_pins[i], max(0, R_intensity));
      LED_Controller.setPWM(LED_pins[i+1], max(0, G_intensity));
      LED_Controller.setPWM(LED_pins[i+2], max(0, B_intensity));
    }
    scalar = (float) scalar - 1.0;
  }
  // finally write the data and turn the LEDs on
  LED_Controller.write();
  digitalWrite(LED_OFF, LOW);
}

/* create the properly formatted string for the Serial monitor
 * that Deej is designed to read. More info on Deej Github page.
 * format should be "Level 1 | Level 2 | Level 3 | Level 4 ..."
 * each level should be a value [0 - 1024] (the raw analog read value)
 */
void printVolumeValues() {
  // create empty string  
  String builtString = String("");

  // build the string mulitply by 0.75 to cap max volume at 75%
  builtString += String( (int) (analogRead(VOL_1) * 0.75));
  builtString += String("|");
  builtString += String( (int) (analogRead(VOL_2) * 0.75));
  builtString += String("|");
  builtString += String( (int) (analogRead(VOL_3) * 0.75));
  builtString += String("|");
  builtString += String( (int) (analogRead(VOL_4) * 0.75));
  
  // print it to Serial Monitor
  Serial.println(builtString);
}
