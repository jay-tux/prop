INSTALL_LOC=/usr/include/prop

test:
	cd test/ && make runtest

install:
	install -d $(INSTALL_LOC)
	install -m 644 inc/event.hpp $(INSTALL_LOC)/
	install -m 644 inc/property.hpp $(INSTALL_LOC)/

coverage:
	cd test/ && make coverage

docs: Doxyfile docs/doxygen-awesome-css/Doxyfile
	doxygen
	cd docs/latex/ && make

docs/doxygen-awesome-css/Doxyfile:
	cd docs && git clone https://github.com/jothepro/doxygen-awesome-css

clean:
	rm docs/* -rf
	cd test && make clean

.PHONY: test install clean docs
