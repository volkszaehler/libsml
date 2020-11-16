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
#include <sml/sml_file.h>

TEST_GROUP(sml_file);

static sml_buffer *buf;

TEST_SETUP(sml_file) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_file) {
	sml_buffer_free(buf);
}

TEST(sml_file, init) {
	sml_file *file = sml_file_init();
	TEST_ASSERT_NOT_NULL(file);
	TEST_ASSERT_EQUAL(0, file->messages_len);
	TEST_ASSERT_NULL(file->messages);
	TEST_ASSERT_NOT_NULL(file->buf);
	sml_file_free( file );
}

TEST(sml_file, parse_crash_report1) {
	unsigned char buffer[388 - 16];

	// this is from an error / crash report from vzlogger.
	// with the following buffer (sadly only partial report)
	// and bytes=388
	// it reports an error type 0900 not yet implemented
	// the fix was to check crc being not null

	unsigned char buffer2[] =
		"\033\033\033\033\001\001\001\001v\vESYA\037\350\005\265\203\021b\000b\000rc\001\001v\001"
		"\004ESY\bESY+"
		"\261\203\021\v\t\001ESY\021\003\246\037\350\001\001c\035R\000v\vESYA\037\350\005\265\203"
		"\022b\000b\000rc\t\000\001ESY\021\003\246\037\350\a\001\000b\n\377\377rb\001e\001\347+"
		"\261yw\a\201\201ǂ\003\377\001\001\001\001\004ESY\001w\a\001\000\000\000\t\377\001\001"
		"\001"
		"\001\v\t\001ESY\021\003\246\037\350\001w\a\001\000\001\b\000\377d\000\000\200\001b\036R"
		"\374Y\000\000\000\tr\231\221\004\001w\a\001\000\020\a\000\377\001\001b\033R\376Y\000\000"
		"\000\000\000\000i\231\001w\a\001\000$";
	// sadly not... just 202 TEST_ASSERT_EQUAL(388, sizeof buffer2);
	memcpy(buffer, buffer2, sizeof buffer2);
	size_t bytes = 388;
	// bytes and buffer like from sml_transport_read
	sml_file *file = sml_file_parse(buffer + 8, bytes - 16);
	TEST_ASSERT_NOT_NULL(file);
	TEST_ASSERT_EQUAL(1, file->messages_len);
	sml_file_free(file);
}

TEST_GROUP_RUNNER(sml_file) {
	RUN_TEST_CASE(sml_file, init);
	RUN_TEST_CASE(sml_file, parse_crash_report1);
}
