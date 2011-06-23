#include "gpi-plugin.h"

static NPError GetURLNotify(NPP instance, const char* url,
                            const char* window, void* notifyData) {
    return browser.geturlnotify(instance, url, window, notifyData);
}

static NPError PostURLNotify(NPP instance, const char* url,
                             const char* window, uint32 len,
                             const char* buf, NPBool file,
                             void* notifyData) {
    return browser.posturlnotify(instance, url, window,
                                 len, buf, file, notifyData);
}

NPNetscapeFuncs int_browser = {
    .geturlnotify = &GetURLNotify,
    .posturlnotify = &PostURLNotify,
};
