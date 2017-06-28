#include <TimerOne.h>
#include <TimerThree.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SPI.h>
#include <Gauges.h>
#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>
#include "Logo.h"
#include <Fonts/FreeMonoBoldOblique9pt7b.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeMonoBoldOblique18pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>
#include <Adafruit_GPS.h>
#include <math.h>
#include <Dusk2Dawn.h>
#include <Logger.h>

#define sclk 11       // SCLK can also use pin 14
#define mosi 10       // MOSI can also use pin 7
#define cs   9        // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define dc   2        //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define rst  6        // RST can use any pin
#define oneWireBus 10 //temperature pin (Dallas DS18B20)

#define tempX 110   // x-Pos Temperatur
#define tempY 120   // y-Pos Temperatur
#define timeX 23     // x-Pos Time
#define timeY 37    // y-Pos Time
#define altX 0      // x-Pos Altitude
#define altY 96    // y-Pos Altitude
#define profileX 0  // x-Pos Altitude Profile
#define profileY 81 // y-Pos Altitude Profile
#define profileW 62 // width Altitude Profile
#define profileH 22 // height Altitude Profile
#define speedX 0   // x-Pos Speed
#define speedY 66  // y-Pos Speed
#define dirX   128  // x-Pos direction
#define dirY   100  // y-Pos direction

#define NORTH 0
#define NORTHEAST 45
#define EAST 90
#define SOUTHEAST 135
#define SOUTH 180
#define SOUTHWEST 225
#define WEST 270
#define NORTHWEST 315

#define textColor 0xffff

#define mode_day 0xffff
#define mode_night 0x0000

#define nmea Serial1

//Adafruit_GPS GPS(&nmea);
Adafruit_GPS GPS(&Serial1);

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);
integerGauge Speed = integerGauge(&tft,speedX,speedY,72,30);
textGauge SpeedLabel = textGauge(&tft,speedX+70,speedY-2,30,34);
textGauge Direction = textGauge(&tft,speedX+111,speedY,48,30);
clockGauge Time = clockGauge(&tft,timeX,timeY,113,30);
integerGauge Altitude = integerGauge(&tft,altX,altY,82,24);
integerGauge Temperature = integerGauge(&tft,100,altY,60,24);

//thermometer (DS18B20)
OneWire oneWire(oneWireBus);
DallasTemperature temp(&oneWire);
DeviceAddress Thermometer = {0x28, 0xEE, 0xCA, 0x19, 0x01, 0x16, 0x01, 0xA3};

// Time Zone    (Central European Time / Central European Daylight Savings Time)
TimeChangeRule  CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule  CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone        CE(CEST, CET);
TimeChangeRule  *tcr;                                        //pointer to the time change rule, use to get the TZ abbrev

// global variables
volatile time_t utc;
volatile time_t utc_a;
volatile uint8_t mydir,dir_a;
volatile float mytemp, myalt, myspeed,myangle;
volatile float alt_a, temp_a, speed_a= -1;
volatile int sats, myfix;
volatile int sats_a, fix_a = 32768;
volatile int seconds;
volatile int altitudes[profileW-2];
int mode;
int Sunrise = 360;
int Sunset = 1080;
int SunCheckDay;
char logString[160];
char c;
time_t t_last;

void localLogger(Logger::Level level, const char* module, const char* message);

void setup() {
  uint32_t t;

  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  
  //enable WAAS if possible
  GPS.sendCommand(PMTK_ENABLE_SBAS);  
  GPS.sendCommand(PMTK_ENABLE_WAAS);
  Logger::setOutputFunction(&localLogger);
  //Logger::setLogLevel(Logger::NOTICE);
  Logger::setLogLevel(Logger::VERBOSE);

  Logger::verbose("Setup()", "start init");
  Logger::verbose("Setup()", "start Display init");

  tft.initR(INITR_GREENTAB);
  setDisplay();
  initDisplay();
  Logger::verbose("Setup()", "Display init done");

  Logger::verbose("Setup()", "start time init");
  setSyncProvider(getTeensy3Time);
  utc = CE.toUTC(compileTime());
  sprintf(logString, "Code compiled %02i.%02i.%04i %02i:%02iUTC\n", day(utc), month(utc), year(utc), hour(utc), minute(utc));
  Logger::notice("Setup()", logString);
  Logger::verbose("Setup()", "time init done");

  tft.setTextColor(textColor xor mode);

  Logger::verbose("Setup()", "start thermometer init");
  temp.begin();
  temp.setResolution(Thermometer, 9);
  Logger::verbose("Setup()", "thermometer init done");

  Logger::verbose("Setup()", "start timer init");
  Timer1.initialize(1000000);      //every second
  Timer1.attachInterrupt(updateDisplayISR,1000000);
  Logger::verbose("Setup()", "timer init done");

  t_last=millis();
  Logger::verbose("Setup()", "start Gauges init");

  Speed.setFont(&FreeMonoBoldOblique18pt7b);
  Speed.setBGColor(ST7735_BLACK);
  Speed.setFGColor(ST7735_WHITE);
  Speed.setFormatString("%3i");
  Speed.setBorder(0);
  Speed.setBorderColor(ST7735_YELLOW);
  Speed.setCursor(4,20);
  Speed.setAutoRedraw(true);
  Speed.setValue(0);
  Speed.redraw();

  SpeedLabel.setFont(&FreeMonoBoldOblique9pt7b);
  SpeedLabel.setBGColor(ST7735_BLACK);
  SpeedLabel.setFGColor(ST7735_RED);
  SpeedLabel.setBorder(0);
  SpeedLabel.setBorderColor(ST7735_YELLOW);
  SpeedLabel.setCursor(1,8);
  SpeedLabel.setAutoRedraw(false);
  SpeedLabel.setValue("km\n/h");
  SpeedLabel.redraw();
  
  Direction.setFont(&FreeMonoBoldOblique12pt7b);
  Direction.setBGColor(ST7735_BLACK);
  Direction.setFGColor(ST7735_WHITE);
  Direction.setBorder(0);
  Direction.setBorderColor(ST7735_YELLOW);
  Direction.setCursor(4,20);
  Direction.setAutoRedraw(true);
  Direction.setValue("--");
  Direction.redraw();
  
  Time.setFont(&FreeMonoBoldOblique18pt7b);
  Time.setBGColor(ST7735_BLACK);
  Time.setFGColor(ST7735_WHITE);
  Time.setBorder(0);
  Time.setBorderColor(ST7735_GREEN);
  Time.setCursor(4,20);
  Time.setAutoRedraw(true);
  utc = now();
  Time.setValue(hour(CE.toLocal(utc, &tcr)), minute(CE.toLocal(utc, &tcr)));
  Time.redraw();

  Altitude.setFont(&FreeMonoBoldOblique12pt7b);
  Altitude.setBGColor(ST7735_BLACK);
  Altitude.setFGColor(ST7735_WHITE);
  Altitude.setBorder(0);
  Altitude.setBorderColor(ST7735_WHITE);
  Altitude.setFormatString("%4im");
  Altitude.setCursor(4,12);
  Altitude.setAutoRedraw(true);
  Altitude.setValue(9999);
  Altitude.redraw();
  
  Temperature.setFont(&FreeMonoBoldOblique12pt7b);
  Temperature.setBGColor(ST7735_BLACK);
  Temperature.setFGColor(ST7735_WHITE);
  Temperature.setBorder(0);
  Temperature.setBorderColor(ST7735_WHITE);
  Temperature.setFormatString("%4im");
  Temperature.setCursor(4,12);
  Temperature.setAutoRedraw(true);
  Temperature.setValue(9999);
  Temperature.redraw();

  Logger::verbose("Setup()", "init done");
}

void loop() {
  Logger::notice("loop()", "enter");

  c = GPS.read();
  if (GPS.newNMEAreceived()) {
    Logger::verbose("loop()", "newNMEAreceived");
    if (!GPS.parse(GPS.lastNMEA())) return;
  }
}

void setDisplay() {
  time_t t_setDisplay=millis();
  sprintf(logString, "display mode: %i",mode);
  Logger::notice("setDisplay()",logString);
  utc = now();
  if (SunCheckDay != day(utc) and GPS.fix and GPS.latitudeDegrees != 0.0 and GPS.longitudeDegrees != 0.0) {
    sprintf(logString, "Lokation: %.3f, %.3f\n", GPS.latitudeDegrees, GPS.longitudeDegrees);
    Logger::notice("setDisplay()", logString);
    Dusk2Dawn here(GPS.latitudeDegrees, GPS.longitudeDegrees, 0);
    Sunrise = here.sunrise(year(utc), month(utc), day(utc), false);
    Sunset = here.sunset(year(utc), month(utc), day(utc), false);
    sprintf(logString, "Neue Werte: \n\t\tSonnenaufgang %i:%02i (%i Minuten nach Mitternacht)\n\t\tSonnenuntergang %i:%02i (%i Minuten nach Mitternacht).\n\t\tZeit seit Mitternacht %i\n", int(Sunrise / 60), Sunrise % 60, Sunrise, int(Sunset / 60), Sunset % 60, Sunset, hour(utc) * 60 + minute(utc));
    Logger::notice("setDisplay()", logString);
    SunCheckDay = day(utc);
  }

  if ( (hour(utc) * 60 + minute(utc) >= Sunrise) and (hour(utc) * 60 + minute(utc) < Sunset) and GPS.fix) {
    if (mode == mode_night) {
      sprintf(logString, "setting mode to \"day\" (was \"night\")\n");
      Logger::notice("setDisplay()", logString);
      mode = mode_day;
      initDisplay();
      updateDisplay(1);
    }
  } else {
    if (mode == mode_day) {
      sprintf(logString, "setting mode to \"night\" (was \"day\")\n");
      Logger::notice("setDisplay()", logString);
      mode = mode_night;
      initDisplay();
      updateDisplay(1);
    }
  }
  sprintf(logString, "setDisplay exit, time elapsed %ims",millis()-t_setDisplay);
  Logger::verbose("setDisplay()", logString);
}

void initDisplay() {
  tft.setRotation(1);
  tft.fillScreen(ST7735_WHITE xor mode);
  tft.fillScreen(ST7735_BLACK xor mode);
  tft.setRotation(3);
  tft.fillScreen(ST7735_GREEN xor mode);
  tft.fillScreen(ST7735_BLACK xor mode);
  displayBitmap(0, 1, TransalpLogo);
  updateDisplay(1);
}

void updateDisplayISR() {
  sprintf(logString, "entering");
  Logger::notice("updateDisplayISR()", logString);
  updateDisplay(0);
}

void updateDisplay(bool f) {
  time_t t_updateDisplay=millis();
  String timeString;
  setDisplay();
  //temp.requestTemperatures();
  
  //timeString=sprintf("%i:%02i",hour(),minute());
  utc = now();
  Time.setValue(hour(CE.toLocal(utc, &tcr)), minute(CE.toLocal(utc, &tcr)));
  Speed.setValue((int)GPS.speed*1.852);
  Temperature.setValue((int)temp.getTempC(Thermometer));
  Altitude.setValue(GPS.altitude);

  myangle=GPS.angle;
    if(myangle>=360-22.5 && myangle<NORTH+22.5){
      Direction.setValue(" N");
    }else if(myangle>=NORTHEAST-22.5 && myangle<NORTHEAST+22.5){
      Direction.setValue("NE");
    }else if(myangle>=EAST-22.5 && myangle<EAST+22.5){
      Direction.setValue(" E");
    }else if(myangle>=SOUTHEAST-22.5 && myangle<SOUTHEAST+22.5){
      Direction.setValue("SE");
    }else if(myangle>=SOUTH-22.5 && myangle<SOUTH+22.5){
      Direction.setValue(" S");
    }else if(myangle>=SOUTHWEST-22.5 && myangle<SOUTHWEST+22.5){
      Direction.setValue("SW");
    }else if(myangle>=WEST-22.5 && myangle<WEST+22.5){
      Direction.setValue(" W");
    }else if(myangle>=NORTHWEST-22.5 && myangle<NORTHWEST+22.5){
      Direction.setValue("NW");
    }
  sats = GPS.satellites;
  sprintf(logString, "Number of Satellites %i", sats);
  Logger::notice("updateDisplay()", logString);
  /*  if (sats != sats_a || f) {
    tft.setFont();
    tft.setCursor(altX, altY - 25);
    tft.setTextColor(ST7735_BLACK xor mode);
    tft.printf("Sat %2.i", sats_a);

    tft.setCursor(altX , altY - 25);
    tft.setTextColor(textColor xor mode);
    tft.printf("Sat %2.i", sats);
    sats_a = sats;
  }
*/
  
 
  myfix = GPS.fix;
  if(myfix){
    setSyncProvider(getGPSTime);
    setTeensy3Time();
  }else{
    setSyncProvider(getTeensy3Time);
  }

  sprintf(logString, GPS.lastNMEA());
  Logger::verbose("setDisplay() - GPS", logString);
  /*
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
    Serial.print("Location: ");
    Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
    Serial.print(", ");
    Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
    Serial.print("Speed (knots): "); Serial.println(GPS.speed);
    Serial.print("Angle: "); Serial.println(GPS.angle);
    Serial.print("Altitude: "); Serial.println(GPS.altitude);
    Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  */
/*
  int maxAlt = 50;
  int minAlt = 10000;
  int index;
  if (seconds == 0 || f) {
    tft.drawRect(profileX, profileY, profileW, profileH, tft.Color565(32, 64, 0) xor mode);
    seconds = 120;
    for (index = 0; index < profileW-3; index++) {
      if (altitudes[index] > maxAlt) maxAlt = altitudes[index];
      if (altitudes[index] < minAlt) {
        if (altitudes[index] > 0) {
          minAlt = altitudes[index];
        } else {
          minAlt = 0.01;
        }
      }
      altitudes[index] = altitudes[index + 1];
    }
    altitudes[index] = GPS.altitude;
    if (altitudes[index] > maxAlt) maxAlt = altitudes[index];
    if (altitudes[index] < minAlt) minAlt = altitudes[index];
    if (minAlt == maxAlt) maxAlt++;
    for (index = 0; index < profileW-2; index++) {
      tft.drawFastVLine(profileX + index + 1, profileY + 1, profileH-2, 0 xor mode);
      tft.drawFastVLine(profileX + index + 1, profileY + (profileH-1) - altitudes[index] * (profileH-2) / maxAlt, altitudes[index] * (profileH-2) / maxAlt, 0xffff xor mode);
    }
    if (GPS.fix) setTeensy3Time();
  }
  seconds--;
  */
  sprintf(logString, "updateDisplay exit, time elapsed %ims",millis()-t_updateDisplay);
  Logger::verbose("updateDisplay()", logString);
}


void displayBitmap(int x0, int y0, const uint16_t *Bitmap) {
  int x, y;
  int pos = 0;
  int c = 1;

  //first two words are width and height
  int w = Bitmap[pos++];
  int h = Bitmap[pos++];
  int f = Bitmap[pos++];
  if ( f == 0x0000) { //uncompressed bitmap
    for (y = h - 1; y >= 0; y--) {
      for (x = 0; x < w; x++) {
        while (Bitmap[pos + c] == Bitmap[pos] && x + c <= w) {
          c++;
        }
        if (c > 1) {
          tft.drawFastHLine(x0 + x, y0 + y, c, Bitmap[pos] xor mode);
          x += c - 1;
          pos += c;
          c = 1;
        } else {
          tft.drawPixel(x0 + x, y0 + y, Bitmap[pos++] xor mode);
          c = 1;
        }
      }
    }
  } else if ( f == 0xffff) { //RLE compressed bitmap
    for (y = h - 1; y >= 0; y--) {
      for (x = 0; x < w; x++) {
        if (Bitmap[pos] == Bitmap[pos + 1]) {
          tft.drawFastHLine(x0 + x, y0 + y, Bitmap[pos + 2], Bitmap[pos] xor mode);
          x += Bitmap[pos + 2] - 1;
          pos += 3;
        } else {
          tft.drawPixel(x0 + x, y0 + y, Bitmap[pos++] xor mode);
        }
      }
    }
  }
}

void compressBitmap(const uint16_t *Bitmap) {
  int x, y;
  int pos = 0;
  int c = 1;

  //first two words are width and height
  const int w = Bitmap[pos++];
  const int h = Bitmap[pos++];
  Serial.printf("0x%04x, 0x%04x, 0x%04x, \n", w, h, 0xffff);
  for (y = h - 1; y >= 0; y--) {
    for (x = 0; x < w; x++) {
      while (Bitmap[pos + c] == Bitmap[pos] && x + c <= w) {
        c++;
      }
      if (c > 1) {
        //tft.drawFastHLine(x0+x,y0+y,c,Bitmap[pos]);
        Serial.printf("0x%04x, 0x%04x, 0x%04x, ", Bitmap[pos], Bitmap[pos], c);
        x += c - 1;
        pos += c;
        c = 1;
      } else {
        Serial.printf("0x%04x, ", Bitmap[pos++]);
        //tft.drawPixel(x0+x,y0+y, Bitmap[pos++]);
        c = 1;
      }
    }
    Serial.println();
  }
}

time_t getTeensy3Time(void) {
  return Teensy3Clock.get();
}

time_t setTeensy3Time(void) {
  if (abs(now() - Teensy3Clock.get()) > 5) {
    sprintf(logString, "adjusting hardware clock, rtc:%i:%02i:%02i, gps:%i:%02i:%02i", hour(now()), minute(now()), second(now()), hour(getGPSTime()), minute(getGPSTime()), second(getGPSTime()));
    Logger::notice("setTeensy3Time", logString);
    Teensy3Clock.set(now());
    return Teensy3Clock.get();
  }
  return NULL;
}

time_t getGPSTime(void) {
  tmElements_t tm;
  tm.Hour = GPS.hour;
  tm.Minute = GPS.minute;
  tm.Second = GPS.seconds;
  tm.Day = GPS.day;
  tm.Month = GPS.month;
  tm.Year = GPS.year - 1970;
  return makeTime(tm);
}

//Function to return the compile date and time as a time_t value
time_t compileTime(void)
{
#define FUDGE 5        //fudge factor to allow for compile time (seconds, YMMV)

  char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char chMon[3], *m;
  int d, y;
  tmElements_t tm;
  time_t t;

  strncpy(chMon, compDate, 3);
  chMon[3] = '\0';
  m = strstr(months, chMon);
  tm.Month = ((m - months) / 3 + 1);

  tm.Day = atoi(compDate + 4);
  tm.Year = atoi(compDate + 7) - 1970;
  tm.Hour = atoi(compTime);
  tm.Minute = atoi(compTime + 3);
  tm.Second = atoi(compTime + 6);
  t = makeTime(tm);
  return t + FUDGE;        //add fudge factor to allow for compile time
}
