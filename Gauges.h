/***************************************************
 *                                                 *
 *       Arduino Gauges Library                    *
 *       v. 0.1			                   *
 *       2017, P. Jakobs                           *
 *				                   *
 ***************************************************/

#ifndef _GAUGESH_
#define _GAUGESH_

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9340.h>
#include <LinkedList.h>

#if defined(__SAM3X8E__)
#include <include/pio.h>
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #define pgm_read_word(addr) (*(const unsigned short *)(addr))
  typedef unsigned char prog_uchar;
#endif
#ifdef __AVR__
  #include <avr/pgmspace.h>
#endif

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
	    setFont(const GFXfont *font),
	    setCursor(uint16_t x, uint16_t y),
            redraw();

        uint8_t
            getDepth();

	    void pushBitmap(uint16_t x, uint16_t y, uint16_t* buffer, uint16_t w, uint16_t h);

  	protected:
  		uint16_t _x,_y,_w,_h,_fg,_bg,_cursor_x,_cursor_y;
  		uint8_t  _z;
  		Adafruit_GFX *_display;
		GFXcanvas16 *_canvas;
	        const GFXfont *_font;

  		bool _autoRedraw,_visible;
};

class textGauge : public Gauge {

	public:
        textGauge();
        //void
            //setTextColor(uint16_t c),
            //setFont(const GFXfont *f);
};

class integerGauge : public textGauge {

	public:
        integerGauge(),
        integerGauge(Adafruit_GFX *display),
        integerGauge(Adafruit_GFX *display,int _val);
	void
            setValue(int val),
	    setFormatString(String format),
            redraw(void);
	int
            getValue();


	private:
		int _val;
		String _format;
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
