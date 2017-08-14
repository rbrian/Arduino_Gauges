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
}

displayGauge::displayGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
}

displayGauge::displayGauge(){
    _x=_y=_w=_h=0;
    _display=NULL;
}

void displayGauge::pushBitmap(uint16_t x, uint16_t y, uint16_t* buffer, uint16_t w, uint16_t h){
  int _p=1;
  int _c=0;
  _display->startWrite();
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
  _display->endWrite();
}

void displayGauge::setPosition(uint16_t x, uint16_t y){
	_x=x;
	_y=y;
}

void displayGauge::setSize(uint16_t w, uint16_t h){
	_w=w;
	_h=h;
}


void displayGauge::setVisible(bool val){
    _visible=val;
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

/*
 *
 * text class, displays different values as text
 *
 */

textGauge::textGauge(){
}

textGauge::textGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
}

textGauge::textGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
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

void textGauge::setGutter(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b){
	_gutter_l=gutter_l;
	_gutter_r=gutter_r;
	_gutter_t=gutter_t;
	_gutter_b=gutter_b;
}

void textGauge::redraw(){
	//Serial.printf("updating Gauge with %s\n",_txtval);
	char _buf[_val.length()+2];
	int16_t  _bounds_x1, _bounds_y1;
	uint16_t _bounds_w, _bounds_h,
					 _position_x, _position_y;
	_canvas = new GFXcanvas16(_w-2*_border,_h-2*_border);
	_canvas->fillScreen(_bg);
	_canvas->setTextColor(_fg);
	_canvas->setFont(_font);
	_val.toCharArray(_buf,_val.length()+1);
	_canvas->getTextBounds(_buf, 0, 0, &_bounds_x1, &_bounds_y1, &_bounds_w, &_bounds_h);

	if(_valign==MIDDLE){
		_position_y=(_h-_bounds_h-_border)>>1;
	}else if(_valign==BOTTOM){
		_position_y=_h-(_bounds_h+_gutter_t+2*_border);
	}else if(_valign==TOP){
		_position_y=_border+_gutter_b;
	}

	if(_halign==CENTER){
		_position_x=(_w-_bounds_w-_border)>>1;
	}else if(_halign==RIGHT){
		_position_x=_w-_border*2-_bounds_w-_gutter_r;
	}else if(_halign==LEFT){
		_position_x=_border+_gutter_l;
	}

	//_canvas->drawRect(_position_x,_position_y,_bounds_w,_bounds_h,0xFFE0);
	//_canvas->drawFastHLine(_position_x,_position_y,8,0xF800);
	//_canvas->drawFastVLine(_position_x,_position_y,8,0xF800);
	_canvas->setCursor(_position_x-_bounds_x1,_position_y-_bounds_y1);

	_canvas->print(_buf);
	//Serial.printf("wrote %s \n",_buf);

	pushBitmap(_x+_border,_y+_border,_canvas->getBuffer(),_w-2*_border,_h-2*_border);
	delete _canvas;
	if(_border!=0){
		for(uint8_t __j=0;__j<_border;__j++) {
			_display->drawRect(_x+1+__j,_y+__j,_w-2*__j-1,_h-2*__j,_bo);
		}
	}
	_display->display();
}
