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

#ifndef SML_TRANSPORT_H_
#define SML_TRANSPORT_H_

#include <sml/sml_file.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// sml_transport_read reads continously bytes from fd and scans
// for the SML transport protocol escape sequences. If a SML file
// is detected it will be copied into the buffer. The total amount of bytes read
// will be returned. If the SML file exceeds the len of the buffer, -1 will be returned
size_t sml_transport_read(int fd, unsigned char *buffer, size_t max_len);

// sml_transport_listen is an endless loop which reads continously
// via sml_transport_read and calls the sml_transporter_receiver
void sml_transport_listen(int fd,
						  void (*sml_transport_receiver)(unsigned char *buffer, size_t buffer_len));

// sml_transport_writes adds the SML transport protocol escape
// sequences and writes the given file to fd. The file must be
// in the parsed format.
// The number of bytes written is returned, 0 if there was an
// error.
// The sml_file must be free'd elsewhere.
int sml_transport_write(int fd, sml_file *file);

#ifdef __cplusplus
}
#endif

#endif /* SML_TRANSPORT_H_ */
