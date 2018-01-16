// Copyright 2011 Juri Glass, Mathias Runge, Nadim El Sayed
// DAI-Labor, TU-Berlin
//
// This file is part of libSML.
// Thanks to Thomas Binder and Axel (tuxedo) for providing code how to
// print OBIS data (see transport_receiver()).
// https://community.openhab.org/t/using-a-power-meter-sml-with-openhab/21923
//
// libSML is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libSML is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libSML.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include <sml/sml_file.h>
#include <sml/sml_transport.h>
#include <sml/sml_value.h>

int serial_port_open(const char* device) {
	int bits;
	struct termios config;
	memset(&config, 0, sizeof(config));

#ifdef O_NONBLOCK
	int fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
#else
	int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
#endif
	if (fd < 0) {
		fprintf(stderr,"error: open(%s): %s\n", device, strerror(errno));
		return -1;
	}

	// set RTS
	ioctl(fd, TIOCMGET, &bits);
	bits |= TIOCM_RTS;
	ioctl(fd, TIOCMSET, &bits);

	tcgetattr( fd, &config ) ;

	// set 8-N-1
	config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	config.c_oflag &= ~OPOST;
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	config.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB);
	config.c_cflag |= CS8;

	// set speed to 9600 baud
	cfsetispeed( &config, B9600);
	cfsetospeed( &config, B9600);

	tcsetattr(fd, TCSANOW, &config);
	return fd;
}

void transport_receiver(unsigned char *buffer, size_t buffer_len) {
	int i;
	// the buffer contains the whole message, with transport escape sequences.
	// these escape sequences are stripped here.
	sml_file *file = sml_file_parse(buffer + 8, buffer_len - 16);
	// the sml file is parsed now

	// this prints some information about the file
	sml_file_print(file);

	// read here some values ...
	printf("OBIS data\n");
	for (i = 0; i < file->messages_len; i++) {
		sml_message *message = file->messages[i];
		if (*message->message_body->tag == SML_MESSAGE_GET_LIST_RESPONSE) {
			double value;
			sml_list *entry;
			sml_get_list_response *body;
			body = (sml_get_list_response *) message->message_body->data;
			for (entry = body->val_list; entry != NULL; entry = entry->next) {
				value = sml_value_to_double(entry->value);
				if (value != 0) {
					int scaler = (entry->scaler) ? *entry->scaler : 1;
					if (scaler == -1)
						value *= 0.1;
					printf("%d-%d:%d.%d.%d*%d#%.1f#",
						entry->obj_name->str[0], entry->obj_name->str[1],
						entry->obj_name->str[2], entry->obj_name->str[3],
						entry->obj_name->str[4], entry->obj_name->str[5], value);
					switch (*entry->unit) {
					case 0x1B:
						printf("W");
						break;
					case 0x1E:
						printf("Wh");
						break;
					}
					printf("\n");
					// flush the stdout puffer, that pipes work without waiting
					fflush(stdout);
				}
			}
		}
	}

	// free the malloc'd memory
	sml_file_free(file);
}

int main(int argc, char **argv) {
	// this example assumes that a EDL21 meter sending SML messages via a
	// serial device. Adjust as needed.
	if (argc != 2) {
		printf("Usage: %s <device>\n", argv[0]);
		printf("device - serial device of connected power meter e.g. /dev/cu.usbserial\n");
		exit(-1); // exit here
	}

	char *device = argv[1];
	int fd = serial_port_open(device);

	if (fd > 0) {
		// listen on the serial device, this call is blocking.
		sml_transport_listen(fd, &transport_receiver);
		close(fd);
	}

	return 0;
}

