
var canvas = $('.demo-canvas')[0];
var context = canvas.getContext('2d');



var ellipticalArcDemoModel = {
	curve: function(t) {
		return SVGCurveLib.pointOnEllipticalArc(this.settings.p0, this.settings.rx, this.settings.ry, this.settings.xAxisRotation, this.settings.largeArcFlag, this.settings.sweepFlag, this.settings.p1, t);
	},
	generateLinearCurve: function() {
		return SVGCurveLib.generateLinearCurve(this.settings.resolution, this.curve.bind(this));
	},

	settings: {
		offset: new Vector2(0, 0),

		p0: new Vector2(40, 50),
		p1: new Vector2(120, 80),
		rx: 3,
		ry: 1,
		largeArcFlag: false,
		sweepFlag: false,
		xAxisRotation: 0,

		resolution: 15,
		subdivision: 35
	},

	draggablePoints: [
		// Endpoints
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p0', 'rgba(255, 0, 0, 1)'),
		generateGenericFreeMoveHandlePresetDraggablePoint('settings.p1', 'rgba(255, 0, 0, 1)'),

		// Radii handles
		generateRadiiHandlePresetDraggablePoint('x', 'rgba(0, 0, 220, 0.6)', 'rgba(0, 0, 220, 0.3)'),
		generateRadiiHandlePresetDraggablePoint('y', 'rgba(220, 0, 0, 0.6)', 'rgba(220, 0, 0, 0.3)')
	]
};




var demoState = {
	context: context,
	// Which models do we want to use in this demo
	modelList: [
		ellipticalArcDemoModel
	]
};


var myDemo = new SVGCurveLibDemo(demoState);


// Make a bunch of text inputs that someone can manually adjust
generateTextInputsForModel(myDemo, $('.demo-input-control-column'));


// Also update the SVG equivalents whenever the demo updates
$(myDemo).on('demo-update', function() {


	var ellipticalArcsettings1 = myDemo.demoState.modelList[0].settings;
	var generateEllipticalArcSVGCommandFromSettings = function(settings) {
		var startPoint = settings.p0.add(settings.offset);
		var endPoint = settings.p1.add(settings.offset);

		return 'M' + startPoint.x + "," + startPoint.y + ' A' + settings.rx + "," + settings.ry + ' ' + settings.xAxisRotation + ' ' + (settings.largeArcFlag ? '1' : '0') + ' ' + (settings.sweepFlag ? '1' : '0') + ' ' + endPoint.x + "," + endPoint.y;
	};

	$('.js-svg-elliptical-arc-path').attr('d', generateEllipticalArcSVGCommandFromSettings(ellipticalArcsettings1));

}).trigger('demo-update');


// Allow the text input toggle box to be expanded/collapsed to get out of the way
$('.js-demo-input-control-visibility-handle').on('click', function() {
	// Toggle the control box
	$('.demo-input-control-column').toggleClass('is-collapsed');

	// Add a helper class to the handle so we can change the direction of the arrow
	$(this).toggleClass('is-input-control-column-collapsed');
});