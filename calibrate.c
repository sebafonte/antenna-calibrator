
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <rtl-sdr.h>


int main(int argc, char **argv) {
	// Check parameters
	
	// Instanciate source

	// Calibrate antenna
	
}

/* AbsoluteCalibrator calibrates giving a full round to the antenna and getting the maximum mean amplitude in the given deltaTime */
public class AbsoluteCalibrator {
public:
	public AbsoluteCalibrator(char * device, int frequency, int bandwith, int deltaTime, int time);

	bool Calibrate(int maxTime) {
		// Set initial position
		
		// Read signal strength
		
		// 
	}
}

public class RtlSource {
public:
	float ReadMeanAmplitude(int frequency, int bandwith, int deltaTime) {
		
	}
}