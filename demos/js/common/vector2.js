// Eric Eastwood <ericeastwood.com>
// v0.1.0


// Pass in two points or another Vector2
function Vector2(x, y)
{
	// You can pass in another Vector2
	if(x && y === undefined) {
		var vec = x;
		x = vec.x;
		y = vec.y;
	}

	this.x = x;
	this.X = x;
	this.y = y;
	this.Y = y;
	
	Object.defineProperties(this, {
		'raw': {
			get: function() {
				return [this.x, this.y];
			}
		}
	});
}
Vector2.prototype.add = function(other) {
	return new Vector2(this.x + other.x, this.y + other.y);
};
Vector2.prototype.subtract = function(other) {
	return new Vector2(this.x - other.x, this.y - other.y);
};
Vector2.prototype.multiply = function(other) {
	return new Vector2(this.x * other.x, this.y * other.y);
};
Vector2.prototype.divide = function(other) {
	return new Vector2(this.x / other.x, this.y / other.y);
};
Vector2.prototype.scale = function(scalar) {
	return new Vector2(this.x*scalar, this.y*scalar);
};
Vector2.prototype.raw = function(x, y) {
	// Used to translate the returned value
	x = x || 0;
	y = y || 0;
	return [this.x + x, this.y + y];
};