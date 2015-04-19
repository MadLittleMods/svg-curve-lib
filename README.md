# SVG Curve Library

Bezier Curve and Elliptical Arc implementations in multiple languages. SVG path commands.

The API is equivalent and we strive for the same syntax across the language implementations.

## Versions

 - `JavaScript`: v0.2.0
 - `C++`: v0.2.0
 	 - Arduino/Teensyduino compatible

# API

 - [Javascript (JS)](/docs/api-js.md)
 - [C++](docs/api-cpp.md)

# Demos

 - [Interactive Bezier Curve Demo (Quadratic, Cubic)](http://madlittlemods.github.io/svg-curve-lib/demos/js/interactive-bezier-curve-demo/)
 - [Interactive Elliptical Arc Demo](http://madlittlemods.github.io/svg-curve-lib/demos/js/interactive-elliptical-arc-demo/)
 - [Barebones Draw Curve Demo](http://madlittlemods.github.io/svg-curve-lib/demos/js/barebones-bezier-curve-demo/)


# Testing

## JavaScript

Run once: Install the dependencies

`npm install`

Run whenever: Whenever you want to run the tests

`npm run test`


## C++

The C++ version of SVGCurveLib requires C++11.

We test on the following platforms:

 - Visual Studio (Community 2013) on Windows
 - Arduino (1.6.3)/Teensyduino (1.22)
 	 - Teensy 3.0/3.1

### Visual Studio

Open up `test/c++/vs_testbench/vs_testbench.sln` and build the project. You can press `Ctrl+F5` to keep the console window open to see the results better.

### Arduino

Run whenever: Whenever you make a change. This will move the library files into the Arduino Sketch directory (required because Arduino forces files to be in the same directory).

`npm run testSetupArduino`

Then compile and upload the sketch to your Arduino or Arduino-like platform. Open a Serial Terminal to see the output streamed every 5 seconds. You can use [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html) as a Serial Terminal to take advantage of the ANSI colored text.

To disable the ANSI escape color codes, open up `test/c++/cpp-test-lib/SVGCurveLibTests.h`, and change `MacchiatoSettings.useAnsiColor` to `false`.