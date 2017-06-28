#include "Gauges.h"

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
        _display->drawFastHLine(__x,__y,_c,buffer[_p]);
        _p+=(_c);
        __x+=(_c-1);
        _c=1;
      }else{
        _display->drawPixel(__x,__y,buffer[_p++]);
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

void textGauge::setFont(const GFXfont *font){
    _font=font;
}

void textGauge::setCursor(uint16_t x, uint16_t y){
	_cursor_x=x;
	_cursor_y=y;
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

textGauge::textGauge(){
}

textGauge::textGauge(Adafruit_GFX *display){
	_x=_y=_w=_h=0;
	_display=display;
	_val[0]=0;
}

textGauge::textGauge(Adafruit_GFX *display, char *val){
	_x=_y=_w=_h=0;
	_display=display;
	strcpy(_val,val); //this will fail for val with more than 255 chars
}

textGauge::textGauge(Adafruit_GFX *display, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	_display=display;
	_x=x;
	_y=y;
	_w=w;
	_h=h;
	_val[0]=0;
}

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

void textGauge::setValue(char *val){
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
    
void integerGauge::setValue(int val){
    bool __redrawFlag=false;
    if(val!=_val) __redrawFlag=true;
    //Serial.printf("setting value %i, was: %i",val,_val);
    _val=val; 
    if(_autoRedraw==true && __redrawFlag==true) redraw();
}

int integerGauge::getValue(){
    return _val;
}

void textGauge::setFormatString(String format){
    _format=format;
}

void textGauge::redraw(){
    _canvas = new GFXcanvas16(_w,_h);
    _canvas->setCursor(_cursor_x,_cursor_y);
    _canvas->fillScreen(_bg);
    _canvas->setTextColor(_fg);
    _canvas->setFont(_font);
	 if(_border!=0){
	 	for(uint8_t __j=0;__j<_border;__j++) {
	 		_canvas->drawRect(1+__j,__j,_w-2*__j-1,_h-2*__j,_bo);
	 	}
	 }    
    if(_format.length()==0){
	 	_canvas->printf("%s",_val);
	 }else{
	   char __format[_format.length()+1];
	   _format.toCharArray(__format,_format.length()+1);
	   _canvas->printf(__format,_val);
	 }
    pushBitmap(_x,_y,_canvas->getBuffer(),_w,_h);
    delete _canvas;
}

void integerGauge::redraw(){
    _canvas = new GFXcanvas16(_w,_h);
    _canvas->setCursor(_cursor_x,_cursor_y);
    _canvas->fillScreen(_bg);
    _canvas->setTextColor(_fg);
    _canvas->setFont(_font);
	Serial.printf("Gauges: Redrawing Integer Gauge, value %i\n", _val);
    if(_border!=0){
	 	for(uint8_t __j=0;__j<_border;__j++) {
	 		_canvas->drawRect(1+__j,__j,_w-2*__j-1,_h-2*__j,_bo);
	 	}
	 }  
    if(_format.length()==0){
	    _canvas->printf("%i",_val);
    }else{
	     char __format[_format.length()+1];
	     _format.toCharArray(__format,_format.length()+1);
	     _canvas->printf(__format,_val);
	 }
    pushBitmap(_x,_y,_canvas->getBuffer(),_w,_h);
    delete _canvas;
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

void clockGauge::redraw(){
    _canvas = new GFXcanvas16(_w,_h);
    _canvas->setCursor(_cursor_x,_cursor_y);
    _canvas->fillScreen(_bg);
    _canvas->setTextColor(_fg);
    _canvas->setFont(_font);
	if(_border!=0){
	 	for(uint8_t __j=0;__j<_border;__j++) {
	 		_canvas->drawRect(1+__j,__j,_w-2*__j-1,_h-2*__j,_bo);
	 	}
	}
	_canvas->printf("%2i:%02i", _hour, _min);
    pushBitmap(_x,_y,_canvas->getBuffer(),_w,_h);
	delete _canvas;
}