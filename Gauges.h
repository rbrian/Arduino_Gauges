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

#define TOP     1
#define MIDDLE  2
#define BOTTOM  3
#define LEFT    1
#define CENTER  2
#define RIGHT   3

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
      setAutoRedraw(bool val),
      redraw();
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
      _gutter_l,
      _gutter_r,
      _gutter_t,
      _gutter_b;
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
      setGutter(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b),
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

#endif
