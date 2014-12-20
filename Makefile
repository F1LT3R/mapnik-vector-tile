BUILDTYPE ?= Release

MAPNIK_PLUGINDIR := $(shell mapnik-config --input-plugins)

.PHONY: all
all: libvtile

./deps/gyp:
	git clone --depth 1 https://chromium.googlesource.com/external/gyp.git ./deps/gyp

#.PHONY: build/Makefile
build/Makefile: ./deps/gyp build.gyp
	deps/gyp/gyp build.gyp --depth=. -DMAPNIK_PLUGINDIR=\"$(MAPNIK_PLUGINDIR)\" -Goutput_dir=. --generator-output=./build -f make

.PHONY: libvtile
libvtile: build/Makefile
	$(MAKE) -C build/ BUILDTYPE=$(BUILDTYPE) V=$(V)

clean:
	rm -rf ./build

test: libvtile
	./build/Release/tests

.PHONY: test
