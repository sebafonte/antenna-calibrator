
public class RtlSource {
	rtlsdr_dev_t  *Device;
	char manufacturer[LENGTH_MANUFACTURER], product[LENGTH_MANUFACTURER], serial[LENGTH_MANUFACTURER];
	
public:
	// Factory methods
	static RtlSource CreateDefault() {
		RtlSource * source = new RtlSource();
		RtlSource->OpenDefault();
		return source;
	}
	
	static RtlSource CreateBySerial(const char * serialName) {
		RtlSource * source = new RtlSource(serialName);
		RtlSource->OpenBySerial(serialName);
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
			Device = null;
		}
	}
	
	float ReadMeanAmplitude(int frequency, int bandwith, int deltaTime) {
		rtlsdr_read_sync();
	}

	void CloseRtl() {	
		if (Device)
			rtlsdr_close(Device);
	}
	
protected:
	void Open(int index) {
		rtlsdr_open(&Device, index);
		rtlsdr_get_device_usb_strings(0, manufacturer, product, serial);
		
		// #TODO: refactor
		printf("Manufacturer: %s\n", manufacturer);
		printf("Product: %s\n", product);
		printf("Serial: %s\n", serial);
	}
	
	void InitializeRtlSdr() {
		rtlsdr_open(&Device, DeviceIndex());
		
		rtlsdr_set_tuner_if_gain();
		rtlsdr_set_tuner_gain_mode();
		rtlsdr_set_sample_rate();
		rtlsdr_set_agc_mode();
	}
}