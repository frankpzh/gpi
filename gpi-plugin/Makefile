LIB = libgpi-plugin.so
EXECS = gpi-plugin-info
OBJS = gpi-plugin.o intercepted.o

CFLAGS = -Ixulrunner-sdk/include -DXP_UNIX
LDFLAGS = -ldl

GECKO_SDK = xulrunner-sdk
GECKO_SDK_URL = http://releases.mozilla.org/pub/mozilla.org/xulrunner/releases/1.9.2.18/sdk/xulrunner-1.9.2.18.en-US.linux-i686.sdk.tar.bz2
GECKO_SDK_PACK = download/xulrunner-sdk.tar.bz2

.PHONY: all
all: $(GECKO_SDK) $(LIB) $(EXECS)

$(LIB): $(OBJS)
	$(CC) $(LDFLAGS) $^ -shared -o $@

$(GECKO_SDK_PACK): download
	wget $(GECKO_SDK_URL) -O $@

download:
	mkdir -p $@

$(GECKO_SDK):
	$(MAKE) $(GECKO_SDK_PACK)
	tar xjf $(GECKO_SDK_PACK)

.PHONY: clean
clean:
	rm -rf $(LIB) $(OBJS) $(EXECS)
