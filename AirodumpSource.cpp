#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

#define WAIT_TIME_AIRMON_START 2000


std::string exec2(std::string cmd, int waitMilliseconds) {
	std::string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmd.append(" 2>&1");

	stream = popen(cmd.c_str(), "r");

  	// Wait until beacons received
	usleep(waitMilliseconds + 1000);

	if (stream) {
		while (!feof(stream)) 
			if (fgets(buffer, max_buffer, stream) != NULL)
				data.append(buffer);
		
		
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
		float value = -999;
		static char buffer[2048];

		sprintf(buffer, "timeout -k %e %e airodump-ng %s 2>tempfile.txt ; awk \'START { power = -999 } { if (($12==\"%s\") || ($11==\"%s\") || ($10==\"%s\")) power = $2 } END { print power }\' file.txt", Delay / 1000.0, Delay / 1000.0, WlanDevice, WlanName, WlanName, WlanName);

		// Execute airodump, waiting some delay and get dbs for SSID
		std::string stringValue = exec2(buffer, Delay);
		const char *charValue = stringValue.c_str();
		
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
