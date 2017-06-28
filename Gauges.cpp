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

void Gauge::setFont(const GFXfont *font){
    _font=font;
}

void Gauge::setCursor(uint16_t x, uint16_t y){
	_cursor_x=x;
	_cursor_y=y;
}

void Gauge::setDisplay(Adafruit_GFX *display){
    _display=display;
    }

textGauge::textGauge(){
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

void integerGauge::setValue(int val){
    bool __redrawFlag=false;
    if(val!=_val) __redrawFlag=true;
    _val=val; 
    if(_autoRedraw==true && __redrawFlag==true) redraw();
    }

int integerGauge::getValue(){
    return _val;
    }

void integerGauge::setFormatString(String format){
   _format=format;
}

void integerGauge::redraw(){
    _canvas = new GFXcanvas16(_w,_h);
    //Serial.printf("created Canvas at 0x%x\n", _canvas);
    //Serial.printf("Canvas has buffer at 0x%x\n", _canvas->getBuffer());
    _canvas->setCursor(_cursor_x,_cursor_y);
    //Serial.printf("setCursor done\n");
    //Serial.printf("using display at 0x%x\n", _display);
    _canvas->fillScreen(_bg);
    _canvas->setTextColor(_fg);
    _canvas->setFont(_font);
    //Serial.printf("painted myCanvas red\n");
    if(_format.length()==0){
	    _canvas->printf("%i",_val);
	}else{
	     char __format[_format.length()+1];
	     _format.toCharArray(__format,_format.length()+1);
	     //Serial.printf("using FormatString %s, ",__format);
	     //Serial.printf(__format,_val);
	     //Serial.println();
	     _canvas->printf(__format,_val);
	}
    //Serial.printf("printed %i\n",_val);
    //Serial.printf("coords: (x:%i,y:%i)(w:%i,h:%i)",_x,_y,_w,_h);
    pushBitmap(_x,_y,_canvas->getBuffer(),_w,_h);
    //_canvas->drawRGBBitmap(_x,_y,_canvas->getBuffer(),_w,_h);
    //Serial.printf("finished drawing on screen\n");
    delete _canvas;
}

