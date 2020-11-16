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
#include <sml/sml_tree.h>
#include <sml/sml_octet_string.h>

TEST_GROUP(sml_tree);

static sml_buffer *buf;

TEST_SETUP(sml_tree) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_tree) {
	sml_buffer_free(buf);
}

TEST(sml_tree, init) {
	sml_tree *t = sml_tree_init();
	TEST_ASSERT_NOT_NULL(t);
	sml_tree_free( t );
}

TEST(sml_tree, add_tree) {
	sml_tree *t = sml_tree_init();
	sml_tree_add_tree(t, sml_tree_init());

	TEST_ASSERT_NOT_NULL(t->child_list[0]);
	TEST_ASSERT_EQUAL(1, t->child_list_len);

	sml_tree_free( t );
}

TEST(sml_tree, write) {
	sml_tree *t = sml_tree_init();
	t->parameter_name = sml_octet_string_init((unsigned char *)"Hallo", 5);
	sml_tree_write(t, buf);
	expected_buf(buf, "730648616C6C6F0101", 9);
	sml_tree_free( t );
}

TEST(sml_tree, parse_with_child) {
	hex2binary("730648616C6C6F0171730648616C6C6F0101", sml_buf_get_current_buf(buf));
	sml_tree *t = sml_tree_parse(buf);

	TEST_ASSERT_NOT_NULL(t);
	TEST_ASSERT_NOT_NULL(t->child_list[0]);
	TEST_ASSERT_EQUAL(1, t->child_list_len);

	sml_tree_free( t );
}

TEST(sml_tree, parse_with_error_child) {
	hex2binary("730648616C6C6F0171720648616C6C6F0101", sml_buf_get_current_buf(buf));
	sml_tree *t = sml_tree_parse(buf);

	TEST_ASSERT_NULL(t);

	sml_tree_free( t );
}

TEST_GROUP_RUNNER(sml_tree) {
	RUN_TEST_CASE(sml_tree, init);
	RUN_TEST_CASE(sml_tree, add_tree);
	RUN_TEST_CASE(sml_tree, write);
	RUN_TEST_CASE(sml_tree, parse_with_child);
	RUN_TEST_CASE(sml_tree, parse_with_error_child);
}



TEST_GROUP(sml_tree_path);

TEST_SETUP(sml_tree_path) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_tree_path) {
	sml_buffer_free(buf);
}

TEST(sml_tree_path, init) {
	sml_tree_path *t = sml_tree_path_init();
	TEST_ASSERT_NOT_NULL(t);
	sml_tree_path_free( t );
}

TEST(sml_tree_path, add_entry) {
	sml_tree_path *t = sml_tree_path_init();
	TEST_ASSERT_NOT_NULL(t);
	TEST_ASSERT_EQUAL(0, t->path_entries_len);
	sml_tree_path_add_path_entry(t, sml_octet_string_init((unsigned char *)"tree", 4));
	TEST_ASSERT_EQUAL(1, t->path_entries_len);
	sml_tree_path_free( t );
}

TEST(sml_tree_path, parse) {
	hex2binary("720648616C6C6F0264", sml_buf_get_current_buf(buf));
	sml_tree_path *t = sml_tree_path_parse(buf);
	TEST_ASSERT_NOT_NULL(t);
	TEST_ASSERT_EQUAL(2, t->path_entries_len);
	TEST_ASSERT_EQUAL(0, sml_octet_string_cmp_with_hex(t->path_entries[0], "48616C6C6F"));
	TEST_ASSERT_EQUAL(0, sml_octet_string_cmp_with_hex(t->path_entries[1], "64"));
	sml_tree_path_free( t );
}

TEST(sml_tree_path, write) {
	sml_tree_path *t = sml_tree_path_init();
	sml_tree_path_add_path_entry(t, sml_octet_string_init((unsigned char *)"Hallo", 5));
	sml_tree_path_add_path_entry(t, sml_octet_string_init((unsigned char *)"Hallo", 5));
	sml_tree_path_write(t, buf);
	expected_buf(buf, "720648616C6C6F0648616C6C6F", 13);
	sml_tree_path_free( t );
}

TEST_GROUP_RUNNER(sml_tree_path) {
	RUN_TEST_CASE(sml_tree_path, init);
	RUN_TEST_CASE(sml_tree_path, add_entry);
	RUN_TEST_CASE(sml_tree_path, parse);
	RUN_TEST_CASE(sml_tree_path, write);
}



TEST_GROUP(sml_proc_par_value);

TEST_SETUP(sml_proc_par_value) {
	buf = sml_buffer_init(512);
}

TEST_TEAR_DOWN(sml_proc_par_value) {
	sml_buffer_free(buf);
}

TEST(sml_proc_par_value, init) {
	sml_proc_par_value *t = sml_proc_par_value_init();
	TEST_ASSERT_NOT_NULL(t);
	sml_proc_par_value_free( t );
}

TEST(sml_proc_par_value, parse_time) {
	hex2binary("72620472620265000000FF", sml_buf_get_current_buf(buf));
	sml_proc_par_value *t = sml_proc_par_value_parse(buf);
	TEST_ASSERT_NOT_NULL(t);
	TEST_ASSERT_EQUAL(SML_PROC_PAR_VALUE_TAG_TIME, *(t->tag));
	TEST_ASSERT_EQUAL(11, buf->cursor);
	sml_proc_par_value_free( t );
}

TEST(sml_proc_par_value, write_time) {
	sml_proc_par_value *ppv = sml_proc_par_value_init();
	ppv->tag = sml_u8_init(SML_PROC_PAR_VALUE_TAG_TIME);
	sml_time *t = sml_time_init();
	t->data.sec_index = sml_u32_init(255);
	t->tag = sml_u8_init(SML_TIME_SEC_INDEX);
	ppv->data.time = t;
	sml_proc_par_value_write(ppv, buf);
	expected_buf(buf, "72620472620165000000FF", 11);
	sml_proc_par_value_free( ppv );
}

TEST_GROUP_RUNNER(sml_proc_par_value) {
	RUN_TEST_CASE(sml_proc_par_value, init);
	RUN_TEST_CASE(sml_proc_par_value, parse_time);
	RUN_TEST_CASE(sml_proc_par_value, write_time);
}

