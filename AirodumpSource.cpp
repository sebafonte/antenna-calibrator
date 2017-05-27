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
	
	// Read output
	int value = !feof(pipe);
   	printf("VALUE FEOF %d\n", value);
        while (value) {
  	    // Wait until beacons received
	    //usleep(waitMilliseconds + 1000);

            if (fgets(buffer, 256, pipe) != NULL) {
                result += buffer;
		printf("RECEIVED: %s\n", buffer);
	    }
	    value = !feof(pipe);
	    printf("VALUE FEOF %d\n", value);
        }
    } catch (...) {
	printf("ERROR CATCH\n");
        pclose(pipe);
        throw;
    }
    printf("SALE CATCH\n");
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

		sprintf(buffer, "awk \'{ if (($12==\"%s\") || ($11==\"%s\") || ($10==\"%s\")) power = $2 } END {print power}\' | timeout %f  airodump-ng %s", WlanName, WlanName, WlanName, Delay / 1000.0, WlanDevice);
		printf("Command: %s\n", buffer);

		// Execute airodump, waiting some delay and get dbs for SSID
		std::string stringValue = exec(buffer, Delay);
		const char *charValue = stringValue.c_str();
		printf("RETURNED VALUE: %s\n", charValue);
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
