


var SVGCurveLibDemo = (function() {

	var rAF = window.requestAnimationFrame ||
		window.webkitRequestAnimationFrame ||
		window.mozRequestAnimationFrame ||
		window.oRequestAnimationFrame ||
		window.msRequestAnimationFrame ||
		null;



	function demo(demoState) {
		this.demoState = {
			context: null,

			// Which models do we want to use in this demo
			modelList: [],
			modelStates: [],

			isDrawing: false,
			updateStopDrawLoopTimeoutId: null,
			cursorHoverTimeoutId: null
		};

		this.demoState = $.extend({}, this.demoState, demoState);

		this.init();
	}

	demo.prototype.init = function(demoState) {
		var self = this;

		this.demoState = $.extend({}, this.demoState, demoState);


		this.demoState.modelList.forEach(function(demoModel, modelIndex) {
			// Init the map
			self.demoState.modelStates[modelIndex] = {};
			self.demoState.modelStates[modelIndex].draggablePointCursorHoveringState = [];

			// Init draggable points
			demoModel.draggablePoints.forEach(function(draggablePoint, pointIndex) {
				draggablePoint.init(demoModel, self.update.bind(self), function(result) {
					self.demoState.modelStates[modelIndex].draggablePointCursorHoveringState[pointIndex] = result.isHovering;

					self.updateCursor();

				});
			});
		});


		// Call the first update to init some values
		this.update();
		// Then draw the first frame
		this.startDrawing();
	};


	demo.prototype.update = function() {
		var self = this;

		this.demoState.modelList.forEach(function(demoModel, index) {
			self.demoState.modelStates[index].linearCurve = demoModel.generateLinearCurve();
			self.demoState.modelStates[index].approxArcLengthResult = SVGCurveLib.approximateArcLengthOfCurve(demoModel.settings.resolution, demoModel.curve.bind(demoModel));
		});

		// Start the loop whenever there are changes
		// and stop it after a little bit of time has passed without any new changes
		this.startDrawing();
		clearTimeout(this.demoState.updateStopDrawLoopTimeoutId);
		this.demoState.updateStopDrawLoopTimeoutId = setTimeout(function() {
			self.stopDrawing();
		}, 100);


		// Notify any listening that the demo updated
		$(this).trigger('demo-update');
	};

	demo.prototype.startDrawing = function() {
		if(!this.demoState.isDrawing) {
			this.demoState.isDrawing = true;
			this.draw();
		}
	};
	demo.prototype.stopDrawing = function() {
		this.demoState.isDrawing = false;
	};
	demo.prototype.draw = function() {
		var self = this;

		this.demoState.context.clearRect(0, 0, 720, 720);

		drawSVGCurveLibDemoModels(this.demoState.context, this.demoState);

		if(this.demoState.isDrawing) {
			rAF(function() {
				self.draw();
			});
		}
	};



	demo.prototype.updateCursor = function() {
		var isHovering = this.demoState.modelStates.reduce(function(prevIsHovering, modelState, modelStateIndex) {
			prevIsHovering = prevIsHovering || modelState.draggablePointCursorHoveringState.some(function(val) {
				return val;
			});

			return prevIsHovering;
		}, false);
		
		if(isHovering) {
			$(this.demoState.context.canvas).css('cursor', 'pointer');
		}
		else {
			$(this.demoState.context.canvas).css('cursor', 'auto');
		}
	};

	return demo;
})();





function drawSVGCurveLibDemoModels(context, demoState) {
	demoState.modelList.forEach(function(demoModel, index) {

		context.save();
		// Apply the offset (helpful for when you want to layout multiple curve models)
		if(demoModel.settings.offset) {
			context.translate.apply(context, demoModel.settings.offset.raw);
		}

		var linearCurve = demoState.modelStates[index].linearCurve;

		// Draw the main Bezier Curve
		// Blue line
		if(linearCurve) {
			var increment = 5/linearCurve.arcLength;
			context.save();
			context.beginPath();
			context.moveTo.apply(context, new Vector2(linearCurve(0)).raw);
			for(var i = 0; i <= 1; i += increment) {
				var pt = new Vector2(linearCurve(i));

				context.lineTo.apply(context, pt.raw);
			}
			context.lineTo.apply(context, new Vector2(linearCurve(1)).raw);
			context.strokeStyle = '#0000cc';
			context.lineWidth = 6;
			context.stroke();
			context.restore();
		}






		// Display the arc length
		var approxArcLengthResult = demoState.modelStates[index].approxArcLengthResult;
		context.fillStyle = '#000000';
		context.font = '18px ' + context.font.split(' ')[1];
		context.fillText('Approx. Arc length: ' + approxArcLengthResult.arcLength, 6, 18);

		// Make sure we are tracing the correct line
		// By drawing connecting lines of the approximation samples
		// Yellow dots and lines
		approxArcLengthResult.approximationLines.forEach(function(line) {
			var prevPoint;
			line.forEach(function(pt) {
				// Draw some dots representing the non-uniform distrubtion of t to arclength
				context.save();
				context.beginPath();
				context.fillStyle = 'rgba(255, 255, 100, 0.6)';
				drawPointAt(pt, 5, context);
				context.restore();
				
				// Draw connecting lines that we summed to get the approximated distance
				if(prevPoint) {
					context.save();
					context.beginPath();
					context.moveTo.apply(context, new Vector2(prevPoint).raw);
					context.lineTo.apply(context, new Vector2(pt).raw);
					context.strokeStyle = 'rgba(255, 255, 0, 0.6)';
					context.lineWidth = 3;
					context.stroke();
					context.restore();     
				}
				
				prevPoint = pt;
			});
		});


		// Draw the uniform points from the linear bezier
		// Purple dots
		var increment = demoModel.settings.subdivision/linearCurve.arcLength;
		for(var i = 0; i <= 1; i += increment) {
			var pt = linearCurve(i);

			context.save();
			context.beginPath();
			context.fillStyle = 'rgba(255, 0, 255, 0.8)';
			drawPointAt(pt, 8, context);
			context.strokeStyle = 'rgba(0, 0, 0, 0.5)';
			context.lineWidth = 1;
			context.stroke();
			context.restore();
		}
		
		


		// Draw all of the handles
		(demoModel.draggablePoints || []).forEach(function(draggablePoint) {
			draggablePoint.draw(demoModel, context);
		});



		context.restore();
	});
}







// Create a free-moveable point
function generateGenericFreeMoveHandlePresetDraggablePoint(pathToPos, color) {
	return {
		_pointSize: 15,
		_radiusDragCoefficient: 20,
		init: function(model, updateCb, /*optional*/readDraggableResultCb) {
			var self = this;

			return createDraggablePoint(
				model.settings.offset,
				this.calculatePosition(model),
				this._pointSize,
				canvas,
				function(result) {
					if(result.isDragging) {
						updateObjectPropertyByString(model, pathToPos, result.pos);

						if(updateCb) {
							updateCb();
						}
					}

					if(readDraggableResultCb) {
						readDraggableResultCb(result);
					}

					return self.calculatePosition(model);
				}
			);
		},
		draw: function(model, context) {
			context.save();
			context.beginPath();
			context.fillStyle = color;
			drawPointAt(this.calculatePosition(model), this._pointSize, context);
			context.strokeStyle = 'rgba(0, 0, 0, 1)';
			context.lineWidth = 1;
			context.stroke();
			context.restore();
		},
		calculatePosition: function(model) {
			return new Vector2(getObjectPropertybyString(model, pathToPos));
		},
	};
}


function generateRadiiHandlePresetDraggablePoint(axis, pointColor, lineColor) {

	return {
		_pointSize: 9,
		_radiusDragCoefficient: 1,
		init: function(model, updateCb, /*optional*/readDraggableResultCb) {
			var self = this;

			return createDraggablePoint(
				model.settings.offset,
				this.calculatePosition(model),
				this._pointSize,
				canvas,
				function(result) {
					if(result.isDragging) {
						if(axis.toLowerCase() == 'x') {
							model.settings.rx += result.deltaDrag.x/self._radiusDragCoefficient;
						}
						else if(axis.toLowerCase() == 'y') {
							model.settings.ry -= result.deltaDrag.y/self._radiusDragCoefficient;
						}

						if(updateCb) {
							updateCb();
						}
					}

					if(readDraggableResultCb) {
						readDraggableResultCb(result);
					}


					return self.calculatePosition(model);
				}
			);
		},
		draw: function(model, context) {
			var centerPos = new Vector2(model.curve(0).ellipticalArcCenter);
			var rHandlePos = this.calculatePosition(model);

			// Draw a connecting line from the center to the handle
			context.save();
			context.beginPath();
			context.moveTo.apply(context, new Vector2(centerPos).raw);
			context.lineTo.apply(context, new Vector2(rHandlePos).raw);
			context.strokeStyle = lineColor || pointColor;
			context.lineWidth = 4;
			context.stroke();
			context.restore();


			// Draw handle point
			context.save();
			context.beginPath();
			context.fillStyle = pointColor;
			drawPointAt(rHandlePos, this._pointSize, context);
			context.strokeStyle = 'rgba(0, 0, 0, 1)';
			context.lineWidth = 1;
			context.stroke();
			context.restore();
		},
		calculatePosition: function(model) {
			var curveResult = model.curve(0);

			var centerPos = new Vector2(curveResult.ellipticalArcCenter);

			var handleOffsetFromCenter = new Vector2(0, 0);
			if(axis.toLowerCase() == 'x') {
				this._radiusDragCoefficient = curveResult.resultantRx/model.settings.rx;
				handleOffsetFromCenter = new Vector2(curveResult.resultantRx, 0);
			}
			else if(axis.toLowerCase() == 'y') {
				this._radiusDragCoefficient = curveResult.resultantRy/model.settings.ry;
				handleOffsetFromCenter = new Vector2(0, -curveResult.resultantRy);
			}

			return handleOffsetFromCenter.add(centerPos);
		}
	};
}




// Create a bunch of manual text inputs so that you can manually type in values
function generateTextInputsForModel(demo, element) {
	var demoInputUpdateCallbackList = [];
	demo.demoState.modelList.forEach(function(model, modelIndex) {
		var $inputGroup = $('<div class="demo-input-model-group"></div>').appendTo(element);
		Object.keys(model.settings).forEach(function(settingKey) {
			var settingsPath = settingKey;
			var rootSettingId = 'form-demo-model-' + modelIndex + '-setting-' + settingsPath;

			var rootSettingsValue = getObjectPropertybyString(demo.demoState.modelList[modelIndex].settings, settingsPath);


			// Add the field to the group
			var $field = $('<div><label for="' + rootSettingId + '">' + settingKey + ': </label></div>').appendTo($inputGroup);


			function createTextInputForSetting(inputId, settingsPath, /*optional*/inputCb, /*optional*/updateCb) {

				var settingsValue = getObjectPropertybyString(demo.demoState.modelList[modelIndex].settings, settingsPath);

				var $input = null;
				if(typeof(settingsValue) === 'number') {
					$input = $('<input type="number" id="' + inputId + '" class="demo-input-number" value="' + settingsValue + '">').appendTo($field);
				}
				else if(typeof(settingsValue) === 'boolean') {
					$input = $('<input id="' + inputId + '" class="demo-input-checkbox" type="checkbox">').appendTo($field);
				
					inputCb = inputCb || function(e) {
						return $(e.target).is(":checked");
					};
					updateCb = updateCb || function() {
						$input.attr('checked', getObjectPropertybyString(demo.demoState.modelList[modelIndex].settings, settingsPath));
					};
				}

				inputCb = inputCb || function(e) {
					return parseFloat($(e.target).val());
				};
				updateCb = updateCb || function() {
					$input.attr('value', getObjectPropertybyString(demo.demoState.modelList[modelIndex].settings, settingsPath));
				};



				$input.on('input propertychange change', function(e) {
					updateObjectPropertyByString(demo.demoState.modelList[modelIndex].settings, settingsPath, inputCb(e));

					demo.update();
				});

				// Now when something changes from some other input, update this box appropriately
				demoInputUpdateCallbackList.push(function() {
					updateCb();
				});
			}

			// Add the input based on setting type to the field
			if(typeof(rootSettingsValue) === 'object' && typeof(rootSettingsValue.x) === 'number' && typeof(rootSettingsValue.y) === 'number') {
				createTextInputForSetting(rootSettingId, settingsPath + '.x');
				createTextInputForSetting(rootSettingId + '-y', settingsPath + '.y');
			}
			// Otherwise we can probably handle it (number or boolean)
			else {
				createTextInputForSetting(rootSettingId, settingsPath);
			}
		});
	});


	
	// Update the manual text inputs whenever the demo updates
	$(demo).on('demo-update', function() {
		demoInputUpdateCallbackList.forEach(function(cb) {
			cb();
		});
	});
}





















// More generic drawing functions

function drawPointAt(pos, size, context) {
	context.save();
	context.rect(pos.x - (size/2), pos.y - (size/2), size, size);
	context.fill();
	context.restore();
}


// Doesn't actually do any of the "dragging". Handle the event/result object in the `cb` yourself and return the updated point
function createDraggablePoint(offset, startingPos, size, el, cb) {

	var currentPos = new Vector2(startingPos);
	var dragStartPos = new Vector2(0, 0);
	var lastMousePosWhileDragging = new Vector2(0, 0);

	var correctMousePos = function(mousePos) {
		// The canvas could be scaled down/up
		// And we want true pixel position over image/element
		var elementScale = new Vector2(
			(el.width || el.clientWidth) / el.clientWidth,
			(el.height || el.clientHeight) / el.clientHeight
		);

		var correctedMousePos = mousePos.multiply(elementScale);
		return correctedMousePos.subtract(offset);
	};

	var isDragging = false;
	var mouseDownHandler = function(e) {
		e = normalizeEvent(e);
		var mousePos = new Vector2(e.offsetX, e.offsetY);
		var correctedMousePos = correctMousePos(mousePos);
		
		if(isWithinBounds(correctedMousePos)) {
			isDragging = true;
			dragStartPos = correctedMousePos;
			lastMousePosWhileDragging = correctedMousePos;

			// Only call if we clicked on the point we are targeting
			if(cb) {
				cb({
					pos: correctedMousePos,
					isHovering: false,
					isDragging: isDragging,
					deltaDrag: new Vector2(0, 0),
					dragStartPos: dragStartPos,
					isMouseDown: false,
					isMouseUp:  false
				});
			}
		}
	};
	var touchStartHandler = function(e) {
		[].slice.call(e.originalEvent.changedTouches).forEach(function(touchListItem) {
			mouseDownHandler(touchListItem);
		});
	};
	$(el).on('mousedown', mouseDownHandler);
	$(el).on('touchstart', touchStartHandler);

	var mouseMoveHandler = function(e) {
		e = normalizeEvent(e);
		var mousePos = new Vector2(e.offsetX, e.offsetY);
		var correctedMousePos = correctMousePos(mousePos);

		var isHovering = false;
		if(isWithinBounds(correctedMousePos)) {
			isHovering = true;
		}

		if(cb) {
			var newCurrentPos = cb({
				pos: correctedMousePos,
				isHovering: isHovering,
				isDragging: isDragging,
				deltaDrag: isDragging ? correctedMousePos.subtract(lastMousePosWhileDragging) : new Vector2(0, 0),
				dragStartPos: dragStartPos,
				isMouseDown: false,
				isMouseUp:  false
			});
			
			currentPos = new Vector2(newCurrentPos);
		}

		if(isDragging) {
			lastMousePosWhileDragging = correctedMousePos;
		}
	};
	var touchMovehandler = function(e) {
		[].slice.call(e.originalEvent.changedTouches).forEach(function(touchListItem) {
			mouseMoveHandler(touchListItem);
		});
	};
	$(el).on('mousemove', mouseMoveHandler);
	$(el).on('touchmove', touchMovehandler);

	var mouseUpHandler = function(e) {
		e = normalizeEvent(e);
		var mousePos = new Vector2(e.offsetX, e.offsetY);
		var correctedMousePos = correctMousePos(mousePos);
		
		var wasDragging = isDragging;
		isDragging = false;

		var isHovering = false;
		if(isWithinBounds(correctedMousePos)) {
			isHovering = true;
		}
		
		if(cb && wasDragging) {
			cb({
				pos: correctedMousePos,
				isHovering: isHovering,
				isDragging: isDragging,
				deltaDrag: new Vector2(0, 0),
				dragStartPos: dragStartPos,
				isMouseDown: false,
				isMouseUp:  true
			});
		}
	};
	var touchEndHandler = function(e) {
		[].slice.call(e.originalEvent.changedTouches).forEach(function(touchListItem) {
			mouseUpHandler(touchListItem);
		});
	};
	$(document).on('mouseup', mouseUpHandler);
	$(document).on('touchend', touchEndHandler);


	function isWithinBounds(pos) {
		if(pos.x > currentPos.x-(size/2) && pos.x < currentPos.x+(size/2) && pos.y > currentPos.y-(size/2) && pos.y < currentPos.y+(size/2)) {
			return true;
		}

		return false;
	}
	
		
	// Return a function that cleans up the bindings if they want to
	return function() {    
		$(el).off('mousedown', mouseDownHandler);
		$(el).on('touchstart', touchStartHandler);
		
		$(el).off('mousemove', mouseMoveHandler);
		$(el).on('touchmove', touchMovehandler);

		$(document).off('mouseup', mouseUpHandler);
		$(document).on('touchend', touchEndHandler);
	};
}


function normalizeEvent(event) {
	if(!event.offsetX) {
		event.offsetX = (event.pageX - $(event.target).offset().left);
		event.offsetY = (event.pageY - $(event.target).offset().top);
	}

	return event;
}

