const int LENGTH_MANUFACTURER = 256;
const int LENGTH_PRODUCT = 256;
const int LENGTH_SERIAL = 256;
const int SAMPLE_RATE = 250000;
const int MAX_SECONDS = 8;
const int READ_BUFFER_LENGTH = SAMPLE_RATE * MAX_SECONDS;


class RtlSource {
	rtlsdr_dev_t  *Device;
	char manufacturer[LENGTH_MANUFACTURER], product[LENGTH_PRODUCT], serial[LENGTH_SERIAL];
	
public:
	// Factory methods
	static RtlSource * CreateDefault() {
		RtlSource * source = new RtlSource();
		source->OpenDefault();
		return source;
	}
	
	static RtlSource * CreateBySerial(const char * serialName) {
		RtlSource * source = new RtlSource();
		source->OpenBySerial(serialName);
		return source;
	}
	
	// Object methods
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
	
	float ReadMeanAmplitude(int frequency, int bandwith, int deltaTime) {
		char buffer[READ_BUFFER_LENGTH];
		int len=0;
		int result = rtlsdr_read_sync(Device, &buffer, READ_BUFFER_LENGTH, &len);
		int mean=0;

		// Calculate mean amplitude
		for (int i=0; i< len; i++) {
			mean += buffer[i];
		}

		printf("Readed samples: %d - Mean: %d\n", len, mean / READ_BUFFER_LENGTH);
		return mean / READ_BUFFER_LENGTH;
	}

	void CloseRtl() {	
		if (Device)
			rtlsdr_close(Device);
	}
	
protected:
	void Open(int index) {
		rtlsdr_open(&Device, index);
		rtlsdr_get_device_usb_strings(0, manufacturer, product, serial);
		rtlsdr_set_sample_rate(Device, SAMPLE_RATE);
		rtlsdr_set_agc_mode(Device, 1);

		// #TODO: refactor
		printf("Manufacturer: %s\n", manufacturer);
		printf("Product: %s\n", product);
		printf("Serial: %s\n", serial);
	}
	
/*	void InitializeRtlSdr() {
		rtlsdr_open(Device, DeviceIndex());
		//rtlsdr_set_tuner_if_gain();
		//rtlsdr_set_tuner_gain_mode();
		rtlsdr_set_sample_rate(Device, SAMPLE_RATE);
		rtlsdr_set_agc_mode(Device, 1);
	}*/
};
