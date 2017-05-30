
/* TestCalibrator just tests antenna movement with some fixed positions */

class TestCalibrator {
public:
	double Frequency;
	int Bandwidth;
	int DeltaTime;
	AntennaMover *Mover;
	RtlSource *Source;


	TestCalibrator(double frequency, int bandwidth, int deltaTime, char * port) {
		Frequency = frequency;
		Bandwidth = bandwidth;
		DeltaTime = deltaTime;	
		Mover = new AntennaMover(port);
		Source = SignalSourceFactory::CreateRtlDefault(frequency, bandwidth, deltaTime);
	}

	void Calibrate(int deltaAngle) {
		for (int i=0; i< 10; i++) {
			Mover->Move(i * 90);
			usleep(1000);
		}
	}
};
