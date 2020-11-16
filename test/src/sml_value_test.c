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
#include <sml/sml_value.h>

TEST_GROUP(sml_value);

static sml_buffer *buf;

TEST_SETUP(sml_value) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_value) {
	sml_buffer_free(buf);
}

TEST(sml_value, init) {
	sml_value *v = sml_value_init();
	TEST_ASSERT_NOT_NULL(v);
	sml_value_free( v );
}

TEST(sml_value, parse_octet_string) {
	hex2binary("0648616C6C6F", sml_buf_get_current_buf(buf));
	sml_value *v = sml_value_parse(buf);

	TEST_ASSERT_NOT_NULL(v);
	TEST_ASSERT_EQUAL(SML_TYPE_OCTET_STRING, v->type);
	expected_octet_string(v->data.bytes, "Hallo", 5);

	sml_value_free( v );
}

TEST(sml_value, parse_boolean) {
	hex2binary("4200", sml_buf_get_current_buf(buf));
	sml_value *v = sml_value_parse(buf);

	TEST_ASSERT_NOT_NULL(v);
	TEST_ASSERT_EQUAL(SML_TYPE_BOOLEAN, v->type);
	TEST_ASSERT_FALSE(*(v->data.boolean));

	sml_value_free( v );
}

TEST(sml_value, parse_unsigned32) {
	hex2binary("6500000001", sml_buf_get_current_buf(buf));
	sml_value *v = sml_value_parse(buf);

	TEST_ASSERT_NOT_NULL(v);
	TEST_ASSERT_EQUAL(1, *(v->data.uint32));
	TEST_ASSERT_EQUAL((SML_TYPE_UNSIGNED | SML_TYPE_NUMBER_32), v->type);
	TEST_ASSERT_EQUAL(5, buf->cursor);

	sml_value_free( v );
}

TEST(sml_value, parse_integer64_fewer_bytes) {
	hex2binary("58FFFFFFFFFFFF0F", sml_buf_get_current_buf(buf));
	sml_value *v = sml_value_parse(buf);

	TEST_ASSERT_EQUAL(-241, *(v->data.int64));
	TEST_ASSERT_EQUAL((SML_TYPE_INTEGER | SML_TYPE_NUMBER_64), v->type);

	sml_value_free( v );
}

TEST(sml_value, parse_optional) {
	hex2binary("01", sml_buf_get_current_buf(buf));
	sml_value *v = sml_value_parse(buf);

	TEST_ASSERT_NULL(v);
	TEST_ASSERT_EQUAL(1, buf->cursor);

	sml_value_free( v );
}

TEST(sml_value, write_octet_string) {
	sml_value *v = sml_value_init();
	v->type = SML_TYPE_OCTET_STRING;
	v->data.bytes = sml_octet_string_init((unsigned char *)"Hallo", 5);

	sml_value_write(v, buf);
	expected_buf(buf, "0648616C6C6F", 6);

	sml_value_free( v );
}

TEST(sml_value, write_boolean) {
	sml_value *v = sml_value_init();
	v->type = SML_TYPE_BOOLEAN;
	v->data.boolean = sml_boolean_init(SML_BOOLEAN_FALSE);

	sml_value_write(v, buf);
	expected_buf(buf, "4200", 2);

	sml_value_free( v );
}

TEST(sml_value, write_unsigned32) {
	sml_value *v = sml_value_init();
	v->type = SML_TYPE_UNSIGNED | SML_TYPE_NUMBER_32;
	v->data.uint32 = sml_u32_init(42);

	sml_value_write(v, buf);
	expected_buf(buf, "650000002A", 5);

	sml_value_free( v );
}

TEST(sml_value, write_integer16) {
	sml_value *v = sml_value_init();
	v->type = SML_TYPE_INTEGER | SML_TYPE_NUMBER_16;
	v->data.int16 = sml_i16_init(-5);

	sml_value_write(v, buf);
	expected_buf(buf, "53FFFB", 3);

	sml_value_free( v );
}

TEST(sml_value, write_optional) {
	sml_value_write(0, buf);
	expected_buf(buf, "01", 1);
}

TEST_GROUP_RUNNER(sml_value) {
	RUN_TEST_CASE(sml_value, init);
	RUN_TEST_CASE(sml_value, parse_octet_string);
	RUN_TEST_CASE(sml_value, parse_boolean);
	RUN_TEST_CASE(sml_value, parse_unsigned32);
	RUN_TEST_CASE(sml_value, parse_integer64_fewer_bytes);
	RUN_TEST_CASE(sml_value, parse_optional);
	RUN_TEST_CASE(sml_value, write_octet_string);
	RUN_TEST_CASE(sml_value, write_boolean);
	RUN_TEST_CASE(sml_value, write_unsigned32);
	RUN_TEST_CASE(sml_value, write_integer16);
	RUN_TEST_CASE(sml_value, write_optional);
}

