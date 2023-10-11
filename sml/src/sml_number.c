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

#include <sml/sml_number.h>
#include <stdio.h>
#include <string.h>

#define SML_BIG_ENDIAN 1
#define SML_LITTLE_ENDIAN 0

static void sml_number_byte_swap(unsigned char *bytes, int bytes_len) {
	int i;
	unsigned char ob[bytes_len];

	memcpy(&ob, bytes, bytes_len);

	for (i = 0; i < bytes_len; i++)
		bytes[i] = ob[bytes_len - (i + 1)];
}

static int sml_number_endian() {
	int i = 1;
	char *p = (char *)&i;

	if (p[0] == 1)
		return SML_LITTLE_ENDIAN;
	else
		return SML_BIG_ENDIAN;
}

void *sml_number_init(u64 number, unsigned char type __attribute__((unused)), int size) {

	const char *bytes = (const char *)&number;

	// Swap bytes of big-endian number so that
	// memcpy copies the right part
	if (sml_number_endian() == SML_BIG_ENDIAN) {
		bytes += sizeof(u64) - size;
	}

	char *np = malloc(size);
	if (np == NULL) {
		goto error;
	}
	memcpy(np, bytes, size);

	return np;

error:
	return NULL;
}

void *sml_number_parse(sml_buffer *buf, unsigned char type, int max_size) {
	if (sml_buf_optional_is_skipped(buf)) {
		fprintf(stderr, "skipping optional number\n");
		return NULL;
	}

	int l, i;
	unsigned char b;
	short negative_int = 0;

	if ((buf->cursor + 1) > buf->buffer_len) { // at least 1 byte for type?
		fprintf(stderr, "at least one byte for type\n");
		buf->error = 1;
		return NULL;
	}

	if (sml_buf_get_next_type(buf) != type) {
		fprintf(stderr, "what?\n");
		buf->error = 1;
		return NULL;
	}

	l = sml_buf_get_next_length(buf);
	if (l < 0 || l > max_size) {
		fprintf(stderr, "length\n");
		buf->error = 1;
		return NULL;
	}

	unsigned char *np = malloc(max_size);
	memset(np, 0, max_size);

	// at least l bytes available?
	if ((buf->cursor + l) > buf->buffer_len) {
		fprintf(stderr, "at least one byte available\n");
		buf->error = 1;
		free(np);
		return NULL;
	}

	if ((buf->cursor + 1) > buf->buffer_len) { // at least 1 byte?
		fprintf(stderr, "libsml: at least one byte\n");
		buf->error = 1;
		free(np);
		return NULL;
	}

	b = sml_buf_get_current_byte(buf);
	if (type == SML_TYPE_INTEGER && (b & 128)) {
		negative_int = 1;
	}

	int missing_bytes = max_size - l;
	memcpy(&(np[missing_bytes]), sml_buf_get_current_buf(buf), l);

	if (negative_int) {
		for (i = 0; i < missing_bytes; i++) {
			np[i] = 0xFF;
		}
	}

	if (!(sml_number_endian() == SML_BIG_ENDIAN)) {
		sml_number_byte_swap(np, max_size);
	}
	sml_buf_update_bytes_read(buf, l);

	return np;
}

void sml_number_write(void *np, unsigned char type, int size, sml_buffer *buf) {
	if (np == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_buf_set_type_and_length(buf, type, size);
	memcpy(sml_buf_get_current_buf(buf), np, size);

	if (!(sml_number_endian() == SML_BIG_ENDIAN)) {
		sml_number_byte_swap(sml_buf_get_current_buf(buf), size);
	}

	sml_buf_update_bytes_read(buf, size);
}

void sml_number_free(void *np) {
	if (np) {
		free(np);
	}
}
