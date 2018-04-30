#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <iostream>

const char * MESSAGE_STRING_MOVEMENT_ENDED = "*";

class AntennaMover {
protected:
	char * Port;
	int CurrentAngle;

	int InitializePort() {
		int USB = open(Port, O_RDWR| O_NOCTTY);
		struct termios tty, tty_old;
		memset (&tty, 0, sizeof tty);

		/* Error Handling */
		if (tcgetattr (USB, &tty) != 0) {
		   printf("Error %d from tcgetattr: %s\n", errno, strerror(errno));
		}

		/* Save old tty parameters */
		tty_old = tty;

		/* Set Baud Rate */
		cfsetospeed (&tty, (speed_t)B9600);
		cfsetispeed (&tty, (speed_t)B9600);

		/* Setting other Port Stuff */
		tty.c_cflag     &=  ~PARENB;            // Make 8n1
		tty.c_cflag     &=  ~CSTOPB;
		tty.c_cflag     &=  ~CSIZE;
		tty.c_cflag     |=  CS8;

		tty.c_cflag     &=  ~CRTSCTS;           // no flow control
		tty.c_cc[VMIN]   =  1;                  // read doesn't block
		tty.c_cc[VTIME]  =  255;                // 25.5 seconds read timeout
		tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

		/* Make raw */
		cfmakeraw(&tty);

		/* Flush Port, then applies attributes */
		tcflush(USB, TCIFLUSH);
		if (tcsetattr(USB, TCSANOW, &tty) != 0) {
		   printf("Error %d from tcsetattr\n", errno);
		}

		return USB;
	}

public:
	AntennaMover(char * port) {
		Port = port;	
	}

	void Move(int angle) {	
		// Moving angle
		printf("Moving ANGLE: %d\n", angle);
		// Initialize port
		int USB = InitializePort();

		// Write
		char cmd[] = "000\r";
		sprintf(cmd, "%d\r", angle);
		int n_written = 0, spot = 0;

		do {
		    n_written = write(USB, &cmd[spot], 1);
		    spot += n_written;
		} while (cmd[spot-1] != '\r' && n_written > 0);

		// Wait for ok
		char buffer[64];
		read(USB, &buffer, 64);
		//printf("Readed: %s\n", buffer);
		//while (strcmp(buffer, MESSAGE_STRING_MOVEMENT_ENDED)) {
		//while (buffer[0] == MESSAGE_STRING_MOVEMENT_ENDED[0]) {
		//	read(USB, &buffer, 64);
			printf("Readed: %s\n", buffer);
		//}

		// Close
		close(USB);
		
		// Store current setted angle
		CurrentAngle = angle;
	}
	
	int GetCurrentAngle() {
		return CurrentAngle;
	}

	// #TODO: 
/*	int SetZero() {		
	}*/
};

