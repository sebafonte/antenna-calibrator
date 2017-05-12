/* AbsoluteCalibrator calibrates giving a full round to the antenna and getting the maximum mean amplitude in the given deltaTime */
public class AbsoluteCalibrator {
public:
	public AbsoluteCalibrator(char * device, int frequency, int bandwith, int deltaTime, int time);

	bool Calibrate(int maxTime) {
		// Set initial position
		SetInitialPosition();
		
		for (int i=0; i< 360; i++) {
			// Read signal strength
			// Save strength in a buffer
			
		}
		
		// Move to the position with maximum amplitude
		AntennaMover.move();
	}
}
