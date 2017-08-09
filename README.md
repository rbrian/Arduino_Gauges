# Arduino_Gauges
A set of gauges to work with Adafruit_GFX

The current implementation can show text and numbers on defined parts of an Adafruit_gfx screen without having to redraw the whole screen. It can even drive multiple screens (provided the underlying hard- and software supports it) and move Gauges from one screen to another.
The base **Gauge** class, that all others inherit from, contains the relevant functions to put it on screen and configure it's basic behaviour:
* setPosition(uint16_t x, uint16_t y)
place the Gauge at a certain screen position. This position is relative to the screen's own orientation
* setSize(uint16_t w, uint16_t h)
define the width and heigth of the Gauge. This can be changed at runtime, but be aware that currently, shrinking will not automatically delete the area left open by it
* setDepth(uint8_t z) *currently unused* There might be stacking in the future, not sure
* setAutoRedraw(bool val) defining the behaviour on value change. If set to true, the Gauge will automatically refresh on screen when the value is chaned
* setVisible(bool val) *currently unused* would render the Gauge hidden when set to false (just not redraw at all)
* setDisplay(Adafruit_GFX \*display) set the Display for this Gauge to render on
* setFGColor(uint16_t fg), setBGColor(uint16_t bg), setBorderColor(uint16_t bo) set the drawing colors for the respective elements in Adafruit_gfx 16 bit 565 format 
* setBorder(uint8_t border) set the width of a border to draw around the Gauge (in fact, it will live inside the rect defined by h and w, the useable gauge area will be reduced by twice the border width) A border of 0 disables drawing the border.
* redraw() *currently unfunctional* pushes a manual redraw of the Gauge
* int getDepth() *currently unused* get the z-depth of this Gauge

The **textGauge** adds what is needed to render textual information
* setFont(const GFXfont \*font) set the font for the Gauge
* setCursor(uint16_t x, uint16_t y) set the cursor to a position *inside* the Gauge (top left corner is 0,0)
* setFormatString(String formatString) set a **printf** format string to format the output inside the Gauge

A sketch should never instanciate one of those base classes and they are just detailed here to document the full interface of the derived classes that are intended for use:

The **integerGauge** class (not perfectly well named, and here's one of my problems: I would love to be a little more free about combining data types and visualzations)
This class displays an integer value as a text Gauge.
The only additional interface is *setValue* and *getValue* that will ... simply set and get the value for this Gauge.

The **stringGauge** class is similar to the integerGauge, but accepts string values for *setValue* and *getValue* 

I believe this might be a case to use class templates such that tthe "textGauge" could actually accept all kinds of parameter input, store it internally and display it, which would help defining a better model for other types of display (graphical rendering or actual mechanical output). 

If you have any suggestions, please contact me or, even better, make a PR :-)

