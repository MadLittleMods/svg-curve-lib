#include "SVGCurveLib.h"


#if defined(ARDUINO)
#include "Arduino.h"
#else
const float PI = 3.14159265359f;
#endif


// min max
#include <algorithm>

// pow and sqrt
// trig functions
#include <math.h>

// fabs
#include <cmath>

#include <array>

#include <vector>


SVGCurveLib::PointGeneric<> SVGCurveLib::PointOnLine(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, float t) {
	auto calculateLinearLineParameter = [](float x0, float x1, float t) {
		float result = x0 + (x1-x0)*t;
		
		return result;
	};
	
	return SVGCurveLib::Point(
		calculateLinearLineParameter(p0.x, p1.x, t),
		calculateLinearLineParameter(p0.y, p1.y, t)
	);
}

SVGCurveLib::PointGeneric<> SVGCurveLib::PointOnQuadraticBezierCurve(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, float t) {

	auto calculateQuadraticBezierParameter = [](float x0, float x1, float x2, float t) {
		float result = pow(1-t, 2)*x0 + 2*t*(1-t)*x1 + pow(t, 2)*x2;
		
		return result;
	};
	
	return SVGCurveLib::Point(
		calculateQuadraticBezierParameter(p0.x, p1.x, p2.x, t),
		calculateQuadraticBezierParameter(p0.y, p1.y, p2.y, t)
	);
}


SVGCurveLib::PointGeneric<> SVGCurveLib::PointOnCubicBezierCurve(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, SVGCurveLib::PointGeneric<> p3, float t) {
	auto calculateCubicBezierParameter = [](float x0, float x1, float x2, float x3, float t) {
		float result = pow(1-t, 3)*x0 + 3*t*pow(1-t, 2)*x1 + 3*(1-t)*pow(t, 2)*x2 + pow(t, 3)*x3;
		
		return result;
	};
	
	return SVGCurveLib::Point(
		calculateCubicBezierParameter(p0.x, p1.x, p2.x, p3.x, t),
		calculateCubicBezierParameter(p0.y, p1.y, p2.y, p3.y, t)
	);
}




SVGCurveLib::PointWithEllipticalArcInfo SVGCurveLib::PointOnEllipticalArc(SVGCurveLib::PointGeneric<> p0, double rx, double ry, double xAxisRotation, bool largeArcFlag, bool sweepFlag, SVGCurveLib::PointGeneric<> p1, float t) {

	rx = fabs(rx);
	ry = fabs(ry);
	xAxisRotation = fmod(xAxisRotation, 360.0f);
	float xAxisRotationRadians = SVGCurveLib::ToRadians(xAxisRotation);
	// If the endpoints are identical, then this is equivalent to omitting the elliptical arc segment entirely.
	if(p0 == p1) {
		return p0;
	}
	
	// If rx = 0 or ry = 0 then this arc is treated as a straight line segment joining the endpoints.    
	if(rx == 0 || ry == 0) {
		return SVGCurveLib::PointOnLine(p0, p1, t);
	}
	
	// Following "Conversion from endpoint to center parameterization"
	// http://www.w3.org/TR/SVG/implnote.html#ArcConversionEndpointToCenter
	
	// Step #1: Compute transformedPoint
	double dx = (p0.x-p1.x)/2;
	double dy = (p0.y-p1.y)/2;
	auto transformedPoint = SVGCurveLib::Point(
		cos(xAxisRotationRadians)*dx + sin(xAxisRotationRadians)*dy,
		-sin(xAxisRotationRadians)*dx + cos(xAxisRotationRadians)*dy
	);
	
	// Ensure radii are large enough
	double radiiCheck = pow(transformedPoint.x, 2)/pow(rx, 2) + pow(transformedPoint.y, 2)/pow(ry, 2);
	if(radiiCheck > 1) {
		rx = sqrt(radiiCheck)*rx;
		ry = sqrt(radiiCheck)*ry;
	}
	
	// Step #2: Compute transformedCenter
	double cSquareNumerator = pow(rx, 2)*pow(ry, 2) - pow(rx, 2)*pow(transformedPoint.y, 2) - pow(ry, 2)*pow(transformedPoint.x, 2);
	double cSquareRootDenom = pow(rx, 2)*pow(transformedPoint.y, 2) + pow(ry, 2)*pow(transformedPoint.x, 2);
	double cRadicand = cSquareNumerator/cSquareRootDenom;
	// Make sure this never drops below zero because of precision
	cRadicand = cRadicand < 0 ? 0 : cRadicand;
	double cCoef = (largeArcFlag != sweepFlag ? 1 : -1) * sqrt(cRadicand);
	auto transformedCenter = SVGCurveLib::Point(
		cCoef*((rx*transformedPoint.y)/ry),
		cCoef*(-(ry*transformedPoint.x)/rx)
	);
	
	// Step #3: Compute center
	auto center = SVGCurveLib::Point(
		cos(xAxisRotationRadians)*transformedCenter.x - sin(xAxisRotationRadians)*transformedCenter.y + ((p0.x+p1.x)/2),
		sin(xAxisRotationRadians)*transformedCenter.x + cos(xAxisRotationRadians)*transformedCenter.y + ((p0.y+p1.y)/2)
	);
	
	// Step #4: Compute start/sweep angles
	// Start angle of the elliptical arc prior to the stretch and rotate operations.
	// Difference between the start and end angles
	auto startVector = SVGCurveLib::Point(
		(transformedPoint.x-transformedCenter.x)/rx,
		(transformedPoint.y-transformedCenter.y)/ry
	);
	float startAngle = SVGCurveLib::AngleBetween(SVGCurveLib::Point(1.0f, 0.0f), startVector);
	
	auto endVector = SVGCurveLib::Point(
		(-transformedPoint.x-transformedCenter.x)/rx,
		(-transformedPoint.y-transformedCenter.y)/ry
	);
	float sweepAngle = SVGCurveLib::AngleBetween(startVector, endVector);
	
	if(!sweepFlag && sweepAngle > 0) {
		sweepAngle -= 2*PI;
	}
	else if(sweepFlag && sweepAngle < 0) {
		sweepAngle += 2*PI;
	}
	// We use % instead of `mod(..)` because we want it to be -360deg to 360deg(but actually in radians)
	sweepAngle = fmod(sweepAngle, 2*PI);
	
	// From http://www.w3.org/TR/SVG/implnote.html#ArcParameterizationAlternatives
	float angle = startAngle+(sweepAngle*t);
	double ellipseComponentX = rx*cos(angle);
	double ellipseComponentY = ry*sin(angle);
	
	auto point = SVGCurveLib::PointWithEllipticalArcInfo(
		cos(xAxisRotationRadians)*ellipseComponentX - sin(xAxisRotationRadians)*ellipseComponentY + center.x,
		sin(xAxisRotationRadians)*ellipseComponentX + cos(xAxisRotationRadians)*ellipseComponentY + center.y
	);
	

	// Attach some extra info to use
	point.ellipticalArcStartAngle = startAngle;
	point.ellipticalArcEndAngle = startAngle+sweepAngle;
	point.ellipticalArcAngle = angle;

	//point.ellipticalArcCenter = SVGCurveLib::Point<float>(center);
	point.ellipticalArcCenter = SVGCurveLib::Point(float(center.x), float(center.y));
	point.resultantRx = rx;
	point.resultantRy = ry;

	return point;
}


SVGCurveLib::PointGeneric<> SVGCurveLib::CalculateQuadraticBezierTOfCriticalPoint(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2) {
	auto calculateQuadraticBezierCriticalPointParameter = [](float x0, float x1, float x2) {
		float result = (x0 - x1) / (x0 - 2*x1 + x2);
		
		return result;
	};

	return {
		calculateQuadraticBezierCriticalPointParameter(p0.x, p1.x, p2.x),
		calculateQuadraticBezierCriticalPointParameter(p0.y, p1.y, p2.y)
	};
};

std::array<SVGCurveLib::PointGeneric<>, 3> SVGCurveLib::CalculateCubicBezierTOfCriticalPoint(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, SVGCurveLib::PointGeneric<> p3) {
	auto calculateCubicBezierCriticalPointParameter1 = [](float x0, float x1, float x2, float x3, float signOfRootPart) {
		signOfRootPart = signOfRootPart > 0 ? 1.0f : -1.0f;

		float denom  = (x0 + 3*x2 - x3 - 3*x1);
		if(denom != 0) {
			float result = (signOfRootPart*sqrt(-x0*x2 + x0*x3 + pow(x2, 2) - x2*x1 - x3*x1 + pow(x1, 2)) + x0 + x2 - 2*x1) / denom;
		
			return result;
		}

		// Return something invalid for t
		return -1.0f;
	};

	auto calculateCubicBezierCriticalPointParameter2 = [&](float x0, float x1, float x2, float x3) {
		return calculateCubicBezierCriticalPointParameter1(x0, x1, x2, x3, -1);
	};

	auto calculateCubicBezierCriticalPointParameter3 = [](float x0, float x1, float x2, float x3) {
		bool constraint1 = x0 == -3*x2 + x3 + 3*x1;
		bool constraint2 = 0 != 2*x2 - x3 - x1;

		if(constraint1 && constraint2) {
			float result = (3*x2 - x3 - 2*x1) / 2*(2*x2 - x3 - x1);

			return result;
		}

		// Return something invalid for t
		return -1.0f;
	};


	std::array<SVGCurveLib::PointGeneric<>, 3> points = {{
		{
			calculateCubicBezierCriticalPointParameter1(p0.x, p1.x, p2.x, p3.x, 1),
			calculateCubicBezierCriticalPointParameter1(p0.y, p1.y, p2.y, p3.y, 1)
		},
		{
			calculateCubicBezierCriticalPointParameter2(p0.x, p1.x, p2.x, p3.x),
			calculateCubicBezierCriticalPointParameter2(p0.y, p1.y, p2.y, p3.y)
		},
		{
			calculateCubicBezierCriticalPointParameter3(p0.x, p1.x, p2.x, p3.x),
			calculateCubicBezierCriticalPointParameter3(p0.y, p1.y, p2.y, p3.y)
		}
	}};

	return points;
};





// Resolution is the number of segments we use to interpolate from
SVGCurveLib::ArcLengthInfo SVGCurveLib::ApproximateArcLengthOfCurve(unsigned int resolution, SVGCurveLib::PointGeneric<> (*pointOnCurveFunc)(float)) {

	unsigned int mapSize = resolution+1;
	SVGCurveLib::ArcLengthInfo resultInfo(mapSize);


	float resultantArcLength = 0;

	SVGCurveLib::PointGeneric<> prevPoint = pointOnCurveFunc(0);
	SVGCurveLib::PointGeneric<> nextPoint;
	int mapIndex = 0;
	for(unsigned int i = 0; i < resolution; i++) {
		float t = SVGCurveLib::Clamp(i*(1.0f/resolution), 0.0f, 1.0f);

		nextPoint = pointOnCurveFunc(t);

		resultantArcLength += SVGCurveLib::Distance(prevPoint, nextPoint);

		SVGCurveLib::ArcLengthMapEntry entry(t, resultantArcLength);
		//resultInfo.arcLengthMap[mapIndex] = entry;
		resultInfo.arcLengthMap.emplace_back(entry);
		mapIndex++;

		prevPoint = nextPoint;
	}
	// Last stretch to the endpoint
	nextPoint = pointOnCurveFunc(1);
	resultantArcLength += SVGCurveLib::Distance(prevPoint, nextPoint);
	SVGCurveLib::ArcLengthMapEntry entry(1, resultantArcLength);
	//resultInfo.arcLengthMap[mapIndex] = entry;
	resultInfo.arcLengthMap.emplace_back(entry);
	mapIndex++;


	resultInfo.arcLength = resultantArcLength;


	return resultInfo;
}






SVGCurveLib::LinearCurve::LinearCurve(unsigned int resolution, SVGCurveLib::PointGeneric<> (*pointOnCurveFunc)(float)) {
	// Resolution is the number of segments we use to approximate
	resolution = resolution ? resolution : 500;

	this->pointOnCurveFunc = pointOnCurveFunc;

	this->resultantInfo = SVGCurveLib::ApproximateArcLengthOfCurve(resolution, pointOnCurveFunc);
};
// Transforms `u`[0-1] into a corresponding point along the curve. `u * arcLength`
SVGCurveLib::PointGeneric<> SVGCurveLib::LinearCurve::PointOnLinearCurve(float u) {
	u = SVGCurveLib::Clamp(u, 0.0f, 1.0f);
	float targetDistanceFromStartingPoint = u * this->resultantInfo.arcLength;

	float resultantT = 0;

	float prevArcLength = 0;
	float prevT = 0;
	for(unsigned int i = 0; i < this->resultantInfo.arcLengthMap.size(); i++) {
		SVGCurveLib::ArcLengthMapEntry entry = this->resultantInfo.arcLengthMap[i];

		float t = entry.t;
		float arcLength = entry.arcLength;

		// Once we go a past our target
		// Lets interpolate from a previous to current
		if(arcLength >= targetDistanceFromStartingPoint) {
			float endDiff = arcLength - targetDistanceFromStartingPoint;
			float startDiff = targetDistanceFromStartingPoint - prevArcLength;
			float linearFactor = 0;
			// Avoid a divide by zero
			if((endDiff+startDiff) != 0) {
				linearFactor = (startDiff/(endDiff+startDiff));
			}

			resultantT = prevT + (t-prevT)*linearFactor;

			break;
		}


		prevArcLength = arcLength;
		prevT = t;

	};

	return pointOnCurveFunc(resultantT);
};








// We expect you to pass in some form of number
template <typename T/* = float*/>
T SVGCurveLib::Clamp(T val, T minVal, T maxVal) {
	T tempMin = (std::min)(val, maxVal);
	return (std::max)(minVal, tempMin);
};

// We expect you to pass in some form of number
template <typename T/* = float*/>
T SVGCurveLib::ToRadians(T angle) {
	return angle * (PI / 180);
};

// We expect you to pass in some form of `SVGCurveLib::PointGeneric<>`
template <typename Tx/* = SVGCurveLib::PointGeneric<>*/, typename Ty/* = Tx*/>
float SVGCurveLib::Distance(Tx p0, Ty p1) {
	return sqrt(pow(p1.x-p0.x, 2) + pow(p1.y-p0.y, 2));
};

// We expect you to pass in some form of `SVGCurveLib::PointGeneric<>`
template <typename Tx/* = SVGCurveLib::PointGeneric<>*/, typename Ty/* = Tx*/>
float SVGCurveLib::AngleBetween(Tx v0, Ty v1) {

	float p = v0.x*v1.x + v0.y*v1.y;
	float n = sqrt((pow(v0.x, 2)+pow(v0.y, 2)) * (pow(v1.x, 2)+pow(v1.y, 2)));
	float sign = v0.x*v1.y - v0.y*v1.x < 0 ? -1.0f : 1.0f;
	float angle = sign*acos(p/n);
	
	//var angle = Math.atan2(v0.y, v0.x) - Math.atan2(v1.y,  v1.x);
	
	return angle;
};

#if defined(ARDUINO)
String SVGCurveLib::FloatToString(float val, byte precision) {
	// returns val with number of decimal places determine by precision
	// precision is a number from 0 to 6 indicating the desired decimial places
	// example: floatToString(3.1415, 2); // returns 3.14 (two decimal places)

	String output = "";

	if(val < 0.0){
		output += "-";
		val = -val;
	}

	output += int(val);  //prints the int part
	if(precision > 0) 
	{
		output += "."; // print the decimal point

		unsigned long frac;
		unsigned long mult = 1;
		byte padding = precision -1;
		while(precision--)
			mult *=10;

		if(val >= 0)
			frac = (val - int(val)) * mult;
		else
			frac = (int(val)- val ) * mult;
		unsigned long frac1 = frac;

		while( frac1 /= 10 )
			padding--;
		while(  padding--)
			output += "0";

		output += frac;
	}

	return output;
};
#endif