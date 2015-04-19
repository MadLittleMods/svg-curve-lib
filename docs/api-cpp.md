# C++ API

```
#include "SVGCurveLib.h"

SVGCurveLib::/*some method here*/
```



### `PointOnLine`

`SVGCurveLib::PointGeneric<> SVGCurveLib::PointOnLine(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, float t)`

 - Returns: `SVGCurveLib::PointGeneric<>`
 - Parameters:
 	 - `p0`: `SVGCurveLib::PointGeneric<>` - starting point
 	 - `p1`: `SVGCurveLib::PointGeneric<>` - end point
 	 - `t`: float [0-1]

### `PointOnQuadraticBezierCurve`

`SVGCurveLib::PointGeneric<> SVGCurveLib::PointOnQuadraticBezierCurve(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, float t)`

 - Returns: SVGCurveLib::Point
 - Parameters:
 	 - `p0`: `SVGCurveLib::PointGeneric<>` - starting point
 	 - `p1`: `SVGCurveLib::PointGeneric<>` - control point
 	 - `p2`: `SVGCurveLib::PointGeneric<>` - end point
 	 - `t`: float [0-1]

### `PointOnCubicBezierCurve`

`SVGCurveLib::PointGeneric<> SVGCurveLib::PointOnCubicBezierCurve(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, SVGCurveLib::PointGeneric<> p3, float t)`

 - Returns: SVGCurveLib::Point
 - Parameters:
 	 - `p0`: `SVGCurveLib::PointGeneric<>` - starting point
 	 - `p1`: `SVGCurveLib::PointGeneric<>` - control point
 	 - `p2`: `SVGCurveLib::PointGeneric<>` - control point
 	 - `p3`: `SVGCurveLib::PointGeneric<>` - end point
 	 - `t`: float [0-1]

### `PointOnEllipticalArc`

`SVGCurveLib::PointWithEllipticalArcInfo SVGCurveLib::PointOnEllipticalArc(SVGCurveLib::PointGeneric<> p0, double rx, double ry, double xAxisRotation, bool largeArcFlag, bool sweepFlag, SVGCurveLib::PointGeneric<> p1, float t)`

 - Returns: `SVGCurveLib::PointWithEllipticalArcInfo` but can be implicitly converted/cast to `SVGCurveLib::PointGeneric<>`
 	 - `SVGCurveLib::PointWithEllipticalArcInfo` members:
 	 	 - `value`: `SVGCurveLib::PointGeneric<>` - The sampled point on curve. This is what will be returned if this is implicitly converted to `SVGCurveLib::PointGeneric<>`
 	 	 - `ellipticalArcStartAngle`: float - Curve start angle from x-axis from a full ellipse (in radians)
 	 	 - point.ellipticalArcEndAngle: float - Curve end angle from x-axis from a full ellipse (in radians)
 	 	 - point.ellipticalArcAngle: float - Angle from x-axis from a full ellipse where the point you are sampling for is at (in radians)
 	 	 - point.ellipticalArcCenter: `SVGCurveLib::PointGeneric<>` - Where the center of the full ellipse would be
 	 	 - point.resultantRx: float - Radii of the x-axis
 	 	 - point.resultantRy: float - Radii of the y-axis
 - Parameters:
 	 - `p0`: `SVGCurveLib::PointGeneric<>` - starting point
 	 - `rx`: double - radius of the x-axis
 	 - `ry`: double - radius of the y-axis
 	 - `xAxisRotation`: double - Angle from the x-axis to rotate the ellipse (in degrees)
 	 - `largeArcFlag`: bool - `false` if an arc spanning less than or equal to 180 degrees is chosen, or `true` if an arc spanning greater than 180 degrees is chosen. This option is a bit hard to understand without an example.
 	 - `sweepFlag`: bool - `false` if the line joining center to arc sweeps through decreasing angles, or `true` if it sweeps through increasing angles. This option is a bit hard to understand without an example.
 	 - `p1`: SVGCurveLib::PointGeneric<> - end point
 	 - `t`: float [0-1]


### `CalculateQuadraticBezierTOfCriticalPoint`

`SVGCurveLib::PointGeneric<> SVGCurveLib::CalculateQuadraticBezierTOfCriticalPoint(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2)`

Returns the t-value for each axis where the curve can change from negative to positive slope, or vice versa

 - Returns: object with format `{x: t-value, y: t-value}`
 - Parameters:
 	 - `p0`: `SVGCurveLib::PointGeneric<>` - starting point
 	 - `p1`: `SVGCurveLib::PointGeneric<>` - control point
 	 - `p2`: `SVGCurveLib::PointGeneric<>` - end point


### `CalculateCubicBezierTOfCriticalPoint`

`std::array<SVGCurveLib::Point, 3> SVGCurveLib::CalculateCubicBezierTOfCriticalPoint(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, SVGCurveLib::PointGeneric<> p3)`

Returns the t-value for each axis where the curve can change from negative to positive slope, or vice versa

 - Returns: an array of size 3 `SVGCurveLib::PointGeneric<>`'s' with format `{ x: t-value, y: t-value }`. `t-value` can be false if not applicable.
 - Parameters:
 	 - `p0`: `SVGCurveLib::PointGeneric<>` - starting point
 	 - `p1`: `SVGCurveLib::PointGeneric<>` - control point
 	 - `p2`: `SVGCurveLib::PointGeneric<>` - control point
 	 - `p3`: `SVGCurveLib::PointGeneric<>` - end point



### `ApproximateArcLengthOfCurve`

`SVGCurveLib::ArcLengthInfo SVGCurveLib::ApproximateArcLengthOfCurve(unsigned int resolution, SVGCurveLib::PointGeneric<> (*pointOnCurveFunc)(float))`

```
auto/*SVGCurveLib::ArcLengthInfo*/ result = SVGCurveLib::ApproximateArcLengthOfCurve(resolution, [&](float t) {
	return SVGCurveLib::PointOnLine({ 0, 0 }, { 10, 10 }, t);
});

// result.arcLength
```

 - Returns: `SVGCurveLib::ArcLengthInfo`
 	 - Members:
 	 	 - `arcLength`: float - length of the curve/arc
 	 	 - `arcLengthMap`: `std::vector<SVGCurveLib::ArcLengthMapEntry>` - Map of t to arc length



### `LinearCurve`

`SVGCurveLib::LinearCurve(unsigned int resolution, SVGCurveLib::PointGeneric<> (*pointOnCurveFunc)(float))`

Once initialized, use `myLinearCurve.PointOnLinearCurve(float u)` to sample the linear curve

```
SVGCurveLib::LinearCurve myLinearCurve = SVGCurveLib::LinearCurve(50, [&](float t) {
	return SVGCurveLib::PointOnLine({ 0, 0 }, { 10, 10 }, t);
});

// Gets a point exactly halfway, distance-wise along the curve
SVGCurveLib::PointGeneric<> myLinearCurve.PointOnLinearCurve(0.5);
```

 - Returns: `SVGCurveLib::LinearCurve`
 	 - Members:
 	 	 - `SVGCurveLib::ArcLengthInfo resultantInfo`
 	 	 - `SVGCurveLib::PointGeneric<> PointOnLinearCurve(float u)`
 - Parameters:
 	 - `resolution`: float - Number of samples to use to interpolate from
 	 - `pointOnCurveFunc`: function that takes `t`[0, 1] as a parameter and returns a `SVGCurveLib::PointGeneric<>`
 	 	 - Curry `PointOnQuadraticBezierCurve`, `PointOnCubicBezierCurve`, or `PointOnEllipticalArc` to fit the description above.


