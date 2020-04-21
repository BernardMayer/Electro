// DC Current Sensor with LCD Display By Solarduino 

// Note Summary
// Note :  Safety is very important when dealing with electricity. We take no responsibilities while you do it at your own risk.
// Note :  This DC Current Sensor Code is for ACS712 current module and Hall effect split core current transformer use.
// Note :  The value shown in Serial Monitor / LCD Display is refreshed every second and is the average value of 1000 sample readings.
// Note :  The current measured is the average value.
// Note :  The auto calibration (currentOffset) is using final value as reference.
// Note :  Press the SELECT button of the LCD Display Shield for auto calibration function.
// Note :  The unit provides reasonable accuracy and may not be comparable with other expensive branded and commercial product.
// Note :  All credit shall be given to Solarduino.

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/////////////*/


        /* 0- General */

        int decimalPrecision = 2;                      // decimal places for all values shown in LED Display & Serial Monitor
    

        /* 1- DC Current Measurement */
   
        int CurrentAnalogInputPin = A1;             // Which pin to measure Current Value (A0 is reserved for LCD Shield buttons function)
        float mVperAmpValue = 185;                  // If using ACS712 current module : for 5A module key in 185, for 20A module key in 100, for 30A module key in 66
                                                    // If using "Hall-Effect" Current Transformer, key in value using this formula: mVperAmp = maximum voltage range (in milli volt) / current rating of CT
                                                    /* For example, a 20A Hall-Effect Current Transformer rated at 20A, 2.5V +/- 0.625V, mVperAmp will be 625 mV / 20A = 31.25mV/A */
        float moduleMiddleVoltage = 2500;           /* when there is no reading, the voltage is at 2500mV;*/
        float moduleSupplyVoltage = 5000;           /* supply voltage to current sensor module, default 5000mV*/
        float currentSampleRead  = 0;               /* to read the value of a sample*/
        float currentLastSample  = 0;               /* to count time for each sample. Technically 1 milli second 1 sample is taken */
        float currentSampleSum   = 0;               /* accumulation of sample readings */
        float currentSampleCount = 0;               /* to count number of sample. */
        float currentMean ;                         /* to calculate the average value from all samples*/ 
        float finalCurrent ;                        /* the final current reading without adding offset value*/
        float finalCurrent2 ;                       /* the final current reading*/


        /* 1.1- DC Current Measurement */
        
              int OffsetRead = 0;                   /* To switch between functions for auto callibation purpose */   
              float currentOffset =0.00 ;           // to Offset deviation and accuracy. Offset any fake current when no current operates. 
                                                    // Offset will automatically callibrate when SELECT Button on the LCD Display Shield is pressed.
                                                    // If you do not have LCD Display Shield, look into serial monitor to add or minus the value manually and key in here.
                                                    // 0.26 means add 0.26A to all current measured value.
              float offsetLastSample = 0;           /* to count time for each sample. Technically 1 milli second 1 sample is taken */
              float offsetSampleCount = 0;          /* to count number of sample. */


        /* 2 - LCD Display  */

        #include<LiquidCrystal.h>                   /*Load the liquid Crystal Library (by default already built-it with arduino solftware)*/
        LiquidCrystal LCD(8,9,4,5,6,7);             /*Creating the LiquidCrystal object named LCD */
        unsigned long startMillisLCD;               /* start counting time for LCD Display */
        unsigned long currentMillisLCD;             /* current counting time for LCD Display */
        const unsigned long periodLCD = 1000;       // refresh every X seconds (in seconds) in LED Display. Default 1000 = 1 second 

 
void setup()                                        /* The Codes only run 1 time only when Arduino started.*/
 
  {

/* 0- General */

    Serial.begin(9600);                             /* In order to see value in serial monitor */
    
/* 2 - LCD Display  */

    LCD.begin(16,2);                                /*Tell Arduino that our LCD has 16 columns and 2 rows*/
    LCD.setCursor(0,0);                             /*Set LCD to upper left corner of display*/  
    startMillisLCD = millis();

      
  }
  
void loop()                                         /* The Codes run repeatly over and over again.*/

  {


        /* 0- General */


              /* 0.1- Button Function */
        
              int buttonRead;
              buttonRead = analogRead (0);                                    // Read analog pin A0. Pin A0 automatically assigned for LCD Display Button function (cannot be changed)

              /*Right button is pressed */
              if (buttonRead < 60) 
              {   LCD.setCursor(0,0); LCD.print ("PRESS <SELECT>   "); }       
     
              /* Up button is pressed */
              else if (buttonRead < 200) 
              {   LCD.setCursor(0,0); LCD.print ("PRESS <SELECT>   "); }    
                 
              /* Down button is pressed */
              else if (buttonRead < 400)
              {   LCD.setCursor(0,0); LCD.print ("PRESS <SELECT>  ");  }      
     
              /* Left button is pressed */
              else if (buttonRead < 600)
              {   LCD.setCursor(0,0); LCD.print ("PRESS <SELECT>   "); } 
     
              /* Select button is pressed */
              else if (buttonRead < 800)
              {   
              OffsetRead = 1;                                                  // to activate offset 
              LCD.setCursor(0,0);
              LCD.print ("INITIALIZING..... ");
              LCD.setCursor(0,1);
              LCD.print ("WAIT 5 SEC ..... ");
              }

    

      
        /* 1- DC Current Measurement */

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
            finalCurrent = (((currentMean /1024) *5000) /mVperAmpValue);                                      /* calculate the final RMS current*/
            finalCurrent2 = finalCurrent + currentOffset;
            Serial.print("The Current value is: ");
            Serial.print(finalCurrent2,decimalPrecision);
            Serial.println(" A ");
            currentSampleSum =0;                                                                              /* to reset accumulate sample values for the next cycle */
            currentSampleCount=0;                                                                             /* to reset number of sample for the next cycle */
          }
          


              /* 1.1 - Offset DC Current */
          
              if(OffsetRead == 1)  
                {
                   currentOffset = 0;                                                                         /* set back currentOffset as default*/
                   if(millis() >= offsetLastSample + 1)                                                       
                      {                                                                            
                        offsetSampleCount = offsetSampleCount + 1;                                                                          
                        offsetLastSample = millis();                                                                          
                      }                                                                             
                      
                   if(offsetSampleCount == 2500)                                                              /* need to wait first offset take into effect.  */
                      {                                                                                       /* So this code is to delay 2.5 seconds after button pressed */
                        currentOffset = - finalCurrent;                                                       /* to offset values */
                        OffsetRead = 0;                                                                       /* until next offset button is pressed*/                      
                        offsetSampleCount = 0;                                                                /* to reset the time again so that next cycle can start again */ 
                      }                                                                             
            } 



   
/* 2 - LCD Display  */

      currentMillisLCD = millis();
      if (currentMillisLCD - startMillisLCD >= periodLCD)
          {
            LCD.setCursor(0,0);                                                                                   /* Set cursor to first colum 0 and second row 1  */
            LCD.print(finalCurrent2,decimalPrecision);                                                             /* display current value in LCD in first row */
            LCD.print("A                        ");  
            LCD.setCursor(0,1);
            LCD.print("                           "); 
            startMillisLCD = currentMillisLCD ;                                                                   /* Set the starting point again for next counting time */
          }



  
    
  }
