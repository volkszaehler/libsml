UNAME := $(shell uname)
CFLAGS += -I../sml/include/ -std=c99 -Wall -Wextra -pedantic
LIBSML = ../sml/lib/libsml.a

ifneq ($(UNAME), Darwin)
LIBS = -luuid
endif

UNITY = \
	unity/unity.o \
	unity/unity_fixture.o

OBJS = \
	src/test_helper.o \
	src/sml_octet_string_test.o \
	src/sml_buffer_test.o \
	src/sml_number_test.o \
	src/sml_boolean_test.o \
	src/sml_value_test.o \
	src/sml_status_test.o \
	src/sml_list_test.o \
	src/sml_time_test.o \
	src/sml_tree_test.o \
	src/sml_file_test.o \
	src/sml_open_request_test.o \
	src/sml_get_profile_pack_request_test.o \
	src/sml_message_test.o

test_run: libsml test
	@./test

test : $(UNITY) $(OBJS) $(LIBSML)
	$(CC) $(CFLAGS) $(LIBS) $^ test_main.c -o test $(LIBS)

.PHONY: code
libsml : 
	@$(MAKE) -C ../sml

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean :
	@rm -f unity/*.o
	@rm -f src/*.o
	@rm -f test

