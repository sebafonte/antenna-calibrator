
class SignalSourceFactory {
public:
	// Factory methods
	static RtlSource * CreateRtlDefault(double frequency, int bandwidth, int deltaTime) {
		RtlSource * source = new RtlSource();
		source->InitializeRtlSource(frequency, bandwidth, deltaTime);
		source->OpenDefault();
		return source;
	}
	
	static RtlSource * CreateRtlBySerial(const char * serialName, double frequency, int bandwidth, int deltaTime) {
		RtlSource * source = new RtlSource();
		source->InitializeRtlSource(frequency, bandwidth, deltaTime);
		source->OpenBySerial(serialName);
		return source;
	}

	static AirodumpSource * CreateAirodumpByName(char * wlanDevice, char * wlanName, int delay) {
		AirodumpSource * value = new AirodumpSource();
		value->InitializeAirodumpSource(wlanDevice, wlanName, delay);
		return value;
	}	
};

