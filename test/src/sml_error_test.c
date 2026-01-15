// Copyright 2020 volkszaehler.org
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
#include <sml/sml_shared.h>

#include <unistd.h>

TEST_GROUP(sml_error);

TEST_SETUP(sml_error) { }

TEST_TEAR_DOWN(sml_error) { }

TEST(sml_error, default) {
	char *tmpfile="./test_sml_error.tmp"; // FIXME: should not be hardcoded
	char *test_string="this is a test";
	char *expected_output="libsml: this is a test\n";

	FILE *capture = fopen(tmpfile, "w");
	TEST_ASSERT_NOT_NULL(capture);
	int stderr_backup=dup(2); // duplicate old stderr so it won't be closed
	dup2(fileno(capture),2); // assign capture-file to stderr

	sml_error(test_string);

	fclose(capture);
	dup2(stderr_backup,2); // restore stderr
	close(stderr_backup); // discard backup
	
	size_t len=strlen(expected_output)+1;
	char *buf=malloc(len);

	capture = fopen(tmpfile, "r");
	size_t got=fread(buf,1,len-1,capture);
	*(buf+got)=0; // add zero termination
	//fprintf(stderr,"got: `%s`\n\n",buf);
	fclose(capture);
	unlink(tmpfile);

	TEST_ASSERT_EQUAL(0, strcmp(expected_output,buf));

	free(buf);
}

const char *msg=NULL;
void my_sml_error(const char *format, ... ){
	msg=format;
}

TEST(sml_error, custom) {
	char *test_string="this is a test";
	sml_error = my_sml_error;
	sml_error(test_string,1,2,3);
	sml_error = sml_error_default;
	TEST_ASSERT_NOT_NULL(msg);
	TEST_ASSERT_EQUAL(0, strcmp(msg,test_string));
}

TEST_GROUP_RUNNER(sml_error) {
	RUN_TEST_CASE(sml_error, default);
	RUN_TEST_CASE(sml_error, custom);
}
