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
#include <sml/sml_shared.h>

TEST_GROUP(sml_buffer);

int buffer_len = 512;
static sml_buffer *buf;

TEST_SETUP(sml_buffer) {
	buf = sml_buffer_init(buffer_len);
}

TEST_TEAR_DOWN(sml_buffer) {
	sml_buffer_free( buf );
}

TEST(sml_buffer, init_defaults) {

	TEST_ASSERT_NOT_NULL(buf);
	TEST_ASSERT_NOT_NULL(buf->buffer);
	TEST_ASSERT_EQUAL(buffer_len, buf->buffer_len);
	TEST_ASSERT_EQUAL(0, buf->cursor);
	TEST_ASSERT_EQUAL(0, buf->error);
	TEST_ASSERT_NULL(buf->error_msg);
}

TEST_GROUP_RUNNER(sml_buffer) {
	RUN_TEST_CASE(sml_buffer, init_defaults);
}

