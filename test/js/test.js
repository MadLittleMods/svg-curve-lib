var Promise = require('bluebird');

var chai = require('chai');
var expect = require('chai').expect;
var chaiAsPromised = require('chai-as-promised');
chai.use(chaiAsPromised);


chai.use(function(_chai, utils) {
	function equalWithinTolerance(val, tolerance, msg) {
		// Default properly if they didn't pass in a tolerance
		if(!msg && !tolerance) {
			msg = tolerance;
			tolerance = 0.0000001;
		}

		if (msg) {
			utils.flag(this, 'message', msg);
		}

		var expect = val;
		var actual = this._obj;
		var isDeep = utils.flag(this, 'deep');

		function recursivelyEqual(expect, actual) {
			// Convert to object if not already
			// It makes the logic easier below
			if(typeof expect !== "object") {
				expect = {
					value: expect
				};
				actual = {
					value: actual
				};
			}

			return Object.keys(expect).every(function(key) {
				if(!actual.hasOwnProperty(key)) {
					return false;
				}

				if(isDeep && typeof expect[key] === "object") {
					return recursivelyEqual(expect[key], shape[key]);
				}
				else if(typeof expect[key] === "number") {
					return (typeof actual[key] === "number" && Math.abs(expect[key] - actual[key]) <= tolerance);
				}

				return expect[key] == actual[key];
			});
		}

		// http://chaijs.com/guide/plugins/#basic-assertion
		this.assert(
			// a boolean (result of a truth test)
			recursivelyEqual(expect, actual),
			// a string error message to be used if the first argument is false
			'expected #{this} to ' + (isDeep ? 'deeply ' : '') + 'equal #{exp} within tolerance', 
			// a string error message to be used if the assertion is negated and the first argument is true
			'expected #{this} to not ' + (isDeep ? 'deeply ' : '') + 'equal #{exp} within tolerance',
			// (optional) the expected value
			expect,
			// (optional) the actual value, which will default to _obj
			actual,
			// Show nice object diffs
			true
		);

		return true;
	}

	_chai.Assertion.addMethod('equalWithinTolerance', equalWithinTolerance);
});


var svgCurveLib = require('../../src/js/svg-curve-lib.js');



var tolerance = 0.0000001;


describe('pointOnLine(...)', function() {

	it('should return starting point if sampling at 0', function() {
		return expect(svgCurveLib.pointOnLine({ x: 0, y: 0 }, { x: 10, y: 10 }, 0)).to.deep.equalWithinTolerance({ x: 0, y: 0 }, tolerance);
	});

	it('should return end point if sampling at 1', function() {
		return expect(svgCurveLib.pointOnLine({ x: 0, y: 0 }, { x: 10, y: 10 }, 1)).to.deep.equalWithinTolerance({ x: 10, y: 10 }, tolerance);
	});

	it('should sample properly', function() {
		return Promise.all([
			expect(svgCurveLib.pointOnLine({ x: 0, y: 0 }, { x: 10, y: 20 }, 0.5)).to.deep.equalWithinTolerance({ x: 5, y: 10 }, tolerance),
			expect(svgCurveLib.pointOnLine({ x: -10, y: 0 }, { x: 90, y: 220 }, 0.25)).to.deep.equalWithinTolerance({ x: 15, y: 55 }, tolerance)
		]);
	});
});

describe('pointOnQuadraticBezierCurve(...)', function() {
	var pointOnQuadraticBezierCurveTestPreset1 = function(t) {
		return svgCurveLib.pointOnQuadraticBezierCurve({ x: 50, y: 150 }, { x: 420, y: 20 }, { x: 450, y: 150 }, t);
	};

	it('should return starting point if sampling at 0', function() {
		return expect(pointOnQuadraticBezierCurveTestPreset1(0)).to.deep.equalWithinTolerance({ x: 50, y: 150 }, tolerance);
	});

	it('should return end point if sampling at 1', function() {
		return expect(pointOnQuadraticBezierCurveTestPreset1(1)).to.deep.equalWithinTolerance({ x: 450, y: 150 }, tolerance);
	});

	it('should sample properly', function() {
		return Promise.all([
			expect(pointOnQuadraticBezierCurveTestPreset1(0.5)).to.deep.equalWithinTolerance({ x: 335, y: 85 }, tolerance),
			expect(pointOnQuadraticBezierCurveTestPreset1(0.75)).to.deep.equalWithinTolerance({ x: 413.75, y: 101.25 }, tolerance)
		]);
	});
});

describe('pointOnCubicBezierCurve(...)', function() {
	var pointOnCubicBezierCurveTestPreset1 = function(t) {
		return svgCurveLib.pointOnCubicBezierCurve({ x: 50, y: 150 }, { x: 220, y: 250 }, { x: 360, y: 20 }, { x: 450, y: 150 }, t);
	};

	it('should return starting point if sampling at 0', function() {
		return expect(pointOnCubicBezierCurveTestPreset1(0)).to.deep.equalWithinTolerance({ x: 50, y: 150 }, tolerance);
	});

	it('should return end point if sampling at 1', function() {
		return expect(pointOnCubicBezierCurveTestPreset1(1)).to.deep.equalWithinTolerance({ x: 450, y: 150 }, tolerance);
	});

	it('should sample properly', function() {
		return Promise.all([
			expect(pointOnCubicBezierCurveTestPreset1(0.5)).to.deep.equalWithinTolerance({ x: 280, y: 138.75 }, tolerance),
			expect(pointOnCubicBezierCurveTestPreset1(0.28)).to.deep.equalWithinTolerance({ x: 185.30496, y: 171.53088 }, tolerance)
		]);
	});
});

describe('pointOnEllipticalArc(...)', function() {
	var pointOnEllipticalArcTestPreset1 = function(t) {
		return svgCurveLib.pointOnEllipticalArc({ x: 40, y: 50 }, 3, 1, 0, false, false, { x: 120, y: 80 }, t);
	};
	var pointOnEllipticalArcTestPreset2 = function(t) {
		return svgCurveLib.pointOnEllipticalArc({ x: 250, y: 100 }, 120, 80, 45, true, true, { x: 250, y: 200 }, t);
	};

	var pointOnEllipticalArcTestPreset3 = function(t, largeArcFlag, sweepFlag) {
		largeArcFlag = largeArcFlag || false;
		sweepFlag = sweepFlag || false;

		return svgCurveLib.pointOnEllipticalArc({ x: 250, y: 100 }, 120, 80, 0, largeArcFlag, sweepFlag, { x: 250, y: 200 }, t);
	};

	it('should return starting point if sampling at 0', function() {
		return expect(pointOnEllipticalArcTestPreset1(0)).to.deep.equalWithinTolerance({ x: 40, y: 50 }, tolerance);
	});

	it('should return end point if sampling at 1', function() {
		return expect(pointOnEllipticalArcTestPreset1(1)).to.deep.equalWithinTolerance({ x: 120, y: 80 }, tolerance);
	});

	it('should sample properly with some arc', function() {
		return Promise.all([
			expect(pointOnEllipticalArcTestPreset1(0.35)).to.deep.equalWithinTolerance({ x: 21.7450864219, y: 70.07022949308 }, tolerance),
			expect(pointOnEllipticalArcTestPreset1(0.82)).to.deep.equalWithinTolerance({ x: 89.660911246025, y: 84.80927614891685 }, tolerance),
			expect(pointOnEllipticalArcTestPreset2(0.5)).to.deep.equalWithinTolerance({ x: 438.38624784078576, y: 222.45624916953307 }, tolerance)
		]);
	});

	describe('when rx or ry is 0', function() {
		it('should return a point on linear line between the start and end points', function() {
			return Promise.all([
				expect(svgCurveLib.pointOnEllipticalArc({ x: 0, y: 0 }, 0, 80, 45, true, true, { x: 10, y: 20 }, 0.5)).to.deep.equalWithinTolerance({ x: 5, y: 10 }, tolerance),
				expect(svgCurveLib.pointOnEllipticalArc({ x: 0, y: 0 }, 120, 0, 45, true, true, { x: 10, y: 20 }, 0.5)).to.deep.equalWithinTolerance({ x: 5, y: 10 }, tolerance)
			]);
		});
	});

	// Diagram of all 4 arcs: http://i.imgur.com/D2GLnqW.png
	describe('largeArcFlag and sweepFlag', function() {
		it('should sample properly with largeArcFlag=false and sweepFlag=false', function() {
			return expect(pointOnEllipticalArcTestPreset3(0.5, false, false)).to.deep.equalWithinTolerance({ x: 223.67496997597596, y: 150 }, tolerance);
		});
		it('should sample properly with largeArcFlag=true and sweepFlag=true', function() {
			return expect(pointOnEllipticalArcTestPreset3(0.5, true, true)).to.deep.equalWithinTolerance({ x: 463.67496997597596, y: 150 }, tolerance);
		});
		it('should sample properly with largeArcFlag=true and sweepFlag=false', function() {
			return expect(pointOnEllipticalArcTestPreset3(0.5, true, false)).to.deep.equalWithinTolerance({ x: 36.325030024024045, y: 150 }, tolerance);
		});
		it('should sample properly with largeArcFlag=false and sweepFlag=true', function() {
			return expect(pointOnEllipticalArcTestPreset3(0.5, false, true)).to.deep.equalWithinTolerance({ x:  276.32503002402404, y: 150 }, tolerance);
		});
	});
});

describe('calculateQuadraticBezierTOfCriticalPoint(...)', function() {
	it('should return correct results', function() {
		return Promise.all([
			expect(svgCurveLib.calculateQuadraticBezierTOfCriticalPoint({ x: 50, y: 150 }, { x: 420, y: 20 }, { x: 450, y: 150 })).to.deep.equalWithinTolerance({ x: 1.088235294117647, y: 0.5 }, tolerance),
			expect(svgCurveLib.calculateQuadraticBezierTOfCriticalPoint({ x: 50, y: 150 }, { x: 570, y: 53 }, { x: 450, y: 150 })).to.deep.equalWithinTolerance({ x: 0.8125, y: 0.5 }, tolerance)
		]);
	});
});

describe('calculateCubicBezierTOfCriticalPoint(...)', function() {
	it('should return correct results', function() {
		var result1 = svgCurveLib.calculateCubicBezierTOfCriticalPoint({ x: 50, y: 150 }, { x: 220, y: 250 }, { x: 360, y: 20 }, { x: 450, y: 150 });
		var result2 = svgCurveLib.calculateCubicBezierTOfCriticalPoint({ x: 311, y: 182 }, { x: 64, y: 196 }, { x: 631, y: 29 }, { x: 450, y: 150 });


		return Promise.all([
			expect(result1.length).to.equal(3),
			expect(result1[0]).to.deep.equalWithinTolerance({ x: 1.7787192621510002, y: 0.18876834267480905 }, tolerance),
			expect(result1[1]).to.deep.equalWithinTolerance({ x: -4.778719262151, y: 0.7677533964556257 }, tolerance),
			expect(result1[2]).to.deep.equalWithinTolerance({ x: false, y: false }, tolerance),

			expect(result2.length).to.equal(3),
			expect(result2[0]).to.deep.equalWithinTolerance({ x: 0.85793902100339, y: 0.040834339501378505 }, tolerance),
			expect(result2[1]).to.deep.equalWithinTolerance({ x: 0.18431450012337042, y: 0.7310206711596023 }, tolerance),
			expect(result2[2]).to.deep.equalWithinTolerance({ x: false, y: false }, tolerance)
		]);
	});
});




describe('approximateArcLengthOfCurve(...)', function() {
	it('should work with pointOnLine(...)', function() {
		var resolution = 500;
		var result = svgCurveLib.approximateArcLengthOfCurve(resolution, function(t) {
			return svgCurveLib.pointOnLine({ x: 0, y: 0 }, { x: 10, y: 10 }, t);
		});

		return Promise.all([
			expect(result.arcLength).to.equalWithinTolerance(14.1421356237, tolerance),
			expect(result.arcLengthMap.length).to.be.at.least(resolution)
		]);
	});
});


describe('generateLinearCurve(...)', function() {
	it('includes `arcLength` as a property for reference', function() {
		var resolution = 50;
		var linearCurve = svgCurveLib.generateLinearCurve(resolution, function(t) {
			return svgCurveLib.pointOnLine({ x: 0, y: 0 }, { x: 10, y: 10 }, t);
		});

		return expect(linearCurve).to.have.keys('arcLength');
	});

	it('should return starting point if sampling at 0', function() {
		var resolution = 50;
		var linearCurve = svgCurveLib.generateLinearCurve(resolution, function(t) {
			return svgCurveLib.pointOnLine({ x: 0, y: 0 }, { x: 10, y: 10 }, t);
		});

		return expect(linearCurve(0)).to.deep.equalWithinTolerance({ x: 0, y: 0 }, tolerance);
	});

	it('should return end point if sampling at 1', function() {
		var resolution = 50;
		var linearCurve = svgCurveLib.generateLinearCurve(resolution, function(t) {
			return svgCurveLib.pointOnLine({ x: 0, y: 0 }, { x: 10, y: 10 }, t);
		});

		return expect(linearCurve(1)).to.deep.equalWithinTolerance({ x: 10, y: 10 }, tolerance);
	});

	it('should sample with evenly spaced points at a constant interval', function() {
		var resolution = 50;
		var linearCurve = svgCurveLib.generateLinearCurve(resolution, function(t) {
			return svgCurveLib.pointOnLine({ x: -20, y: -50 }, { x: 100, y: 200 }, t);
		});
		
		var distanceIncrement = 5;
		var prevPt = linearCurve(0);
		var tIncrement = distanceIncrement/linearCurve.arcLength;
		// Since we already populated our previous pt to the start(0) point, move are first increment to the next point
		for(var i = tIncrement; i <= 1; i += tIncrement) {
			var pt = linearCurve(i);

			expect(distance(prevPt, pt)).to.equalWithinTolerance(distanceIncrement, tolerance);
			
			prevPt = pt;
		}

	});

});



function distance(p0, p1) {
	return Math.sqrt(Math.pow(p1.x-p0.x, 2) + Math.pow(p1.y-p0.y, 2));
}