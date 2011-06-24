#include <string.h>
#include <dlfcn.h>
#include <libgen.h>
#include <npapi.h>
#include <npupp.h>

#include "gpi-plugin.h"

static char* (*pNP_GetMIMEDescription)(void) = 0;
static NPError (*pNP_Initialize)(NPNetscapeFuncs*, NPPluginFuncs*) = 0;
static NPError (*pNP_Shutdown)(void) = 0;
static NPError (*pNP_GetValue)(void *future,
                               NPPVariable aVariable, void *aValue) = 0;

static void *plugin_handle = 0;

NPPluginFuncs plugin;
NPNetscapeFuncs browser;

typedef struct {
    const char      *dli_fname;
    void            *dli_fbase;
    const char      *dli_sname;
    void            *dli_saddr;
} Dl_info;

void __attribute__ ((constructor)) gpi_init(void) {
    static char path[256], fname[256];
    Dl_info info;

    if (!dladdr(gpi_init, &info))
        return;

    strcpy(path, GPI_PLUGIN_PATH);
    strcpy(fname, info.dli_fname);
    strcat(path, basename(fname));
    plugin_handle = dlopen(path, RTLD_LAZY);

    if (plugin_handle) {
        pNP_GetMIMEDescription = dlsym(plugin_handle, "NP_GetMIMEDescription");
        pNP_Initialize = dlsym(plugin_handle, "NP_Initialize");
        pNP_Shutdown = dlsym(plugin_handle, "NP_Shutdown");
        pNP_GetValue = dlsym(plugin_handle, "NP_GetValue");
    }
}

void __attribute__ ((destructor)) gpi_fini(void) {
    if (plugin_handle) {
        dlclose(plugin_handle);
        plugin_handle = 0;
    }
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
