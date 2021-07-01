#pragma once


#ifdef MAXMSPSDK
    #include "ext.h"
    #include "ext_obex.h"
#else
    #include <string.h>
#endif


#ifdef MAXMSPSDK
    #define malloc(size) sysmem_newptrclear(size)
    #define realloc(size) sysmem_resizeptr(size)
#endif
