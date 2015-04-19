// SVG Curve Library
// https://github.com/MadLittleMods/svg-curve-lib
// v0.2.0

#ifndef SVGCurveLib_h
#define SVGCurveLib_h


#if defined(ARDUINO)
#include "Arduino.h"
#else
#include <string>
#endif

// min max
#include <algorithm>
#include <math.h>

// fabs
#include <cmath>

#include <array>

#include <vector>
#if defined(ARDUINO)
	// We just add a define in case anyone wants to detect it later to avoid multiple declaration errors
	#ifndef std_vector_arduino_throw_polyfill
	#define std_vector_arduino_throw_polyfill
		// via https://forum.pjrc.com/threads/23467-Using-std-vector?p=69787&viewfull=1#post69787
		namespace std {
			inline void __throw_bad_alloc() {
				Serial.println("Unable to allocate memory");
			}

			inline void __throw_length_error( char const*e ) {
				Serial.print("Length Error :");
				Serial.println(e);
			}
		}
	#endif
#endif


// std::decay_t is in C++14 by default
// via: http://en.cppreference.com/w/cpp/types/decay
template<class T>
using decay_t = typename std::decay<T>::type;




class SVGCurveLib
{
	public:

		template <typename Tx = float, typename Ty = Tx>
		struct PointGeneric {
			
			struct x_getter {
				public:
					x_getter(PointGeneric *t)
						: thisPointer{t} {};

					// Getter
					operator Tx() const {
						return this->thisPointer->_x;
					};

				private:
					PointGeneric<Tx, Ty>* thisPointer;
			};
			x_getter x{this};

			struct y_getter {
				public:
					y_getter(PointGeneric *t)
						: thisPointer{t} {};

					// Getter
					operator Ty() const {
						return this->thisPointer->_y;
					};

				private:
					PointGeneric<Tx, Ty>* thisPointer;
			};
			y_getter y{this};



			PointGeneric() 
				: PointGeneric { 0, 0 } {};

			PointGeneric(Tx x, Ty y)
				: x(this), y(this), _x{x}, _y{y} {};

			// Copy Constructor
			PointGeneric(const PointGeneric<Tx, Ty>& other)
				: x(this), y(this), _x(std::move(other._x)), _y(std::move(other._y)) {};


			bool equalsWithinTolerance(PointGeneric<Tx, Ty> other) {
				return equalsWithinTolerance(other, 0.0001f);
			};

			bool equalsWithinTolerance(PointGeneric<Tx, Ty> other, float tolerance) {
				return fabs(this->x - other.x) <= tolerance && fabs(this->y - other.y) <= tolerance;
			};

			// equality operators
			bool operator == (const PointGeneric<Tx, Ty> &v) { return (this->x == v.x) && (this->y == v.y); }
			bool operator != (const PointGeneric<Tx, Ty> &v) { return !this->operator==(v); }

			
			#if defined(ARDUINO)
				operator String() {
					return String("(" + SVGCurveLib::FloatToString((float)this->x, 4) + ", " +  SVGCurveLib::FloatToString((float)this->y, 4) + ")");
				}
			#else
				operator std::string() const {
					return std::string("(" + SVGCurveLib::RemoveTrailingZeros(std::to_string(this->x)) + ", " +  SVGCurveLib::RemoveTrailingZeros(std::to_string(this->y)) + ")");
				}

				friend std::ostream& operator << (std::ostream& stream, const PointGeneric<Tx, Ty> &p) {
					return stream << std::string(p);
				};
			#endif


			private:
				Tx _x;
				Ty _y;
		};

		template <typename Tx = float, typename Ty = Tx>
		static PointGeneric<decay_t<Tx>, decay_t<Ty>> Point(Tx&& x, Ty&& y) {
			return { std::forward<Tx>(x), std::forward<Ty>(y) };
		};



		struct PointWithEllipticalArcInfo {
			SVGCurveLib::PointGeneric<> point;

			float ellipticalArcStartAngle = 0;
			float ellipticalArcEndAngle = 0;
			float ellipticalArcAngle = 0;
			SVGCurveLib::PointGeneric<> ellipticalArcCenter;
			float resultantRx = 0;
			float resultantRy = 0;

			PointWithEllipticalArcInfo() {};
			PointWithEllipticalArcInfo(SVGCurveLib::PointGeneric<> p) : point{p} {};
			PointWithEllipticalArcInfo(float x, float y) {
				this->point = SVGCurveLib::Point(x, y);
			};

			// Implicit conversion
			operator SVGCurveLib::PointGeneric<>() { 
				return this->point;
			};
		};

		static SVGCurveLib::PointGeneric<> PointOnLine(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, float t);
		static SVGCurveLib::PointGeneric<> PointOnQuadraticBezierCurve(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, float t);
		static SVGCurveLib::PointGeneric<> PointOnCubicBezierCurve(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, SVGCurveLib::PointGeneric<> p3, float t);
		static SVGCurveLib::PointWithEllipticalArcInfo PointOnEllipticalArc(SVGCurveLib::PointGeneric<> p0, double rx, double ry, double xAxisRotation, bool largeArcFlag, bool sweepFlag, SVGCurveLib::PointGeneric<> p1, float t);

		static SVGCurveLib::PointGeneric<> CalculateQuadraticBezierTOfCriticalPoint(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2);
		static std::array<SVGCurveLib::PointGeneric<>, 3> CalculateCubicBezierTOfCriticalPoint(SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1, SVGCurveLib::PointGeneric<> p2, SVGCurveLib::PointGeneric<> p3);


		struct ArcLengthMapEntry {
			float t = 0;
			float arcLength = 0;

			ArcLengthMapEntry() {};
			ArcLengthMapEntry(float t, float arcLength) : t(t), arcLength(arcLength) {};
		};

		struct ArcLengthInfo {
			float arcLength = 0;
			
			std::vector<SVGCurveLib::ArcLengthMapEntry> arcLengthMap;

			ArcLengthInfo() {};
			ArcLengthInfo(unsigned int arcLengthMapReserveSize) {
				arcLengthMap.reserve(arcLengthMapReserveSize);
			};
		};

		static SVGCurveLib::ArcLengthInfo ApproximateArcLengthOfCurve(unsigned int resolution, SVGCurveLib::PointGeneric<> (*pointOnCurveFunc)(float));



		struct LinearCurve {
			SVGCurveLib::ArcLengthInfo resultantInfo;
				
			LinearCurve(unsigned int resolution, SVGCurveLib::PointGeneric<> (*pointOnCurveFunc)(float));
			SVGCurveLib::PointGeneric<> PointOnLinearCurve(float u);

			private:
				SVGCurveLib::PointGeneric<> (*pointOnCurveFunc)(float);

		};


	private:
		// Some utility functions used throughout

		// We expect you to pass in some form of number
		template <typename T = float>
		static T Clamp(T val, T minVal, T maxVal);

		// We expect you to pass in some form of number
		template <typename T = float>
		static T ToRadians(T angle);

		// We expect you to pass in some form of `SVGCurveLib::PointGeneric<>`
		template <typename Tx = SVGCurveLib::PointGeneric<>, typename Ty = Tx>
		static float Distance(Tx p0, Ty p1);

		// We expect you to pass in some form of `SVGCurveLib::PointGeneric<>`
		template <typename Tx = SVGCurveLib::PointGeneric<>, typename Ty = Tx>
		static float AngleBetween(Tx v0, Ty v1);


		#if defined(ARDUINO)
		static String FloatToString(float val, byte precision);
		#else
		static std::string RemoveTrailingZeros(std::string str) {
			return str.erase((std::min)((int)str.find_last_not_of('0') + 2, (int)str.length()), std::string::npos);
		};
		#endif
};

#endif