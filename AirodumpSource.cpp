#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

const int WAIT_TIME = 8000;
/*
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}*/

class AirodumpSource : public AbstractSource {
	char * WlanDevice, * WlanName;
	
public:	
	// Object methods
	AirodumpSource(char * wlanDevice, char * wlanName) {
		WlanDevice = wlanDevice;
		WlanName = wlanName;
	}

	virtual float ReadMeanAmplitude() {
		int value = -999;

		// Execute airodump
		// Wait some time (WAIT_TIME)
		// Get dbs for SSID
		
		return value;
	}

	virtual void Initialize() {
		
	}	
};
