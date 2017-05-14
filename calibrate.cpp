
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <rtl-sdr.h>

#include "AntennaMover.cpp"
#include "RtlSource.cpp"
#include "AbsoluteCalibrator.cpp"


int main(int argc, char **argv) {
	int frequency=400000, bandwidth=1200, deltaTime=1000;

	// Check parameters
		// Frequency
		// BW
		// deltaTime
		// Source Spec: serial, default, index
		// Source Type: rtl, usrp
		// Serial port for antenna mover
		// Mode: mean, filter:FILTERSPEC

	// Instanciate source
	AbsoluteCalibrator *calibrator = new AbsoluteCalibrator(frequency, bandwidth, deltaTime, (char *) "/dev/ttyUSB0");
	
	// Calibrate antenna
	float maxValue; 
	int maxAngle;
	calibrator->Calibrate(5000, &maxAngle, &maxValue);

	// Print report
	
}

