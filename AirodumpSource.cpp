#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

#define WAIT_TIME_BEACONS_GUARD 	1000
#define AIRODUMP_RESULT_BUFFER_SIZE	256
#define COMMAND_BUFFER_SIZE		2048


std::string exec2(std::string cmd, int waitMilliseconds) {
	std::string data;
	FILE * stream;
	char buffer[AIRODUMP_RESULT_BUFFER_SIZE];

	cmd.append(" 2>&1");
	stream = popen(cmd.c_str(), "r");
	
	// Print command
	printf("Command: %s\n", cmd.c_str());
 
  	// Wait until beacons received
	usleep(waitMilliseconds + WAIT_TIME_BEACONS_GUARD);

	// Read airodump result
	if (stream) {
		while (!feof(stream)) 
			if (fgets(buffer, AIRODUMP_RESULT_BUFFER_SIZE, stream) != NULL) {
				printf("Part: %s\n", buffer);			
				data.append(buffer);
			}

		printf("Result: %s\n", data.c_str());		
		pclose(stream);
	}

	return data;
}


class AirodumpSource : public AbstractSource {
	char * WlanDevice, * WlanName;
	int Delay;
	
public:	
	// Object methods
	AirodumpSource(char * wlanDevice, char * wlanName, int delay) {
		WlanDevice = wlanDevice;
		WlanName = wlanName;
		Delay = delay;
	}

	virtual float ReadMeanAmplitude() {
		float value = MINIMUM_QUALITY;
		char buffer[COMMAND_BUFFER_SIZE];

		sprintf(buffer, "timeout -k %e %e airodump-ng %s 2>tempfile.txt ; awk \'START { power = -999 } { if (($12==\"%s\") || ($11==\"%s\") || ($10==\"%s\")) power = $2 } END { print power }\' tempfile.txt", 
			Delay / 1000.0, Delay / 1000.0, WlanDevice, WlanName, WlanName, WlanName);

		// Execute airodump, waiting some delay and get dbs for SSID
		std::string stringValue = exec2(buffer, Delay);
		const char *charValue = stringValue.c_str();
		
		// If power report has been found, use it
		if (strlen(charValue) > 1)
			value = atof(charValue);
		
		return value;
	}

	virtual void Initialize() {
	/*	// Initialize WlanDevice in monitor mode using airmon-ng start
		char buffer[256];
		sprintf(buffer, "airmon-ng start %s", WlanDevice);
		std::string stringValue = exec(buffer, WAIT_TIME_AIRMON_START);	*/
	}	
};
