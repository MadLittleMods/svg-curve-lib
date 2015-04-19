
var canvas = $('.demo-canvas')[0];
var context = canvas.getContext('2d');


var quadraticBezierCurveDemoModel = {
	curve: function(t) {
		return SVGCurveLib.pointOnQuadraticBezierCurve(this.settings.p0, this.settings.p1, this.settings.p2, t);
	},
	generateLinearCurve: function() {
		return SVGCurveLib.generateLinearCurve(this.settings.resolution, this.curve.bind(this));
	},

	settings: {
		offset: new Vector2(0, 0),

		p0: new Vector2(50, 150),
		p1: new Vector2(420, 20),
		p2: new Vector2(450, 150),

		resolution: 25,
		subdivision: 35
	},

	draggablePoints: [
		// Control points
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p0', 'rgba(255, 0, 0, 1)'),
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p1', 'rgba(0, 255, 0, 1)'),
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p2', 'rgba(255, 0, 0, 1)')
	]
};

var cubicBezierCurveDemoModel = {
	curve: function(t) {
		return SVGCurveLib.pointOnCubicBezierCurve(this.settings.p0, this.settings.p1, this.settings.p2, this.settings.p3, t);
	},
	generateLinearCurve: function() {
		return SVGCurveLib.generateLinearCurve(this.settings.resolution, this.curve.bind(this));
	},

	settings: {
		offset: new Vector2(0, 200),

		p0: new Vector2(50, 150),
		p1: new Vector2(220, 250),
		p2: new Vector2(360, 20),
		p3: new Vector2(450, 150),

		resolution: 25,
		subdivision: 35
	},

	draggablePoints: [
		// Control points
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p0', 'rgba(255, 0, 0, 1)'),
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p1', 'rgba(0, 255, 0, 1)'),
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p2', 'rgba(0, 255, 0, 1)'),
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p3', 'rgba(255, 0, 0, 1)')
	]
};






var demoState = {
	context: context,
	// Which models do we want to use in this demo
	modelList: [
		quadraticBezierCurveDemoModel,
		cubicBezierCurveDemoModel
	]
};

// Create the demo
var myDemo = new SVGCurveLibDemo(demoState);


// Make a bunch of text inputs that someone can manually adjust
generateTextInputsForModel(myDemo, $('.demo-input-control-column'));


// Also update the SVG equivalents whenever the demo updates
$(myDemo).on('demo-update', function() {


	var quadraticSettings1 = myDemo.demoState.modelList[0].settings;
	var generateQuadraticCurveSVGCommandFromSettings = function(settings) {
		var startPoint = settings.p0.add(settings.offset);
		var controlPoint1 = settings.p1.add(settings.offset);
		var endPoint = settings.p2.add(settings.offset);

		return 'M' + startPoint.x + "," + startPoint.y + ' Q' + controlPoint1.x + "," + controlPoint1.y + ' ' + endPoint.x + "," + endPoint.y;
	};

	$('.js-svg-quadratic-bezier-curve-path').attr('d', generateQuadraticCurveSVGCommandFromSettings(quadraticSettings1));


	var cubicSettings1 = myDemo.demoState.modelList[1].settings;
	var generateCubicCurveSVGCommandFromSettings = function(settings) {
		var startPoint = settings.p0.add(settings.offset);
		var controlPoint1 = settings.p1.add(settings.offset);
		var controlPoint2 = settings.p2.add(settings.offset);
		var endPoint = settings.p3.add(settings.offset);

		return 'M' + startPoint.x + "," + startPoint.y + ' C' + controlPoint1.x + "," + controlPoint1.y + ' ' + controlPoint2.x + "," + controlPoint2.y + ' ' + endPoint.x + "," + endPoint.y;
	};
	$('.js-svg-cubic-bezier-curve-path').attr('d', generateCubicCurveSVGCommandFromSettings(cubicSettings1));


}).trigger('demo-update');



// Allow the text input toggle box to be expanded/collapsed to get out of the way
$('.js-demo-input-control-visibility-handle').on('click', function() {
	// Toggle the control box
	$('.demo-input-control-column').toggleClass('is-collapsed');

	// Add a helper class to the handle so we can change the direction of the arrow
	$(this).toggleClass('is-input-control-column-collapsed');
});