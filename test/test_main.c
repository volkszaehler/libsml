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

#include "unity/unity_fixture.h"

static void runAllTests() {
	RUN_TEST_GROUP(sml_octet_string);
	RUN_TEST_GROUP(sml_buffer);
	RUN_TEST_GROUP(sml_number);
	RUN_TEST_GROUP(sml_boolean);
	RUN_TEST_GROUP(sml_value);
	RUN_TEST_GROUP(sml_status);
	RUN_TEST_GROUP(sml_list);
	RUN_TEST_GROUP(sml_sequence);
	RUN_TEST_GROUP(sml_time);
	RUN_TEST_GROUP(sml_tree);
	RUN_TEST_GROUP(sml_tree_path);
	RUN_TEST_GROUP(sml_proc_par_value);
	RUN_TEST_GROUP(sml_open_request);
	RUN_TEST_GROUP(sml_get_profile_pack_request);
	RUN_TEST_GROUP(sml_message);
	RUN_TEST_GROUP(sml_file);
}

int main(int argc, char * argv[]) {
	return UnityMain(argc, argv, runAllTests);
}
