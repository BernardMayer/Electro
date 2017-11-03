
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <TFT.h>// Arduino LCD library

#include <Adafruit_BMP085.h>

#define SIZEOFRECORD 8

 // pin definition for the Uno
#define tft_cs 10
#define sd_cs 4
#define dc   9
#define rst  8  
#define BUZ 7

#define FREQBUZ 2500

#define PREF_PRESSURE_MIN 98000
#define PREF_PRESSURE_MAX 102500
#define NORM_PRESSURE 101325

//Nbre de pixel de l'écran
#define W_SCR 160
#define H_SCR 128

//Hauteur des caractère en pixel
#define HEIGHT_CHAR 7

//Ecran
TFT TFTscreen = TFT(tft_cs, dc, rst);

//Capteur de pression et de temperature
Adafruit_BMP085 bmp;

unsigned long lastMeasurementTime = 0;
unsigned long lastRefreshDisplay = 0;
byte heightsToDisplay[W_SCR];

//Tendance de la pression sur 1 et 3 heures
int32_t pressure = 0;
int32_t min_pressure = PREF_PRESSURE_MIN;
int32_t max_pressure = PREF_PRESSURE_MAX;
byte norm_pressure_i = 0;

//Liste des échelles en nombre de seconde par pixel horizontal
prog_uint8_t scales[] PROGMEM = {3, 6, 12, 24, 48, 72, 96, 120}; //Correspond à la visualisation de 3H, 6H, 12H, 24H, 48H, 72H}
volatile byte iscale = 0;

volatile unsigned long lastAlarm = 0;
volatile bool isAlarmOn = false;
volatile bool isAlarmAck = false;

volatile unsigned long lastButtonAction = 0;
volatile boolean isScaleChanged = false;

void setup () {
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  //Serial.begin(9600);
  
  if(!SD.begin(sd_cs)){
    //Serial.println("Erreur 1");
    return;
  }
  
  bmp.begin();
  // initialize the display
  TFTscreen.begin();
  pinMode(BUZ, OUTPUT);
  
  pressure = bmp.readPressure();
  
  // clear the screen with a pretty color
  TFTscreen.background(0,0,0); 
  updateDataToDisplay();
  displayDisplayedPeriod();
  
  //Bouton géré par interruption
  attachInterrupt(0, buttonActionPerformed, RISING);
}

void loop () {
  
  unsigned long gap = millis() - lastMeasurementTime;
  //Mise à jour de la pression chaque seconde
  if(gap >= 1000){
    lastMeasurementTime += gap;
    
    updatePressure();
    computePressureTrend();
    appendPressureInHistoric();
  }
  
  //Mise à jour du graphe chaque minute
  gap = millis() - lastRefreshDisplay;
  if(gap > 60000){
    lastRefreshDisplay += gap;
    updateDataToDisplay();
  }
  
  //Si un changement d'échelle a eu lieu, on fait le rafraichissement de l'écran
  //Dans le thread principal (si on peut parler de thread ...)
  if(isScaleChanged){
    isScaleChanged = false;
    updateDataToDisplay();
    displayDisplayedPeriod();
  }
}

void computePressureTrend(){
  File historicFile = SD.open("historic.txt", FILE_WRITE);
  if(!historicFile){
    //Serial.println("1");
    return;
  }
  
  long fileSize = historicFile.size();
  long pos = fileSize - (long)10800* (long)SIZEOFRECORD;
  
  historicFile.seek(pos);
  
  char pressureArray[7];
  for(int j = 0; j < 6; j++){
    pressureArray[j] = historicFile.read();
    if(pressureArray[j] == ' '){
      pressureArray[j] = 0;
    }
  }
  pressureArray[6] = 0;
  long pressure3HValue = atol(pressureArray);

  pos = fileSize - (long)1800 * (long)SIZEOFRECORD;
  historicFile.seek(pos);
  
  pressureArray[7];
  for(int j = 0; j < 6; j++){
    pressureArray[j] = historicFile.read();
    if(pressureArray[j] == ' '){
      pressureArray[j] = 0;
    }
  }
  pressureArray[6] = 0;
  long pressure30MinValue = atol(pressureArray);
  historicFile.close();
  
  int oneHourTrend = (int)((pressure - pressure30MinValue)*(long)2);
  int threeHourTrend = (int)(pressure - pressure3HValue);

  String oneHTrendStr = String(oneHourTrend);
  oneHTrendStr += String("Pa/h");
  displayText(oneHTrendStr, 54, 50, 0, 255, 0, 0);
  
  String threeHTrendStr = String(threeHourTrend);
  threeHTrendStr += String("Pa/3h");
  displayText(threeHTrendStr, 54, 105, 0, 255, 0, 0);
  
  unsigned long gaplastAlarm = millis() - lastAlarm;
  if(oneHourTrend < (int)-140 && !isAlarmOn){
    tone(BUZ, FREQBUZ);
    isAlarmOn = true;
  }
  else if(oneHourTrend >= (int)-140 && !isAlarmAck && isAlarmOn){
    noTone(BUZ);
    isAlarmOn = false;
  }
  else if(isAlarmAck && oneHourTrend >= (int)-140 && isAlarmOn && gaplastAlarm > (unsigned long)10800000){
    isAlarmAck = false;
    isAlarmOn = false;
  }
}

void buttonActionPerformed(){
  //Avant d'executer l'action du bouton, on filtre les rebonds
  if(millis() - lastButtonAction > 300){
    lastButtonAction = millis();
    if(isAlarmOn && !isAlarmAck){
      noTone(BUZ);
      isAlarmAck = true;
      lastAlarm = millis();
    }
    else{
      iscale++;
      if(iscale == 8){
        iscale = 0;
      }
  
      isScaleChanged = true;
    }
  }
}

void displayDisplayedPeriod(){
  byte period = (byte)pgm_read_word_near(scales + iscale);
  String periodTxt = String(period);
  periodTxt += "h";
  displayText(periodTxt, 24, 45, 120, 127, 127, 127);
}

void updatePressure(){
  //Mise à jour de la pression
  long pressureTemp = bmp.readPressure();
  long var;
  if(pressureTemp > pressure){
    var = pressureTemp - pressure;
  }
  else{
    var = pressure - pressureTemp;
  }
  
  //Cela permet de limiter du bruit observé lors du fonctionnement
  if(var < (long)20){
    pressure = pressureTemp;
  }
  
  displayPressure(pressure, 1, 75, 120, 255, 0, 0);
}

void displayText(String msg, int pxlg, int i, int j, byte r, byte g, byte b){
  TFTscreen.stroke(0,0,0);
  for(int k = i; k < i + pxlg; k++){
    TFTscreen.line(k, j, k, j + HEIGHT_CHAR);
  }
  
  TFTscreen.stroke(r,g,b);
  
  char txtmsg[11];
  msg.toCharArray(txtmsg, 11);
  
  TFTscreen.text(txtmsg, i, j);
}

void displayPressure(int32_t pressureval, byte res, byte i, byte j, byte r, byte g, byte b){
  TFTscreen.stroke(0,0,0);
  byte lgth;
  if(res == 0 || res == 1){
    lgth = 48;
  }
  else if(res == 2){
    lgth = 42;
  }
  else{
    lgth = 48;
    res = 0;
  }
  
  for(int k = i; k < i + lgth; k++){
    TFTscreen.line(k, j, k, j + HEIGHT_CHAR);
  }
  
  int32_t resAbs = 1;
  for(int i = 0; i < res; i++){
    resAbs = resAbs * 10;
  }
  
  pressureval = (int32_t)(pressureval + resAbs/2)/resAbs;
  String myString = String(pressureval);
  
  
  String unit = String("Pa");;
  if(res == 1){
    unit = String("dPa");
  }
  else if(res == 2){
    unit = String("hPa");
  }
  
  //myString += String(unit);
  myString += unit;
  char pressureStr[10];
  myString.toCharArray(pressureStr, 10);
  TFTscreen.stroke(r,g,b);
  TFTscreen.text(pressureStr, i, j);
}

void appendPressureInHistoric(){
  char pressureArray[7];
  String pressureStr = String(pressure);
  pressureStr.toCharArray(pressureArray, 7);
  if(pressure < 100000){
    pressureArray[5]=' ';
  }
  pressureArray[6]=0;
 
  File historicFile = SD.open("historic.txt", FILE_WRITE);
  if(!historicFile){
    return;
  }
  
  historicFile.println(pressureArray);
  historicFile.close();
}

void displayEmptyGraph(){
  {
    byte norm_pressure_i_temp = (byte)(((long)NORM_PRESSURE - (long)min_pressure) * (long)(H_SCR - 10 - 10) /((long)max_pressure - (long)min_pressure));
    norm_pressure_i_temp = H_SCR - 10 - 10 - norm_pressure_i_temp;
    if(norm_pressure_i_temp != norm_pressure_i){
      drawHDotLine(10 + norm_pressure_i, 0, 0, 0);
      norm_pressure_i = norm_pressure_i_temp;
    }
    drawHDotLine(10 + norm_pressure_i, 0, 0, 255);
  }
  
  for(int i = 0; i < 4; i++){
    drawHDotLine(10 + i * 36, 127, 127, 127);
    displayPressure(max_pressure - i * (max_pressure - min_pressure)/(long)3, 2, 0, 1 + i * 36, 127, 127, 127);
  }
}

void updateDataToDisplay(){
  
  displayEmptyGraph();
  //Nombre de seconde entre 2 mesures à afficher
  byte scale = (byte)pgm_read_word_near(scales + iscale);
  
  File historicFile = SD.open("historic.txt", FILE_WRITE);
  if(!historicFile){
    return;
  }
  
  uint32_t fileSize = historicFile.size();
  long pos = (long)fileSize - (long)scale * (long)3600 * (long)SIZEOFRECORD;
  long increment = ((long)scale * (long)3600 + (long)W_SCR/(long)2) / (long)W_SCR;
  increment = increment * (long)SIZEOFRECORD;
 
  char pressureArray[7];
  int i = 0;
  int32_t temp_min_pressure = PREF_PRESSURE_MIN;
  int32_t temp_max_pressure = PREF_PRESSURE_MAX;
  
  while(i < W_SCR && pos < (long)fileSize){ 
    //Si il y a de l'historique
    if(pos >= 0){
      historicFile.seek(pos);
      
      for(int j = 0; j < 6; j++){
        pressureArray[j] = historicFile.read();
        if(pressureArray[j] == ' '){
          pressureArray[j] = 0;
        }
      }
      pressureArray[6] = 0;
      
      long pressureValue = atol(pressureArray);
      if(temp_min_pressure > pressureValue){
        temp_min_pressure = pressureValue;
      }
      if(temp_max_pressure < pressureValue){
        temp_max_pressure = pressureValue;
      }
      
      TFTscreen.stroke(0,0,0);
      TFTscreen.point(i, H_SCR-heightsToDisplay[i]);
      
      heightsToDisplay[i] = map(pressureValue, min_pressure, max_pressure, 10, H_SCR - 10);
      
      TFTscreen.stroke(0,255,0);
      TFTscreen.point(i, H_SCR-heightsToDisplay[i]);
      
      i++;
    }
    //Autrement
    else{
      if(heightsToDisplay[i] != 0){
        TFTscreen.stroke(0,0,0);
        TFTscreen.point(i, H_SCR - heightsToDisplay[i]);
      }
      
      heightsToDisplay[i++] = 0;
    }
    pos += increment;
  }
  historicFile.close();
  
  //Les nouvelles valeurs maximum seront prises en compte lors du prochain rafraichissement.
  max_pressure = temp_max_pressure;
  min_pressure = temp_min_pressure;
}

void drawHDotLine(byte j, byte r, byte g, byte b){
  TFTscreen.stroke(r, g, b);
  for(int i = 0; i < W_SCR; i++){
    if(i%3 == 0){
      TFTscreen.point(i, j);
    }
  }
}

