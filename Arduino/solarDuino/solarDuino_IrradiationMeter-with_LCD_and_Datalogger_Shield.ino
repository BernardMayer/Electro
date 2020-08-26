/* Irradiation meter using Solar Cell by Solarduino */

// Note :  Safety is very important when dealing with electricity. We take no responsibilities while you do it at your own risk.
// Note :  Irradiation meter is designed to measure and record the irradiation level for PV system performance check and feasibility study.
// Note :  Irradiation can measure and record (in SD card) instantaneous short circuit current (Isc) of panel, instantaneous Irradiation (W/m2) and daily Irradiation Energy in (W/m2/day)
// Note :  The daily Irradiation Energy is reset everyday or reset upon power outage (power supply to Arduino).
// Note :  The accuracy of the Irradiation is subject to the quality of user's solar cell and current module sensor.
// Note :  The Short Circuit Current of panel is recommended to be slightly lower or near the maximum range of current sensor for better accuracy results.
// Note :  The Irradiation is 100% subject to measured current and temperature coeffecient is neglected as it has almost negligible effect 
// Note :  The temperature coefficient is neglected so that do not need additional temperature probe, furthermore 65 degree celcius under hot sun (40 degree temperature diference from STC) only performing 2.5% difference.
// Note :  The unit provides reasonable accuracy and may not be comparable with other expensive branded and commercial irradiation meter.
// Note :  All credit shall be given to Solarduino.

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


        /* 0- General */

        int decimalPrecision = 2;                   /* decimal places for only for current value shown in LED Display */
    
        /* 1- DC Current & Irradiation */

        int CurrentAnalogInputPin = A1;             // Which pin to measure Current Value
        float mVperAmpValue = 185;                  // If using ACS712 current module : for 5A module key in 185, for 20A module key in 100, for 30A module key in 66
                                                    // If using WCS current module : for 0.25A module key in 7000, for 0.5A module key in 3500, for 1.0A module key in 2000, for 2.0A module key in 1000.
        float moduleMiddleVoltage = 2500;           // key in middle voltage value in mV. For 5V power supply key in 2500, for 3.3V power supply, key in 1650 mV
        float moduleSupplyVoltage = 5000;           // supply voltage to current sensor module in mV, default 5000mV, may use 3300mV 
        float currentSampleRead  = 0;               /* to read the value of a sample*/
        float currentLastSample  = 0;               /* to count time for each sample. Technically 1 milli second 1 sample is taken */
        float currentSampleSum   = 0;               /* accumulation of sample readings */
        float currentSampleCount = 0;               /* to count number of sample. */
        float currentMean ;                         /* to calculate the average value from all samples*/ 
        float finalCurrent ;                        /* the final current reading without taking offset value*/
        float finalCurrent2 ;                       /* the final current reading*/
        float ShortCircuitCurrentSTC = 2.9   ;      // Key in the Short Circuit Current (At STC condition) of your Solar Panel or Solar Cell. Value 9 showing 9.0A Isc Panel.
        float Irradiation = 0.00;                   /* This shows the irradiation level in W/m2.

            /* 1.1 - Offset DC Current */
            int   OffsetRead = 0;                   /* To switch between functions for auto callibation purpose */
            float currentOffset =0.00;              // to Offset deviation and accuracy. Offset any fake current when no current operates. 
                                                    // the offset will automatically done when you press the <SELECT> button on the LCD display module.
                                                    // you may manually set offset here if you do not have LCD shield
            float offsetLastSample = 0;             /* to count time for each sample. Technically 1 milli second 1 sample is taken */
            float offsetSampleCount = 0;            /* to count number of sample. */
      
            /* 1.2 - Average Accumulate Irradiation */
                           
            float accumulateIrradiation = 0;                          /* Amount of accumulate irradiation*/
            unsigned long startMillisIrradiation;                     /* start counting time for irradiation energy */
            unsigned long currentMillisIrradiation;                   /* current counting time for irradiation energy */
            const unsigned long periodIrradiation = 1000;             // refresh every X seconds (in seconds) Default 1000 = 1 second 
            float FinalAccumulateIrradiationValue = 0;                /* shows the final accumulate irradiation reading*/
            
        
        /* 2 - LCD Display  */
        
        #include<LiquidCrystal.h>                                   /*Load the liquid Crystal Library (by default already built-it with arduino solftware)*/
        LiquidCrystal LCD(8,9,4,5,6,7);                             /*Creating the LiquidCrystal object named LCD */
        unsigned long startMillisLCD;                               /* start counting time for LCD Display */
        unsigned long currentMillisLCD;                             /* current counting time for LCD Display */
        const unsigned long periodLCD = 1000;                       // refresh every X seconds (in seconds) in LED Display. Default 1000 = 1 second 

      
        /* 3 - SD memory card shield */
        
        #include<Wire.h>                                            /*Load the library for talking over I2C (by default already built-it with arduino solftware)*/
        #include<SD.h>                                              /*Load the SD card Library (by default already built-it with arduino solftware)*/
        #include<SPI.h>                                             /*Load the SPI of SPI communication Library (by default already built-it with arduino solftware)*/
        #include "RTClib.h"                                         //Must download the library from Tools > manage library > type RTCLib by Adafruit
        RTC_DS1307 RTC;                                             /* initiate Real Time Cloak for DS1307 chip */
        DateTime now;                                               /* initiate time variable*/
        int h = now.hour();                                         /* create h as variable for hour */
        int chipSelect = 10;                                        /* ChipSelect pin based on the SC card shield using SPI communication. By default the pin is 10 on Arduino Uno. This pin cannot be changed */
        File mySensorData;                                          // Variable for working with our file object
        unsigned long startMillisSD;                                /* start counting time for SD Card */
        unsigned long currentMillisSD;                              /* current counting time for SD Card */
        const unsigned long periodSD = 60000;                       // refresh every X seconds (in seconds) Default 60000 = 1 minute 

    
void setup() 

{

        /* 0- General */

        Serial.begin(9600);                               /* In order to see value in serial monitor */

        /* 1.2 - Average Accumulate Irradiation */

        startMillisIrradiation = millis();                /* Record initial starting time for daily irradiation */
        
        /* 2 - LCD Display  */

        LCD.begin(16,2);                                  /* Tell Arduino that our LCD has 16 columns and 2 rows*/
        LCD.setCursor(0,0);                               /* Set LCD to upper left corner to start display*/  
        startMillisLCD = millis();                        /* Record initial starting time for LCD Display refresh rate. */

        /* 3 - SD memory card shield */
            
        pinMode(chipSelect,OUTPUT);                       /*reserve pin 10 as an ouput by default*/
        RTC.begin();
        Wire.begin();
        SD.begin(chipSelect);                             /* Initialise the SD Card with chipSelect connected to pin 10 by default*/
        startMillisSD = millis();                         /* Record initial starting time for SD card module */
        RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));   /*set the Real Time Cloak to the time same as your PC or Laptop time*/
            
        if(! RTC.isrunning())                             /* if RTC not running give warning in Serial Monitor */
            {
              Serial.println("RTC is not running !");
            }
        mySensorData=SD.open("Irrad.txt",FILE_WRITE);     // Open or create Irrad.txt on the SD card as a file to write to
        if(mySensorData)                                  // only to do these if data file opened sucessfully
        {
          DateTime now = RTC.now();                       /* get time information */
          mySensorData.print("Date");           
          mySensorData.print(",");  
          mySensorData.print("Current (A)");
          mySensorData.print(",");                  
          mySensorData.print("Irradiation (W/m2)");
          mySensorData.print(",");                  
          mySensorData.println("Daily Irradiation (Wh/m2/day)");
          mySensorData.close();                          //close the file
        }


}

void loop() 

{

  /* 0.1- Button Function */
        
              int buttonRead;
              buttonRead = analogRead (0);                                    // Read analog pin A0. By default the LCD Display shield already assigned A0 as button function. Cannot change.

              /*Right button is pressed */
              if (buttonRead < 60) 
              {   LCD.setCursor(0,0); LCD.print ("PRESS <SELECT>   ");      }       
     
              /* Up button is pressed */
              else if (buttonRead < 200) 
              {   LCD.setCursor(0,0); LCD.print ("PRESS <SELECT>   ");      }    
                 
              /* Down button is pressed */
              else if (buttonRead < 400)
              {   LCD.setCursor(0,0); LCD.print ("PRESS <SELECT>  ");       }      
     
              /* Left button is pressed */
              else if (buttonRead < 600)
              {   LCD.setCursor(0,0); LCD.print ("PRESS <SELECT>   ");      } 
     
              /* Select button is pressed */
              else if (buttonRead < 800)
              {   
              OffsetRead = 1;                                                 // to activate offset when button <SELECT> is pressed
              LCD.setCursor(0,0);
              LCD.print ("INITIALIZING..... ");
              LCD.setCursor(0,1);
              LCD.print ("WAIT 5 SEC ..... ");
              }



  /* 1- DC Current & Irradiation */

        if(millis() >= currentLastSample + 1 )                                                                /* every 1 milli second taking 1 reading */
          {
            currentSampleRead = analogRead(CurrentAnalogInputPin)-((moduleMiddleVoltage/moduleSupplyVoltage)*1024);      /* read the sample value */ 
            currentSampleSum = currentSampleSum + currentSampleRead ;                                         /* accumulate value with older sample readings*/  
            currentSampleCount = currentSampleCount + 1;                                                      /* to move on to the next following count */
            currentLastSample = millis();                                                                     /* to reset the time again so that next cycle can start again*/ 
          }
    
        if(currentSampleCount == 1000)                                                                        /* after 1000 count or 1000 milli seconds (1 second), do the calculation and display value*/
          {
            currentMean = currentSampleSum/currentSampleCount;                                                /* calculate average value of all sample readings taken*/
            finalCurrent = (((currentMean /1024)*moduleSupplyVoltage)/mVperAmpValue);                         /* calculate the final current (without offset)*/
            finalCurrent2 = finalCurrent+currentOffset;                                                       /* The final current */
            Irradiation = (finalCurrent2/ShortCircuitCurrentSTC*1000);
            Serial.print(finalCurrent2,decimalPrecision);
            Serial.print(" A  ");
            Serial.print(Irradiation,decimalPrecision);
            Serial.print(" W/m2  ");
            currentSampleSum =0;                                                                              /* to reset accumulate sample values for the next cycle */
            currentSampleCount=0;                                                                             /* to reset number of sample for the next cycle */
          }
         

            /* 1.1 - Offset DC Current */

            if(OffsetRead == 1)  
              {
                currentOffset = 0;                                                             /* set back currentOffset as default first*/
                if(millis() >= offsetLastSample + 1)                                           /* offset 1 - to centralise analogRead waveform*/
                  {                                                                            
                    offsetSampleCount = offsetSampleCount + 1;                                                                          
                    offsetLastSample = millis();                                                                          
                  }   

                   if(offsetSampleCount == 2500)                                             /* need to wait awhile as to get new value before offset take into calculation.  */
                {                                                                             /* So this code is to delay 2.5 seconds after button pressed */
                  currentOffset = - finalCurrent;                                             /* to offset values */
                  OffsetRead = 0;                                                             /* until next offset button is pressed*/                      
                  offsetSampleCount = 0;                                                      /* to reset the time again so that next cycle can start again */ 
                  LCD.setCursor(0,0);
                  LCD.print ("OFFSET.....     ");
                  LCD.setCursor(0,1);
                  LCD.print ("DONE  .....     ");
                }                                                                             
            }    


         /* 1.2 - Average Accumulate Irradiation */
         
         currentMillisIrradiation = millis();                                                                 /* Count the time for current */
        
        if (currentMillisIrradiation - startMillisIrradiation >= periodIrradiation)
        {
            accumulateIrradiation = Irradiation/3600*(periodIrradiation/1000);                                /* for smoothing calculation*/
            FinalAccumulateIrradiationValue =  FinalAccumulateIrradiationValue + accumulateIrradiation ;
            Serial.print(FinalAccumulateIrradiationValue,decimalPrecision); 
            Serial.println(" Wh/m2/day"); 
            startMillisIrradiation = currentMillisIrradiation ;                                               /* Set the starting point again for next counting time */
            DateTime now = RTC.now();
            if((now.hour() >=23) && (now.second()>=58))                                                       /* daily irradiation will reset to 0 at 12am everyday*/
            {                                                                                                 // if do not want to reset everyday, then delete these codes
              FinalAccumulateIrradiationValue =  0;
            }
        }
        
         
        /* 2 - LCD Display  */
    
        currentMillisLCD = millis();
        if (currentMillisLCD - startMillisLCD >= periodLCD)
          {
            LCD.setCursor(0,0);                                                                           /* Set cursor to first colum 0 and second row 1  */
            LCD.print(finalCurrent2,decimalPrecision);                                                    /* display voltage value in LCD in first row  */
            LCD.print(" A   ");
            LCD.setCursor(8,0);  
            LCD.print(Irradiation,0);                                                                     /* display current value in LCD in first row */
            LCD.print(" W/m2   ");  
            LCD.setCursor(0,1); 
            LCD.print(FinalAccumulateIrradiationValue,0);                                                 /* display current value in LCD in first row */
            LCD.print(" Wh/m2/day        "); 
            startMillisLCD = currentMillisLCD ;                                                           /* Set the starting point again for next counting time */
          }

        /* 3 - SD memory card shield */
    
        currentMillisSD = millis();                                                                       /* keep counting time for SD memory function */
        if (currentMillisSD - startMillisSD >= periodSD)                                                  /* if the time difference from begining is 1 minute (by default) do the following code */
          {
            mySensorData=SD.open("Irrad.txt",FILE_WRITE);                                                 //Open Irrad.txt on the SD card as a file to write to
            if(mySensorData)                                                                              //only to do these if data file opened sucessfully
            {
              DateTime now = RTC.now();                                                                   /* get time information */
              mySensorData.print(now.year(),DEC);                                                         /* print the year today */
              mySensorData.print("/");  
              mySensorData.print(now.month(),DEC);                                                        /* print the month today */
              mySensorData.print("/");  
              mySensorData.print(now.day(),DEC);                                                          /* print the date today */
              mySensorData.print(" ");  
              mySensorData.print(now.hour(),DEC);                                                         /* print the hour now */
              mySensorData.print(":");  
              mySensorData.print(now.minute(),DEC);                                                       /* print the minute now */
              mySensorData.print(":");  
              mySensorData.print(now.second(),DEC);                                                       /* print the second now */
              mySensorData.print(",");   
              mySensorData.print(finalCurrent2);                                                          /* print the short circuit current */
              mySensorData.print(",");                  
              mySensorData.print(Irradiation);                                                            /* print the irradiance in W/m2 */
              mySensorData.print(",");                  
              mySensorData.println(FinalAccumulateIrradiationValue);                                      /* print the total daily irradiance in W/m2/day */
              mySensorData.close();                                                                       //close the file
              Serial.println("written to SD Card Successfully !"); 
              startMillisSD = currentMillisSD ; 
            }
        }
   

}
