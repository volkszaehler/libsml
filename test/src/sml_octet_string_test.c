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

#include "../unity/unity_fixture.h"
#include "test_helper.h"
#include <sml/sml_octet_string.h>

TEST_GROUP(sml_octet_string);

static sml_buffer *buf;

TEST_SETUP(sml_octet_string) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_octet_string) {
	sml_buffer_free(buf);
}

TEST(sml_octet_string, init) {
	octet_string *str = sml_octet_string_init((unsigned char *)"hallo", 5);
	TEST_ASSERT_EQUAL(5, str->len);
	TEST_ASSERT_EQUAL_MEMORY("hallo", str->str, 5);
	sml_octet_string_free( str );
}

TEST(sml_octet_string, parse) {
	hex2binary("0648616C6C6F", sml_buf_get_current_buf(buf));

	octet_string *str = sml_octet_string_parse(buf);
	expected_octet_string(str, "Hallo", 5);
	sml_octet_string_free( str );
}

TEST(sml_octet_string, parse_multiple_tl_fields) {
	hex2binary("8102616161616F6161616161616161616161", sml_buf_get_current_buf(buf));

	octet_string *str = sml_octet_string_parse(buf);
	expected_octet_string(str, "aaaaoaaaaaaaaaaa", 16);
	sml_octet_string_free( str );
}

TEST(sml_octet_string, parse_optional) {
	hex2binary("01", sml_buf_get_current_buf(buf));
	octet_string *str = sml_octet_string_parse(buf);

	TEST_ASSERT_FALSE(sml_buf_has_errors(buf));
	TEST_ASSERT_NULL(str);
	TEST_ASSERT_EQUAL(1, buf->cursor);

	sml_octet_string_free( str );
}

TEST(sml_octet_string, write) {
	octet_string *str = sml_octet_string_init((unsigned char *)"Hallo", 5);
	sml_octet_string_write(str, buf);
	expected_buf(buf, "0648616C6C6F", 6);
	sml_octet_string_free( str );
}

TEST(sml_octet_string, write_multiple_tl_fields) {
	octet_string *str = sml_octet_string_init((unsigned char *)"aaaaoaaaaaaaaaaa", 16);
	sml_octet_string_write(str, buf);
	expected_buf(buf, "8102616161616F6161616161616161616161", 18);
	sml_octet_string_free( str );
}

TEST(sml_octet_string, write_optional) {
	sml_octet_string_write(0, buf);
	expected_buf(buf, "01", 1);
}

TEST(sml_octet_string, cmp) {
	octet_string *s1 = sml_octet_string_init((unsigned char *)"Hallo", 5);
	octet_string *s2 = sml_octet_string_init((unsigned char *)"Hi", 2);
	octet_string *s3 = sml_octet_string_init((unsigned char *)"Hallo", 5);

	TEST_ASSERT_TRUE(sml_octet_string_cmp(s1, s2) != 0);
	TEST_ASSERT_EQUAL(0, sml_octet_string_cmp(s1, s3));

	sml_octet_string_free( s3 );
	sml_octet_string_free( s2 );
	sml_octet_string_free( s1 );
}

TEST(sml_octet_string, cmp_with_hex) {
	octet_string *s = sml_octet_string_init((unsigned char *)"Hallo", 5);
	TEST_ASSERT_EQUAL(0, sml_octet_string_cmp_with_hex(s, "48616C6C6F"));
	sml_octet_string_free( s );
}

TEST_GROUP_RUNNER(sml_octet_string) {
	RUN_TEST_CASE(sml_octet_string, init);
	RUN_TEST_CASE(sml_octet_string, parse);
	RUN_TEST_CASE(sml_octet_string, parse_multiple_tl_fields);
	RUN_TEST_CASE(sml_octet_string, parse_optional);
	RUN_TEST_CASE(sml_octet_string, write);
	RUN_TEST_CASE(sml_octet_string, write_multiple_tl_fields);
	RUN_TEST_CASE(sml_octet_string, write_optional);
	RUN_TEST_CASE(sml_octet_string, cmp);
	RUN_TEST_CASE(sml_octet_string, cmp_with_hex);
}

