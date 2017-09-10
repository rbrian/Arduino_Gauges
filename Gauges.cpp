#ifndef _GAUGESH_
	#include "Gauges.h"
#endif

/*
 *
 * Base Gauge class, knows where on what screen it is and has some basic properties.
 *
 */

Gauge::Gauge(){
	_val="";
}

Gauge::~Gauge(){
	//default destructor
}

void Gauge::redraw(){}

void Gauge::setValue(int val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw();
}

void Gauge::setValue(short val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw();
}

void Gauge::setValue(long val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw();
}

void Gauge::setValue(double val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw();
}

void Gauge::setValue(float val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw();
}

void Gauge::setValue(String val){
	(_val!=val)?_changed=true:_changed=false;
	_val=val;
	if(_changed == true && _autoRedraw==true) redraw();
}

void Gauge::setValue(char* val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw();
}

void Gauge::setValue(const char* val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw();
}

void Gauge::setAutoRedraw(bool val){
    _autoRedraw=val;
}

displayGauge::displayGauge(Adafruit_GFX *display,uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_visible=true;
}

displayGauge::displayGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
	_visible=true;
}

displayGauge::displayGauge(){
    _x=_y=_w=_h=0;
    _display=NULL;
		_visible=true;
}

displayGauge::~displayGauge(){
	_display->fillRect(_x, _y, _w, _h, _bg);
}

void displayGauge::pushBitmap(uint16_t x, uint16_t y, uint16_t* buffer, uint16_t w, uint16_t h){
  uint16_t _c=0;
	uint16_t _p;
  _display->startWrite();
	if(w>=h){
		/*
		 *  rect is wider than tall, optimizing for horizontal structures (drawing longest possible line)
		 */
  	//Serial.println("drawing horizontally");
		_p=1;
		for(uint16_t __y=y;__y<y+h;__y++){
		  for(uint16_t __x=x;__x<x+w;__x++){
		    while(buffer[_p]==buffer[_p+_c] and __x+_c+1<x+w){
		      _c++;
		    }
		    if(_c>1){
		      _display->writeFastHLine(__x,__y,_c,buffer[_p]);
		      _p+=(_c);
		      __x+=(_c-1);
		      _c=1;
		    }else{
		      _display->writePixel(__x,__y,buffer[_p++]);
		    }
		  }
	  }
	}else{
		/*
		 *  rect is taller than wide, optimizing for vertical structures (drawing longest possible line)
		 */
		 //Serial.println("drawing vertically");
		 for(uint16_t __x=x-1;__x<x+w;__x++){
			 _p=__x-x+1;
			 for(uint16_t __y=y;__y<y+h-1;__y++){
				 //_p=__y*w+__x;
				 while(buffer[_p]==buffer[_p+_c*w] and __y+_c+1<y+h){
					 _c++;
				 }
				 if(_c>1){
					 _display->writeFastVLine(__x,__y,_c,buffer[_p]);
					 _p+=(_c*w);
					 __y+=(_c-1);
					 _c=1;
				 }else{
					 _display->writePixel(__x,__y,buffer[_p+=w]);
				 }
			 }
		 }
	}
  _display->endWrite();
}

void displayGauge::setPosition(uint16_t x, uint16_t y){
	if(_h!=0 && _w!=0){
		// in case we're moving this, blank out where it was
		_display->drawRect(_x, _y, _w, _h, _bg);
	}
	_x=x;
	_y=y;
}

void displayGauge::setSize(uint16_t w, uint16_t h){
	if(_x!=0 && _y!=0){
		// in case we're moving this, blank out where it was
		_display->drawRect(_x, _y, _w, _h, _bg);
	}
	_w=w;
	_h=h;
}

void displayGauge::setVisible(bool val){
    _visible=val;
		if(_visible) redraw();
}

void displayGauge::setFGColor(uint16_t fg){
    _fg=fg;
}

void displayGauge::setBGColor(uint16_t bg){
    _bg=bg;
}

void displayGauge::setDisplay(Adafruit_GFX *display){
    _display=display;
}

void displayGauge::setBorder(uint8_t border){
	_border=border;
}

void displayGauge::setBorderColor(uint16_t bo){
	_bo=bo;
}

void displayGauge::setGutter(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b){
	_gutter_l=gutter_l;
	_gutter_r=gutter_r;
	_gutter_t=gutter_t;
	_gutter_b=gutter_b;
}

void displayGauge::setGutter(uint8_t gutter){
	setGutter(gutter,gutter,gutter,gutter);
}

void displayGauge::setMargins(uint8_t gutter){
	setGutter(gutter,gutter,gutter,gutter);
}

void displayGauge::setMargins(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b){
	setGutter(gutter_l,gutter_r,gutter_t,gutter_b);
}

/*
 *
 * text class, displays different values as text
 *
 */

textGauge::textGauge(){
	_x=_y=_w=_h=0;
	_display=0;
	_visible=true;
}

textGauge::textGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
	_visible=true;
}

textGauge::textGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_visible=true;
}

void textGauge::setFont(const GFXfont *font){
    _font=font;
}

void textGauge::setVAlign(uint8_t valign){
	_valign=valign;
}

void textGauge::setHAlign(uint8_t halign){
	_halign=halign;
}
void textGauge::setCursor(uint16_t x, uint16_t y){
	_cursor_x=x;
	_cursor_y=y;
}

void textGauge::redraw(){
	if(_visible){
		//Serial.printf("updating Gauge with %s\n",_txtval);
		char _buf[_val.length()+2];
		int16_t  _bounds_x1, _bounds_y1;
		uint16_t _bounds_w, _bounds_h,
						 _position_x, _position_y = 0;

		_val.toCharArray(_buf,_val.length()+1);
		#ifdef _CONSERVE_RAM_
			_display->setTextColor(_fg);
			_display->setFont(_font);
			_display->getTextBounds(_buf, _x+_border, _y+_border, &_bounds_x1, &_bounds_y1, &_bounds_w, &_bounds_h);
		#else
			_canvas = new GFXcanvas16(_w-2*_border,_h-2*_border);
			_canvas->fillScreen(_bg);
			_canvas->setTextColor(_fg);
			_canvas->setFont(_font);
			_canvas->getTextBounds(_buf, 0, 0, &_bounds_x1, &_bounds_y1, &_bounds_w, &_bounds_h);
		#endif

			if(_valign==TEXT_MIDDLE){
				_position_y=(_h-_bounds_h-_border)>>1;
			}else if(_valign==TEXT_BOTTOM){
				_position_y=_h-(_bounds_h+_gutter_t+2*_border);
			}else if(_valign==TEXT_TOP){
				_position_y=_border+_gutter_b;
			}

			if(_halign==TEXT_CENTER){
				_position_x=(_w-_bounds_w-_border)>>1;
			}else if(_halign==TEXT_RIGHT){
				_position_x=_w-_border*2-_bounds_w-_gutter_r;
			}else if(_halign==TEXT_LEFT){
				_position_x=_border+_gutter_l;
			}
		#ifdef _CONSERVE_RAM_
			_display->fillRect(_x+_border,_y+_border,_w-2*_border,_h-2*_border,_bg);
			_display->setCursor(_x+_position_x,_y+_bounds_h+_position_y);
			_display->print(_buf);
		#else
			_canvas->setCursor(_position_x-_bounds_x1,_position_y-_bounds_y1);
			_canvas->print(_buf);
			pushBitmap(_x+_border,_y+_border,_canvas->getBuffer(),_w-2*_border,_h-2*_border);
			delete _canvas;
		#endif
				if(_border!=0){
				for(uint8_t __j=0;__j<_border;__j++) {
					_display->drawRect(_x+1+__j,_y+__j,_w-2*__j-1,_h-2*__j,_bo);
				}
			}
		_display->display();
	}
}

/*
 * tapeGauge, displays a tape (or bar) corresponding to the numeric
 * value of _val. Can be configured to have up to three color areas
 * (using the setColors method)
 */
tapeGauge::tapeGauge(){
	_x=_y=_w=_h=0;
	_display=0;
	_direction=TAPE_LEFTRIGHT;
	_color0=_color1=_color2=_fg;
	_limit0=_limit1=_min;
	_visible=true;
}

tapeGauge::tapeGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
	_direction=TAPE_LEFTRIGHT;
	_color0=_color1=_color2=_fg;
	_limit0=_limit1=_min;
	_visible=true;
}

tapeGauge::tapeGauge(Adafruit_GFX *display,uint8_t direction){
	_x=_y=_w=_h=0;
	_display=display;
	(direction>=TAPE_LEFTRIGHT&&direction<=TAPE_BOTTOMUP)?_direction=direction:_direction=TAPE_LEFTRIGHT;
	_color0=_color1=_color2=_fg;
	_limit0=_limit1=_min;
	_visible=true;
}

tapeGauge::tapeGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_color0=_color1=_color2=_fg;
	_limit0=_limit1=_min;
	_visible=true;
}

tapeGauge::tapeGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t direction){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	(direction>=TAPE_LEFTRIGHT&&direction<=TAPE_BOTTOMUP)?_direction=direction:_direction=TAPE_LEFTRIGHT;
	_color0=_color1=_color2=_fg;
	_limit0=_limit1=_min;
	_visible=true;
}

void tapeGauge::setMinMax(float minimum, float maximum){
	//Serial.printf("maxmin: min: %i, max: %i\n",minimum, maximum);
	//Serial.printf("maxmin: min: %f, max: %f\n",minimum, maximum);
	if(minimum<=maximum){
		_min=minimum;
		_max=maximum;
	}else{
		_min=maximum;
		_max=minimum;
	}
}

void tapeGauge::setColors(uint16_t color0,float limit0, uint16_t color1, float limit1, uint16_t color2){
	/*
	if(limit0>_min && limit0<_max){
		_limit0=limit0;
		_color0=color0;
		_color1=color1;

		if (limit1>limit0 && limit1<_max) {
			_limit1=limit1;
			_color2=color2;
		}

	}else{
		_color0=_color1=_color2=_fg;
		_limit0=_limit1=_min;
	}
	*/
	_color0=color0;
	_color1=color1;
	_color2=color2;
	_limit0=limit0;
	_limit1=limit1;

}

void tapeGauge::setColors(uint16_t color0,float limit0, uint16_t color1){
	setColors(color0, limit0, color1, limit0, color1);
}

void tapeGauge::setColors(uint16_t color0){
	setColors(color0,_min, color0,_min,color0);
}

void tapeGauge::setDirection(uint8_t direction){
	(direction>=TAPE_LEFTRIGHT&&direction<=TAPE_BOTTOMUP)?_direction=direction:_direction=TAPE_LEFTRIGHT;
}

void tapeGauge::setTicks(uint16_t major, uint16_t minor){
	_tickMajor=major;
	_tickMinor=minor;
}

void tapeGauge::redraw(){
	if(_visible){

		/*
		 * A tape gauge is a horizontal or vertical "tape", representing the value.
		 * There can be numerous different tapes:
		 * - tapes that grow from "min" value to "max" value
		 * - tapes that grow from zero to either negative or positive values
		 * - tapes that actually are not tapes but a "moving needle" that points to
		 *   the corresponding value
		 *
		 * A tape can have, as any other on screen Gauge, a border and a gutter. Both
		 * are "keep out" areas for the Gauge itself. They only differ in color.
		 *
		 * A tape gauge has the following layout:
		 *
		 *     Y
		 *     v
		 *   X>BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB  ^
		 *     BGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGB  |
		 *     BGTTTTTTTTTTTTTTT              GB  |
		 *     BGTTTTTTTTTTTTTTT              GB  h
		 *     BGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGB  |
		 *     BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB  v
	   *     <--------------w---------------->
		 * In tis case, both border and gutter are drawn as 1px wide, but they can
		 * of course be wider.
		 * while the border has a fixed width all around the Gauge, the gutter can
		 * have different values for the four sides, allowing to place the Gauge more
		 * freely in it's cell
		 */
		uint16_t _tape_length, _tapeLimit0, _tapeLimit1,_canvas_w, _canvas_h,_tape_w,_tape_h;

		float __val;
		__val=_val.toFloat();
		_canvas_w=_w-2*_border-_gutter_l-_gutter_r;
		_canvas_h=_h-2*_border-_gutter_t-_gutter_b;
		_tape_w=_canvas_w;
		_tape_h=_canvas_h;
		/*
		 * _tape_[wh] and _canvas_[wh] are synonymous right now, one of them should be removed
		 */

		 /*
		  *
			* Variables:
			* _canvas_[wh], _tape_[wh] - the maximum width and height of the actual tape to be drawn.
			* _gutter_[tblr]           - "free" space incide the border
			* __val                    - the value as float
			* _min, _max               - the minimum and maximum that is accepted for __val
			* _limit0, _limit1         - two limits that are used to set up to three color areas
			* _tape_length             - the scaled length of the tape to be drawn
			* __tape_length						 - stores the previous _tape_length to avoid unecessary redraws when the actual graphic has not changed
			* _tapeLimit0, _tapeLimit1 - limits scaled and shifted to tape_length
			*
			*  -|======>      |-
			*   ^min   ^val   ^max
      *   |<----------->|
			* min: -50
			* max: 50
			* val: -10
			* tape_width: 100
			* 100/(50-(-50))*(-10-(-50)))=1*40
			*    tape_width
			*   tape_length=(tape_width)/(max-min)*(val-min)
			*/
		(_direction==TAPE_LEFTRIGHT || _direction==TAPE_RIGHTLEFT)?_tape_length=(int16_t)((__val-(float)_min)*(float)_tape_w/(_max-_min)+0.5):_tape_length=(int16_t)((__val-(float)_min)*(float)_tape_h/(_max-_min)+0.5);
		if(_min<0){
			char _buf[_val.length()+2];
			_val.toCharArray(_buf,_val.length()+1);
			Serial.printf("min: %f, max: %f, $val: %s val: %f, tape: %i\n",_min,_max,_buf,__val,_tape_length);
		}else{
			Serial.println("normal");
		}
		if(_tape_length!=__tape_length){
			#ifdef _CONSERVE_RAM_
				_display->fillRect(_x,_y,_w,_h,_bg);
			#else
				_canvas = new GFXcanvas16(_canvas_w,_canvas_h);
				_canvas->fillScreen(_bg);
			#endif
			/*
			 *
			 * The whole tape placement isn't right yet. Too much mucking around with paddings...
			 * and right now, they don't line up between the two drawing methods either.
			 *
			 */

			if(_direction==TAPE_LEFTRIGHT || _direction==TAPE_RIGHTLEFT){
				(__val>_limit0)?_tapeLimit0=(uint16_t)((_limit0-(float)_min)*_tape_w/(_max-_min)+0.5):_tapeLimit0=0;
				(__val>_limit1)?_tapeLimit1=(uint16_t)((_limit1-(float)_min)*_tape_w/(_max-_min)+0.5):_tapeLimit1=0;
				if(_min<0)Serial.printf("tape_limit0: %i, tape_limit1: %i, tape_length: %i\ncolor0: 0x%04x, color1: 0x%04x ,color2: 0x%04x\n\n",_tapeLimit0,_tapeLimit1,_tape_length,_color0,_color1,_color2);
				if(_direction==TAPE_LEFTRIGHT){
					if(_tapeLimit1>0) {
						fillRectHelper(0,0,_tapeLimit0,_tape_h,_color0);
						fillRectHelper(_tapeLimit0,0,_tapeLimit1-_tapeLimit0,_tape_h,_color1);
						fillRectHelper(_tapeLimit1,0,_tape_length-_tapeLimit1,_tape_h,_color2);
					}else if(_tapeLimit0>0){
						fillRectHelper(0,0,_tapeLimit0,_tape_h,_color0);
						fillRectHelper(_tapeLimit0,0,_tape_length-_tapeLimit0,_tape_h,_color1);
					}else{
						fillRectHelper(0,0,_tape_length,_tape_h,_color0);
					}
				}else{
					if(_tapeLimit1>0) {
						fillRectHelper(_tape_w-_tapeLimit0,0,_tapeLimit0,_tape_h,_color0);
						fillRectHelper(_tape_w-_tapeLimit1,0,_tapeLimit1-_tapeLimit0,_tape_h,_color1);
						fillRectHelper(_tape_w-_tape_length,0,_tape_length-_tapeLimit1,_tape_h,_color2);
					}else if(_tapeLimit0>0){
						fillRectHelper(_tape_w-_tapeLimit0,0,_tapeLimit0,_tape_h,_color0);
						fillRectHelper(_tape_w-_tape_length,0,_tape_length-_tapeLimit0,_tape_h, _color1);
					}else{
						fillRectHelper(_tape_w-_tape_length,0,_tape_length-_tapeLimit1,_tape_h,_color0);
					}
				}
			}else{
				(__val>_limit0)?_tapeLimit0=(uint16_t)(_limit0*_tape_h/(_max-_min)+0.5):_tapeLimit0=0;
				(__val>_limit1)?_tapeLimit1=(uint16_t)(_limit1*_tape_h/(_max-_min)+0.5):_tapeLimit1=0;
				if(_direction==TAPE_TOPDOWN){
					if(_tapeLimit1>0) {
						fillRectHelper(0,0,_tape_w,_tapeLimit0,_color0);
						fillRectHelper(0,_tapeLimit0,_tape_w,_tapeLimit1-_tapeLimit0,_color1);
						fillRectHelper(0,_tapeLimit1,_tape_w,_tape_length-_tapeLimit1,_color2);
					}else if(_tapeLimit0>0){
						fillRectHelper(0,0,_tape_w,_tapeLimit0,_color0);
						fillRectHelper(0,_tapeLimit0, _tape_w, _tape_length-_tapeLimit0, _color1)
					}else{
						fillRectHelper(0,0,_tape_w,_tape_length,_color0);
					}
				}else{
					if(_tapeLimit1>0) {
						fillRectHelper(0,_h-_tapeLimit0,_tape_w,_tapeLimit0,_color0);
						fillRectHelper(0,_h-_tapeLimit1,_tape_w,_tapeLimit1-_tapeLimit0,_color1);
						fillRectHelper(0,_h-_tape_length,_tape_w, _tape_length-_tapeLimit1,_color2);
					}else if(_tapeLimit0>0){
						fillRectHelper(0,_h-_tapeLimit0,_tape_w,_tapeLimit0,_color0);
						fillRectHelper(0,_h-_tape_length,_tape_w,_tape_length-_tapeLimit0,_color1);
					}else{
						fillRectHelper(0,_h-_tape_length,_tape_w,_tape_length,_color0);
					}
				}
			}

			//Serial.printf("data: value: %f, tape_length: %i\n min: %f, max: %f \nlimit0: %f, limit1: %f\ntape_limit0: %i, tape_limit1: %i\n\n",__val, _tape_length, _min,_max,_limit0,_limit1,_tapeLimit0,_tapeLimit1);

			__tape_length=_tape_length; //store current length so we don't have to re-draw on no visible change
			#ifndef _CONSERVE_RAM_
				pushBitmap(_x+_border+_gutter_l+1,_y+_border+_gutter_t,_canvas->getBuffer(),_canvas_w,_canvas_h);
				delete _canvas;
			#endif

			if(_border!=0){
				for(uint8_t __j=0;__j<_border;__j++) {
					_display->drawRect(_x+1+__j,_y+__j,_w-2*__j-1,_h-2*__j,_bo);
				}
			}
			//_display->drawRect(_x+_border+1,_y+_border,_w-2*_border-1,_h-2*_border,0xff00);
			_display->display();
		}
	}
}
