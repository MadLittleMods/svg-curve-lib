# JavaScript API

We use the Universal Module Definition (UMD) so you can use it in your CommonJS, AMD, or global.

## Usage

```
// CommonJS
var SVGCurveLib = require('../src/js/svg-curve-lib.js');

// AMD
define(['svg-curve-lib', function(SVGCurveLib) {
	// Use SVGCurveLib./*some method here*/
});

// Browser/Window global
SVGCurveLib./*some method here*/
```


### `pointOnLine`

`SVGCurveLib.pointOnLine(p0, p1, t)`

 - Returns: object with format `{x: 0, y: 0}`
 - Parameters:
 	 - `p0`: object with format `{x: 0, y: 0}` - starting point
 	 - `p1`: object with format `{x: 0, y: 0}` - end point
 	 - `t`: number [0-1]

### `pointOnQuadraticBezierCurve`

`SVGCurveLib.pointOnQuadraticBezierCurve(p0, p1, p2, t)`

 - Returns: object with format `{x: 0, y: 0}`
 - Parameters:
 	 - `p0`: object with format `{x: 0, y: 0}` - starting point
 	 - `p1`: object with format `{x: 0, y: 0}` - control point
 	 - `p2`: object with format `{x: 0, y: 0}` - end point
 	 - `t`: number [0-1]

### `pointOnCubicBezierCurve`

`SVGCurveLib.pointOnCubicBezierCurve(p0, p1, p2, p3, t)`

 - Returns: object with format `{x: 0, y: 0}`
 - Parameters:
 	 - `p0`: object with format `{x: 0, y: 0}` - starting point
 	 - `p1`: object with format `{x: 0, y: 0}` - control point
 	 - `p2`: object with format `{x: 0, y: 0}` - control point
 	 - `p3`: object with format `{x: 0, y: 0}` - end point
 	 - `t`: number [0-1]

### `.pointOnEllipticalArc`

`SVGCurveLib.pointOnEllipticalArc(p0, rx, ry, xAxisRotation, largeArcFlag, sweepFlag, p1, t)`

 - Returns: object with format `{x: 0, y: 0}` with some added props
 	 - Additional Object Props.:
 	 	 - `ellipticalArcStartAngle`: number - Curve start angle from x-axis from a full ellipse (in radians)
 	 	 - point.ellipticalArcEndAngle: number - Curve end angle from x-axis from a full ellipse (in radians)
 	 	 - point.ellipticalArcAngle: number - Angle from x-axis from a full ellipse where the point you are sampling for is at (in radians)
 	 	 - point.ellipticalArcCenter: object with format `{x: 0, y: 0}` - Where the center of the full ellipse would be
 	 	 - point.resultantRx: number - Radii of the x-axis
 	 	 - point.resultantRy: number - Radii of the y-axis
 - Parameters:
 	 - `p0`: object with format `{x: 0, y: 0}` - starting point
 	 - `rx`: number - radius of the x-axis
 	 - `ry`: number - radius of the y-axis
 	 - `xAxisRotation`: number - Angle from the x-axis to rotate the ellipse (in degrees)
 	 - `largeArcFlag`: bool - `false` if an arc spanning less than or equal to 180 degrees is chosen, or `true` if an arc spanning greater than 180 degrees is chosen. This option is a bit hard to understand without an example.
 	 - `sweepFlag`: bool - `false` if the line joining center to arc sweeps through decreasing angles, or `true` if it sweeps through increasing angles. This option is a bit hard to understand without an example.
 	 - `p1`: object with format `{x: 0, y: 0}` - end point
 	 - `t`: number [0-1]


### `calculateQuadraticBezierTOfCriticalPoint`

`SVGCurveLib.calculateQuadraticBezierTOfCriticalPoint(p0, p1, p2)`

Returns the t-value for each axis where the curve can change from negative to positive slope, or vice versa

 - Returns: object with format `{x: t-value, y: t-value}`
 - Parameters:
 	 - `p0`: object with format `{x: 0, y: 0}` - starting point
 	 - `p1`: object with format `{x: 0, y: 0}` - control point
 	 - `p2`: object with format `{x: 0, y: 0}` - end point


### `calculateCubicBezierTOfCriticalPoint`

`SVGCurveLib.calculateCubicBezierTOfCriticalPoint(p0, p1, p2, p3)`

Returns the t-value for each axis where the curve can change from negative to positive slope, or vice versa

 - Returns: an array of size 3 object with format `{x: t-value, y: t-value}`. `t-value` can be false if not applicable.
 - Parameters:
 	 - `p0`: object with format `{x: 0, y: 0}` - starting point
 	 - `p1`: object with format `{x: 0, y: 0}` - control point
 	 - `p2`: object with format `{x: 0, y: 0}` - control point
 	 - `p3`: object with format `{x: 0, y: 0}` - end point


### `approximateArcLengthOfCurve`

`SVGCurveLib.approximateArcLengthOfCurve(resolution, pointOnCurveFunc)`

 - Returns: object
 	 - Object Props.:
 	 	 - `arcLength`: number - length of the curve/arc
 	 	 - `arcLengthMap`: array of objects with shape `{t: 0, arcLength: 0}`
 	 	 - `approximationLines`: array of arrays - List of polylines(always size 2 in this case but code wisely if using for debugging (start and end point))


### `generateLinearCurve`

`SVGCurveLib.generateLinearCurve(resolution, pointOnCurveFunc)`

Returns a function that will linearly interpolate along the curve u: [0, 1]

 - Returns: function as well as an attached `arcLength` property
 - Parameters:
 	 - `resolution`: number - Number of samples to use to interpolate from
 	 - `pointOnCurveFunc`: function that takes `t`[0, 1] as a parameter and returns a object with format `{x: 0, y: 0}`
 	 	 - Curry `pointOnQuadraticBezierCurve`, `pointOnCubicBezierCurve`, `pointOnEllipticalArc` to fit the description above (see the Usage section)


