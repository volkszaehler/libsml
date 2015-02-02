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

#include <sml/sml_file.h>
#include <sml/sml_transport.h>

#define MC_SML_BUFFER_LEN 8096

void transport_receiver(unsigned char *buffer, size_t buffer_len) {
	sml_file *file = sml_file_parse(buffer + 8, buffer_len - 16);
	sml_file_print(file);
	sml_file_free(file);
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Usage: ./sml_parser <file>\n");
		exit(1);
	}

	if (access(argv[1], F_OK) == -1) {
		printf("Error: no such file (%s)\n", argv[1]);
		exit(2);
	}

	FILE* file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("Error: can''t open file (%s)\n", argv[1]);
		exit(3);
	}

	unsigned char buffer[MC_SML_BUFFER_LEN];
	size_t bytes;

	while (1) {
		bytes = sml_transport_read(file->_fileno, buffer, MC_SML_BUFFER_LEN);
		if (bytes > 0) {
			transport_receiver(buffer, bytes);
		}
		if (feof(file)) {
			break;
		}
	}

	fclose(file);
	return 0;
}
