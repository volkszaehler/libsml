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

#include <errno.h> // for errno
#include <sml/sml_crc16.h>
#include <sml/sml_shared.h>
#include <sml/sml_transport.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define MC_SML_BUFFER_LEN 8096

unsigned char esc_seq[] = {0x1b, 0x1b, 0x1b, 0x1b};
unsigned char start_seq[] = {0x1b, 0x1b, 0x1b, 0x1b, 0x01, 0x01, 0x01, 0x01};
unsigned char end_seq[] = {0x1b, 0x1b, 0x1b, 0x1b, 0x1a};

size_t sml_read(int fd, fd_set *set, unsigned char *buffer, size_t len) {

	ssize_t r = 0;
	size_t tr = 0;

	while (tr < len) {
		select(fd + 1, set, 0, 0, 0);
		if (FD_ISSET(fd, set)) {

			r = read(fd, &(buffer[tr]), len - tr);
			if (r == 0)
				return 0; // EOF
			if (r < 0) {
				if (errno == EINTR || errno == EAGAIN)
					continue; // should be ignored
				fprintf(stderr, "libsml: sml_read(): read error\n");
				return 0;
			}
			tr += r;
		}
	}
	return tr;
}

size_t sml_transport_read(int fd, unsigned char *buffer, size_t max_len) {

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);

	unsigned char buf[max_len];
	memset(buf, 0, max_len);
	unsigned int len = 0;

	if (max_len < 8) {
		// prevent buffer overflow
		fprintf(stderr, "libsml: error: sml_transport_read(): passed buffer too small!\n");
		return 0;
	}

	while (len < 8) {
		if (sml_read(fd, &readfds, &(buf[len]), 1) == 0) {
			return 0;
		}

		if ((buf[len] == 0x1b && len < 4) || (buf[len] == 0x01 && len >= 4)) {
			len++;
		} else {
			len = 0;
		}
	}

	// found start sequence
	while ((len + 8) < max_len) {
		if (sml_read(fd, &readfds, &(buf[len]), 4) == 0) {
			return 0;
		}

		if (memcmp(&buf[len], esc_seq, 4) == 0) {
			// found esc sequence
			len += 4;
			if (sml_read(fd, &readfds, &(buf[len]), 4) == 0) {
				return 0;
			}

			if (buf[len] == 0x1a) {
				// found end sequence
				len += 4;
				memcpy(buffer, &(buf[0]), len);
				return len;
			} else {
				// don't read other escaped sequences yet
				fprintf(stderr, "libsml: error: unrecognized sequence\n");
				return 0;
			}
		}
		len += 4;
	}

	return 0;
}

void sml_transport_listen(int fd, void (*sml_transport_receiver)(unsigned char *buffer,
																 size_t buffer_len)) {
	unsigned char buffer[MC_SML_BUFFER_LEN];
	size_t bytes;

	while ((bytes = sml_transport_read(fd, buffer, MC_SML_BUFFER_LEN)) > 0) {
		sml_transport_receiver(buffer, bytes);
	}
}

int sml_transport_write(int fd, sml_file *file) {
	sml_buffer *buf = file->buf;
	buf->cursor = 0;

	// add start sequence
	memcpy(sml_buf_get_current_buf(buf), start_seq, 8);
	buf->cursor += 8;

	// add file
	sml_file_write(file);

	// add padding bytes
	int padding = (buf->cursor % 4) ? (4 - buf->cursor % 4) : 0;
	if (padding) {
		// write zeroed bytes
		memset(sml_buf_get_current_buf(buf), 0, padding);
		buf->cursor += padding;
	}

	// begin end sequence
	memcpy(sml_buf_get_current_buf(buf), end_seq, 5);
	buf->cursor += 5;

	// add padding info
	buf->buffer[buf->cursor++] = (unsigned char)padding;

	// add crc checksum
	u16 crc = sml_crc16_calculate(buf->buffer, buf->cursor);
	buf->buffer[buf->cursor++] = (unsigned char)((crc & 0xFF00) >> 8);
	buf->buffer[buf->cursor++] = (unsigned char)(crc & 0x00FF);

	size_t wr = write(fd, buf->buffer, buf->cursor);
	if (wr == buf->cursor) {
		return wr;
	}

	return 0;
}
