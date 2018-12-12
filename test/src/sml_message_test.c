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
#include <sml/sml_message.h>

TEST_GROUP(sml_message);

sml_buffer *buf;

TEST_SETUP(sml_message) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_message) {
	sml_buffer_free(buf);
}

TEST(sml_message, init) {
	sml_message *msg = sml_message_init();
	TEST_ASSERT_NOT_NULL(msg);
	TEST_ASSERT_NOT_NULL(msg->transaction_id);
	sml_message_free( msg );
}

TEST(sml_message, init_unique_transaction_id) {
	sml_message *msg1 = sml_message_init();
	sml_message *msg2 = sml_message_init();
	TEST_ASSERT_TRUE(sml_octet_string_cmp(msg1->transaction_id, msg2->transaction_id) != 0);
	sml_message_free( msg2 );
	sml_message_free( msg1 );
}

TEST(sml_message, parse) {
	hex2binary("7607003800003FB662006200726301017601010700380042153D0B06454D48010271533BCD010163820800", sml_buf_get_current_buf(buf));
	sml_message *msg = sml_message_parse(buf);
	TEST_ASSERT_NOT_NULL(msg);
	sml_message_free( msg );
}

TEST(sml_message, crc_error) {
	hex2binary("7607003800003FB662006200736301017601010700380042153D0B06454D48010271533BCD010163820800", sml_buf_get_current_buf(buf));
	//                                   ^ 1 bit corrupted here
	sml_message *msg = sml_message_parse(buf);
	TEST_ASSERT_NULL(msg);
}

TEST_GROUP_RUNNER(sml_message) {
	RUN_TEST_CASE(sml_message, init);
	RUN_TEST_CASE(sml_message, init_unique_transaction_id);
	RUN_TEST_CASE(sml_message, parse);
	RUN_TEST_CASE(sml_message, crc_error);
}

