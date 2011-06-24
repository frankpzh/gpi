#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <libgen.h>
#include <npapi.h>
#include <npfunctions.h>

static NPError (*Initialize)(NPNetscapeFuncs*, NPPluginFuncs*) = 0;
static NPError (*Shutdown)(void) = 0;
static char* (*GetMIMEDescription)(void) = 0;
static NPError (*GetValue)(void *instance,
                           NPPVariable aVariable, void *aValue) = 0;

const char* UserAgent(NPP instance) {
    return "";
}

void* MemAlloc(uint32_t size) {
    return malloc(size);
}

void MemFree(void* ptr) {
    free(ptr);
}

NPPluginFuncs plugin;
NPNetscapeFuncs browser = {
    .size = sizeof(browser),
    .version = 14,
    .uagent = UserAgent,
    .memalloc = MemAlloc,
    .memfree = MemFree,
};

int main(int argc, char **argv) {
    void *plugin_handle;
    char *ret;

    if (argc != 2)
        goto failed;
    if (!(plugin_handle = dlopen(argv[1], RTLD_LAZY)))
        goto failed;
    if (!(Initialize = dlsym(plugin_handle, "NP_Initialize")))
        goto failed;
    if (!(Shutdown = dlsym(plugin_handle, "NP_Shutdown")))
        goto failed;
    if (!(GetMIMEDescription = dlsym(plugin_handle, "NP_GetMIMEDescription")))
        goto failed;
    if (!(GetValue = dlsym(plugin_handle, "NP_GetValue")))
        goto failed;

    printf("File Name: %s\n", basename(argv[1]));
    if (GetValue(plugin_handle, NPPVpluginNameString, (void *)&ret)
        == NPERR_NO_ERROR)
        printf("Plugin Name: %s\n", ret);
    if (GetValue(plugin_handle, NPPVpluginDescriptionString, (void *)&ret)
        == NPERR_NO_ERROR)
        printf("Plugin Description: %s\n", ret);
    printf("\n");

    return 0;

  failed:
    fprintf(stderr, "Usage: %s <plugin path>\n\n", argv[0]);
    return 1;
}
