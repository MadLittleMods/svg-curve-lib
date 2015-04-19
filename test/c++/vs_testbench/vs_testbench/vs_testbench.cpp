#pragma comment (lib, "libuv.lib")
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "psapi.lib")
#pragma comment (lib, "Iphlpapi.lib")

//#include "stdafx.h"
//#include <iostream>


#include <iostream>
//#include <stdio.h>
#include <string.h>
//#include <unistd.h>
//#include <io.h>
#include <uv.h>

#include <sstream>
#include <regex>


#include "SVGCurveLibTests.h"



std::string stripAnsiEscapeCodes(std::string input) {
	// from: https://github.com/sindresorhus/ansi-regex/blob/47fb974630af70998157b30fad6eb5e5bd7c7cd6/index.js
	std::regex reg("(?:(?:\033\\[)|\233)(?:(?:[0-9]{1,3})?(?:(?:;[0-9]{0,3})*)?[A-M|f-m])|\033[A-M]");
	std::string output = std::regex_replace(input, reg, "");

	return output;
}

void ccout(std::string message, uv_write_t* req, uv_stream_t* handle) {
	// If the terminal doesn't support ANSI, strip it for clarity
	if (uv_guess_handle(1) != UV_TTY) {
		message = stripAnsiEscapeCodes(message);
	}

	uv_buf_t buf;
	buf.base = (char*)message.c_str();
	buf.len = strlen(buf.base);
	uv_write(req, handle, &buf, 1, NULL);
};




uv_loop_t *loop;
uv_tty_t tty;
int main()
{
	loop = uv_default_loop();

	uv_tty_init(loop, &tty, 1, 0);
	uv_tty_set_mode(&tty, UV_TTY_MODE_NORMAL);

	//ccout("\033[41;37mHello TTY\n\033[0m", &uv_write_t(), (uv_stream_t*)&tty);
	//std::cout << "\033[31masdf\033[0m" << std::endl;


	// Redirect/Capture cout
	std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
	std::ostringstream strCout;
	std::cout.rdbuf(strCout.rdbuf());

	// Run the tests
	SVGCurveLibTests::RunTests([&](PlatformString message) {
		std::cout << message.value;// << std::endl;
	});



	// You could call ccout directly but we instead redirect, capture, and handle it at the end here
	// This way libraries/other-code that doesn't know about this can just std::cout and we got it
	ccout(strCout.str(), &uv_write_t(), (uv_stream_t*)&tty);
	


	uv_tty_reset_mode();
	return uv_run(loop, UV_RUN_DEFAULT);

	return 0;
}

