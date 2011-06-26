LIB = libgpi-plugin.so
LIB_OBJS = gpi-plugin.o intercepted.o
GECKO_HEADER = $(GECKO_SDK)/include/npfunctions.h $(GECKO_SDK)/include/npapi.h

AM_CFLAGS = -Ixulrunner-sdk/include -DXP_UNIX
AM_LDFLAGS = -ldl

GECKO_SDK = xulrunner-sdk
GECKO_SDK_URL = http://releases.mozilla.org/pub/mozilla.org/xulrunner/releases/1.9.2.18/sdk/xulrunner-1.9.2.18.en-US.linux-i686.sdk.tar.bz2
GECKO_SDK_PACK = download/xulrunner-sdk.tar.bz2

moduledir = $(libdir)
module_SCRIPTS = $(LIB)
bin_PROGRAMS = gpi-plugin-info
dist_bin_SCRIPTS = gpi-plugin-config

gpi_plugin_info_SOURCES = gpi-plugin-info.c
nodist_gpi_plugin_info_SOURCES = $(GECKO_HEADER)
EXTRA_DIST = gpi-plugin.c intercepted.c gpi-plugin.h

$(GECKO_SDK_PACK): download
	wget $(GECKO_SDK_URL) -O $@

download:
	mkdir -p $@

$(GECKO_SDK):
	$(MAKE) $(GECKO_SDK_PACK)
	tar xjf $(GECKO_SDK_PACK)

$(GECKO_HEADER): $(GECKO_SDK)

$(LIB): $(LIB_OBJS) $(GECKO_HEADER)
	$(CC) $(LDFLAGS) $(LIB_OBJS) -shared -Wl,-soname,$@ -o $@

clean-local:
	-rm -rf $(LIB)
