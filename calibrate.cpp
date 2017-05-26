
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <rtl-sdr.h>

#include "AntennaMover.cpp"

#include "AbstractSource.cpp"
#include "AirodumpSource.cpp"
#include "RtlSource.cpp"
#include "SignalSourceFactory.cpp"

#include "AbsoluteCalibrator.cpp"
#include "TestCalibrator.cpp"


#define SOURCE_RTL 		1
#define SOURCE_AIRODUMP 	2


int SourceMode() {
	return SOURCE_AIRODUMP;
}


int main(int argc, char **argv) {
	// Check parameters
		// Frequency, BW, DeltaTime for RtlSource
		// DeltaTime, SSID name, interface name for AirodumpSource
		// Source Spec: serial, default, index
		// Source Type: rtl, usrp
		// Serial port for antenna mover
		// Mode: mean, filter:FILTERSPEC
	int sourceMode = SOURCE_AIRODUMP;	
	AbstractSource *source = NULL;

	// Instanciate source
	if (sourceMode == SOURCE_RTL) {
		double frequency = 100.0;
		int bandwidth = 12000;
		int deltaTime = 2000;
		source = SignalSourceFactory::CreateRtlDefault(frequency, bandwidth, deltaTime);
	}
	else if (sourceMode == SOURCE_AIRODUMP) {
		char *wlanDevice = (char *) "wlan0mon";
		char *wlanName = (char *) "Speedy-Fibra";
		int deltaTime = 5000;
		source = SignalSourceFactory::CreateAirodumpByName(wlanDevice, wlanName, deltaTime);	
	}

	// Instanciate calibrator
	AbsoluteCalibrator *calibrator = new AbsoluteCalibrator((char *) "/dev/ttyUSB0", source);
	//TestCalibrator *calibrator = new TestCalibrator(frequency, bandwidth, deltaTime, (char *) "/dev/ttyUSB0");
	
	// Calibrate antenna
	float maxValue; 
	int maxAngle;
	calibrator->Calibrate(5000, &maxAngle, &maxValue);

	// Print report
	
}

