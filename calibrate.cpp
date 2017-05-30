
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/time.h>
#include <rtl-sdr.h>

const float MINIMUM_QUALITY = -999.0;

#include "AntennaMover.cpp"
#include "AbstractSource.cpp"
#include "AirodumpSource.cpp"
#include "RtlSource.cpp"
#include "SignalSourceFactory.cpp"
#include "BasicCalibrator.cpp"
#include "TestCalibrator.cpp"

#define SOURCE_RTL 		1
#define SOURCE_AIRODUMP 	2
#define MAX_TIMEOUT		5000
#define MAX_NAMES_BUFFER_SIZE	256


void showUsage(void)
{
	fprintf(stderr,
		"Help:\nantenna-calibrator, a simple calibrator for antennas for rtl-sdr an airodump.\n\n"
		"There are two usages for this command:\n"
		"Use for rtl-sdr:\n\tantenna-calibrate -s rtl -d 200 -b 12000 -f 100000000 -p /dev/ttyUSB0\n"
		"\t-s source: rtl or airodump\n"
		"\t-d delayMilliseconds\n"
		"\t-a moving angle delta\n"
		"\t-n rtl-sdr device serial number\n"
		"\t-b bandwidth\n"
		"\t-f frequency\n"
		"\t-p antennaPort\n\t]\n"
		"\n"
		"Use for airodump-ng:\n\tantenna-calibrate -s airodump -d 5000 -w Speedy-Fibra -m mon0 -p dev/ttyUSB0\n"
		"\t-s source: rtl or airodump\n"
		"\t-d delay to take signal media quality\n"
		"\t-a moving angle delta\n"
		"\t-w Wlan to maximize power reception\n"
		"\t-m Wlan monitor device\n"
		"\t-p antenna hardware port (chech pungas.space/blog/antenna-controller.html)\n\n\n");
	exit(1);
}

int SourceMode() {
	return SOURCE_AIRODUMP;
}


int main(int argc, char **argv) {
	int sourceMode = -1;	
	AbstractSource *source = NULL;
	int c;
	double frequency = -1.0;
	int bandwidth = -1;
	int deltaTime = -1;
	int deltaAngle = -1;
	char wlanDevice[MAX_NAMES_BUFFER_SIZE] = "";
	char wlanName[MAX_NAMES_BUFFER_SIZE] = "";
	char antennaPort[MAX_NAMES_BUFFER_SIZE] = "";
	
	// Check input parameters
	while ((c = getopt (argc, argv, "s:d:b:f:w:m:p:a:")) != -1) {
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
		case 'p':
			strcpy(antennaPort, optarg);
			break;
		case 'a':
			deltaAngle = atoi(optarg);
			break;
		}	
	}

	// Check basic parameters
	if (sourceMode < 0) {
		printf("No source specified.\n");
		showUsage();
		exit(1);
	}
	if (!strcmp(antennaPort, "")) {
		printf("No antenna port specified.\n");		
		showUsage();
		exit(1);
	}
	if (deltaAngle < 0) {
		printf("No delta angle specified.\n");
		showUsage();
		exit(1);
	}

	printf("Source mode: %d.\n", sourceMode);
	printf("Antenna rotator port: %s.\n", antennaPort);

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
	BasicCalibrator *calibrator = new BasicCalibrator((char *) "/dev/ttyUSB0", source);
	
	// Calibrate antenna
	calibrator->Calibrate(deltaAngle);

	// Print report
	int currentAngle = calibrator->GetCurrentAngle();
	float lastQualityValue = calibrator->GetLastQualityValue();
	float qualityValue = calibrator->GetQualityValue();
	printf("\nSetted at: %d° - Last: %f - Current: %f\n\n", currentAngle, lastQualityValue, qualityValue);
}

