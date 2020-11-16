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
#include <sml/sml_get_profile_pack_request.h>

TEST_GROUP(sml_get_profile_pack_request);

static sml_buffer *buf;

TEST_SETUP(sml_get_profile_pack_request) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_get_profile_pack_request) {
	sml_buffer_free(buf);
}

TEST(sml_get_profile_pack_request, init) {
	sml_get_profile_pack_request *r = sml_get_profile_pack_request_init();
	TEST_ASSERT_NOT_NULL(r);
	sml_get_profile_pack_request_free( r );
}

TEST(sml_get_profile_pack_request, parse) {
	hex2binary("7901010101010101730648616C6C6F0648616C6C6F0648616C6C6F01", sml_buf_get_current_buf(buf));
	sml_get_profile_pack_request *r = sml_get_profile_pack_request_parse(buf);
	TEST_ASSERT_NOT_NULL(r);
	TEST_ASSERT_NOT_NULL(r->object_list);
	TEST_ASSERT_NOT_NULL(r->object_list->next);
	TEST_ASSERT_NOT_NULL(r->object_list->next->next);
	TEST_ASSERT_NULL(r->object_list->next->next->next);
	sml_get_profile_pack_request_free( r );
}

TEST_GROUP_RUNNER(sml_get_profile_pack_request) {
	RUN_TEST_CASE(sml_get_profile_pack_request, init);
	RUN_TEST_CASE(sml_get_profile_pack_request, parse);
}

