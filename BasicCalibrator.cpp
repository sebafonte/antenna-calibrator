
/* BasicCalibrator calibrates giving a full round to the antenna and getting the maximum mean amplitude in the given deltaTime */

class BasicCalibrator {
public:
	BasicCalibrator(char * port, AbstractSource * source) {
		Mover = new AntennaMover(port);
		Source = source;
	}

	void Calibrate(int deltaAngle) {
		float positions[360];
		float maxValue = MINIMUM_QUALITY;
		int maxAngle = 0;
		int lastPosition = 0;
		float value;

		// Set center frequency and discard data
		Source->Initialize();

		for (int i=0; i< 360; i+=deltaAngle) {
			// Move
			Mover->Move(i);
			// Read signal strength
			value = GetQualityValue();
			printf("Angle: %d - Amplitude: %f\n", i, value);
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
		value = GetQualityValue();
		Mover->Move(maxAngle);

		// Save last quality value
		LastQualityValue = value;
	}

	int GetCurrentAngle() {
		return Mover->GetCurrentAngle();
	}

	float GetLastQualityValue() {
		return LastQualityValue;
	}

	float GetQualityValue() {
		return Source->ReadMeanAmplitude();
	}

protected:
	AntennaMover *Mover;
	AbstractSource *Source;
	float LastQualityValue;
};
