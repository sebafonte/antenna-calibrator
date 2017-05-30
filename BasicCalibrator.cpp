const float MINIMUM_QUALITY = -999.0;


/* BasicCalibrator calibrates giving a full round to the antenna and getting the maximum mean amplitude in the given deltaTime */
class BasicCalibrator {
public:
	BasicCalibrator(char * port, AbstractSource * source) {
		Mover = new AntennaMover(port);
		Source = source;
	}

	void Calibrate(int maxTime, int *returnMaxAngle, float *returnMaxValue) {
		float positions[360];
		float maxValue = MINIMUM_QUALITY;
		int maxAngle = 0;
		int delta = 27.0;
		int lastPosition = 0;

		// Set center frequency and discard data
		Source->Initialize();
		
		for (int i=0; i< 360; i+=delta) {
			// Move
			Mover->Move(i);
			// Read signal strength
			float value = Source->ReadMeanAmplitude();
			printf("Angle: %f - Amplitude: %f\n", (float) i, value);
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
		float value = Source->ReadMeanAmplitude();
		printf("\nMoved to: %f - Last: %f - Current: %f\n", (float) maxAngle, maxValue, value);
		Mover->Move(maxAngle);

		// Save last quality value
		LastQualityValue = value;
	}

	float GetCurrentAngle() {
		return Mover->GetCurrentAngle();
	}

	float GetLastQualityValue() {
		return LastQualityValue;
	}

protected:
	AntennaMover *Mover;
	AbstractSource *Source;
	float LastQualityValue;
};
