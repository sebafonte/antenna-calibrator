
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
	// Source Spec: serial, default, index
	// Serial port for antenna mover
	// Mode: mean, filter:FILTERSPEC

	int sourceMode = -1;	
	AbstractSource *source = NULL;
	int c;

	// Vemos source de datos para determinar otros parametros
	double frequency = -1.0;
	int bandwidth = -1;
	int deltaTime = -1;
	char wlanDevice[256] = "";
	char wlanName[256] = "";

	
	while ((c = getopt (argc, argv, "s:d:b:f:w:m:")) != -1) {
		switch (c) {
		case 's':
			if (!strcmp(optarg, "rtl"))
				sourceMode = SOURCE_RTL;
			else if (!strcmp(optarg, "airodump"))
				sourceMode = SOURCE_AIRODUMP;
			else {
				printf("No source input specified (rtl - airodump).\n");
				exit(1);
			}
			break;
		case 'd':
			deltaTime = atoi(optarg);
			break;
		case 'b':
			bandwidth = atoi(optarg);
			break;
		case 'f':
			frequency = atof(optarg);
			break;
		case 'w':
			strcpy(wlanName, optarg);
			break;
		case 'm':
			strcpy(wlanDevice, optarg);
			break;
		}	
	}

	printf("Source mode: %d.\n", sourceMode);

	// Instanciate source
	if (sourceMode == SOURCE_RTL) {
		// Check parameters
		if (deltaTime < 0) {
			printf("Invalid deltaTime %d.\n", deltaTime);
			exit(1);
		}
		else
			printf("DeltaTime: %d\n", deltaTime);

		if (bandwidth < 0) {
			printf("Invalid bandwidth: %d.\n", bandwidth);
			exit(1);
		}
		else
			printf("Bandwidth: %d\n", bandwidth);

		if (frequency < 0) {
			printf("Invalid frequency: %e.\n", frequency);
			exit(1);
		}
		else
			printf("Frequency: %e\n", frequency);

		// Check unusefull parameters
		if (strcmp(wlanName, "")) {
			printf("wlanName not necessary for RTL mode.\n");
			exit(1);
		}

		if (strcmp(wlanDevice, "")) {
			printf("wlanDevice not necessary for RTL mode.\n");
			exit(1);
		}

		printf("\n");
		source = SignalSourceFactory::CreateRtlDefault(frequency, bandwidth, deltaTime);
	}
	else if (sourceMode == SOURCE_AIRODUMP) {
		// Check parameters
		if (deltaTime < 0) {
			printf("Invalid deltaTime %d.\n", deltaTime);
			exit(1);
		}
		else
			printf("DeltaTime: %d\n", deltaTime);

		if (!strcmp(wlanName, "")) {
			printf("Invalid wlanName: %s.\n", wlanName);
			exit(1);
		}
		else
			printf("wlanName: %s\n", wlanName);

		if (!strcmp(wlanDevice, "")) {
			printf("Invalid wlanDevice: %s.\n", wlanDevice);
			exit(1);
		}
		else
			printf("wlanDevice: %s\n", wlanDevice);

		// Check unusefull parameters
		if (bandwidth > 0) {
			printf("bandwidth not necessary for Airodump mode.\n");
			exit(1);
		}

		if (frequency > 0) {
			printf("frequency not necessary for RTL mode.\n");
			exit(1);
		}

		printf("\n");
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

