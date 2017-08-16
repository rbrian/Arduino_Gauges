/***************************************************
 *                                                 *
 *       Arduino Gauges Library                    *
 *       v. 0.4        	                           *
 *       2017, P. Jakobs                           *
 *				                   							         *
 ***************************************************/

#ifndef _GAUGESH_
#define _GAUGESH_

#include <Adafruit_GFX.h>
//#include <Adafruit_ILI9340.h>
//#include <LinkedList.h>

#if defined(__SAM3X8E__)
  #include <include/pio.h>
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #define pgm_read_word(addr) (*(const unsigned short *)(addr))
  typedef unsigned char prog_uchar;
  #define _MAX_STR_LEN 255
#endif

#ifdef __AVR__
  #include <avr/pgmspace.h>
  #define _MAX_STR_LEN 63
#endif

#ifndef _MAX_STR_LEN
  #define _MAX_STR_LEN 63
#endif

/*
 * Text positioning values
 * Vertical (TEXT_TOP,TEXT_MIDDLE,TEXT_BOTTOM)
 * Horizontal (TEXT_LEFT, TEXT_CENTER, TEXT_RIGHT)
 * TEXT_CENTER and TEXT_MIDDLE are synonymous and work for both
 */
#define TEXT_TOP     1
#define TEXT_MIDDLE  0
#define TEXT_BOTTOM  2
#define TEXT_LEFT    1
#define TEXT_CENTER  0
#define TEXT_RIGHT   2

/*
 * Tape direction values
 */
#define TAPE_LEFTRIGHT  0
#define TAPE_TOPDOWN    1
#define TAPE_RIGHTLEFT  2
#define TAPE_BOTTOMUP   3


/*
 * Gauge baseclass
 */

class Gauge{
  public:
    Gauge();
    void
      setValue(short val),
      setValue(int val),
      setValue(long val),
      setValue(float val),
      setValue(double val),
      setValue(char* val),
      setValue(const char* val),
      setValue(String val),
      setAutoRedraw(bool val);
    virtual void redraw();
  protected:
    String
      _val;
    bool
      _autoRedraw,
      _changed;
};

class displayGauge : public Gauge{
public:
  displayGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h),
  displayGauge(Adafruit_GFX *display),
  displayGauge();
void
  setPosition(uint16_t x, uint16_t y),
  setSize(uint16_t w, uint16_t h),
  setDepth(uint8_t z),
  setVisible(bool val),
  setDisplay(Adafruit_GFX *display),
  setFGColor(uint16_t fg),
  setBGColor(uint16_t bg),
  setBorder(uint8_t border),
  setBorderColor(uint16_t bo),
  setGutter(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b),
  setGutter(uint8_t gutter),
  setMargins(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b),
  setMargins(uint8_t gutter),
  pushBitmap(uint16_t x, uint16_t y, uint16_t* buffer, uint16_t w, uint16_t h);
uint8_t
  getDepth();
protected:
    uint16_t
      _x, _y,
      _w, _h,
      _fg, _bg, _bo;
    uint8_t
      _z,
      _border,
      _gutter_l, _gutter_r, _gutter_t, _gutter_b;
    Adafruit_GFX
      *_display;
    GFXcanvas16
      *_canvas;
    bool
      _visible;
};

class textGauge : public displayGauge {
  public:
      textGauge(),
      textGauge(Adafruit_GFX *display),
      textGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void
      setFont(const GFXfont *font),
      setCursor(uint16_t x, uint16_t y),
      setVAlign(uint8_t align),
      setHAlign(uint8_t align),
      redraw();
	protected:
    uint16_t
      _cursor_x,
      _cursor_y,
      _str_len;
    uint8_t
      _valign, _halign;
  	const GFXfont *_font;
};

class tapeGauge : public displayGauge {
public:
    tapeGauge(),
    tapeGauge(Adafruit_GFX *display),
    tapeGauge(Adafruit_GFX *display, uint8_t direction),
    tapeGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h),
    tapeGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint8_t direction);
  void
    setMinMax(float min, float max),
    setColors(uint16_t color0,float limit0, uint16_t color1, float limit1, uint16_t color2),
    setDirection(uint8_t dir),
    setTicks(uint16_t major, uint16_t minor),
    redraw();
protected:
  uint16_t
    _color0,_color1,_color2,
    _tickMajor, _tickMinor;
  uint8_t
    _direction;
  float
    _min, _max,
    _limit0,_limit1;
  private:
    uint16_t __tape_length;
};
#endif
