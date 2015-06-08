/****************************************************************************
FILE
        le.h  -  bcsp link establishment

CONTAINS
        abcsp_bcsple_init  -  initialise the link establishment engine
        abcsp_bcsple_newmsg  -  create a bcsp-le message
        abcsp_bcsple_putbyte  -  add a byte to a received bcsp-le message
        abcsp_bcsple_flush  -  push outstanding bytes back to bcsp-le message
        abcsp_bcsple_done  -  signal that bcsp-le message is complete
        abcsp_bcsple_destroy  -  abandon construction of bcsp-le message

DESCRIPTION
        This code block implements the majority of the BCSP Link
        Establishment engine, described in bc01-s-010e (aka AN005).

        This code block's interface is very similar to that presented
        by rxmsg.h and rxmsgdmux.h.  If somebody (daft) wishes not to
        use BCSP-LE or to write their BCSP-LE engine above rxmsg.c then
        it should be simple to snip this file from the build.

MODIFICATION HISTORY
        1.1  6:jul:01   cjo     Created.
*/

#ifndef __LE_H__
#define __LE_H__

#ifdef RCS_STRINGS
static const char le_h_id[]
  = "$Id: le.h,v 1.1 2001/07/06 19:15:35 cjo Exp $";
#endif

#include "abcsp_private.h"

/* BCSP-LE messages are all fixed-format. */
extern const uint8 abcsp_le_sync_msg[];
extern const uint8 abcsp_le_syncresp_msg[];
extern const uint8 abcsp_le_conf_msg[];
extern const uint8 abcsp_le_confresp_msg[];


/****************************************************************************
NAME
        abcsp_bcsple_init  -  initialise the link establishment engine

FUNCTION
        Initialises the BCSP Link Establishment engine, abandoning any
        work in progress.

        This must be called before all other functions declared in this
        file.

        This may be called at any time to reinitialise the engine.
*/

extern void abcsp_bcsple_init(void);


/****************************************************************************
NAME
        abcsp_bcsple_newmsg  -  create a bcsp-le message

FUNCTION
        Destroys any BCSP-LE message currently being assembled.

        Creates a fresh message.

RETURNS
        TRUE if the fresh message was created, else FALSE.
*/

extern bool abcsp_bcsple_newmsg(void);


/****************************************************************************
NAME
        abcsp_bcsple_putbyte  -  add a byte to a received bcsp-le message

RETURNS
        TRUE if the assembler has consumed the byte "c" and wishes to
        be given another byte.  FALSE if the byte has not been consumed
        and the assembler does not wish to be called again immediately.
*/

extern bool abcsp_bcsple_putbyte(uint8 c);


/****************************************************************************
NAME
        abcsp_bcsple_flush  -  push outstanding bytes back to bcsp-le message

FUNCTION
        This is just a stub.
*/

extern void abcsp_bcsple_flush(void);


/****************************************************************************
NAME
        abcsp_bcsple_done  -  signal that bcsp-le message is complete

FUNCTION
        Marks the message currently being assembled as complete and 
        delivers it.
*/

extern void abcsp_bcsple_done(void);


/****************************************************************************
NAME
        abcsp_bcsple_destroy  -  abandon construction of bcsp-le message

FUNCTION
        Destroys any message currently being assembled.
*/

extern void abcsp_bcsple_destroy(void);


#endif /* __LE_H__ */
