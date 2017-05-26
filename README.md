
This linux command can used to calibrate an antenna in order to maximize reception.<br /><br />
Two modes are currently available:<br />
	* rtl-sdr mode: 	Reads values from a rtl-sdr source.<br />
	* Airodump:		Read signal power from airodump-ng command output<br />

rtl-sdr installation is needed to build this project (https://github.com/osmocom/rtl-sdr) and It is necessary to have installed airodump-ng to run in Airodump mode.<br />

Usage:<br />
	* To calibrate a rtl source at 100 Mhz signal with 1200 bandwith, during 200 milliseconds of listening<br />
		antenna-calibrate -s rtl -d 200 -b 12000 -f 100000000<br />
	* To calibrate a wifi signal called "Speedy-Fibra" at wlan0mon<br />
		antenna-calibrate -s airodump -d 5000 -w Speedy-Fibra -m wlan0mon<br />

Bad usage examples:<br />
	* antenna-calibrate -s rtl -d 200 -b 12000 -f 100000000 -m wlan0mon<br />
	* antenna-calibrate -s airodump -d 5000 -w Speedy-Fibra -m wlan0mon -f 100000000<br />

	



	
