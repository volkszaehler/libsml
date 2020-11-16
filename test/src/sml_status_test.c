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
#include <sml/sml_status.h>

TEST_GROUP(sml_status);

static sml_buffer *buf;

TEST_SETUP(sml_status) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_status) {
	sml_buffer_free(buf);
}

TEST(sml_status, init) {
	sml_status *s = sml_status_init();
	TEST_ASSERT_NOT_NULL(s);
	sml_status_free( s );
}

TEST(sml_status, parse_status8) {
	hex2binary("6201", sml_buf_get_current_buf(buf));
	sml_status *s = sml_status_parse(buf);

	TEST_ASSERT_NOT_NULL(s);
	TEST_ASSERT_EQUAL(1, *(s->data.status8));
	TEST_ASSERT_EQUAL((SML_TYPE_UNSIGNED | SML_TYPE_NUMBER_8), s->type);

	sml_status_free( s );
}

TEST(sml_status, parse_optional) {
	hex2binary("01", sml_buf_get_current_buf(buf));
	sml_status *s = sml_status_parse(buf);

	TEST_ASSERT_NULL(s);
	TEST_ASSERT_FALSE(sml_buf_has_errors(buf));
	TEST_ASSERT_EQUAL(1, buf->cursor);

	sml_status_free( s );
}

TEST(sml_status, parse_not_unsigned) {
	hex2binary("5001", sml_buf_get_current_buf(buf));
	sml_status *s = sml_status_parse(buf);

	TEST_ASSERT_NULL(s);
	TEST_ASSERT_TRUE(sml_buf_has_errors(buf));

	sml_status_free( s );
}

TEST(sml_status, write_status32) {
	sml_status *s = sml_status_init();
	s->type = SML_TYPE_UNSIGNED | SML_TYPE_NUMBER_32;
	s->data.status32 = sml_u32_init(42);

	sml_status_write(s, buf);
	expected_buf(buf, "650000002A", 5);

	sml_status_free( s );
}

TEST(sml_status, write_optional) {
	sml_status_write(0, buf);
	expected_buf(buf, "01", 1);
}


TEST_GROUP_RUNNER(sml_status) {
	RUN_TEST_CASE(sml_status, init);
	RUN_TEST_CASE(sml_status, parse_status8);
	RUN_TEST_CASE(sml_status, parse_optional);
	RUN_TEST_CASE(sml_status, parse_not_unsigned);
	RUN_TEST_CASE(sml_status, write_status32);
	RUN_TEST_CASE(sml_status, write_optional);
}

