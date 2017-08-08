#ifndef _GAUGESH_
	#include "Gauges.h"
#endif

/*
 *
 * Base Gauge class, knows where on what screen it is and has some basic properties.
 *
 */

Gauge::Gauge(Adafruit_GFX *display,uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
}

Gauge::Gauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
}

Gauge::Gauge(){
    _x=_y=_w=_h=0;
    _display=NULL;
}

void Gauge::pushBitmap(uint16_t x, uint16_t y, uint16_t* buffer, uint16_t w, uint16_t h){
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

void Gauge::setPosition(uint16_t x, uint16_t y){
	_x=x;
	_y=y;
}

void Gauge::setSize(uint16_t w, uint16_t h){
	_w=w;
	_h=h;
}

void Gauge::setAutoRedraw(bool val){
    _autoRedraw=val;
}

void Gauge::setVisible(bool val){
    _visible=val;
}

void Gauge::setFGColor(uint16_t fg){
    _fg=fg;
}

void Gauge::setBGColor(uint16_t bg){
    _bg=bg;
}

void Gauge::setDisplay(Adafruit_GFX *display){
    _display=display;
}

void Gauge::setBorder(uint8_t border){
	_border=border;
}

void Gauge::setBorderColor(uint16_t bo){
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
	_txtval[0]=0;
}

textGauge::textGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_txtval[0]=0;
}

void textGauge::setFont(const GFXfont *font){
    _font=font;
}

void textGauge::setCursor(uint16_t x, uint16_t y){
	_cursor_x=x;
	_cursor_y=y;
}

void textGauge::setFormatString(String format){
    _format=format;
}

void textGauge::redraw(){
	//Serial.printf("updating Gauge with %s\n",_txtval);
	_canvas = new GFXcanvas16(_w-2*_border,_h-2*_border);
	_canvas->fillScreen(_bg);
	_canvas->setCursor(_cursor_x,_cursor_y);
	_canvas->setTextColor(_fg);
	_canvas->setFont(_font);
	_canvas->printf("%s",_txtval);

	pushBitmap(_x+_border,_y+_border,_canvas->getBuffer(),_w-2*_border,_h-2*_border);
	delete _canvas;
	if(_border!=0){
		for(uint8_t __j=0;__j<_border;__j++) {
			_display->drawRect(_x+1+__j,_y+__j,_w-2*__j-1,_h-2*__j,_bo);
		}
	}
	_display->display();
}

void textGauge::update(char* val){
	strcpy(_txtval,val);
	//Serial.printf("update textGauge from char*, input: %s, update: %s\n",val,_txtval);
	redraw();
}
void textGauge::update(const char* val){
	strcpy(_txtval,val);
	//Serial.printf("update textGauge from const char*, input: %s, update: %s\n",val,_txtval);
	redraw();
}
void textGauge::update(int val){
	if(_format.length()==0){
		sprintf(_txtval,"%i",val);
	}else{
		char __format[_format.length()+1];
		_format.toCharArray(__format,_format.length()+1);
		sprintf(_txtval,__format,val);
	}
	//Serial.printf("update textGauge from int, input: %i, update: %s\n",val,_txtval);
	redraw();
}

void textGauge::update(float val){
	if(_format.length()==0){
		sprintf(_txtval,"%f",val);
	}else{
		char __format[_format.length()+1];
		_format.toCharArray(__format,_format.length()+1);
		sprintf(_txtval,__format,val);
	}
	//Serial.printf("update textGauge from float, input: %f, update: %s\n",val,_txtval);
	redraw();
}
void textGauge::update(double val){
	if(_format.length()==0){
		sprintf(_txtval,"%f",val);
	}else{
		char __format[_format.length()+1];
		_format.toCharArray(__format,_format.length()+1);
		sprintf(_txtval,__format,val);
	}
	//Serial.printf("update textGauge from double, input: %f, update: %s\n",val,_txtval);
	redraw();
}
/* void textGauge::update(time_t val){
	sprintf(_txtval,"%02i:%02i",val);
	redraw();
}*/

/*
 * integerGauge - holds a basic integer value
 *
 */

integerGauge::integerGauge(){
}

integerGauge::integerGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
	_val=0;
}

integerGauge::integerGauge(Adafruit_GFX *display, int val){
	_x=_y=_w=_h=0;
	_display=display;
	_val=val;
}

integerGauge::integerGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_val=0;
}

void integerGauge::setValue(int val){
    bool __redrawFlag=false;
    if(val!=_val) __redrawFlag=true;
    //Serial.printf("setting value %i, was: %i",val,_val);
    _val=val;
    if(_autoRedraw==true && __redrawFlag==true) update(_val);
}

int integerGauge::getValue(){
    return _val;
}

/*
 * stringGauge - holds a c string
 *
 */

stringGauge::stringGauge(){
}

stringGauge::stringGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
	_val[0]=0;
}

stringGauge::stringGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_val[0]=0;
}

void stringGauge::setValue(char *val){
	bool __redrawFlag=false;
	if(strcmp(val,_val)!=0) __redrawFlag=true;
	strcpy(_val,val);
	if(_autoRedraw==true && __redrawFlag==true) update(_val);
}

void stringGauge::setValue(const char *val){
    bool __redrawFlag=false;
		if(strcmp(val,_val)!=0) __redrawFlag=true;
		strcpy(_val,val);
		if(_autoRedraw==true && __redrawFlag==true) update(_val);
}

void stringGauge::setValue(String val){
    bool __redrawFlag=false;
		char __val[val.length()+1];
		val.toCharArray(__val,val.length()+1);
    if(strcmp(__val,_val)!=0) __redrawFlag=true;
    strcpy(_val,__val);
		if(_autoRedraw==true && __redrawFlag==true) update(_val);
}

char* stringGauge::getValue(){
    return _val;
}

clockGauge::clockGauge(){
}

clockGauge::clockGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
	_hour=0;
	_min=0;
	_second=0;
}

clockGauge::clockGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
}

void clockGauge::setValue(uint8_t hour, uint8_t min){
	bool __redrawFlag;
	if(hour!=_hour || min!=_min) __redrawFlag=true;
	_hour=hour;
	_min=min;
	if(_autoRedraw==true && __redrawFlag==true) redraw();
}

void clockGauge::setValue(uint8_t hour, uint8_t min, uint8_t second){
	bool __redrawFlag;
	if(hour!=_hour || min!=_min ||second != _second) __redrawFlag=true;
	_hour=hour;
	_min=min;
	_second=second;
	if(_autoRedraw==true && __redrawFlag==true) redraw();
}
/*
void textGauge::setValue(char *valtextGauge::update
    bool __redrawFlag=false;
    if(!strcmp(val,_val)) __redrawFlag=true;
    strcpy(_val,val); //this will fail for val with more than 255 chars
    if(_autoRedraw==true && __redrawFlag==true) redraw();
}

void textGauge::setValue(const char *val){
    bool __redrawFlag=false;
    if(!strcmp(val,_val)) __redrawFlag=true;
    strcpy(_val,val);
    if(_autoRedraw==true && __redrawFlag==true) redraw();
}
*/
