// DC Voltage Module with Datalogger Shield By Solarduino 

// Note Summary
// Note :  Safety is very important when dealing with electricity. We take no responsibilities while you do it at your own risk.
// Note :  This DC Votlage Sensor Code is for Voltage Divider Method or related DC voltage module use.
// Note :  The value shown in Serial Monitor is refreshed every second.
// Note :  The value is recorded in the SC Card every 5 seconds.
// Note :  The voltage measured is direct measurement based on 1 sample only and is not an average value.
// Note :  No calibration is needed. 
// Note :  The unit provides reasonable accuracy and may not be comparable with other expensive branded and commercial product.
// Note :  All credit shall be given to Solarduino.

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/////////////*/


/* 1 - DC Voltage Measurement */

    int analogInputPin = A2;                    // This is the analog input pin number in arduino for voltage sensing.  
    float vArduino = 0.0;                       /* This is the value voltage sensed by arduino in 0-5 volt */ 
    float vActual = 0.0;                        /* This is the actual voltage that being measured or monitored*/
    float R1 = 30000.0;                         // This is the resistance (in ohm) of R1. You may change this value based on the resistor you use. 
    float R2 = 7500.0;                          // This is the resistance (in ohm) of R2. You may change this value based on the resistor you use. 
    int rawValueRead= 0;                        /* This is the raw value / analog value detected by Arduino ranges 0 - 1023*/


/* 2 - Data Logger shield */
    
    #include<Wire.h>                            /* Load the library for I2C communication (for RTC module). By default already built-in with Arduino software*/
    #include "RTClib.h"                         // Must download the library from manage library > type RTCLib by Adafruit
    #include<SPI.h>                             /* Load the SPI of SPI communication Library (by default already built-it with arduino softtware)*/
    #include<SD.h>                              /* Load the SD card Library (by default already built-it with arduino solftware)*/
    
    RTC_DS1307 RTC;                             /*Load RTC codes for DS1307 microchip from RTClib.h library*/
    DateTime now;                               /*assign to get time now*/
    
    int chipSelect = 10;                        // Select ChipSelect pin based on the Arduino Board. If using SD Card Shield as shown, the pin is 10 on Arduino Uno
    
    File mySensorData;                          // Variable for working with our file object
    unsigned long startMillisSD;                /* start counting time for SD Card */
    unsigned long currentMillisSD;              /* current counting time for SD Card */
    const unsigned long periodSD = 5000;        // refresh every X seconds (in seconds) Default 5000 = 5 seconds 


void setup()
{
     
      /* 1 - DC Voltage Measurement */
      
      pinMode(analogInputPin, INPUT);             /* Declare analog pin as an input*/
      Serial.begin(9600);                         /* Initialise the Serial Monitor function. The Serial Monitor available at 9600 baud rate*/
      Serial.println("DC VOLTMETER");             /* Describe the function code displayed in Serial Monitor*/
  
  
      /* 2 - Data Logger shield */
      
      pinMode(chipSelect,OUTPUT);                       /* define pin 10 as an ouput by default*/
      RTC.begin();                                      /* define clock begin*/
      Wire.begin();                                     /* define I2C befin*/
      SD.begin(chipSelect);                             /* Initialise the SD Card with chipSelect connected to pin 10 by default*/
      startMillisSD = millis();                         /* Start counting time for SD card*/
      RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));   // set the Real Time Cloak to the time same as your PC or Laptop time.
                                                        // when this code is uploaded to the Arduino, it will auto check the time. Just do it once and it will have battery and memory to keep going on.
      if(! RTC.isrunning())                             /* if RTC not running give warning in Serial Monitor */
      {
        Serial.println("RTC is not running !");
      }
    
}


void loop()

{
      /* 1 - DC Voltage Measurement */
      
      rawValueRead = analogRead(analogInputPin);        /* Read and collect sensor from analog input pin in raw data (0-1023) values */
      vArduino = (rawValueRead * 5.0) / 1024.0;         /* Convert the raw data value into 0 - 5V measurable voltage at analog input pin*/
      vActual = vArduino / (R2/(R1+R2));                /* Calculate the expected monitoring voltage in full voltage*/
      
      Serial.print("Vdc = "); 
      Serial.println(vActual,2);                        /* Print out the result in Serial Monitor, in 2 decimal places*/
      delay(1000);                                      /* Delay or pause for 1.000 seconds*/


      /* 2 - Data Logger shield */

      currentMillisSD = millis();
      if (currentMillisSD - startMillisSD >= periodSD)
        {
          mySensorData=SD.open("SOLARD.txt",FILE_WRITE);                                //Open OR Create SOLARD.txt on the SD card as a file to write to
          if(mySensorData)                                                              /*only to do these if data file opened sucessfully*/
            {
              DateTime now = RTC.now();                                                 /* Get the time now from the SD Card module */
              mySensorData.print(now.year(),DEC);
              mySensorData.print("/");  
              mySensorData.print(now.month(),DEC);
              mySensorData.print("/");  
              mySensorData.print(now.day(),DEC);
              mySensorData.print(" ");  
              mySensorData.print(now.hour(),DEC);
              mySensorData.print(":");  
              mySensorData.print(now.minute(),DEC);
              mySensorData.print(":");  
              mySensorData.print(now.second(),DEC);
              mySensorData.print(",");                                                  /* print out comma to seperate between values when export to excel format*/
              
              mySensorData.println(vActual,2);                                          /* print value*/
  
              mySensorData.close();                                                     // close the file
              Serial.println("written to SD Card !");                                   // Display in serial monitor that SD Card is written successfully
              startMillisSD = currentMillisSD ;                                         // Start the counting time again
            }
          }

    
}
