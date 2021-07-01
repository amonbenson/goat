/**
 * @file mem.h
 * @author Thomas Resch <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * University of Applied Sciences Nordwestschweiz (FHNW), Music-Academy, Research and Development <br>
 * Tools for calculating convolution based virtual acoustics (mainly dynamic binaural synthesis) <br>
 * <br>
 * @brief Utilties for dynamic memory allocation <br>
 * <br>
 * Wrapper for memory allocation
 * Max/MSP SDK suggests using the Max/MSP "sysmem_" - routines
 * instead of malloc/calloc/free
 * So for Max/MSP define the Preprocessor macro "MAXMSPSDK"
 */

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
