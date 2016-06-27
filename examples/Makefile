UNAME := $(shell uname)
CFLAGS += -I../sml/include/ -g -std=c99 -Wall -Wextra -pedantic
OBJS = sml_server.o sml_parser.o
LIBSML = ../sml/lib/libsml.a

ifeq ($(UNAME), Linux)
LIBS = -luuid
endif

default: sml_server sml_parser

sml_server : $(OBJS) $(LIBSML)
	$(CC) $(CFLAGS) sml_server.o $(LIBS) $(LIBSML) -o sml_server $(LIBS)

sml_parser: $(LIBSML) $(OBJS)
	$(CC) $(CFLAGS) sml_parser.o $(LIBS) $(LIBSML) -o sml_parser $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	@rm -f *.o
	@rm -f sml_server sml_parser
