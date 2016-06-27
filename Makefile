
all:
	@$(MAKE) -C sml
	@$(MAKE) -C examples
	@$(MAKE) -C test

.PHONY: clean
clean :
	@$(MAKE) -C sml clean
	@$(MAKE) -C examples clean
	@$(MAKE) -C test clean

.PHONY: install
install:
	@$(MAKE) -C sml install
