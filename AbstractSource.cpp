
class AbstractSource {
public:
	virtual float ReadMeanAmplitude() { return -999.0; } // = 0;
	virtual void Initialize() { }
};
