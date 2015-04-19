
#include "SVGCurveLibTests.h"


// Keeps track since we last sent a serial in our debug serial
elapsedMicros sinceSerial;


void setup() {
	Serial.begin(9600);

}

int aliveCount = 0;
void loop() {
	

	if (sinceSerial >= 5 * 1000000) {
		
		Serial.print("Alive: " + String(aliveCount) + "\n");

		SVGCurveLibTests::RunTests([&](PlatformString message) {
			Serial.print(message.value);
		});

		aliveCount++;
		sinceSerial = 0;
	}
}




