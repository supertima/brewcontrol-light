#include <math.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>                    //  Connect lib to use I2C bus (for display)
#include <SPI.h>                     //  Lib for display

int firstrun = 1;       // Variable to check if brewing function was run first time. It is used to clear chart from any previous results.
int brewState = 0;    // Variable to see, if coffeemaker motor is still running.

float steinhart;

Adafruit_ADS1115 ads;

float starttime = 0;    // Variable, which stores time passed from the beginning of brewing. Perhaps worth changing to long long type.

int firstrun = 1;       // Variable to check if brewing function was run first time. It is used to clear chart from any previous results.
int brewState = 0;    // Variable to see, if coffeemaker motor is still running.

void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2, INPUT); // Input to detect if brewing has started.
  ads.begin();
  delay(100);
  ads.setGain(GAIN_ONE);
  delay(100);

}


void loop() {

  // The main script basically prints sensors data and constantly checks if coffeemaker pump is running or not.

  sensordata();  // Call a function, which prints pressure and temperature data in stand-by mode.

  firstrun = 1;  // 1 is set for the next IF script to run and clear chart before plotting
  xPosTemp = 21; // X axis value for the first point of the chart
  xPosPress = 21;
  
  starttime = (millis());        // Let's remember the exact time we checked the pump

  checkpump();

  while(brewState > 0){    // A-HA! We detected, that coffeemaker pump is running, meaning that we'd better start plotting sensors data on a chart
     if (firstrun > 0) {         // Let's check if it is first time, when pump was activated.
        firstrun = 0;            // Variable is set to 0, so on next run this script is gonna be skipped.
        
        // For cleaning chart are from previous results, we basically draw a black rectangle over it. Same goes over timer data.
       
        ucg.setColor(0, 0, 0);
        ucg.drawBox(21, 40, 270, 199);
        ucg.drawBox(217, 10, 45, 18);
        ucg.setColor(255, 255, 255);
       
        // OldTempHeight = map(thermocouple.readCelsius()*100, 0, 10000, 0, 400); // This is to ensure the first segment of the chart will not be drawn from Y=0
     }

    brewtime(); // Call a chart drawing function
    checkpump(); // Chek if pump is still running.

  }
  
}


void brewtime() {

  // This function draws chart and prints sensors data.
  
  sensordata(); // We still need to print actual sensors data, why not to export it to separate function and use when neccessary?

  // Print Timer Data
  ucg.setColor(200, 200, 200); 
  ucg.setPrintPos(217, 25);
  ucg.print((millis() - starttime)/1000);
  
  // Print Temperature chart
  
  int tempHeight = map(steinhart*100, 0, 10000, 0, 400);  // Temperature is from 0 to 100, but virtual chart size for it is 400 pixels in heigth. Here we use map function to adapt data. We mulitply incomming data by 100 as MAP can not process float point numbers.

  ucg.setColor(255, 0, 0);
  ucg.drawLine(xPosTemp, 440-OldTempHeight,(xPosTemp+4), 440-tempHeight);  // The line is drawn from previous temp probe to current. Don't forger that virtual chart size is bigger than real screen, so thats why we have "440-" part.


  xPosTemp = xPosTemp + 4;     // Next point will be 4 pixels right. On my hardware it makes total chart scale equal to nearly 30 seconds, ideal for espresso brewing.
  OldTempHeight = tempHeight;  // Store last Y temperature position for the next cycle.
  

  // Print Pressure chart.

  int pressHeight = map(pressure_group*100, 0, 1200, 0, 200);  // Temperature is from 0 to 100, but virtual chart size for it is 400 pixels in heigth. Here we use map function to adapt data. We mulitply incomming data by 100 as MAP can not process float point numbers.

  ucg.setColor(0, 0, 255);
  ucg.drawLine(xPosPress, 240-OldPressHeight,(xPosPress+4), 240-pressHeight);  // The line is drawn from previous press probe to current. Don't forger that virtual chart size is bigger than real screen, so thats why we have "440-" part.

  xPosPress = xPosPress + 4;     // Next point will be 4 pixels right. On my hardware it makes total chart scale equal to nearly 30 seconds, ideal for espresso brewing.
  OldPressHeight = pressHeight;  // Store last Y temperature position for the next cycle.

}


void sensordata() {                           // Function to get the group temperature
  float adc0 = ads.readADC_SingleEnded(0);
  float u0 = float(adc0) * 0.125 / 1000.0;
  float R0 = ((3.3-u0)*28700)/u0;
  steinhart = R0 / 30000;                     // (R/Ro)
  steinhart = log(steinhart);                 // ln(R/Ro)
  steinhart /= 3964;                          // 1/B * ln(R/Ro)
  steinhart += 1.0 / (25 + 273.15);           // + (1/To)
  steinhart = 1.0 / steinhart;                // инвертируем
  steinhart -= 273.15;                        // конвертируем в градусы по Цельсию
}


void checkpump() {                            // Function to check if brewing is going on

  brewState = 0;
  int sensorread = 0;   
  for (int i = 0; i < 30; i++) {
    if (analogRead(A2) > 1000) sensorread++;
  }
  if (sensorread > 0) {
    brewState = 1;
  }
}
