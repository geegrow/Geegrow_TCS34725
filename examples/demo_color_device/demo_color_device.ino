#include <Geegrow_TCS34725.h>

#define R_PIN   11
#define G_PIN   10
#define B_PIN   9

Geegrow_TCS34725* color_dev;

void setup() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  color_dev = new Geegrow_TCS34725(TCS34725_I2C_ADDRESS, RN_ATIME_INTEG_TIME_154, RN_CONTROL_GAIN_1X);

  /* Auto-calibration */
  color_dev->calibrate();

  /* Manual calibration */
//  RGBC_value_t arr[4] = {
//    {3300, 3400, 3300, 10000},
//    {1800, 2300, 1900, 6000},
//    {290, 400, 300, 1000},
//    {150, 180, 160, 500},
//  };
//  color_dev->calibrateManual(arr, 4);
}

void loop() {
  // put your main code here, to run repeatedly:
  int16_t red = 0, green = 0, blue = 0, cl = 0;
  
//  color_dev->getRawData(red, green, blue, cl);
//  Serial.print("R: "); Serial.print(red);
//  Serial.print(" G: "); Serial.print(green);
//  Serial.print(" B: "); Serial.print(blue);
//  Serial.print(" Clear: "); Serial.print(cl);
//  Serial.println();

  color_dev->getRGB_255(red, green, blue);
  Serial.print("R: "); Serial.print(red);
  Serial.print(" G: "); Serial.print(green);
  Serial.print(" B: "); Serial.print(blue);
  Serial.print(" "); Serial.print(red, HEX);Serial.print(green, HEX);Serial.print(blue, HEX);
  Serial.println();
  setColor(red, green, blue);
}

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(R_PIN, r);
  analogWrite(G_PIN, g);
  analogWrite(B_PIN, b);
}
