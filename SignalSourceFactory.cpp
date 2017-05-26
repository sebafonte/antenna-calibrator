
class SignalSourceFactory {
	// Factory methods
	static RtlSource * CreateDefault(double frequency, int bandwidth, int deltaTime) {
		RtlSource * source = new RtlSource(frequency, bandwidth, deltaTime);
		source->OpenDefault();
		return source;
	}
	
	static RtlSource * CreateBySerial(const char * serialName, double frequency, int bandwidth, int deltaTime) {
		RtlSource * source = new RtlSource(frequency, bandwidth, deltaTime);
		source->OpenBySerial(serialName);
		return source;
	}
};

