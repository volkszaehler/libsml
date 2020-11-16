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
#include <sml/sml_list.h>

TEST_GROUP(sml_list);

static sml_buffer *buf;

TEST_SETUP(sml_list) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_list) {
	sml_buffer_free(buf);
}

TEST(sml_list, init) {
	sml_list *l = sml_list_init();
	TEST_ASSERT_NOT_NULL(l);
	TEST_ASSERT_NULL(l->next);
	sml_list_free( l );
}

TEST(sml_list, add) {
	sml_list *l = sml_list_init();
	sml_list *n = sml_list_init();
	sml_list_add(l, n);
	TEST_ASSERT_NOT_NULL(l);
	TEST_ASSERT_NOT_NULL(l->next);
	TEST_ASSERT_TRUE(n == l->next);
	sml_list_free( l );
}

TEST(sml_list, parse_two_entries) {
	hex2binary("727702610101010142000177026101010101420001",  sml_buf_get_current_buf(buf));
	sml_list *l = sml_list_parse(buf);

	TEST_ASSERT_FALSE(sml_buf_has_errors(buf));
	TEST_ASSERT_NOT_NULL(l);
	TEST_ASSERT_NOT_NULL(l->next);
	TEST_ASSERT_EQUAL(0, sml_octet_string_cmp_with_hex(l->obj_name, "61"));

	sml_list_free( l );
}

TEST(sml_list, parse_optional) {
	hex2binary("01",  sml_buf_get_current_buf(buf));
	sml_list *l = sml_list_parse(buf);
	TEST_ASSERT_NULL(l);
	TEST_ASSERT_FALSE(sml_buf_has_errors(buf));
	sml_list_free( l );
}

TEST(sml_list, write_one_entry) {
	sml_list *l = sml_list_init();
	l->obj_name = sml_octet_string_init((unsigned char *)"Hallo", 5);
	l->value = sml_value_init();
	l->value->type = SML_TYPE_OCTET_STRING;
	l->value->data.bytes = sml_octet_string_init((unsigned char *)"Hallo", 5);

	sml_list_write(l, buf);
	expected_buf(buf, "71770648616C6C6F010101010648616C6C6F01", 19);

	sml_list_free( l );
}

TEST(sml_list, write_optional) {
	sml_list_write(0, buf);
	expected_buf(buf, "01", 1);
}

TEST_GROUP_RUNNER(sml_list) {
	RUN_TEST_CASE(sml_list, init);
	RUN_TEST_CASE(sml_list, add);
	RUN_TEST_CASE(sml_list, parse_two_entries);
	RUN_TEST_CASE(sml_list, parse_optional);
	RUN_TEST_CASE(sml_list, write_one_entry);
	RUN_TEST_CASE(sml_list, write_optional);
}



TEST_GROUP(sml_sequence);

static sml_buffer *buf;

TEST_SETUP(sml_sequence) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_sequence) {
	sml_buffer_free(buf);
}

TEST(sml_sequence, init) {
	sml_sequence *seq = sml_sequence_init(&free);
	TEST_ASSERT_NOT_NULL(seq);
	sml_sequence_free( seq );
}

static void * sml_octet_string_parse_( sml_buffer * buf ) {
	return sml_octet_string_parse( buf );
}

static void sml_octet_string_free_( void * p ) {
	sml_octet_string_free( p );
}

TEST(sml_sequence, parse_octet_string) {
	hex2binary("720648616C6C6F0648616C6C6F", sml_buf_get_current_buf(buf));

	sml_sequence *seq = sml_sequence_parse(buf, sml_octet_string_parse_, sml_octet_string_free_);
	TEST_ASSERT_NOT_NULL(seq);
	TEST_ASSERT_EQUAL(2, seq->elems_len);
	sml_sequence_free( seq );
}

static void sml_octet_string_write_( void * p, sml_buffer * buf ) {
	sml_octet_string_write( p, buf );
}

TEST(sml_sequence, write_octet_string) {
	sml_sequence *seq = sml_sequence_init( sml_octet_string_free_ );
	sml_sequence_add(seq, sml_octet_string_init((unsigned char *)"Hallo", 5));
	sml_sequence_add(seq, sml_octet_string_init((unsigned char *)"Hallo", 5));

	sml_sequence_write(seq, buf, sml_octet_string_write_ );
	expected_buf(buf, "720648616C6C6F0648616C6C6F", 13);

	sml_sequence_free( seq );
}

TEST(sml_sequence, free_octet_string) {
	sml_sequence *seq = sml_sequence_init( sml_octet_string_free_ );
	sml_sequence_add(seq, sml_octet_string_init((unsigned char *)"Hallo", 5));
	sml_sequence_free(seq);
}

TEST_GROUP_RUNNER(sml_sequence) {
	RUN_TEST_CASE(sml_sequence, init);
	RUN_TEST_CASE(sml_sequence, parse_octet_string);
	RUN_TEST_CASE(sml_sequence, write_octet_string);
	RUN_TEST_CASE(sml_sequence, free_octet_string);
}

