#include <string.h>
#include <dlfcn.h>
#include <npapi.h>
#include <npupp.h>

#include "gpi-plugin.h"

static char* (*pNP_GetMIMEDescription)(void);
static NPError (*pNP_Initialize)(NPNetscapeFuncs*, NPPluginFuncs*);
static NPError (*pNP_Shutdown)(void);
static NPError (*pNP_GetValue)(void *future,
                               NPPVariable aVariable, void *aValue);

static void *plugin_handle = 0;

NPPluginFuncs plugin;
NPNetscapeFuncs browser;

void __attribute__ ((constructor)) gpi_init(void) {
    plugin_handle = dlopen("./libflashplayer.so", RTLD_LAZY);
    pNP_GetMIMEDescription = dlsym(plugin_handle, "NP_GetMIMEDescription");
    pNP_Initialize = dlsym(plugin_handle, "NP_Initialize");
    pNP_Shutdown = dlsym(plugin_handle, "NP_Shutdown");
    pNP_GetValue = dlsym(plugin_handle, "NP_GetValue");
}

void __attribute__ ((destructor)) gpi_fini(void) {
    dlclose(plugin_handle);
    plugin_handle = 0;
}

NPError OSCALL
NP_Initialize(NPNetscapeFuncs *npnf, NPPluginFuncs *nppfuncs)
{
    int i;
    NPError ret;

    memcpy(&browser, npnf, sizeof(NPNetscapeFuncs));
    for (i = 0; i < browser.size - 4; i += sizeof(void *)) {
        void **bfunc, **int_bfunc;
        bfunc = (void **)((unsigned long)&browser + 4 + i * sizeof(void *));
        int_bfunc = (void **)((unsigned long)&int_browser + 4 + i * sizeof(void *));
        if (*int_bfunc == 0)
            *int_bfunc = *bfunc;
    }

    ret = pNP_Initialize(&int_browser, nppfuncs);

    memcpy(&plugin, nppfuncs, sizeof(NPPluginFuncs));

    return ret;
}

NPError
OSCALL NP_Shutdown() {
    return pNP_Shutdown();
}

char *
NP_GetMIMEDescription(void) {
    return pNP_GetMIMEDescription();
}

NPError OSCALL
NP_GetValue(void *npp, NPPVariable variable, void *value) {
    return pNP_GetValue(npp, variable, value);
}
