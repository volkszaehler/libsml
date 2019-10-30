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

#ifndef SML_FILE_H_
#define SML_FILE_H_

#include "sml_message.h"
#include "sml_shared.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// a SML file consist of multiple SML messages
typedef struct {
	sml_message **messages;
	short messages_len;
	sml_buffer *buf;
} sml_file;

sml_file *sml_file_init();
// parses a SML file.
sml_file *sml_file_parse(unsigned char *buffer, size_t buffer_len);
void sml_file_add_message(sml_file *file, sml_message *message);
void sml_file_write(sml_file *file);
void sml_file_free(sml_file *file);
void sml_file_print(sml_file *file);

#ifdef __cplusplus
}
#endif

#endif /* SML_FILE_H_ */
