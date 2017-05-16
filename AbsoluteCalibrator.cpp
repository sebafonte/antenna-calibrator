/* AbsoluteCalibrator calibrates giving a full round to the antenna and getting the maximum mean amplitude in the given deltaTime */
class AbsoluteCalibrator {
public:
	double Frequency;
	int Bandwidth;
	int DeltaTime;
	AntennaMover *Mover;
	RtlSource *Source;


	AbsoluteCalibrator(double frequency, int bandwidth, int deltaTime, char * port) {
		Frequency = frequency;
		Bandwidth = bandwidth;
		DeltaTime = deltaTime;	
		Mover = new AntennaMover(port);
		Source = RtlSource::CreateDefault();
	}
	
	void SetInitialPosition() {
		Mover->MoveZero();
	}

	void Calibrate(int maxTime, int *returnMaxAngle, float *returnMaxValue) {
		float positions[360];
		float maxValue = -999.0;
		int maxAngle = 0;
		int delta = 9.0;
		int lastPosition=0;

		// Set center frequency and discard data
		Source->SetFrequency(Frequency);

		// Set initial position
		SetInitialPosition();
		
		for (int i=0; i< 360; i+=delta) {
			// Read signal strength
			float value = Source->ReadMeanAmplitude(Frequency, Bandwidth, DeltaTime);

			// Save strength in a buffer
			positions[i] = value;

			// Check maximum
			if (value > maxValue) {
				maxValue = value;
				maxAngle = i;
			}

			lastPosition++;
		}

		// Move to the position with maximum amplitude
		Mover->Move(maxAngle);
		
		*returnMaxAngle = maxAngle;
		*returnMaxValue = maxValue;
	}
};
