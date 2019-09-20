
This linux command can used to calibrate an antenna in order to maximize reception.

Two modes are currently available:

	* rtl-sdr mode: 	Reads values from a rtl-sdr source.
	* Airodump:		Read signal power from airodump-ng command output.

rtl-sdr installation is needed to build this project (https://github.com/osmocom/rtl-sdr) and It is necessary to have installed airodump-ng to run in Airodump mode.


Usage, assuming antenna-controler hardware at /dev/ttyUSB0:

	* To calibrate a rtl source at 100 Mhz signal with 1200 bandwith, during 200 milliseconds of listening
		antenna-calibrate -s rtl -a 30 -d 200 -b 12000 -f 100000000 -p /dev/ttyUSB0

	* To calibrate a wifi signal called "Speedy-Fibra" at wlan0mon<br />
		antenna-calibrate -s airodump -a 30 -d 5000 -w Speedy-Fibra -m mon0 -p /dev/ttyUSB0
	

Bad usage examples:

	* antenna-calibrate -s rtl -d 200 -b 12000 -f 100000000 -m wlan0mon
	* antenna-calibrate -s airodump -d 5000 -w Speedy-Fibra -m wlan0mon -f 100000000

Hardware notes:

	* Pololu version of the arduino controller module seems to produce too much heat, better use the H bridge.
	* 16 position hex rotary switch added for position correction if needed.
	* Used a 4:1 reduction with Nema17 high torque for handling a yagi with a 8mm metal edge to avoid losing steps.
	
Deployed:
	Hehe deployed image at https://cloud.pungas.space/s/bxByCPdN6iF2XSi !
