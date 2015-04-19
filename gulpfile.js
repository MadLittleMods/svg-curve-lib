var gulp = require('gulp');

var runSequence = require('run-sequence');
var del = require('del');



// Since Arduino, requires libraries/external files to be within the same directory,
// we have to move them
gulp.task('move-lib-files', function() {
	return gulp.src(['./src/c++/**/*'])
		.pipe(gulp.dest('./test/c++/arduino_testbench'));
});

gulp.task('move-test-lib-files', function() {
	return gulp.src(['./test/c++/cpp-test-lib/**/*'])
		.pipe(gulp.dest('./test/c++/arduino_testbench'));
});


// Default Task
gulp.task('default', function(callback) {
	runSequence(
		['move-lib-files', 'move-test-lib-files'],
		callback
	);
});