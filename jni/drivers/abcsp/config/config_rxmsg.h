#ifndef __CONFIG_RXMSG_H__
#define __CONFIG_RXMSG_H__

/****************************************************************************

   				� Cambridge Silicon Radio Ltd, 2004
 	 			
				All rights reserved

FILE:			config_rxmsg.h  -  wire to the environment's rx message support

CONTAINS:       ABCSP_DELIVERMSG  -  pass abcsp message to higher level code
				ABCSP_RXMSG_CREATE  -  create a abcsp receive message
				ABCSP_RXMSG_GETBUF  -  obtain storage to write to a message
				ABCSP_RXMSG_WRITE  -  write to a higher-level message
				ABCSP_RXMSG_COMPLETE  -  finish a higher-level message
			    ABCSP_RXMSG_DESTROY  -  destroy a abcsp receive message

DESCRIPTION:	The environment is required to provide a set of functions to support
				the construction of inbound messages.

				The abcsp library knows nothing about the structure of the messages
				passed out of the top of the abcsp stack - it deals only with message
				references.  Each message reference is used to access external
				functions that support the construction of the higher layer code's
				messages.

				The abcsp library treats the higher layer code's messages in a manner
				analogous to the way a unix program treats files: a message reference
				takes the role of a unix file handle and the abcsp library writes
				message contents via the reference/handle.  This approach pushes all
				knowledge of the structure of higher layer code messages outside the
				abcsp library, including knowledge of how the messages' storage is
				managed.

				This file configures the binding between the abcsp library and
				environment functions that construct messages, i.e., messages that
				are derived from the input BCSP UART byte stream and are are passed
				up into the host.  Typically these messages are passed into the lower
				interface of the l2cap layer.

				The ABCSP_RXMSG_CREATE() function creates a new, empty message.
				Pairs of calls to ABCSP_RXMSG_GETBUF() and ABCSP_RXMSG_WRITE() add
				raw higher-level message bytes to the new message.  A call to
				ABCSP_RXMSG_COMPLETE() tells the message constructor that the new
				message is complete.  This will always be followed by a call to
				ABCSP_DELIVERMSG() to pass the completed message to higher-level
				code.

				The normal pattern of calls for constructing and delivering a single
				BCSP message is:

                ABCSP_RXMSG_CREATE()
                while(message is being constructed from UART bytes)
                        if(ABCSP_RXMSG_GETBUF() obtains a buffer)
                                ABCSP_RXMSG_WRITE() to the buffer
                ABCSP_RXMSG_COMPLETE()
                ABCSP_DELIVERMSG()

				 The abcsp library constructs only a single message at a time.

				ABCSP_RXMSG_DESTROY() aborts construction of a message, returning all
				resources to the surrounding code.

				This file holds no prototypes for functions in the abcsp library; it
				holds only descriptions of macros that must be supplied by the
				surrounding code.

REVISION:		$Revision: #1 $ 

******************************************************************************/


/* Use #defines rather than typedefs to aid portability.  The abcsp code
treats these types as opaque references, so void* is appropriate. */

#include "abcsp_intf_hl.h" 

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ABCSP_RXMSG
#define ABCSP_RXMSG             void
#endif

/* The CRC field is optional on BCSP messages.  If ABCSP_RXCRC is #defined
then the CRC fields found on all received BCSP messages are checked.  If
ABCSP_RXCRC is not #defined then no received CRC field is checked; this means
that packets with invalid CRCs may be accepted as good messages. */

#define ABCSP_RXCRC


/* The maximum length of the payload field of a received BCSP packet.  

A check is made on the payload length field in received BCSP headers
and any that declare a payload length greater than this value are
consumed and discarded without bothering the ABCSP_RXMSG_*()
functions.  It is presumed the length value is invalid because of UART
line corruption.  If this value is #defined then its value must be set
with knowledge of the transmit packet length settings on the peer.
This length excludes the BCSP header and possible CRC.

If this value is not #defined then no check or limit on the length of packets
is imposed. */


/****************************************************************************
NAME
        ABCSP_DELIVERMSG  -  pass abcsp message to higher level code

SYNOPSIS
        void ABCSP_DELIVERMSG(abcsp *_this, ABCSP_RXMSG *msg, unsigned chan, unsigned rel);

FUNCTION
	Passes the BCSP message "msg" up to higher level code.  The message
	was received on BCSP channel "chan".  If the message was received on
	the reliable BCSP path then "rel" is 1, else it is 0.

	This function only passes a reference to the message to the higher
	level code.  The reference was obtained by a previous call to
	ABCSP_RXMSG_CREATE() - see config_rxmsg.h.  This call indicates that
	the abcsp library passes all control of the message back to external
	code.
*/
#define ABCSP_DELIVERMSG(m,c,r)	//hcCom_ReceiveMsg(m, c, r)

/****************************************************************************
NAME
        ABCSP_RXMSG_CREATE  -  create a abcsp receive message

SYNOPSIS
        ABCSP_RXMSG *ABCSP_RXMSG_CREATE((abcsp *_this, unsigned len);

FUNCTION
	Creates a new, empty message and returns its reference.  This
	reference, analogous to a unix file handle, will be used for all
	subsequent operations on the message.

	When the new message is complete its length will be "len" bytes.  The
	function may use the value to allocate resources ready for
	ABCSP_RXMSG_GETBUF() and ABCSP_RXMSG_WRITE() calls.

	The contents of the message must not be changed by external code
	until either the message is delivered to higher layer code via
	ABCSP_DELIVERMSG() or until the message is destroyed via
	ABCSP_RXMSG_DESTROY().

RETURNS
        A reference for a freshly-created, empty message, or address zero
        (NULL) if the reference could not be created.
*/

#define ABCSP_RXMSG_CREATE(l)		(ABCSP_RXMSG *) abcsp_rxmsg_create(l)


/****************************************************************************
NAME
        ABCSP_RXMSG_GETBUF  -  obtain storage to write to a message

SYNOPSIS
        char *ABCSP_RXMSG_GETBUF(ABCSP_RXMSG *msg, unsigned *bufsiz);

FUNCTION
	Obtains a buffer into which to write raw bytes of the higher layer
	message being constructed on "msg".

RETURNS
	The address of a buffer, or zero (NULL) if no buffer could be
	provided.

        If a buffer is provided its size is written at "bufsiz".
*/
#define ABCSP_RXMSG_GETBUF(m,n)		abcsp_rxmsg_getbuf(m, n)
/****************************************************************************
NAME
        ABCSP_RXMSG_WRITE  -  write to a higher-level message

SYNOPSIS
        void ABCSP_RXMSG_WRITE(ABCSP_RXMSG *msg, char *buf, unsigned n);

FUNCTION
	Appends the "n" bytes in the buffer "buf" to the end of the message
	"msg".

	This call passes control of "buf" back to "msg"; the abcsp library
	code no longer has any direct interest in it.

        The buffer was obtained by a preceding call to ABCSP_RXMSG_GETBUF().
*/

#define ABCSP_RXMSG_WRITE(m,b,n)	abcsp_rxmsg_write(m, b, n)

/****************************************************************************
NAME
        ABCSP_RXMSG_COMPLETE  -  finish a higher-level message

SYNOPSIS
        void ABCSP_RXMSG_COMPLETE(abcsp *_this, ABCSP_RXMSG *msg);

FUNCTION
	Tells the receive message construction code that the message "msg" is
	complete and that no more calls to ABCSP_RXMSG_GETBUF() and
	ABCSP_RXMSG_WRITE() will be made on it.
*/

#define ABCSP_RXMSG_COMPLETE(m)		abcsp_rxmsg_complete(m)

/****************************************************************************
NAME
        ABCSP_RXMSG_DESTROY  -  destroy a abcsp receive message

SYNOPSIS
        void ABCSP_RXMSG_DESTROY(abcsp *_this, ABCSP_RXMSG *m);

FUNCTION
        Destroys the message identified by the message reference "m".

	All resources identified by "m" are no longer of interest to the
	abcsp library.
*/

#define ABCSP_RXMSG_DESTROY(m)	AbcspRxMmsgDestroy(m)

#ifdef __cplusplus
}
#endif 

#endif  /* __CONFIG_RXMSG_H__ */
