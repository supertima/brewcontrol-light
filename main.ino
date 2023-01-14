#include <math.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>                    //  Connect lib to use I2C bus (for display)
#include <SPI.h>                     //  Lib for display

#include <U8g2lib.h>
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

// ADS1115 address is 0x48
// Display address is 

int firstrun = 1;     // Variable to check if brewing function was run first time. It is used to clear chart from any previous results.
int brewState = 0;    // Variable to see, if coffeemaker motor is still running.

float steinhart, starttime = 0; // Variable, which stores time passed from the beginning of brewing. Perhaps worth changing to long long type.

Adafruit_ADS1115 ads;

void setup() {
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT);;  
  pinMode(A2, INPUT); // Input to detect if brewing has started. Motor sensor.
  ads.begin();
  delay(100);
  ads.setGain(GAIN_ONE);
  delay(100);
  u8g2.begin();

  u8g2.clearBuffer();  
  u8g2.setFont(u8g2_font_6x12_t_cyrillic);
  u8g2.drawUTF8(0, 7, "Нагрев");
  u8g2.drawUTF8(63, 7, "Время");  
  u8g2.updateDisplay();

  Serial.begin(9600);
  
  // Here will be the code to print display basics. TBD
  
}


void loop() {

  // The main script basically prints sensors data and constantly checks if coffeemaker pump is running or not.

  firstrun = 1;  // 1 is set for the next IF script to run and clear chart before plotting
  
  starttime = (millis());        // Let's remember the exact time we checked the pump
  checkpump();

  while(brewState > 0){    // A-HA! We detected, that coffeemaker pump is running, meaning that we'd better start plotting sensors data on a chart
    
     if (firstrun > 0) {         // Let's check if it is first time, when pump was activated.
        firstrun = 0;            // Variable is set to 0, so on next run this script is gonna be skipped.
        // This block is for cleaning chart are from previous results. TBD  
     }
    
    brewtime(); // Call a brew time print function
    // sensordata(); // Call a temperature print function
    checkpump(); // Check if pump is still running.
   
  }
  
  sensordata();  // Print temperature data in stand-by.
}


void brewtime() {                             // Print Timer Data
  u8g2.setDrawColor(0);
  u8g2.drawBox(62,9,64,23);
  u8g2.setDrawColor(1);  
  u8g2.setCursor(62, 32);
  u8g2.setFont(u8g2_font_logisoso18_tn);
  u8g2.print((millis() - starttime)/1000,1);
  u8g2.updateDisplayArea(7,1,8,6);
}


void sensordata() {                           // Function to get the group temperature

//  float adc0 = ads.readADC_SingleEnded(0);
//  float u0 = float(adc0) * 0.125 / 1000.0;
//  float R0 = ((3.3-u0)*28700)/u0;
//  steinhart = R0 / 30000;                     // (R/Ro)
//  steinhart = log(steinhart);                 // ln(R/Ro)
//  steinhart /= 3964;                          // 1/B * ln(R/Ro)
//  steinhart += 1.0 / (25 + 273.15);           // + (1/To)
//  steinhart = 1.0 / steinhart;                // инвертируем
//  steinhart -= 273.15;                        // конвертируем в градусы по Цельсию
  
  steinhart = 92.34;

  u8g2.setDrawColor(0);
  u8g2.drawBox(0,9,63,23);
  u8g2.setDrawColor(1);  
  u8g2.setCursor(0, 32);
  u8g2.setFont(u8g2_font_logisoso18_tn);
  u8g2.print(steinhart,1);
  u8g2.updateDisplayArea(0,1,8,6);

}


// void checkpump2() {                            // Function to check if brewing is going on
// brewState = 0;
//  int sensorread = 0;   
//  for (int i = 0; i < 30; i++) {
//    if (analogRead(A2) > 1000) sensorread++;
//  }
//  if (sensorread > 0) {
//    brewState = 1;
//  }
//
//  Serial.println(brewState);
//}

void checkpump() {                            // Function to check if brewing is going on
  brewState = 0;
  int sensorread = 0;
  
  for (int i = 0; i < 30; i++) {
    if (digitalRead(2) == 1) sensorread++;
  }
  if (sensorread > 20) {
    brewState = 1;
  }

  Serial.println(digitalRead(6));
}
