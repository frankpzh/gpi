#include <string.h>
#include <dlfcn.h>
#include <npapi.h>
#include <npupp.h>

char* (*pNP_GetMIMEDescription)(void);
NPError (*pNP_Initialize)(NPNetscapeFuncs*, NPPluginFuncs*);
NPError (*pNP_Shutdown)(void);
NPError (*pNP_GetValue)(void *future, NPPVariable aVariable, void *aValue);

void *plugin_handle = 0;
NPPluginFuncs plugin;
NPNetscapeFuncs browser, int_browser;

void gpi_init() {
    plugin_handle = dlopen("./libflashplayer.so", RTLD_LAZY);
    pNP_GetMIMEDescription = dlsym(plugin_handle, "NP_GetMIMEDescription");
    pNP_Initialize = dlsym(plugin_handle, "NP_Initialize");
    pNP_Shutdown = dlsym(plugin_handle, "NP_Shutdown");
    pNP_GetValue = dlsym(plugin_handle, "NP_GetValue");
}

NPError NPN_GetURLNotify(NPP instance, const char* url,
                         const char* window, void* notifyData) {
    return browser.geturlnotify(instance, url, window, notifyData);
}

NPError NPN_PostURLNotify(NPP instance, const char* url,
                          const char* window, uint32 len,
                          const char* buf, NPBool file,
                          void* notifyData) {
    return browser.posturlnotify(instance, url, window,
                                 len, buf, file, notifyData);
}

#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_Initialize(NPNetscapeFuncs *npnf, NPPluginFuncs *nppfuncs)
{
    NPError ret;

    if (!plugin_handle)
        gpi_init();
    memcpy(&browser, npnf, sizeof(NPNetscapeFuncs));
    memcpy(&int_browser, npnf, sizeof(NPNetscapeFuncs));
    int_browser.geturlnotify = &NPN_GetURLNotify;
    int_browser.posturlnotify = &NPN_PostURLNotify;
    ret = pNP_Initialize(&int_browser, nppfuncs);
    memcpy(&plugin, nppfuncs, sizeof(NPPluginFuncs));
    return ret;
}

NPError
OSCALL NP_Shutdown() {
    NPError ret;

    ret = pNP_Shutdown();
    dlclose(plugin_handle);
    plugin_handle = 0;
    return ret;
}

char *
NP_GetMIMEDescription(void) {
    if (!plugin_handle)
        gpi_init();
    return pNP_GetMIMEDescription();
}

NPError OSCALL
NP_GetValue(void *npp, NPPVariable variable, void *value) {
    if (!plugin_handle)
        gpi_init();
    return pNP_GetValue(npp, variable, value);
}

#ifdef __cplusplus
}
#endif
