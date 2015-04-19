#ifndef SVGCurveLibTests_h
#define SVGCurveLibTests_h

#include "Macchiato.h"
using namespace Macchiato;

#include <array>
// pow and sqrt
#include <math.h>

#include "SVGCurveLib.h"


struct SVGCurveLibTests {

	static void RunTests(function<void, PlatformString> logFunc) {

		// Necessary in case they run multiple times
		Macchiato::ClearTestResults();

		MacchiatoSettings.useAnsiColor = true;
		// `"\t" is good but can"t change tab-width/size in CMD prompt.
		MacchiatoSettings.indentToken = "    ";



		float tolerance = 0.0001f;

		// Wrap around Macchiato::expect to make it easier
		auto testHelperForSVGCurveLibPoint = [&](SVGCurveLib::PointGeneric<> expected, SVGCurveLib::PointGeneric<> actual) {
			return expect(actual)
				.to
				->satisfy(
					[&](SVGCurveLib::PointGeneric<> actual) {
						return actual.equalsWithinTolerance(expected, tolerance);
					},
					[&](SVGCurveLib::PointGeneric<> actual, testFlags flags) {
						return PlatformString("Expected ") + PlatformString(actual) + " to " + (flags.negate ? "not " : "") + "equal " + PlatformString(expected) + " within tolerance of " + PlatformString(tolerance);
					}
				)
				->getResult();
		};


		describe("SVGCurveLib::PointOnLine", [&]() {
			it("should return starting point if sampling at 0", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 0, 0 },
					SVGCurveLib::PointOnLine({ 0, 0 }, { 10, 10 }, 0)
				);
			});
			it("should return end point if sampling at 1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 10, 10 },
					SVGCurveLib::PointOnLine({ 0, 0 }, { 10, 10 }, 1)
				);
			});

			it("SVGCurveLib::PointOnLine should sample properly", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 5, 10 },
					SVGCurveLib::PointOnLine({ 0, 0 }, { 10, 20 }, 0.5)
				);
			});
			it("SVGCurveLib::PointOnLine should sample properly2", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 15, 55 },
					SVGCurveLib::PointOnLine({ -10, 0 }, { 90, 220 }, 0.25)
				);
			});
		});

		describe("SVGCurveLib::PointOnQuadraticBezierCurve", [&]() {
			auto pointOnQuadraticBezierCurveTestPreset1 = [&](float t) {
				return SVGCurveLib::PointOnQuadraticBezierCurve({ 50, 150 }, { 420, 20 }, { 450, 150 }, t);
			};

			it("should return starting point if sampling at 0", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 50, 150 },
					pointOnQuadraticBezierCurveTestPreset1(0)
				);
			});
			it("should return end point if sampling at 1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 450, 150 },
					pointOnQuadraticBezierCurveTestPreset1(1)
				);
			});

			it("should sample properly1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 335, 85 },
					pointOnQuadraticBezierCurveTestPreset1(0.5)
				);
			});
			it("should sample properly2", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 413.75f, 101.25f },
					pointOnQuadraticBezierCurveTestPreset1(0.75)
				);
			});
		});


		describe("SVGCurveLib::PointOnCubicBezierCurve", [&]() {
			auto pointOnCubicBezierCurveTestPreset1 = [&](float t) {
				return SVGCurveLib::PointOnCubicBezierCurve({ 50, 150 }, { 220, 250 }, { 360, 20 }, { 450, 150 }, t);
			};
			
			it("should return starting point if sampling at 0", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 50, 150 },
					pointOnCubicBezierCurveTestPreset1(0)
				);
			});
			it("should return end point if sampling at 1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 450, 150 },
					pointOnCubicBezierCurveTestPreset1(1)
				);
			});

			it("should sample properly1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 280, 138.75f },
					pointOnCubicBezierCurveTestPreset1(0.5f)
				);
			});
			it("should sample properly2", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 185.30496f, 171.53088f },
					pointOnCubicBezierCurveTestPreset1(0.28f)
				);
			});
		});



		describe("SVGCurveLib::PointOnEllipticalArc", [&]() {
			auto pointOnEllipticalArcTestPreset1 = [&](float t) {
				return SVGCurveLib::PointOnEllipticalArc({ 40, 50 }, 3, 1, 0, false, false, { 120, 80 }, t);
			};

			auto pointOnEllipticalArcTestPreset2 = [&](float t) {
				return SVGCurveLib::PointOnEllipticalArc({ 250, 100 }, 120, 80, 45, true, true, { 250, 200 }, t);
			};

			auto pointOnEllipticalArcTestPreset3 = [&](float t, bool largeArcFlag, bool sweepFlag) {
				return SVGCurveLib::PointOnEllipticalArc({ 250, 100 }, 120, 80, 0, largeArcFlag, sweepFlag, { 250, 200 }, t);
			};
			
			it("should return starting point if sampling at 0", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 40, 50 },
					pointOnEllipticalArcTestPreset1(0)
				);
			});
			it("should return end point if sampling at 1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 120, 80 },
					pointOnEllipticalArcTestPreset1(1)
				);
			});

			it("should sample properly1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 21.7450864219f, 70.07022949308f },
					pointOnEllipticalArcTestPreset1(0.35f)
				);
			});
			it("should sample properly2", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 89.660911246025f, 84.80927614891685f },
					pointOnEllipticalArcTestPreset1(0.82f)
				);
			});
			it("should sample properly3", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 438.38624784078576f, 222.45624916953307f },
					pointOnEllipticalArcTestPreset2(0.5f)
				);
			});

			describe("when rx or ry is 0", [&]() {
				it("should return a point on linear line between the start and end points1", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 5, 10 },
						SVGCurveLib::PointOnEllipticalArc({ 0, 0 }, 0, 80, 45, true, true, { 10, 20 }, 0.5f)
					);
				});
				it("should return a point on linear line between the start and end points2", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 5, 10 },
						SVGCurveLib::PointOnEllipticalArc({ 0, 0 }, 120, 0, 45, true, true, { 10, 20 }, 0.5f)
					);
				});
			});

			// Diagram of all 4 arcs: http://i.imgur.com/D2GLnqW.png
			describe("largeArcFlag and sweepFlag", [&]() {
				it("should sample properly with largeArcFlag=false and sweepFlag=false", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 223.67496997597596f, 150 },
						pointOnEllipticalArcTestPreset3(0.5, false, false)
					);
				});
				it("should sample properly with largeArcFlag=true and sweepFlag=true", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 463.67496997597596f, 150 },
						pointOnEllipticalArcTestPreset3(0.5, true, true)
					);
				});
				it("should sample properly with largeArcFlag=true and sweepFlag=false", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 36.325030024024045f, 150 },
						pointOnEllipticalArcTestPreset3(0.5, true, false)
					);
				});
				it("should sample properly with largeArcFlag=false and sweepFlag=true", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 276.32503002402404f, 150 },
						pointOnEllipticalArcTestPreset3(0.5, false, true)
					);
				});
			});
		});



		describe("SVGCurveLib::CalculateQuadraticBezierTOfCriticalPoint", [&]() {
			it("should return correct results1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 1.088235294117647f, 0.5f },
					SVGCurveLib::CalculateQuadraticBezierTOfCriticalPoint({ 50, 150 }, { 420, 20 }, { 450, 150 })
				);
			});
			it("should return correct results2", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 0.8125f, 0.5f },
					SVGCurveLib::CalculateQuadraticBezierTOfCriticalPoint({ 50, 150 }, { 570, 53 }, { 450, 150 })
				);
			});
		});


		describe("SVGCurveLib::CalculateCubicBezierTOfCriticalPoint", [&]() {

			std::array<SVGCurveLib::PointGeneric<>, 3> result1 = SVGCurveLib::CalculateCubicBezierTOfCriticalPoint({ 50, 150 }, { 220, 250 }, { 360, 20 }, { 450, 150 });
			describe("preset1", [&]() {
				it("should return array of size 3", [&]() {
					return expect<int>(3)
						.to
						->equal(result1.size())
						->getResult();
				});

				it("should return correct result item0", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 1.7787192621510002f, 0.18876834267480905f },
						result1[0]
					);
				});
				it("should return correct result item1", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ -4.778719262151f, 0.7677533964556257f },
						result1[1]
					);
				});
				it("should return correct result item2", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ -1.0f, -1.0f },
						result1[2]
					);
				});
			});

			std::array<SVGCurveLib::PointGeneric<>, 3> result2 = SVGCurveLib::CalculateCubicBezierTOfCriticalPoint({ 311, 182 }, { 64, 196 }, { 631, 29 }, { 450, 150 });
			describe("preset2", [&]() {
				it("should return array of size 3", [&]() {
					return expect<int>(3)
						.to
						->equal(result1.size())
						->getResult();
				});

				it("should return correct result item0", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 0.85793902100339f, 0.040834339501378505f },
						result2[0]
					);
				});
				it("should return correct result item1", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ 0.18431450012337042f, 0.7310206711596023f },
						result2[1]
					);
				});
				it("should return correct result item2", [&]() {
					return testHelperForSVGCurveLibPoint(
						{ -1.0f, -1.0f },
						result2[2]
					);
				});
			});
		});

		describe("SVGCurveLib::ApproximateArcLengthOfCurve", [&]() {

			describe("with SVGCurveLib::PointOnLine", [&]() {
				unsigned int resolution = 100;
				SVGCurveLib::ArcLengthInfo result = SVGCurveLib::ApproximateArcLengthOfCurve(resolution, [&](float t) {
					return SVGCurveLib::PointOnLine({ 0, 0 }, { 10, 10 }, t);
				});

				it("should return the correct arcLength", [&]() {
					return expect(result.arcLength)
						.to->be
						->closeTo(14.1421356237f, tolerance)
						->getResult();
				});

				it("The arcLengthMap should have at least as many entries as the resolution", [&]() {
					return expect(result.arcLengthMap.size())
						.to->be->at
						->least(resolution)
						->getResult();
				});
			});
		});


		describe("SVGCurveLib::LinearCurve", [&]() {
			SVGCurveLib::LinearCurve linearCurvePreset1 = SVGCurveLib::LinearCurve(50, [&](float t) {
				return SVGCurveLib::PointOnLine({ 0, 0 }, { 10, 10 }, t);
			});
			SVGCurveLib::LinearCurve linearCurvePreset2 = SVGCurveLib::LinearCurve(50, [&](float t) {
				return SVGCurveLib::PointOnLine({ -20, -50 }, { 100, 200 }, t);
			});

			auto distance = [&](SVGCurveLib::PointGeneric<> p0, SVGCurveLib::PointGeneric<> p1) {
				return sqrt(pow(p1.x-p0.x, 2) + pow(p1.y-p0.y, 2));
			};

			it("includes `arcLength` as a property for reference", [&]() {
				return expect(linearCurvePreset1.resultantInfo.arcLength)
					.to->be
					->closeTo(14.1421356237f, tolerance)
					->getResult();
			});

			it("should return starting point if sampling at 0", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 0, 0 },
					linearCurvePreset1.PointOnLinearCurve(0)
				);
			});
			it("should return end point if sampling at 1", [&]() {
				return testHelperForSVGCurveLibPoint(
					{ 10, 10 },
					linearCurvePreset1.PointOnLinearCurve(1)
				);
			});

			it("should sample with evenly spaced points at a constant interval", [&]() {
				float distanceIncrement = 5.0f;
				SVGCurveLib::PointGeneric<> prevPt = linearCurvePreset2.PointOnLinearCurve(0);
				float tIncrement = distanceIncrement/linearCurvePreset2.resultantInfo.arcLength;
				// Since we already populated our previous pt to the start(0) point, move are first increment to the next point
				for(float i = tIncrement; i <= 1; i += tIncrement) {
					SVGCurveLib::PointGeneric<> pt = linearCurvePreset2.PointOnLinearCurve(i);

					TestResult testResult = expect(distance(prevPt, pt))
						.to->be
						->closeTo(distanceIncrement, tolerance)
						->getResult();

					if(!testResult.didPass) {
						return testResult;
					}
					
					prevPt = pt;
				}

				// If at the end there are no errors, return something good
				return expect<bool>(true).to->equal(true)->getResult();
			});
		});


		logFunc(Macchiato::GetResultantTestOutput());

					

	};

};


#endif