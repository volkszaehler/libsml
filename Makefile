
VERSION=$(shell dpkg-parsechangelog --show-field Version 2>/dev/null)

all:
	@$(MAKE) -C sml
	@$(MAKE) -C examples
	@$(MAKE) -C test

.PHONY: clean install uninstall version
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

version:
	sed -i 's#"version":.*#"version": "${VERSION}"#' library.json 
	sed -i 's#"Version:.*#Version: ${VERSION}#' sml.pc
	# We set a preliminary tag. Since sml.pc and library.json are not yet 
	# commited this can not be final  
	git tag -f "v${VERSION}"

