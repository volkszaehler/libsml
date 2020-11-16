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
#include "../unity/unity.h"
#include "test_helper.h"
#include <sml/sml_number.h>

TEST_GROUP(sml_number);

static sml_buffer *buf;

TEST_SETUP(sml_number) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_number) {
	sml_buffer_free(buf);
}

TEST(sml_number, init_unsigned8) {
	u8 *n = sml_u8_init(1);
	TEST_ASSERT_NOT_NULL(n);
	TEST_ASSERT_EQUAL(1, *n);
	sml_u8_free( n );
}

TEST(sml_number, init_integer16) {
	i16 *n = sml_i16_init(-1);
	TEST_ASSERT_NOT_NULL(n);
	TEST_ASSERT_EQUAL(-1, *n);
	sml_i16_free( n );
}

TEST(sml_number, parse_unsigned8) {
	hex2binary("6201", sml_buf_get_current_buf(buf));
	u8 *n = sml_u8_parse(buf);
	TEST_ASSERT_EQUAL(1, *n);
	TEST_ASSERT_EQUAL(2, buf->cursor);
	sml_u8_free( n );
}

TEST(sml_number, parse_unsigned16) {
	hex2binary("630101", sml_buf_get_current_buf(buf));
	u16 *n = sml_u16_parse(buf);
	TEST_ASSERT_EQUAL(257, *n);
	sml_u16_free( n );
}

TEST(sml_number, parse_unsigned32) {
	hex2binary("6500000001", sml_buf_get_current_buf(buf));
	u32 *n = sml_u32_parse(buf);
	TEST_ASSERT_EQUAL(1, *n);
	sml_u32_free( n );
}

TEST(sml_number, parse_unsigned32_fewer_bytes) {
	hex2binary("64010001", sml_buf_get_current_buf(buf));
	u32 *n = sml_u32_parse(buf);
	TEST_ASSERT_EQUAL(65537, *n);
	sml_u32_free( n );
}

TEST(sml_number, parse_unsigned32_optional) {
	hex2binary("01", sml_buf_get_current_buf(buf));
	u32 *n = sml_u32_parse(buf);
	TEST_ASSERT_NULL(n);
	TEST_ASSERT_EQUAL(1, buf->cursor);
	sml_u32_free( n );
}

TEST(sml_number, parse_unsigned64) {
	hex2binary("690000000000000001", sml_buf_get_current_buf(buf));
	u64 *n = sml_u64_parse(buf);
	TEST_ASSERT_EQUAL(1, *n);
	sml_u64_free( n );
}

TEST(sml_number, parse_unsigned64_fewer_bytes) {
	hex2binary("67000000000001", sml_buf_get_current_buf(buf));
	u64 *n = sml_u64_parse(buf);
	TEST_ASSERT_EQUAL(1, *n);
	sml_u64_free( n );
}

TEST(sml_number, parse_int8) {
	hex2binary("52FF", sml_buf_get_current_buf(buf));
	i8 *n = sml_i8_parse(buf);
	TEST_ASSERT_EQUAL(-1, *n);
	sml_i8_free( n );
}

TEST(sml_number, parse_int16) {
	hex2binary("53EC78", sml_buf_get_current_buf(buf));
	i16 *n = sml_i16_parse(buf);
	TEST_ASSERT_EQUAL(-5000, *n);
	sml_i16_free( n );
}

TEST(sml_number, parse_int32) {
	hex2binary("55FFFFEC78", sml_buf_get_current_buf(buf));
	i32 *n = sml_i32_parse(buf);
	TEST_ASSERT_EQUAL(-5000, *n);
	sml_i32_free( n );
}

TEST(sml_number, parse_int64) {
	hex2binary("59FFFFFFFFFFFFFFFF", sml_buf_get_current_buf(buf));
	i64 *n = sml_i64_parse(buf);
	TEST_ASSERT_EQUAL(-1, *n);
	sml_i64_free( n );
}

TEST(sml_number, parse_int64_fewer_bytes) {
	hex2binary("58FFFFFFFFFFEC78", sml_buf_get_current_buf(buf));
	i64 *n = sml_i64_parse(buf);
	TEST_ASSERT_EQUAL(-5000, *n);
	sml_i64_free( n );
}

TEST(sml_number, write_unsigned8) {
	u8 *n = sml_u8_init(1);
	sml_u8_write(n, buf);
	expected_buf(buf, "6201", 2);
	sml_u8_free( n );
}

TEST(sml_number, write_integer32) {
	i32 *n = sml_i32_init(-5000);
	sml_i32_write(n, buf);
	expected_buf(buf, "55FFFFEC78", 5);
	sml_i32_free( n );
}

TEST(sml_number, write_integer8_optional) {
	sml_i8_write(0, buf);
	expected_buf(buf, "01", 1);
}

TEST_GROUP_RUNNER(sml_number) {
	RUN_TEST_CASE(sml_number, init_unsigned8);
	RUN_TEST_CASE(sml_number, init_integer16);

	RUN_TEST_CASE(sml_number, parse_unsigned8);
	RUN_TEST_CASE(sml_number, parse_unsigned16);
	RUN_TEST_CASE(sml_number, parse_unsigned32);
	RUN_TEST_CASE(sml_number, parse_unsigned64);
	RUN_TEST_CASE(sml_number, parse_unsigned32_fewer_bytes);
	RUN_TEST_CASE(sml_number, parse_unsigned64_fewer_bytes);
	RUN_TEST_CASE(sml_number, parse_unsigned32_optional);
	RUN_TEST_CASE(sml_number, parse_int8);
	RUN_TEST_CASE(sml_number, parse_int16);
	RUN_TEST_CASE(sml_number, parse_int32);
	RUN_TEST_CASE(sml_number, parse_int64);
	RUN_TEST_CASE(sml_number, parse_int64_fewer_bytes);

	RUN_TEST_CASE(sml_number, write_unsigned8);
	RUN_TEST_CASE(sml_number, write_integer32);
	RUN_TEST_CASE(sml_number, write_integer8_optional);
}

