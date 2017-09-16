UNAME := $(shell uname)
CFLAGS += -I../sml/include/ -g -std=c99 -Wall -Wextra -pedantic
OBJS = sml_server.o
LIBSML = ../sml/lib/libsml.a

ifneq ($(UNAME), Darwin)
LIBS = -luuid
endif

sml_server : $(OBJS) $(LIBSML)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LIBSML) -o sml_server $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	@rm -f *.o
	@rm -f sml_server
