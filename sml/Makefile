UNAME := $(shell uname)
CFLAGS += -I./include/ -fPIC -fno-stack-protector -g -std=c99 -Wall -Wextra -pedantic

# Available Flags:
# SML_NO_UUID_LIB - compile without uuid lib

ifneq ($(UNAME), Darwin)
LIBS=-luuid
endif

NAME=libsml
SOVERSION=1
ifeq ($(UNAME), Darwin)
SONAME=$(NAME).$(SOVERSION).dylib
else
SONAME=$(NAME).so.$(SOVERSION)
endif

LDFLAGS += -Wl,-soname=$(SONAME) -shared

LIB_DIR=./lib
INC_DIR=./include
OBJ_LIB=$(LIB_DIR)/$(NAME).o
DYN_LIB=$(LIB_DIR)/$(SONAME)
ST_LIB=$(LIB_DIR)/$(NAME).a

INSTALL = /usr/bin/install
prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include

OBJS = \
	src/sml_file.o \
	src/sml_attention_response.o \
	src/sml_transport.o \
	src/sml_octet_string.o \
	src/sml_shared.o \
	src/sml_number.o \
	src/sml_message.o \
	src/sml_time.o \
	src/sml_list.o \
	src/sml_status.o \
	src/sml_value.o \
	src/sml_tree.o \
	src/sml_boolean.o \
	src/sml_crc16.o \
	src/sml_open_request.o \
	src/sml_open_response.o \
	src/sml_get_list_request.o \
	src/sml_get_list_response.o \
	src/sml_close_request.o \
	src/sml_close_response.o \
	src/sml_set_proc_parameter_request.o \
	src/sml_get_proc_parameter_request.o \
	src/sml_get_proc_parameter_response.o \
	src/sml_get_profile_pack_request.o \
	src/sml_get_profile_pack_response.o \
	src/sml_get_profile_list_request.o \
	src/sml_get_profile_list_response.o

all: libsml

libsml: $(DYN_LIB) $(ST_LIB) $(OBJ_LIB)

$(DYN_LIB): $(OBJS)
ifeq ($(UNAME), Darwin)
	$(CC) $(LIBS) -dynamiclib -install_name $(SONAME) -o $@ $^
else
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
endif

$(OBJ_LIB): $(OBJS)
	$(LD) -r -o $@ $^

$(ST_LIB): $(OBJS)
	$(AR) -rs $@ $^

.PHONY: clean install uninstall
clean:
	@rm -f src/*.o
	@rm -f $(DYN_LIB) $(OBJ_LIB) $(ST_LIB)

install: $(DYN_LIB) $(ST_LIB)
	install -d $(DESTDIR)$(libdir)
	$(INSTALL) $(DYN_LIB) $(ST_LIB) $(DESTDIR)$(libdir)
ifeq ($(UNAME), Darwin)
	@cd $(DESTDIR)$(libdir); ln -sf $(SONAME) $(NAME).dylib
else
	@cd $(DESTDIR)$(libdir); ln -sf $(SONAME) $(NAME).so
endif
	install -d $(DESTDIR)$(includedir)
	cp -a $(INC_DIR)/* $(DESTDIR)$(includedir)

uninstall:
	@rm -f $(DESTDIR)$(libdir)/$(SONAME) $(DESTDIR)$(libdir)/$(NAME).so $(DESTDIR)$(libdir)/$(NAME).dylib $(DESTDIR)$(libdir)/$(NAME).a
	@rm -rf $(DESTDIR)$(includedir)/sml
