/*

This program provides cartesian type graph function

It requires and Arduino Mega (or UNO) and an Adafruit 3.5" TFT 320x480 + Touchscreen Breakout Board
https://learn.adafruit.com/adafruit-3-5-color-320x480-tft-touchscreen-breakout/overview

Adafruit libraries
https://github.com/adafruit/Adafruit_HX8357_Library/archive/master.zip
https://github.com/adafruit/Adafruit-GFX-Library/archive/master.zip

optional touch screen libraries
https://github.com/adafruit/Touch-Screen-Library/archive/master.zip

Revisions
rev     date        author      description
1       12-24-2015  kasprzak    initial creation
2       04-19-2024  kasprzak    added example in loop for automatic rollover for continued data plotting--and fixed graph function to accomodate

This pin setting will also operate the SD card

Pin settings

  Arduino   device
  5V        Vin
  GND       GND
  A0
  A1
  A2         Y+ (for touch screen use)
  A3         X- (for touch screen use)
  A4
  A5
  1
  2
  3
  4         CCS (42 for mega)
  5
  6
  7         Y- (44 for mega)
  8         X+ (46 for mega)
  9         DC (48 on mega * change define)
  10        CS (53 for mega * change define)
  11        MOSI (51 for mega)
  12        MISO  (50 for mega)
  13        CLK (SCK) (52 for mega)
  44        Y- (for touch screen only)
  46        X+ (for touch screen only)
  48        DC
  SDA
  SLC

*/

#include <SPI.h>
#include "Adafruit_HX8357.h"


// These are 'flexible' lines that can be changed

// for a nano
#define TFT_CS 10
#define TFT_DC 9


#define LTBLUE 0xB6DF
#define LTTEAL 0xBF5F
#define LTGREEN 0xBFF7
#define LTCYAN 0xC7FF
#define LTRED 0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW 0xFFF8
#define LTORANGE 0xFE73
#define LTPINK 0xFDDF
#define LTPURPLE 0xCCFF
#define LTGREY 0xE71C

#define BLUE 0x001F
#define TEAL 0x0438
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define ORANGE 0xFC00
#define PINK 0xF81F
#define PURPLE 0x8010
#define GREY 0xC618
#define WHITE 0xFFFF
#define BLACK 0x0000

#define DKBLUE 0x000D
#define DKTEAL 0x020C
#define DKGREEN 0x03E0
#define DKCYAN 0x03EF
#define DKRED 0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW 0x8400
#define DKORANGE 0x8200
#define DKPINK 0x9009
#define DKPURPLE 0x4010
#define DKGREY 0x4A49


#define ADJ_PIN A0

double a1, b1, c1, d1, r2, r1, vo, tempC, tempF, tempK;
double x, y;
double xLow = 0, xHigh = 60;
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC);

// this is the only external variable used by the graph
// it's a flat to draw the coordinate system only on the first pass
boolean display1 = true;
boolean display2 = true;
boolean display3 = true;
boolean display4 = true;
boolean display5 = true;
boolean display6 = true;
boolean display7 = true;
boolean display8 = true;
boolean display9 = true;
double ox, oy;

void setup() {

  Serial.begin(9600);

  pinMode(ADJ_PIN, INPUT);
  tft.begin(HX8357D);
  tft.fillScreen(BLACK);

  tft.setRotation(3);

  for (x = 0; x <= 6.3; x += .1) {

    y = sin(x);
    Graph(tft, x, y, 60, 290, 390, 260, 0, 6.5, 1, -1, 1, .25, "Sin Function - Large Graph", "x", "sin(x)", DKBLUE, RED, YELLOW, WHITE, BLACK, display1);
  }

  delay(5000);

  tft.fillScreen(BLACK);
  for (x = 0; x <= 6.3; x += .1) {

    y = sin(x);
    Graph(tft, x, y, 100, 280, 100, 240, 0, 6.5, 3.25, -1, 1, .25, "Sin Function - Narrow Graph", "x", "sin(x)", GREY, GREEN, RED, YELLOW, BLACK, display9);
  }

  delay(5000);

  tft.fillScreen(BLACK);
  for (x = 0; x <= 25.2; x += .1) {

    y = sin(x);
    Graph(tft, x, y, 50, 190, 400, 60, 0, 25, 5, -1, 1, .5, "Sin Function - Short Graph", "x", "sin(x)", DKYELLOW, YELLOW, GREEN, WHITE, BLACK, display8);
  }

  delay(5000);

  tft.fillScreen(BLACK);
  for (x = 0.001; x <= 10; x += .1) {

    y = log(x);
    Graph(tft, x, y, 50, 240, 300, 180, 0, 10, 1, -10, 5, 1, "Natural Log Function - Small Graph", "x", "ln(x)", BLUE, RED, WHITE, WHITE, BLACK, display2);
  }

  delay(5000);
  tft.fillScreen(BLACK);

  for (x = 0; x <= 10; x += 1) {

    y = x * x;
    Graph(tft, x, y, 50, 290, 390, 260, 0, 10, 1, 0, 100, 10, "Square Function", "x", "x^2", DKRED, RED, YELLOW, WHITE, BLACK, display3);
  }

  delay(5000);
  tft.fillScreen(BLACK);

  for (x = 0.00; x <= 20; x += .01) {

    y = ((sin(x)) * x + cos(x)) - log(x);
    Graph(tft, x, y, 50, 290, 390, 260, 0, 20, 1, -20, 20, 5, "Weird Function", "x", " y = sin(x) + cos(x) - log(x)", ORANGE, YELLOW, CYAN, WHITE, BLACK, display4);
  }

  delay(5000);
  tft.fillScreen(BLACK);

  tft.setRotation(2);
  for (x = 0; x <= 12.6; x += .1) {

    y = sin(x);
    Graph(tft, x, y, 50, 250, 150, 150, 0, 13, 3.5, -1, 1, 1, "Sin(x)", "x", "sin(x)", DKBLUE, RED, YELLOW, WHITE, BLACK, display5);
  }

  while (1) {}
  tft.setRotation(3);
  delay(5000);
  tft.fillScreen(WHITE);

  for (x = 0; x <= 6.3; x += .05) {

    y = cos(x);
    Graph(tft, x, y, 100, 250, 300, 200, 0, 6.5, 3.25, -1, 1, 1, "Cos Function", "x", "cos(x)", DKGREY, GREEN, BLUE, BLACK, WHITE, display6);
  }

  delay(5000);
  tft.fillScreen(BLACK);
}


void loop(void) {

  x++;
  y = analogRead(ADJ_PIN) / 204.6;
  Graph(tft, x, y, 50, 290, 390, 260, xLow, xHigh, 10, 0, 5, 1, "A0 Input", " Time [s]", "Volts", DKBLUE, RED, GREEN, WHITE, BLACK, display7);
  delay(250);

  if (x >= xHigh) {
    //x = 0;
    xLow += 60;
    xHigh += 60;
    display7 = true;
    tft.fillScreen(BLACK);
  }
}

/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = flag to redraw graph on fist call only
*/


void Graph(Adafruit_HX8357 &d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean &redraw) {

  double ydiv, xdiv;
  // initialize old x and old y in order to draw the first point of the graph
  // but save the transformed value
  // note my transform funcition is the same as the map function, except the map uses long and we need doubles
  //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
  //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  double i;
  double temp;
  int rot, newrot;

  if (redraw == true) {

    redraw = false;
    ox = (x - xlo) * (w) / (xhi - xlo) + gx;
    oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    // draw y scale
    for (i = ylo; i <= yhi; i += yinc) {
      // compute the transform
      temp = (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == ylo) {
        d.drawLine(gx, temp, gx + w, temp, acolor);
      } else {
        d.drawLine(gx, temp, gx + w, temp, gcolor);
      }

      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(gx - 30, temp);
      // precision is default Arduino--this could really use some format control
      d.println(i);
    }
    // draw x scale
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform

      temp = (i - xlo) * (w) / (xhi - xlo) + gx;
      if (i == xlo) {
        d.drawLine(temp, gy, temp, gy - h, acolor);
      } else {
        d.drawLine(temp, gy, temp, gy - h, gcolor);
      }

      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(temp, gy + 10);
      // precision is default Arduino--this could really use some format control
      d.println(i);
    }

    //now draw the labels
    d.setTextSize(2);
    d.setTextColor(tcolor, bcolor);
    d.setCursor(gx, gy - h - 30);
    d.println(title);

    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(gx, gy + 20);
    d.println(xlabel);

    /*
 // some day auto rotate
    d.setTextSize(1);
  
    d.setTextColor(acolor, bcolor);
    d.setCursor(gy - h - 10, gx-40);
    d.println(ylabel);
*/
  }

  //graph drawn now plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized as static above
  x = (x - xlo) * (w) / (xhi - xlo) + gx;
  y = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  d.drawLine(ox, oy, x, y, pcolor);
  d.drawLine(ox, oy + 1, x, y + 1, pcolor);
  d.drawLine(ox, oy - 1, x, y - 1, pcolor);
  ox = x;
  oy = y;
}

/*
  End of graphing functioin
*/
