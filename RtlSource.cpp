const int LENGTH_MANUFACTURER = 256;
const int LENGTH_PRODUCT = 256;
const int LENGTH_SERIAL = 256;
const int SAMPLE_RATE = 250000;
const int MAX_SECONDS = 8;
//const int READ_BUFFER_LENGTH = SAMPLE_RATE * MAX_SECONDS;

#define DEFAULT_SAMPLE_RATE		2048000
#define DEFAULT_BUF_LENGTH		(16 * 16384)
#define MINIMAL_BUF_LENGTH		512
#define MAXIMAL_BUF_LENGTH		(256 * 16384)

const int READ_BUFFER_LENGTH = MAXIMAL_BUF_LENGTH;

#define MHZ(x)	((x)*1000*1000)

#define PPM_DURATION			10
#define PPM_DUMP_TIME			5



class RtlSource : public AbstractSource {
	rtlsdr_dev_t  *Device;
	char manufacturer[LENGTH_MANUFACTURER], product[LENGTH_PRODUCT], serial[LENGTH_SERIAL];
	
public:
	double Frequency;
	int Bandwidth, DeltaTime;

	
	// Object methods
	RtlSource(double frequency, int bandwidth, int deltaTime) {
		Frequency = frequency;
		Bandwidth = bandwidth;
		DeltaTime = deltaTime;
	}

	~RtlSource() {
		CloseRtl();
	}
	
	void OpenDefault() {
		Open(0);
	}
	
	void OpenBySerial(const char *serial) {
		int index = rtlsdr_get_index_by_serial(serial);
		
		if (index >= 0) {
			Open(index);
		}
		else {
			// #TODO: show error
			Device = NULL;
		}
	}
	
	virtual float ReadMeanAmplitude(int frequency, int bandwith, int deltaTime) {
		char buffer[READ_BUFFER_LENGTH];
		int len=0, sum=0;
		int result = rtlsdr_read_sync(Device, &buffer, READ_BUFFER_LENGTH, &len);

		// Calculate mean amplitude
		for (int i=0; i< len; i++) {
			sum += buffer[i];
		}

		int mean = sum / READ_BUFFER_LENGTH;
		printf("Return: %d - Readed: %d - Mean: %d\n", result, len, mean);

		return mean;
	}

	void CloseRtl() {	
		if (Device)
			rtlsdr_close(Device);
	}

	virtual void Initialize() {
		printf("Center frequency: %f\n", (float) Frequency);

		rtlsdr_set_center_freq(Device, Frequency);		
		rtlsdr_read_sync(Device, NULL, READ_BUFFER_LENGTH, NULL);
		usleep(5000);
	}
	
protected:
	void Open(int index) {
		int result = rtlsdr_get_device_usb_strings(index, manufacturer, product, serial);
		printf("rtlsdr_get_device_usb_string: %d\n", result);
		result = rtlsdr_open(&Device, index);
		printf("rtlsdr_open: %d\n", result);

		result = rtlsdr_set_sample_rate(Device, DEFAULT_SAMPLE_RATE);
		printf("rtlsdr_set_sample_rate: %d\n", result);

		result = rtlsdr_set_tuner_gain_mode(Device, 1);
		printf("rtlsdr_set_tuner_gain_mode: %d\n", result);
		
		result = rtlsdr_set_tuner_gain(Device, 0);
		printf("rtlsdr_set_tuner_gain: %d\n", result);

		result = rtlsdr_set_agc_mode(Device, 1);
		printf("rtlsdr_set_agc_mode: %d\n", result);

		result = rtlsdr_reset_buffer(Device);
		printf("rtlsdr_reset_buffer: %d\n", result);


		// #TODO: refactor
		printf("Manufacturer: %s\n", manufacturer);
		printf("Product: %s\n", product);
		printf("Serial: %s\n\n", serial);
	}
};
