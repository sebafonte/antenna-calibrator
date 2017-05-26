#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions


class AntennaMover {
protected:
	char * Port;

	int InitializePort() {
		int USB = open(Port, O_RDWR| O_NOCTTY );

		struct termios tty;
		struct termios tty_old;
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
		tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
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
		char cmd[] = "000\n";
		sprintf(cmd, "%d\n", angle);
		int n_written = 0, spot = 0;

		do {
		    n_written = write(USB, &cmd[spot], 1);
		    spot += n_written;
		} while (cmd[spot-1] != '\n' && n_written > 0);

		// Close
		close(USB);
	}

	void MoveZero() {
		Move(0);
	}
};
