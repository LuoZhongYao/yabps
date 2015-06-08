/****************************************************************************
FILE
        chw.h  -  hardware specific stuff

DESCRIPTION
        This should be #included by every C source file in the abcsp
	library.

        This file maps the code's common types to the local machine; it
        will need to be set for each target.

        This file should be changed only infrequently and with great care.

        This file should contain the minimum necessary to get the job done;
	it must not become a dumping ground for quick-fix globals.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Ripped off from the bc01's btchiphw.h.
        1.2   30:jul:01  sm     Added Cygwin support.
        1.3   30:jul:01  cjo    Updated comments.
*/

#ifndef __CHW_H__
#define __CHW_H__
 
#ifdef RCS_STRINGS
static const char chw_h_id[]
  = "$Id: chw.h,v 1.4 2002/04/08 11:19:21 sm Exp $";
#endif

#if defined(__linux__)
#include <stdint.h>

typedef int8_t  int8;
typedef int16_t  int16;
typedef int32_t  int32;

typedef uint8_t  uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;

#else /* linux/386 */
#if defined(__sun) && defined(__sparc) && defined(__SVR4)

typedef signed char     int8;
typedef short           int16;
typedef long            int32;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned long   uint32;

#else /* sun/sparc/SVR4 */
#if defined(__CYGWIN__)

typedef signed char     int8;
typedef short           int16;
typedef long            int32;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned long   uint32;

#else /* cygwin */
#if defined(WIN32)

typedef signed char     int8;
typedef short           int16;
typedef long            int32;

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned long   uint32;

#else /* windows */
#error "must define machine type"

#endif /* windows */
#endif /* cygwin */
#endif /* sun/sparc/SVR4 */
#endif /* linux/386 */


#endif  /* __CHW_H__ */
