LIB = libgpi-plugin.so
OBJS = gpi-plugin.o

CFLAGS = -Igecko-sdk/include -DXP_UNIX
LDFLAGS = -ldl

GECKO_SDK_URL = ftp://ftp.mozilla.org/pub/mozilla.org/xulrunner/releases/1.8.0.4/sdk/gecko-sdk-i686-pc-linux-gnu-1.8.0.4.tar.bz2
GECKO_SDK_PACK = download/gecko-sdk.tar.bz2

.PHONY: all
all: $(LIB)

$(LIB): $(OBJS)
	$(CC) $(LDFLAGS) $^ -shared -o $@

$(GECKO_SDK_PACK): dldir
	wget $(GECKO_SDK_URL) -O $@

.PHONY: dldir
dldir:
	mkdir -p download

.PHONY: gecko-sdk
gecko-sdk: $(GECKO_SDK_PACK)
	tar xjf $^

.PHONY: clean
clean:
	rm -rf $(LIB) $(OBJS)
