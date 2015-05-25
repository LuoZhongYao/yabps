/****************************************************************************
FILE
        abcsp_panics.h  -  wire to an panic-reporting function

DESCRIPTION
	The macro ABCSP_PANIC() is used to route event information from the
	abcsp library to its environment.  This file defines the panic event
	codes used as arguments to ABCSP_PANIC().

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
*/

#ifndef __ABCSP_PANICS_H__
#define __ABCSP_PANICS_H__
 
#ifdef RCS_STRINGS
static const char abcsp_panics_h_id[]
  = "$Id: abcsp_panics.h,v 1.1 2001/07/06 20:11:03 cjo Exp $";
#endif


/* The abcsp library's transmit path checks that each message payload does
not exceed the limit #defined by ABCSP_MAX_MSG_LEN, set in config_txmsg.h.
This panic code is emitted if the limit is exceeded. */

#define ABCSP_PANIC_BCSP_MSG_OVERSIZE    ((unsigned)(1))


#endif  /* __ABCSP_PANICS_H__ */
