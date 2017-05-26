/* AbsoluteCalibrator calibrates giving a full round to the antenna and getting the maximum mean amplitude in the given deltaTime */
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
	
	void SetInitialPosition() {
		Mover->MoveZero();
	}

	void Calibrate(int maxTime, int *returnMaxAngle, float *returnMaxValue) {
		Mover->Move(0);
		usleep(1000);
		Mover->Move(90);
	}
};
