# Arduino_Gauges
A set of gauges to work with Adafruit_GFX

![screenshot](https://github.com/pljakobs/Arduino_Gauges/blob/master/Gauges.jpeg?raw=true)

The current implementation can show text, numbers and tape gauges on defined parts of an Adafruit_gfx screen without having to redraw the whole screen. It can even drive multiple screens (provided the underlying hard- and software supports it) and move Gauges from one screen to another.
The base **Gauge** class that all others inherit from, just holds the functions to update the value to be displayed. The value is internally stored as String, but the **setValue()** function is overloaded for *short*, *int*, *long*, *float*, *duble*, *char\**, *const char\** and *String* values thus almost any value can be thrown at it. 
If a value is not a number, it's up to the visualization class to handle the error 

In addition to **setValue** the only other public functions are **setAutoRedraw()** and **redraw()**

* **setAutoRedraw(bool val)** sets or clears the *autoRedraw* flag. If the flag is set, a change to value stored by this Gauge will automatically trigger a redraw() for this gauge. This is probably what you want in most cases.
* **redraw()** manually redraws a gauge. Sometimes, you might want to manually force a redraw or to redraw Gauges on a given fixed interval. This can be done by calling redraw().

For all Gauges that can be shown on an Adafruit_Gfx dislay, the intermediary class **displayGauge** holds the relevant properties and functions. Note that you won't normally instantiate this class but the more specific classes further down.
It provides the following member functions:
* **setPosition(uint16_t x, uint16_t y)**
place the Gauge at a certain screen position. This position is relative to the screen's own orientation
* **setSize(uint16_t w, uint16_t h)**
define the width and heigth of the Gauge. This can be changed at runtime, but be aware that currently, shrinking will not automatically delete the area left open by it
* **setDepth(uint8_t z)** *currently unused* There might be stacking in the future, not sure
* **setVisible(bool val)** *currently unused* would render the Gauge hidden when set to false (just not redraw at all)
* **setDisplay(Adafruit_GFX \*display)** set the Display for this Gauge to render on
* **setFGColor(uint16_t fg), setBGColor(uint16_t bg), setBorderColor(uint16_t bo)** set the drawing colors for the respective elements in Adafruit_gfx 16 bit 565 format 
* **setBorder(uint8_t border)** set the width of a border to draw around the Gauge (in fact, it will live inside the rect defined by h and w, the useable gauge area will be reduced by twice the border width) A border of 0 disables drawing the border.
* **setGutter(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b), setGutter(uint8_t gutter),setMargins(uint8_t gutter_l,uint8_t gutter_r, uint8_t gutter_t, uint8_t gutter_b), setMargins(uint8_t gutter)** (setMargins and setGutter are synonymous) set additional free space *inside* the border. This will be painted in the background color. When using just one argument, the same value is used for all vour edges.
* **int getDepth()** *currently unused* get the z-depth of this Gauge

The **textGauge** adds what is needed to render textual information
* **setFont(const GFXfont \*font)** set the font for the Gauge
* **setVAlign(uint8_t align)** set vertical alignment of data in the box, align is one of **TEXT_TOP, TEXT_MIDDLE, TEXT_BOTTOM**
* **setHAlign(uint8_t align)** set horizontal alignment of data in the box, align is one of **TEXT_LEFT, TEXT_CENTER, TEXT_RIGHT**

The **tapeGauge** allows for vertical or horizontal Tapes to be displayed. Those tapes can have up to three color areas defined by two limits
* **setMinMax(float min, float max)** set the minimum and maximum allowable value for this tape - required to correctly scale
* **setColors(uint16_t color0,float limit0, uint16_t color1, float limit1, uint16_t color2), setColors(uint16_t color0,float limit0, uint16_t), setColors(uint16_t color0)** set the coloring scheme. Five parameters define thre colored segments, three parameters define two segments and a single color means the whole bar has just that color.
* **setDirection(uint8_t dir)** sets the direction (or orientation) of the tape. dir is one of **TAPE_LEFTRIGHT, TAPE_TOPDOWN, TAPE_RIGHTLEFT, TAPE_BOTTOMUP** 
* **setTicks(uint16_t major, uint16_t minor)** (not yet implemented) draw minor and major ticks along the cardinal axis

**Beware:**
due to memory requirements, this will currently only work on platforms that have enough free memory available. This library uses Adafruit_Gfx::canvas16 to avoid on screen flicker when refreshing parts of the display. A canvas needs w\*h\*2 bytes of memory. For a 40x200px canvas, that's almost 16kB! That buffer is only held for a very short time (during the redraw() function call) but if it cannot be fully allocated, the display will probably show gabled information.

**dependecies**
I have tested this library with the following Adafruit display classes:
* Adafruit_ILI9340.h
* Adafruit_ILI9341.h
* Adafruit_SSD1306.h
* Adafruit_ST7735.h

The OLED displays require an additional call to actually update the display (display.display()). Since the Gauges use a reference to the Adafruit_Gfx baseclass and that does not know this function, I had to update this as well. So, to use the gauges, download Adafruit_Gfx from here: https://github.com/pljakobs/Adafruit-GFX-Library
