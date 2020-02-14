/*

  This program provides bar graph (horizontal and vertical orientations) and a dial type graph functions

  It requires and Arduino Mega (or UNO) and an Adafruit 3.5" TFT 320x480 + Touchscreen Breakout Board
  https://learn.adafruit.com/adafruit-3-5-color-320x480-tft-touchscreen-breakout/overview

  Adafruit libraries
  https://github.com/adafruit/Adafruit_HX8357_Library/archive/master.zip
  https://github.com/adafruit/Adafruit-GFX-Library/archive/master.zip

  optional touch screen libraries
  https://github.com/adafruit/Touch-Screen-Library/archive/master.zip

  Revisions
  rev     date        author      description
  1       12-19-2015  kasprzak    initial creation


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
#include <Adafruit_HX8357.h>

// http://www.barth-dev.de/online/rgb565-color-picker/
#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN         0xBFF7
#define LTCYAN    0xC7FF
#define LTRED           0xFD34
#define LTMAGENTA       0xFD5F
#define LTYELLOW        0xFFF8
#define LTORANGE        0xFE73
#define LTPINK          0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY          0xE71C

#define BLUE            0x001F
#define TEAL    0x0438
#define GREEN           0x07E0
#define CYAN          0x07FF
#define RED           0xF800
#define MAGENTA       0xF81F
#define YELLOW        0xFFE0
#define ORANGE        0xFD20
#define PINK          0xF81F
#define PURPLE    0x801F
#define GREY        0xC618
#define WHITE         0xFFFF
#define BLACK         0x0000

#define DKBLUE        0x000D
#define DKTEAL    0x020C
#define DKGREEN       0x03E0
#define DKCYAN        0x03EF
#define DKRED         0x6000
#define DKMAGENTA       0x8008
#define DKYELLOW        0x8400
#define DKORANGE        0x8200
#define DKPINK          0x9009
#define DKPURPLE      0x4010
#define DKGREY        0x4A49

#define TFT_CS 53
#define TFT_DC 48
#define ADJ_PIN A0

double volts;
double bvolts;
double pmvolts;

boolean graph_1 = true;
boolean graph_2 = true;
boolean graph_3 = true;
boolean graph_4 = true;
boolean graph_5 = true;
boolean graph_6 = true;
boolean graph_7 = true;

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC);

void setup() {
  Serial.begin(9600);
  tft.begin(HX8357D);
  tft.fillScreen(BLACK);
  tft.setRotation(3);
  pinMode(A0, INPUT);
}

void loop(void) {

  bvolts = analogRead(A0);
  volts = (bvolts  / 204.6 ) ;
  pmvolts = (bvolts  / 204.6 ) - 2.5 ;

  DrawBarChartV(tft, 10,  290, 30, 260, 0, 1200 , 100, bvolts , 4 , 0, BLUE, DKBLUE, BLUE, WHITE, BLACK, "Bits", graph_1);

  DrawBarChartH(tft, 100, 250, 300, 30, -2.5, 2.5, .5, pmvolts, 2, 1, GREEN, DKGREEN,  GREEN, WHITE, BLACK, "Offset", graph_6);

  DrawDial(tft, 260, 120, 110, 0, 5 , 1, 240, volts,  1 , 2, RED, WHITE, BLACK, "Volts", graph_7);

}


/*
  This method will draw a vertical bar graph for single input
  it has a rather large arguement list and is as follows

  &d = display object name
  x = position of bar graph (lower left of bar)
  y = position of bar (lower left of bar
  w = width of bar graph
  h =  height of bar graph (does not need to be the same as the max scale)
  loval = lower value of the scale (can be negative)
  hival = upper value of the scale
  inc = scale division between loval and hival
  curval = date to graph (must be between loval and hival)
  dig = format control to set number of digits to display (not includeing the decimal)
  dec = format control to set number of decimals to display (not includeing the decimal)
  barcolor = color of bar graph
  voidcolor = color of bar graph background
  bordercolor = color of the border of the graph
  textcolor = color of the text
  backcolor = color of the bar graph's background
  label = bottom lable text for the graph
  redraw = flag to redraw display only on first pass (to reduce flickering)

*/

void DrawBarChartV(Adafruit_HX8357 & d, double x , double y , double w, double h , double loval , double hival , double inc , double curval ,  int dig , int dec, unsigned int barcolor, unsigned int voidcolor, unsigned int bordercolor, unsigned int textcolor, unsigned int backcolor, String label, boolean & redraw)
{

  double stepval, range;
  double my, level;
  double i, data;
  // draw the border, scale, and label once
  // avoid doing this on every update to minimize flicker
  if (redraw == true) {
    redraw = false;

    d.drawRect(x - 1, y - h - 1, w + 2, h + 2, bordercolor);
    d.setTextColor(textcolor, backcolor);
    d.setTextSize(2);
    d.setCursor(x , y + 10);
    d.println(label);
    // step val basically scales the hival and low val to the height
    // deducting a small value to eliminate round off errors
    // this val may need to be adjusted
    stepval = ( inc) * (double (h) / (double (hival - loval))) - .001;
    for (i = 0; i <= h; i += stepval) {
      my =  y - h + i;
      d.drawFastHLine(x + w + 1, my,  5, textcolor);
      // draw lables
      d.setTextSize(1);
      d.setTextColor(textcolor, backcolor);
      d.setCursor(x + w + 12, my - 3 );
      data = hival - ( i * (inc / stepval));
      d.println(Format(data, dig, dec));
    }
  }
  // compute level of bar graph that is scaled to the  height and the hi and low vals
  // this is needed to accompdate for +/- range
  level = (h * (((curval - loval) / (hival - loval))));
  // draw the bar graph
  // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update
  d.fillRect(x, y - h, w, h - level,  voidcolor);
  d.fillRect(x, y - level, w,  level, barcolor);
  // write the current value
  d.setTextColor(textcolor, backcolor);
  d.setTextSize(2);
  d.setCursor(x , y - h - 23);
  d.println(Format(curval, dig, dec));

}

/*
  This method will draw a dial-type graph for single input
  it has a rather large arguement list and is as follows

  &d = display object name
  cx = center position of dial
  cy = center position of dial
  r = radius of the dial
  loval = lower value of the scale (can be negative)
  hival = upper value of the scale
  inc = scale division between loval and hival
  sa = sweep angle for the dials scale
  curval = date to graph (must be between loval and hival)
  dig = format control to set number of digits to display (not includeing the decimal)
  dec = format control to set number of decimals to display (not includeing the decimal)
  needlecolor = color of the needle
  dialcolor = color of the dial
  textcolor = color of all text (background is dialcolor)
  label = bottom lable text for the graph
  redraw = flag to redraw display only on first pass (to reduce flickering)
*/

void DrawDial(Adafruit_HX8357 & d, int cx, int cy, int r, double loval , double hival , double inc, double sa, double curval,  int dig , int dec, unsigned int needlecolor, unsigned int dialcolor, unsigned int  textcolor, String label, boolean & redraw) {

  double ix, iy, ox, oy, tx, ty, lx, rx, ly, ry, i, offset, stepval, data, angle;
  double degtorad = .0174532778;
  static double px = cx, py = cy, pix = cx, piy = cy, plx = cx, ply = cy, prx = cx, pry = cy;

  // draw the dial only one time--this will minimize flicker
  if ( redraw == true) {
    redraw = false;
    d.fillCircle(cx, cy, r - 2, dialcolor);
    d.drawCircle(cx, cy, r, needlecolor);
    d.drawCircle(cx, cy, r - 1, needlecolor);
    d.setTextColor(textcolor, dialcolor);
    d.setTextSize(2);
    d.setCursor(cx - 25, cy + 40);
    d.println(label);

  }
  // draw the current value
  d.setTextSize(2);
  d.setTextColor(textcolor, dialcolor);
  d.setCursor(cx - 25, cy + 20 );
  d.println(Format(curval, dig, dec));
  // center the scale about the vertical axis--and use this to offset the needle, and scale text
  offset = (270 +  sa / 2) * degtorad;
  // find hte scale step value based on the hival low val and the scale sweep angle
  // deducting a small value to eliminate round off errors
  // this val may need to be adjusted
  stepval = ( inc) * (double (sa) / (double (hival - loval))) + .00;
  // draw the scale and numbers
  // note draw this each time to repaint where the needle was
  for (i = 0; i <= sa; i += stepval) {
    angle = ( i  * degtorad);
    angle = offset - angle ;
    ox =  (r - 2) * cos(angle) + cx;
    oy =  (r - 2) * sin(angle) + cy;
    ix =  (r - 10) * cos(angle) + cx;
    iy =  (r - 10) * sin(angle) + cy;
    tx =  (r - 30) * cos(angle) + cx;
    ty =  (r - 30) * sin(angle) + cy;
    d.drawLine(ox, oy, ix, iy, textcolor);
    d.setTextSize(1.5);
    d.setTextColor(textcolor, dialcolor);
    d.setCursor(tx - 10, ty );
    data = hival - ( i * (inc / stepval)) ;
    d.println(Format(data, dig, dec));
  }
  // compute and draw the needle
  angle = (sa * (1 - (((curval - loval) / (hival - loval)))));
  angle = angle * degtorad;
  angle = offset - angle  ;
  ix =  (r - 10) * cos(angle) + cx;
  iy =  (r - 10) * sin(angle) + cy;
  // draw a triangle for the needle (compute and store 3 vertiticies)
  lx =  5 * cos(angle - 90 * degtorad) + cx;
  ly =  5 * sin(angle - 90 * degtorad) + cy;
  rx =  5 * cos(angle + 90 * degtorad) + cx;
  ry =  5 * sin(angle + 90 * degtorad) + cy;
  // first draw the previous needle in dial color to hide it
  // note draw performance for triangle is pretty bad...

  //d.fillTriangle (pix, piy, plx, ply, prx, pry, dialcolor);
  //d.fillTriangle (pix, piy, plx, ply, prx, pry, dialcolor);

  //d.fillTriangle (pix, piy, plx, ply, prx - 20, pry - 20, dialcolor);
  //d.fillTriangle (pix, piy, prx, pry, prx + 20, pry + 20, dialcolor);

  d.fillTriangle (pix, piy, plx, ply, prx, pry, dialcolor);
  d.drawTriangle (pix, piy, plx, ply, prx, pry, dialcolor);

  // then draw the old needle in need color to display it
  d.fillTriangle (ix, iy, lx, ly, rx, ry, needlecolor);
  d.drawTriangle (ix, iy, lx, ly, rx, ry, textcolor);

  // draw a cute little dial center
  d.fillCircle(cx, cy, 8, textcolor);
  //save all current to old so the previous dial can be hidden
  pix = ix;
  piy = iy;
  plx = lx;
  ply = ly;
  prx = rx;
  pry = ry;

}

/*
  This method will draw a horizontal bar graph for single input
  it has a rather large arguement list and is as follows

  &d = display object name
  x = position of bar graph (upper left of bar)
  y = position of bar (upper left of bar (add some vale to leave room for label)
  w = width of bar graph (does not need to be the same as the max scale)
  h =  height of bar graph
  loval = lower value of the scale (can be negative)
  hival = upper value of the scale
  inc = scale division between loval and hival
  curval = date to graph (must be between loval and hival)
  dig = format control to set number of digits to display (not includeing the decimal)
  dec = format control to set number of decimals to display (not includeing the decimal)
  barcolor = color of bar graph
  voidcolor = color of bar graph background
  bordercolor = color of the border of the graph
  textcolor = color of the text
  back color = color of the bar graph's background
  label = bottom lable text for the graph
  redraw = flag to redraw display only on first pass (to reduce flickering)
*/

void DrawBarChartH(Adafruit_HX8357 & d, double x , double y , double w, double h , double loval , double hival , double inc , double curval ,  int dig , int dec, unsigned int barcolor, unsigned int voidcolor, unsigned int bordercolor, unsigned int textcolor, unsigned int backcolor, String label, boolean & redraw)
{
  double stepval, range;
  double mx, level;
  double i, data;

  // draw the border, scale, and label once
  // avoid doing this on every update to minimize flicker
  // draw the border and scale
  if (redraw == true) {
    redraw = false;
    d.drawRect(x , y , w, h, bordercolor);
    d.setTextColor(textcolor, backcolor);
    d.setTextSize(2);
    d.setCursor(x , y - 20);
    d.println(label);
    // step val basically scales the hival and low val to the width
    stepval =  inc * (double (w) / (double (hival - loval))) - .00001;
    // draw the text
    for (i = 0; i <= w; i += stepval) {
      d.drawFastVLine(i + x , y + h + 1,  5, textcolor);
      // draw lables
      d.setTextSize(1);
      d.setTextColor(textcolor, backcolor);
      d.setCursor(i + x , y + h + 10);
      // addling a small value to eliminate round off errors
      // this val may need to be adjusted
      data =  ( i * (inc / stepval)) + loval + 0.00001;
      d.println(Format(data, dig, dec));
    }
  }
  // compute level of bar graph that is scaled to the width and the hi and low vals
  // this is needed to accompdate for +/- range capability
  // draw the bar graph
  // write a upper and lower bar to minimize flicker cause by blanking out bar and redraw on update
  level = (w * (((curval - loval) / (hival - loval))));
  d.fillRect(x + level + 1, y + 1, w - level - 2, h - 2,  voidcolor);
  d.fillRect(x + 1, y + 1 , level - 1,  h - 2, barcolor);
  // write the current value
  d.setTextColor(textcolor, backcolor);
  d.setTextSize(2);
  d.setCursor(x + w + 10 , y + 5);
  d.println(Format(curval, dig, dec));
}


String Format(double val, int dec, int dig ) {
  int addpad = 0;
  char sbuf[20];
  String condata = (dtostrf(val, dec, dig, sbuf));


  int slen = condata.length();
  for ( addpad = 1; addpad <= dec + dig - slen; addpad++) {
    condata = " " + condata;
  }
  return (condata);

}

