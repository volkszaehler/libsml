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
#include <sml/sml_boolean.h>

TEST_GROUP(sml_boolean);

static sml_buffer *buf;

TEST_SETUP(sml_boolean) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_boolean) {
	sml_buffer_free(buf);
}

TEST(sml_boolean, init) {
	sml_boolean *b = sml_boolean_init(SML_BOOLEAN_TRUE);
	TEST_ASSERT_NOT_NULL(b);
	TEST_ASSERT_EQUAL(SML_BOOLEAN_TRUE, *b);
	sml_boolean_free( b );
}

TEST(sml_boolean, parse_true) {
	hex2binary("420F", sml_buf_get_current_buf(buf));
	sml_boolean *b = sml_boolean_parse(buf);
	TEST_ASSERT_NOT_NULL(b);
	TEST_ASSERT_EQUAL(SML_BOOLEAN_TRUE, *b);
	sml_boolean_free( b );
}

TEST(sml_boolean, parse_false) {
	hex2binary("4200", sml_buf_get_current_buf(buf));
	sml_boolean *b = sml_boolean_parse(buf);
	TEST_ASSERT_NOT_NULL(b);
	TEST_ASSERT_EQUAL(SML_BOOLEAN_FALSE, *b);
	TEST_ASSERT_EQUAL(2, buf->cursor);
	sml_boolean_free( b );
}

TEST(sml_boolean, parse_optional) {
	hex2binary("01", sml_buf_get_current_buf(buf));
	sml_boolean *b = sml_boolean_parse(buf);
	TEST_ASSERT_NULL(b);
	TEST_ASSERT_FALSE(sml_buf_has_errors(buf));
	sml_boolean_free( b );
}

TEST(sml_boolean, write_true) {
	sml_boolean *b = sml_boolean_init(SML_BOOLEAN_TRUE);
	sml_boolean_write(b, buf);
	expected_buf(buf, "42FF", 2);
	sml_boolean_free( b );
}

TEST(sml_boolean, write_false) {
	sml_boolean *b = sml_boolean_init(SML_BOOLEAN_FALSE);
	sml_boolean_write(b, buf);
	expected_buf(buf, "4200", 2);
	sml_boolean_free( b );
}

TEST(sml_boolean, write_optional) {
	sml_boolean_write(0, buf);
	expected_buf(buf, "01", 1);
}


TEST_GROUP_RUNNER(sml_boolean) {
	RUN_TEST_CASE(sml_boolean, init);
	RUN_TEST_CASE(sml_boolean, parse_true);
	RUN_TEST_CASE(sml_boolean, parse_false);
	RUN_TEST_CASE(sml_boolean, parse_optional);
	RUN_TEST_CASE(sml_boolean, write_true);
	RUN_TEST_CASE(sml_boolean, write_false);
	RUN_TEST_CASE(sml_boolean, write_optional);
}

