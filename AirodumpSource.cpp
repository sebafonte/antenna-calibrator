#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

#define WAIT_TIME_AIRMON_START 2000


std::string exec(const char* cmd, int waitMilliseconds) {
    char buffer[256];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");

    if (!pipe) throw std::runtime_error("popen() failed!");

    try {
	// Wait until beacons received
	usleep(waitMilliseconds);
	// Read output
        while (!feof(pipe)) {
            if (fgets(buffer, 256, pipe) != NULL) {
                result += buffer;
		printf("RECEIVED: %s\n", buffer);
	    }
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
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
		char buffer[256];

		sprintf(buffer, "awk \'{ if (($12==\"%s\") || ($11==\"%s\") || ($10==\"%s\")) power = $2 } END {print power}\' | airodump-ng %s", WlanName, WlanName, WlanName, WlanDevice);
		
		printf("Command: %s\n", buffer);

		// Execute airodump, waiting some delay and get dbs for SSID
		std::string stringValue = exec(buffer, Delay);
		const char *charValue = stringValue.c_str();		
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
