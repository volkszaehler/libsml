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

#include <fcntl.h>
#include <sml/sml_octet_string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef SML_NO_UUID_LIB
#include <uuid/uuid.h>
#else
#include <stdlib.h> // for rand()
#endif

uint8_t ctoi(uint8_t c);
uint8_t c2toi(uint8_t c1, uint8_t c2);
uint8_t c2ptoi(char *c);

octet_string *sml_octet_string_init(unsigned char *str, int length) {
	octet_string *s = (octet_string *)malloc(sizeof(octet_string));
	*s = (octet_string){.str = NULL, .len = 0};
	if (length > 0) {
		s->str = (unsigned char *)malloc(length);
		memcpy(s->str, str, length);
		s->len = length;
	}

	return s;
}

octet_string *sml_octet_string_init_from_hex(char *str) {
	int i, len = strlen(str);
	if (len % 2 != 0) {
		return NULL;
	}
	unsigned char bytes[len / 2];
	for (i = 0; i < (len / 2); i++) {
		bytes[i] = c2ptoi(&(str[i * 2]));
	}
	return sml_octet_string_init(bytes, len / 2);
}

void sml_octet_string_free(octet_string *str) {
	if (str) {
		if (str->str) {
			free(str->str);
		}
		free(str);
	}
}

octet_string *sml_octet_string_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		fprintf(stderr, "optional is skipped\n");
		return NULL;
	}

	int l;
	if (sml_buf_get_next_type(buf) != SML_TYPE_OCTET_STRING) {
		fprintf(stderr, "not a string\n");
		buf->error = 1;
		return NULL;
	}

	l = sml_buf_get_next_length(buf);
	if (l < 0 || l >= (buf->buffer_len - buf->cursor)) { // sanity check: doesnt fit into buffer...
		// libFuzzer fix for crash-3b12f21fdd346700ac1a10dfebba7604be8f070c
		fprintf(stderr, "does not fit\n");
		buf->error = 1;
		return NULL;
	}

	octet_string *str = sml_octet_string_init(sml_buf_get_current_buf(buf), l);
	sml_buf_update_bytes_read(buf, l);
	return str;
}

void sml_octet_string_write(octet_string *str, sml_buffer *buf) {
	if (str == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_OCTET_STRING, (unsigned int)str->len);
	memcpy(sml_buf_get_current_buf(buf), str->str, str->len);
	buf->cursor += str->len;
}

octet_string *sml_octet_string_generate_uuid() {
#ifndef SML_NO_UUID_LIB
	uuid_t uuid;
	uuid_generate(uuid);
#else
	unsigned char uuid[16];

// TODO add support for WIN32 systems
#ifdef __linux__
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, uuid, 16);
#else
	int i;
	for (i = 0; i < 16; i++) {
		uuid[i] = rand() % 0xFF;
	}
#endif /* __linux__ */
	uuid[6] = (uuid[6] & 0x0F) | 0x40; // set version
	uuid[8] = (uuid[8] & 0x3F) | 0x80; // set reserved bits
#endif /* SML_NO_UUID_LIB */
	return sml_octet_string_init(uuid, 16);
}

int sml_octet_string_cmp(octet_string *s1, octet_string *s2) {
	if (s1->len != s2->len) {
		return -1;
	}
	return memcmp(s1->str, s2->str, s1->len);
}

int sml_octet_string_cmp_with_hex(octet_string *str, char *hex) {
	octet_string *hstr = sml_octet_string_init_from_hex(hex);
	if (str->len != hstr->len) {
		sml_octet_string_free(hstr);
		return -1;
	}
	int result = memcmp(str->str, hstr->str, str->len);
	sml_octet_string_free(hstr);
	return result;
}

uint8_t ctoi(uint8_t c) {
	uint8_t ret = 0;

	if ((c >= '0') && (c <= '9')) {
		ret = c - '0';
	} else if ((c >= 'a') && (c <= 'f')) {
		ret = c - 'a' + 10;
	} else if ((c >= 'A') && (c <= 'F')) {
		ret = c - 'A' + 10;
	}
	return ret;
}

uint8_t c2toi(uint8_t c1, uint8_t c2) { return ctoi(c1) << 4 | ctoi(c2); }

uint8_t c2ptoi(char *c) { return ctoi((uint8_t)c[0]) << 4 | ctoi((uint8_t)c[1]); }
