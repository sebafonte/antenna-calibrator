/* AbsoluteCalibrator calibrates giving a full round to the antenna and getting the maximum mean amplitude in the given deltaTime */
class AbsoluteCalibrator {
public:
	AntennaMover *Mover;
	AbstractSource *Source;


	AbsoluteCalibrator(char * port, AbstractSource * source) {
		Mover = new AntennaMover(port);
		Source = source;
	}

	void SetInitialPosition() {
		Mover->MoveZero();
	}

	void Calibrate(int maxTime, int *returnMaxAngle, float *returnMaxValue) {
		float positions[360];
		float maxValue = -999.0;
		int maxAngle = 0;
		int delta = 27.0;
		int lastPosition=0;

		// Set center frequency and discard data
		Source->Initialize();

		// Set initial position
		//SetInitialPosition();
		
		for (int i=0; i< 360; i+=delta) {
			// Move
			Mover->Move(i);
			// Read signal strength
			float value = Source->ReadMeanAmplitude();
			printf("Amplitude: %f\n", value);
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
