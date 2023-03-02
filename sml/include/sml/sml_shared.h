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

#ifndef SML_SHARED_H_
#define SML_SHARED_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define SML_MESSAGE_END 0x0

#define SML_TYPE_FIELD 0x70
#define SML_LENGTH_FIELD 0xF
#define SML_ANOTHER_TL 0x80

#define SML_TYPE_OCTET_STRING 0x0
#define SML_TYPE_BOOLEAN 0x40
#define SML_TYPE_INTEGER 0x50
#define SML_TYPE_UNSIGNED 0x60
#define SML_TYPE_LIST 0x70

#define SML_OPTIONAL_SKIPPED 0x1

#define SML_TYPE_NUMBER_8 sizeof(u8)
#define SML_TYPE_NUMBER_16 sizeof(u16)
#define SML_TYPE_NUMBER_32 sizeof(u32)
#define SML_TYPE_NUMBER_64 sizeof(u64)

// This sml_buffer is used in two different use-cases.
//
// Parsing: the raw data is in the buffer field,
//          the buffer_len is the number of raw bytes received,
//          the cursor points to the current position during parsing
//
// Writing: At the beginning the buffer field is malloced and zeroed with
//          a default length, this default length is stored in buffer_len
//          (i.e. the maximum bytes one can write to this buffer)
//          cursor points to the position, where on can write during the
//          writing process. If a file is completely written to the buffer,
//          cursor is the number of bytes written to the buffer.
typedef struct {
	unsigned char *buffer;
	size_t buffer_len;
	size_t cursor;
	int error;
	char *error_msg;
} sml_buffer;

sml_buffer *sml_buffer_init(size_t length);

void sml_buffer_free(sml_buffer *buf);

// Returns the length of the following data structure. Sets the cursor position to
// the value field.
int sml_buf_get_next_length(sml_buffer *buf);

void sml_buf_set_type_and_length(sml_buffer *buf, unsigned int type, unsigned int l);

// Checks if a error is occured.
int sml_buf_has_errors(sml_buffer *buf);

// Returns the type field of the current byte.
int sml_buf_get_next_type(sml_buffer *buf);

// Returns the current byte.
unsigned char sml_buf_get_current_byte(sml_buffer *buf);

// Returns a pointer to the current buffer position.
unsigned char *sml_buf_get_current_buf(sml_buffer *buf);

void sml_buf_optional_write(sml_buffer *buf);

// Sets the number of bytes read (moves the cursor forward)
void sml_buf_update_bytes_read(sml_buffer *buf, int bytes);

// Checks if the next field is a skipped optional field, updates the buffer accordingly
int sml_buf_optional_is_skipped(sml_buffer *buf);

// Prints arbitrarily byte string to stdout with printf
void sml_hexdump(unsigned char *buffer, size_t buffer_len);

#ifdef __cplusplus
}
#endif

#endif /* SML_SHARED_H_ */
