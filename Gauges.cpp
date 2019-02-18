#ifndef _GAUGESH_
	#include "Gauges.h"
#endif

/*
 *
 * Base Gauge class, only knows how to hold and convert a value
 *
 */

Gauge::Gauge(){
	_val="";
}

Gauge::~Gauge(){
	//default destructor
}

void Gauge::redraw(){}
void Gauge::redraw(bool full){}

void Gauge::setValue(int val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw(false);
}

void Gauge::setValue(short val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw(false);
}

void Gauge::setValue(long val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw(false);
}

void Gauge::setValue(double val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw(false);
}

void Gauge::setValue(float val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw(false);
}

void Gauge::setValue(String val){
	(_val!=val)?_changed=true:_changed=false;
	_val=val;
	if(_changed == true && _autoRedraw==true) redraw(false);
}

void Gauge::setValue(char* val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw(false);
}

void Gauge::setValue(const char* val){
	(_val!=String(val))?_changed=true:_changed=false;
	_val=String(val);
	if(_changed == true && _autoRedraw==true) redraw(false);
}

/*
 * End base Gauge class
 */

 /*
  *
  * Base Gauge class, knows where on what screen it is, has dimensions, visibility, persistent and an associated canvas
  *
  */
/*
displayGauge::displayGauge(Adafruit_GFX *display,uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_visible=true;
	_persistent=true;
}

displayGauge::displayGauge(Adafruit_GFX *display,uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool persistent, bool autoRedraw){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_visible=true;
	_persistent=persistent;
}

displayGauge::displayGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
	_visible=true;
	_persistent=true;
}

displayGauge::displayGauge(Adafruit_GFX *display, bool persistent){
	_x=_y=_w=_h=0;
	_display=display;
	_visible=true;
	_persistent=persistent;
}
*/
displayGauge::displayGauge(){
    _x=_y=_w=_h=0;
    _display=NULL;
		_visible=true;
		_persistent=true;
}

displayGauge::~displayGauge(){
	_display->fillRect(_x, _y, _w, _h, _bg);
	if(_persistent){
		delete _canvas;
	}
}

void displayGauge::setPosition(uint16_t x, uint16_t y){
	if(!(_h==0 || _w==0) && (x!=_x || y!=_y)){  // only if neither _h nor _w are zero and
																							// if either the x or y component has been changed
																							// that is: if the location has changed in any way
		bool __visible=_visible;
		if(_visible){
			setVisible(false); 											// hide for resize
			if(!_autoRedraw) redraw();							// force redraw, even if no autoRedraw
		}
		(x&0x8000)?_x=0:_x=x; 										// reject values that would be negative if x/y were signed.
		(y&0x8000)?_y=0:_y=y;
		if(__visible){
			setVisible(__visible);
			if(!_autoRedraw) redraw();							// force redraw, even if no autoRedraw
		}
	}
}

void displayGauge::setSize(uint16_t w, uint16_t h){
	if(w!=_w || h!=_h){
		bool __visible=_visible;
		if(_visible){
			setVisible(false); //hide for resize
			if(!_autoRedraw) redraw();
		}
		_w=w;
		_h=h;
		if(__visible){
			setVisible(__visible);
			if(!_autoRedraw) redraw();
		}
	}
}

void displayGauge::setVisible(bool val){
    _visible=val;
		if(_autoRedraw==true) redraw();
}

void displayGauge::setTransparent(bool val){
    _transparent=val;
		if(_autoRedraw==true) redraw();
}

void displayGauge::setPersistent(bool val){
	_persistent=val;
}

void displayGauge::setFGColor(color24 fg){
    _fg=fg;
		if(_canvas) {
			_canvas->setColor(FG,fg);
			_canvas->setTextColor(FG);
		}
		if(_autoRedraw==true) redraw();
}

void displayGauge::setBGColor(color24 bg){
    _bg=bg;
		if(_canvas) _canvas->setColor(BG,bg);
		if(_autoRedraw==true) redraw();
}

void displayGauge::setBorderColor(color24 bo){
	_bo=bo;
	if(_canvas) _canvas->setColor(BO,bo);
	if(_autoRedraw==true) redraw();
}

void displayGauge::setAccentColor(color24 ac){
	_ac=ac;
	if(_canvas) _canvas->setColor(AC,ac);
	if(_autoRedraw==true) redraw();
}

void displayGauge::setDisplay(Adafruit_GFX *display){
	if(display!=_display){
		bool __visible=_visible;
		if(_visible){
			setVisible(false); //hide to move from display to display
			if(!_autoRedraw) redraw();
		}
		_display=display;
		if(__visible){
			setVisible(__visible);
			if(!_autoRedraw) redraw();
		}
	}
}

void displayGauge::setBorder(uint8_t border){
	_border=border;
	if(_autoRedraw==true) redraw();
}

void displayGauge::setRotation(uint8_t rot){
	_rotation=rot;
	if(_canvas) _canvas->setRotation(rot);
	if(_autoRedraw==true) redraw();
}

void displayGauge::setGutter(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b){
	_gutter_l=gutter_l;
	_gutter_r=gutter_r;
	_gutter_t=gutter_t;
	_gutter_b=gutter_b;
	if(_autoRedraw==true) redraw();
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

void Gauge::setAutoRedraw(bool val){
    _autoRedraw=val;
		if(_autoRedraw==true) redraw();
}

boolean displayGauge::collisionDetect(displayGauge *other){
	if(other->_display != this->_display) return false; //not the same display. Can't collide

	uint16_t _oX1=other->_x;
	uint16_t _oX2=other->_x+other->_w;
	uint16_t _oY1=other->_y;
	uint16_t _oY2=other->_y+other->_h;
  //Serial.printf("oX1 %i, oX2 %i, oY1 %i, oY2 %i\n",_oX1, _oX2,_oY1, _oY2);
	uint16_t _tX1=this->_x;
	uint16_t _tX2=this->_x+this->_w;
	uint16_t _tY1=this->_y;
	uint16_t _tY2=this->_y+this->_h;
	//Serial.printf("tX1 %i, tX2 %i, tY1 %i, tY2 %i\n",_tX1, _tX2,_tY1, _tY2);
  static bool _in;

	if((_oX1 > _tX1) && (_oX1 <= _tX2)){  // starts within width of this
		if((_oX2 > _tX1) && (_oX2 <= _tX2)) {
			_in=true;
			return true;
		} // ends within width of this
		if((_oX2 > _tY2) && (_oY2 <= _tY2)) {
			_in=true;
			return true;
		} // ends within hight of this
	}
	if((_oY1 > _tY1) && (_oY1 <= _tY2)) { // starts within hight of this
		if((_oX2 > _tX1 && _oX2 <= _tX2)) {
			_in=true;
			return true;
		}	// ends within width of this
		if((_oX2 > _tY2 && _oY2 <= _tY2)) {
			_in=true;
			return true; }	// ends within hight of this
	}
	if (_in){
		_in=false;
		return true;
	} // was in on last call, now out, need refresh
	return false;
}


/*
 *
 * text class, displays different values as text
 *
 */

textGauge::textGauge(){
	/*
	 * minimal constructor
	 */

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
	_persistent=true;
}

textGauge::textGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool persistent){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_visible=true;
	_persistent=persistent;
}

void textGauge::setFont(const GFXfont *font){
    _font=font;
		if(_canvas) _canvas->setFont(font);
		if(_autoRedraw==true) redraw();
}

void textGauge::setVAlign(uint8_t valign){
	_valign=valign;
	if(_autoRedraw==true) redraw();
}

void textGauge::setHAlign(uint8_t halign){
	_halign=halign;
	if(_autoRedraw==true) redraw();
}

void textGauge::setTextWrap(uint8_t wrap){
	_textWrap=wrap;
	if(_canvas) _canvas->setTextWrap(wrap);
	if(_autoRedraw==true) redraw();
}

void textGauge::setCursor(uint16_t x, uint16_t y){
	_cursor_x=x;
	_cursor_y=y;
}

void textGauge::redraw(){
	redraw(true);
}

void textGauge::redraw(bool full){
	/*
	*  v gauge._x, gauge._y
	*  +-------------------------------------------------+
	*  |#################################################|
	*  |#                                               #|
	*  |#        ___  ____                              #|
	*  |#       |__ \|___ \                             #|
  *  |#         ) | __) |                             #|
  *  |#        / / |__ <                              #|
  *  |#       / /_ ___) |                             #|
  *  |#      |____|____/                              #|
	*  |#                                               #|
	*  |#                                               #|
	*  |#                                               #|
	*  |#                                               #|
	*  |#                                               #|
	*  |#################################################|
	*  +-------------------------------------------------+
	*
  */

	if(_visible){
		//Serial.printf("updating Gauge with %s\n",_txtval);
		char _buf[_val.length()+2];
		int16_t  _bounds_x1, _bounds_y1;
		uint16_t _bounds_w, _bounds_h,
						 _position_x, _position_y = 0;

		_val.toCharArray(_buf,_val.length()+1);
		if(_wr==0 && _hr==0) full=true; //first time do a full redraw
		#ifdef _CONSERVE_RAM_
			_display->setTextColor(_fg);
			_display->setTextWrap(_textWrap);
			_display->setFont(_font);
			_display->getTextBounds(_buf, _x+_border, _y+_border, &_bounds_x1, &_bounds_y1, &_bounds_w, &_bounds_h);
		#else
			if(!_persistent || _canvas==0) {
				//_canvas = new GFXiCanvas(_w-2*_border,_h-2*_border,2);
				//Serial.printf("creating Canvas with w: %i, h: %i\n",_w, _h);
				_canvas = new GFXiCanvas(_w,_h,2);
				_canvas->setRotation(_rotation);
				_canvas->setColor(BG,_bg);
				_canvas->setColor(FG,_fg);
				_canvas->setColor(AC,_ac);
				_canvas->setColor(BO,_bo);
				_canvas->setTextWrap(_textWrap);
				_canvas->setTransparent((uint8_t) BG,false);
				_canvas->setTextColor(FG);
				_canvas->setFont(_font);
				_canvas->fillScreen(BG);
				}

			_canvas->getTextBounds(_buf, 0, 0, &_bounds_x1, &_bounds_y1, &_bounds_w, &_bounds_h);
			Serial.printf("<===========================================>\n");
			Serial.printf("Value       :  %s\n",_buf);
			Serial.printf("Text bounds :  x=%03i, y=%03i, w=%03i, h=%03i\n", _bounds_x1, _bounds_y1, _bounds_w, _bounds_h);
			
		#endif
			Serial.printf("Gauge size  :  w=%03i, h=%03i\n",_w,_h);

			// positioning
			if(_bounds_h < _h){
				if(_valign==TEXT_MIDDLE){
					_position_y=(_h+abs(_bounds_h))>>1;
					//_position_y=(_h-abs(_bounds_h)+abs(_bounds_h)>>1-_bounds_y1)>>1;
				}else if(_valign==TEXT_BOTTOM){
					_position_y=_h-(_bounds_h+_gutter_t+2*_border);
				}else if(_valign==TEXT_TOP){
					_position_y=_border+_gutter_b;
				}
			}else{
				_position_y=_border+_gutter_b;
			}
	
			if(_halign==TEXT_CENTER){
				_position_x=(_w-abs(_bounds_w))>>1;
			}else if(_halign==TEXT_RIGHT){
				_position_x=_w-_border*2-_bounds_w-_gutter_r;
			}else if(_halign==TEXT_LEFT){
				_position_x=_border+_gutter_l;
			}

		#ifdef _CONSERVE_RAM_
			_display->fillRect(_x+_border,_y+_border,_w-2*_border,_h-2*_border,_bg);
			_display->setCursor(_x+_position_x,_y+_bounds_h+_position_y);
			_display->print(_buf);
			if(_border!=0){
			for(uint8_t __j=0;__j<_border;__j++) {
				_display->drawRect(_x+1+__j,_y+__j,_w-2*__j-1,_h-2*__j,_bo);
			}
		#else
			_canvas->setCursor(_position_x,_position_y);
			Serial.printf("cursr pos:     x=%03i, y=%03i\n",_position_x+_bounds_x1,_position_y+_bounds_y1);
			Serial.printf("Text bounds :  x=%03i, y=%03i, w=%03i, h=%03i\n", _bounds_x1, _bounds_y1, _bounds_w, _bounds_h);
			Serial.printf("Rect to draw:  x=%03i, y=%03i, w=%03i, h=%03i\n",_position_x+_bounds_x1,_position_y+_bounds_y1, _bounds_w, _bounds_h);
			if(full){
				_canvas->fillScreen(BG);
				Serial.printf("doing full redraw\n");
			}else{
				Serial.printf("Rect before  : x=%03i, y=%03i, w=%03i, h=%03i\n",_xr,_yr,_wr,_hr);
				if(_bounds_w>_wr) _wr=_bounds_w;
				if(_bounds_h>_hr) _hr=_bounds_h;
				if(_position_x+_bounds_x1<_xr) _xr=(_position_x+_bounds_x1)<0?0:_position_x+_bounds_x1;
				if(_position_y+_bounds_y1<_yr) _yr=(_position_y+_bounds_y1)<0?0:_position_y+_bounds_y1;
				if(_xr<0) _xr=0;
				if(_yr<0) _yr=0;
				Serial.printf("Rect to clear: x=%03i, y=%03i, w=%03i, h=%03i\n",_xr,_yr,_wr,_hr);

				_canvas->fillRect(_xr,_yr,_wr+1,_hr+1,BG);
			}
			
			_canvas->print(_buf);
			if(_border!=0){
			for(uint8_t __j=0;__j<_border;__j++) {
				_canvas->drawRect(__j,__j,_w-2*__j,_h-2*__j,BO);
			}
			//_canvas->drawFastHLine(0,_h>>1,_w,BO);
			//_canvas->drawFastVLine(_w>>1,0,_h,BO);
			_canvas->setTextHint(true);
			if(full){
				_canvas->draw((uint16_t)_x,(uint16_t)_y,_display);
				_xr,_yr=0;
				_wr=_w;
				_hr=_h;
				Serial.printf("Rect to store: x=%03i, y=%03i, w=%03i, h=%03i\n",_xr,_yr,_wr,_hr);
			}else{
				_canvas->draw((uint16_t)_x,(uint16_t)_y,_display,_xr,_yr,_wr,_hr);
				_xr=_position_x+_bounds_x1;
				_yr=_position_y+_bounds_y1;
				_wr=_bounds_w;
				_hr=_bounds_h;
				Serial.printf("Rect to store: x=%03i, y=%03i, w=%03i, h=%03i\n",_xr,_yr,_wr,_hr);
			}


			if(!_persistent) {
				delete _canvas;
			}
		#endif

			}
	}else{
		_display->fillRect(_x,_y,_w,_h,_bg); //clear if invisible
	}
	_display->display();
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
	if(_autoRedraw) redraw();
}

void tapeGauge::setColors(color24 color0,float limit0, color24 color1, float limit1, color24 color2){
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
	if(_autoRedraw) redraw();
}

void tapeGauge::setColors(color24 color0,float limit0, color24 color1){
	setColors(color0, limit0, color1, limit0, color1);
}

void tapeGauge::setColors(color24 color0){
	setColors(color0,_min, color0,_min,color0);
}

void tapeGauge::setFGColor(color24 color0){
	setColors(color0);
}

void tapeGauge::setDirection(uint8_t direction){
	(direction>=TAPE_LEFTRIGHT&&direction<=TAPE_BOTTOMUP)?_direction=direction:_direction=TAPE_LEFTRIGHT;
	if(_autoRedraw) redraw();
}

void tapeGauge::setTicks(uint16_t major, uint16_t minor){
	_tickMajor=major;
	_tickMinor=minor;
	if(_autoRedraw) redraw();
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
				if(!_persistent || _canvas==0) {
					_canvas = new GFXiCanvas(_canvas_w,_canvas_h,3);
					_canvas->setColor(BG,_bg);
					_canvas->setColor(FG,_fg);
					_canvas->setColor(AC,_ac);
					_canvas->setColor(BO,_bo);
					_canvas->setColor(TAPE_C0,_color0);
					_canvas->setColor(TAPE_C1,_color1);
					_canvas->setColor(TAPE_C2,_color2);
					_canvas->setTransparent((uint8_t) BG,false);
					_canvas->setTextColor(FG);
					_canvas->fillScreen(BG);
				}

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
						fillRectHelper(0,0,_tapeLimit0,_tape_h,TAPE_C0);
						fillRectHelper(_tapeLimit0,0,_tapeLimit1-_tapeLimit0,_tape_h,TAPE_C1);
						fillRectHelper(_tapeLimit1,0,_tape_length-_tapeLimit1,_tape_h,TAPE_C2);
					}else if(_tapeLimit0>0){
						fillRectHelper(0,0,_tapeLimit0,_tape_h,TAPE_C0);
						fillRectHelper(_tapeLimit0,0,_tape_length-_tapeLimit0,_tape_h,TAPE_C1);
					}else{
						fillRectHelper(0,0,_tape_length,_tape_h,TAPE_C0);
					}
				}else{
					if(_tapeLimit1>0) {
						fillRectHelper(_tape_w-_tapeLimit0,0,_tapeLimit0,_tape_h,TAPE_C0);
						fillRectHelper(_tape_w-_tapeLimit1,0,_tapeLimit1-_tapeLimit0,_tape_h,TAPE_C1);
						fillRectHelper(_tape_w-_tape_length,0,_tape_length-_tapeLimit1,_tape_h,TAPE_C2);
					}else if(_tapeLimit0>0){
						fillRectHelper(_tape_w-_tapeLimit0,0,_tapeLimit0,_tape_h,TAPE_C0);
						fillRectHelper(_tape_w-_tape_length,0,_tape_length-_tapeLimit0,_tape_h, TAPE_C1);
					}else{
						fillRectHelper(_tape_w-_tape_length,0,_tape_length-_tapeLimit1,_tape_h,TAPE_C0);
					}
				}
			}else{
				(__val>_limit0)?_tapeLimit0=(uint16_t)(_limit0*_tape_h/(_max-_min)+0.5):_tapeLimit0=0;
				(__val>_limit1)?_tapeLimit1=(uint16_t)(_limit1*_tape_h/(_max-_min)+0.5):_tapeLimit1=0;
				if(_direction==TAPE_TOPDOWN){
					if(_tapeLimit1>0) {
						fillRectHelper(0,0,_tape_w,_tapeLimit0,TAPE_C0);
						fillRectHelper(0,_tapeLimit0,_tape_w,_tapeLimit1-_tapeLimit0,TAPE_C1);
						fillRectHelper(0,_tapeLimit1,_tape_w,_tape_length-_tapeLimit1,TAPE_C2);
					}else if(_tapeLimit0>0){
						fillRectHelper(0,0,_tape_w,_tapeLimit0,TAPE_C0);
						fillRectHelper(0,_tapeLimit0, _tape_w, _tape_length-_tapeLimit0, TAPE_C1)
					}else{
						fillRectHelper(0,0,_tape_w,_tape_length,TAPE_C0);
					}
				}else{
					if(_tapeLimit1>0) {
						fillRectHelper(0,_h-_tapeLimit0,_tape_w,_tapeLimit0,TAPE_C0);
						fillRectHelper(0,_h-_tapeLimit1,_tape_w,_tapeLimit1-_tapeLimit0,TAPE_C1);
						fillRectHelper(0,_h-_tape_length,_tape_w, _tape_length-_tapeLimit1,TAPE_C2);
					}else if(_tapeLimit0>0){
						fillRectHelper(0,_h-_tapeLimit0,_tape_w,_tapeLimit0,TAPE_C0);
						fillRectHelper(0,_h-_tape_length,_tape_w,_tape_length-_tapeLimit0,TAPE_C1);
					}else{
						fillRectHelper(0,_h-_tape_length,_tape_w,_tape_length,TAPE_C0);
					}
				}
			}

			//Serial.printf("data: value: %f, tape_length: %i\n min: %f, max: %f \nlimit0: %f, limit1: %f\ntape_limit0: %i, tape_limit1: %i\n\n",__val, _tape_length, _min,_max,_limit0,_limit1,_tapeLimit0,_tapeLimit1);

			__tape_length=_tape_length; //store current length so we don't have to re-draw on no visible change
			#ifndef _CONSERVE_RAM_
				_canvas->draw(_x+_border+_gutter_l+1,_y+_border+_gutter_t,_display);
				if(!_persistent){
					delete _canvas;
				}
			#endif

			if(_border!=0){
				for(uint8_t __j=0;__j<_border;__j++) {
					_display->drawRect(_x+1+__j,_y+__j,_w-2*__j-1,_h-2*__j,_bo);
				}
			}
			//_display->drawRect(_x+_border+1,_y+_border,_w-2*_border-1,_h-2*_border,0xff00);
		}
	}else{
		_display->fillRect(_x,_y,_w,_h,_bg); //clear if invisible
	}
	_display->display();
}
