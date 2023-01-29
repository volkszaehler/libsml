
all:
	@$(MAKE) -C sml
	@$(MAKE) -C examples
	@$(MAKE) -C test

.PHONY: clean install uninstall
clean:
	@$(MAKE) -C sml clean
	@$(MAKE) -C examples clean
	@$(MAKE) -C test clean

install: all
	@$(MAKE) -C sml install
	@$(MAKE) -C examples install

uninstall:
	@$(MAKE) -C sml uninstall
	@$(MAKE) -C examples uninstall
