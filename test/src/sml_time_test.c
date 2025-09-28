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
#include <sml/sml_time.h>

TEST_GROUP(sml_time);

static sml_buffer *buf;

TEST_SETUP(sml_time) { buf = sml_buffer_init(512); }

TEST_TEAR_DOWN(sml_time) { sml_buffer_free(buf); }

TEST(sml_time, init) {
	sml_time *t = sml_time_init();
	TEST_ASSERT_NOT_NULL(t);
	sml_time_free(t);
}

TEST(sml_time, parse_sec_index) {
	hex2binary("72620165000000FF", sml_buf_get_current_buf(buf));
	sml_time *t = sml_time_parse(buf);

	TEST_ASSERT_NOT_NULL(t);
	TEST_ASSERT_EQUAL(SML_TIME_SEC_INDEX, *(t->tag));
	TEST_ASSERT_EQUAL(8, buf->cursor);

	sml_time_free(t);
}

TEST(sml_time, parse_timestamp) {
	hex2binary("72620265000000FF", sml_buf_get_current_buf(buf));
	sml_time *t = sml_time_parse(buf);

	TEST_ASSERT_NOT_NULL(t);
	TEST_ASSERT_EQUAL(SML_TIME_TIMESTAMP, *(t->tag));
	TEST_ASSERT_EQUAL(8, buf->cursor);

	sml_time_free(t);
}

TEST(sml_time, parse_timestamp_local) {
	hex2binary("7262037365000000FF53003C530001", sml_buf_get_current_buf(buf));
	sml_time *t = sml_time_parse(buf);

	TEST_ASSERT_NOT_NULL(t);
	TEST_ASSERT_EQUAL(SML_TIME_LOCAL_TIMESTAMP, *(t->tag));
	TEST_ASSERT_EQUAL(15, buf->cursor);

	sml_time_free(t);
}

TEST(sml_time, parse_optional) {
	hex2binary("01", sml_buf_get_current_buf(buf));
	sml_time *t = sml_time_parse(buf);

	TEST_ASSERT_NULL(t);
	TEST_ASSERT_EQUAL(1, buf->cursor);

	sml_time_free(t);
}

TEST(sml_time, write_sec_index) {
	sml_time *t = sml_time_init();
	t->data.sec_index = sml_u32_init(255);
	t->tag = sml_u8_init(SML_TIME_SEC_INDEX);

	sml_time_write(t, buf);
	expected_buf(buf, "72620165000000FF", 8);

	sml_time_free(t);
}

TEST(sml_time, write_timestamp_local) {
	sml_time *t = sml_time_init();
	sml_timestamp_local *local_t = sml_timestamp_local_init();
	local_t->timestamp = sml_u32_init(255);
	local_t->local_offset = sml_i16_init(20);
	local_t->season_time_offset = sml_i16_init(10);
	t->data.local_timestamp = local_t;
	t->tag = sml_u8_init(SML_TIME_LOCAL_TIMESTAMP);

	sml_time_write(t, buf);
	expected_buf(buf, "7262037365000000FF53001453000A", 15);

	sml_time_free(t);
}

TEST(sml_time, write_optional) {
	sml_time_write(0, buf);
	expected_buf(buf, "01", 1);
}

TEST_GROUP_RUNNER(sml_time) {
	RUN_TEST_CASE(sml_time, init);
	RUN_TEST_CASE(sml_time, parse_sec_index);
	RUN_TEST_CASE(sml_time, parse_timestamp);
	RUN_TEST_CASE(sml_time, parse_timestamp_local);
	RUN_TEST_CASE(sml_time, parse_optional);
	RUN_TEST_CASE(sml_time, write_sec_index);
	RUN_TEST_CASE(sml_time, write_timestamp_local);
	RUN_TEST_CASE(sml_time, write_optional);
}
