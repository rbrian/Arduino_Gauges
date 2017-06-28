/***************************************************
 *                                                 *
 *       Arduino Gauges Library                    *
 *       v. 0.1			                   			*
 *       2017, P. Jakobs                           *
 *				                   							*
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
	    	setBorder(uint8_t border),
	    	setBorderColor(uint16_t bo),
	      pushBitmap(uint16_t x, uint16_t y, uint16_t* buffer, uint16_t w, uint16_t h),
	      //redraw();
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

class textGauge : public Gauge{
	public:
	       textGauge(),
	       textGauge(Adafruit_GFX *display),
	       textGauge(Adafruit_GFX *display,char *_val),
	       textGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	       //textGauge(Adafruit_GFX *display,const char _val);
		void
	 	    setFont(const GFXfont *font),
		    setCursor(uint16_t x, uint16_t y),
	       setFormatString(String format),
	       setValue(char *val),
	       setValue(const char* val),
	       redraw();
	protected:
		String _format;
		uint16_t
  			_cursor_x,
  			_cursor_y,
  			_str_len;
  		char _val[255];
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
	        redraw();
		int
	       getValue();

	private:
		int _val;
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
