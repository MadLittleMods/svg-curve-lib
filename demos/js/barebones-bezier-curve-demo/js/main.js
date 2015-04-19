var canvas = document.querySelectorAll('.demo-canvas')[0];
var context = canvas.getContext('2d');

function pointOnMyCurve(t) {
	return SVGCurveLib.pointOnCubicBezierCurve(
		{
			x: 50, y: 100
		},
		{
			x: 220, y: 250
		},
		{
			x: 360, y: 20
		},
		{
			x: 450, y: 100
		},
		t
	);
}


// Draw the curve
// Blue line
context.save();
context.beginPath();
context.moveTo(pointOnMyCurve(0).x, pointOnMyCurve(0).y);
for(var t = 0; t <= 1; t += 0.01) {
	var point = pointOnMyCurve(t);

	context.lineTo(point.x, point.y);
}
context.moveTo(pointOnMyCurve(1).x, pointOnMyCurve(1).y);
context.strokeStyle = '#0000cc';
context.lineWidth = 6;
context.stroke();
context.restore();
