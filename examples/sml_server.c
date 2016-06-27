// Copyright 2011 Juri Glass, Mathias Runge, Nadim El Sayed
// DAI-Labor, TU-Berlin
//
// This file is part of libSML.
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

#include <math.h>

#include <sml/sml_file.h>
#include <sml/sml_transport.h>

int serial_port_open(const char* device) {
	int bits;
	struct termios config;
	memset(&config, 0, sizeof(config));

	int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
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
	// the buffer contains the whole message, with transport escape sequences.
	// these escape sequences are stripped here.
	sml_file *file = sml_file_parse(buffer +8 , buffer_len );
	// the sml file is parsed now

	sml_get_list_response *body;
	sml_list *entry;
	int m=0;
	int n=32;

	// read here some values ..
	int i;
	for (i = 0; i < file->messages_len; i++) {
		sml_message *message = file->messages[i];

		if (*message->message_body->tag == SML_MESSAGE_GET_LIST_RESPONSE) {
			body = (sml_get_list_response *) message->message_body->data;
			entry = body->val_list;

			/* iterating through linked list */
			for (m = 0; m < n && entry != NULL; m++) {
				int scaler = (entry->scaler) ? *entry->scaler : 1;
				double value=(sml_value_to_double(entry->value) * pow(10, scaler));
				printf("Read value: %d %d %d %d %d %d %f\n",
						(unsigned char) entry->obj_name->str[0],
						(unsigned char) entry->obj_name->str[1],
						(unsigned char) entry->obj_name->str[2],
						(unsigned char) entry->obj_name->str[3],
						(unsigned char) entry->obj_name->str[4],
						(unsigned char) entry->obj_name->str[5]
						,value);
				entry = entry->next;
			}
		}
	}

	// this prints some information about the file
//	sml_file_print(file);

	// free the malloc'd memory
	sml_file_free(file);
}
#define BUFLEN 8096

int main(void) {
	// this example assumes that a EDL21 meter sending SML messages via a
	// serial device. Adjust as needed.
	if(argc > 1 ) {
		FILE *fp;
		if((fp=fopen(argv[1],"r")) == NULL) {
			fprintf(stderr,"Can not open %s",argv[1]);
			return(1);
		}
		unsigned char buffer[BUFLEN];
		int len=fread(buffer,1,BUFLEN,fp);
		fclose(fp);
		printf("read %d bytes from file\n",len);
		transport_receiver(buffer,len-11);
	} else {
	char *device = "/dev/cu.usbserial";
	int fd = serial_port_open(device);

	if (fd > 0) {
		// listen on the serial device, this call is blocking.
		sml_transport_listen(fd, &transport_receiver);
		close(fd);
	}
	}
	return 0;
}

