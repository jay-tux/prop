INSTALL_LOC=/usr/include/prop

test:
	cd test/ && make runtest

install:
	install -d $(INSTALL_LOC)
	install -m 644 inc/event.hpp $(INSTALL_LOC)/
	install -m 644 inc/property.hpp $(INSTALL_LOC)/

coverage:
	cd test/ && make coverage

.PHONY: test install
