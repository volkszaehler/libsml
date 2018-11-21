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

#include "test_helper.h"
#include "../unity/unity_fixture.h"
#include <stdint.h>
#include <string.h>

uint8_t test_helper_ctoi(uint8_t c){
    uint8_t ret = 0;

    if((c >= '0') && (c <= '9')){
        ret = c - '0';
    } else if((c >= 'a') && (c <= 'f')){
        ret = c - 'a' + 10;
    } else if((c >= 'A') && (c <= 'F')){
        ret = c - 'A' + 10;
    }

    return ret;
}

// fix compiler warning - uncomment if needed
// static inline uint8_t test_helper_c2toi(uint8_t c1, uint8_t c2){
//     return test_helper_ctoi(c1) << 4 | test_helper_ctoi(c2);
// }

static inline uint8_t test_helper_c2ptoi(char* c){
    return test_helper_ctoi((uint8_t)c[0]) << 4 | test_helper_ctoi((uint8_t)c[1]);
}

int hex2binary(char *hex, unsigned char *buf) {
	int i;
	int len = strlen(hex);
	for (i = 0; i < (len /2); i++) {
		buf[i] = test_helper_c2ptoi(&(hex[i * 2]));
	}
	return i;
}

void expected_buf(sml_buffer *buf, char *hex, int len) {
	unsigned char expected_buf[len];
	hex2binary(hex, expected_buf);
	TEST_ASSERT_EQUAL_MEMORY(expected_buf, buf->buffer, len);
	TEST_ASSERT_EQUAL(len, buf->cursor);
}

void expected_octet_string(octet_string *str, char *content, int len) {
	TEST_ASSERT_NOT_NULL(str);
	TEST_ASSERT_EQUAL(len, str->len);
	TEST_ASSERT_EQUAL_MEMORY(content, str->str, len);
}

