#ifndef loader_win32_h
#define loader_win32_h


#ifdef AVIFILE
#ifdef __GNUC__
#include "avm_output.h"
#ifndef __cplusplus
#define printf(a, ...)  avm_printf("Win32 plugin", a, ## __VA_ARGS__)
#endif
#endif
#endif

extern void my_garbagecollection(void);





extern void* LookupExternal(const char* library, int ordinal);
extern void* LookupExternalByName(const char* library, const char* name);
extern int expRegisterClassA(const void/*WNDCLASSA*/ *wc);
extern int expUnregisterClassA(const char *className, HINSTANCE hInstance);

#endif
