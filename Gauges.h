/***************************************************
 *                                                 *
 *       Arduino Gauges Library                    *
 *       v. 0.3        	                           *
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
 * Gauge baseclass
 */

class Gauge {
	public:
		  Gauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h),
		  Gauge(Adafruit_GFX *display),
		  Gauge();
	  void
	    setPosition(uint16_t x, uint16_t y),
	    setSize(uint16_t w, uint16_t h),
	    setDepth(uint8_t z),
	    setAutoRedraw(bool val),
	    setVisible(bool val),
	    setDisplay(Adafruit_GFX *display),
	    setFGColor(uint16_t fg),
	    setBGColor(uint16_t bg),
	    setBorder(uint8_t border),
	    setBorderColor(uint16_t bo),
	    pushBitmap(uint16_t x, uint16_t y, uint16_t* buffer, uint16_t w, uint16_t h),
	    redraw();
	  uint8_t
	    getDepth();
protected:
    uint16_t
  		_x,
  		_y,
  		_w,
  		_h,
  		_fg,
  		_bg,
  		_bo;
  	uint8_t
  		_z,
  		_border;
  	Adafruit_GFX
  		*_display;
		GFXcanvas16
			*_canvas;
		bool
			_autoRedraw,
			_visible;
};

class textGauge : public Gauge {
  public:
      textGauge(),
      textGauge(Adafruit_GFX *display),
      textGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void
      setFont(const GFXfont *font),
      setCursor(uint16_t x, uint16_t y),
      setFormatString(String format),
      update(int val),
      update(char *val),
      update(const char *val),
      update(float val),
      update(double val),
      //update(time_t val),
      redraw();
	protected:
    String
      _format;
		uint16_t
      _cursor_x,
      _cursor_y,
      _str_len;
  	char _txtval[_MAX_STR_LEN];
    const GFXfont *_font;
};

class integerGauge : public textGauge {
	public:
	     integerGauge(),
       integerGauge(Adafruit_GFX *display),
  	   integerGauge(Adafruit_GFX *display,int _val),
  	   integerGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	  void
       setValue(int val),
	     display();
		int
	     getValue();
	private:
		int
			 _val;
};

class stringGauge : public textGauge {
public:
      stringGauge(),
      stringGauge(Adafruit_GFX *display),
      stringGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void
      setValue(char *val),
      setValue(const char *val),
      setValue(String val),
	    display();
		char*
	     getValue();
	private:
		char
			 _val[_MAX_STR_LEN];
};

class clockGauge : public textGauge {
	public:
      clockGauge(),
	    clockGauge(Adafruit_GFX *display),
	    clockGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void
	    setValue(uint8_t hour, uint8_t min),
			setValue(uint8_t hour, uint8_t mmin, uint8_t second),
	    display();
	private:
		uint8_t
			_hour,
			_min,
			_second;
};


/*
class stringGauge : public textGauge {

	public:
        stringGauge(String val);
		void
            setValue(String val),
            redraw(void),
		String
            getValue();

	private:
		String val;
};
*/
#endif
