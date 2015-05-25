#ifndef __CONFIG_TXMSG_H__
#define __CONFIG_TXMSG_H__

/****************************************************************************
	
   				? Cambridge Silicon Radio Ltd, 2004
 	 			
				All rights reserved

FILE:			config_txmsg.h  -  wire to the environment's tx message support

CONTAINS:		ABCSP_UART_GETTXBUF  -  obtain buffer for uart output
				ABCSP_UART_SENDBYTES  -  pass a block of bytes to the output uart
				ABCSP_TXMSG_INIT_READ  -  initialise reading a bcsp transmit message
				ABCSP_TXMSG_LENGTH  -  how long is a transmit message
			    ABCSP_TXMSG_GETBUF  -  access raw message bytes in a message
				ABCSP_TXMSG_TAKEN  -  tell message how many bytes have been read
				ABCSP_TXMSG_DONE  -  signal that message has been delivered

DESCRIPTION:	The environment is required to provide a set of functions to support
				the reading of outbound messages.

				The abcsp library knows nothing about the structure of the messages
				passed into the top of the bcsp stack - it deals only with message
				references.  Each message reference is used to access external
				functions that support the reading of the higher layer code's
				messages.

				The abcsp library treats the higher layer code's messages in a manner
				analogous to the way a unix program treats files: a message reference
				takes the role of a unix file handle and the abcsp library reads
				message contents via the reference/handle.  This approach pushes all
				knowledge of the structure of higher layer code messages outside the
				abcsp library, including knowledge of how the messages' storage is
				managed.

				This file configures the binding between the abcsp library and
				environment functions that read messages, i.e., higher-level messages
				are passed into abcsp_sendmsg(), are read by functions described in
				this file to translate their contents into a BCSP UART byte stream,
				and are then passed to the UART via ABCSP_UART_SENDBYTES().

				The ABCSP_TXMSG_INIT_READ() function tells external code that it is
				about to start reading raw message bytes from the start of a
				message. The abcsp library calls ABCSP_TXMSG_LENGTH() to find the
				length of a message.  Pairs of calls to ABCSP_TXMSG_GETBUF() and
				ABCSP_TXMSG_TAKEN() take raw higher-level message bytes from the
				message.   A call to ABCSP_TXMSG_DONE() tells the message constructor
				that it has finished with the message.

				The ABCSP_TXMSG_DONE() function indicates that the abcsp library has
				completed all of its work with the message.  This may be used by
				external code to destroy the message's resources.  It may also form
				the basis of a "recorded delivery" mechanism:

				- for reliable BCSP channels it indicates that the peer BCSP stack
				  has acknowledged reception of the message.
				- for unreliable BCSP channels it indicates that all of the
				  message has been passed to the local UART for transmission.

				External code normally drives the abcsp library's transmit path with
				code roughly of the form:

                abcsp_sendmsg()
                while(there's work to do)
                abcsp_pumptxmsgs();

				This causes the abcsp library code to make calls of the following
				(simplified) form:

                ABCSP_TXMSG_INIT_READ()
                ABCSP_TXMSG_LENGTH()
                while(ABCSP_TXMSG_GETBUF() obtains a buffer)
                read from the buffer and write slipped bytes to the uart

                ABCSP_TXMSG_TAKEN()
                if(message is reliable)
                wait for peer bcsp stack to acknowledge reception
                ABCSP_TXMSG_DONE()

				A given message may need to be read more than once, e.g., because the
				peer BCSP stack fails to acknowledge reception.  This will normally
				provoke extra calls to abcsp_pumptxmsgs() via ABCSP_START_BCSP_TIMER().

				The abcsp library needs to handle multiple ABCSP_TXMSGs at a time, so
				the functions in this file must support a set of messages.  The
				number of messages depends on settings within this file.

				This file holds no definitions for functions in the abcsp library; it
				holds only descriptions of macros that must be supplied by the
				surrounding code.

REVISION:		$Revision: #1 $ 

*************************************************************************************/

#include "abcsp.h" 

#ifdef __cplusplus
extern "C" {
#endif

/* Use #defines rather than typedefs to aid portability.  The abcsp code
treats these types as opaque references, so void* is appropriate. */

/* If ABCSP_TXCRC is #defined then the optional CRC field is appended to each
BCSP message transmitted, else the CRC is not appended. Do NOT define in this
file, must be a global definition, i.e. give it as argument to the compiler */
#define ABCSP_TXCRC

/* The size of the BCSP transmit window.  This must be between 1 and 7.  This
is normally set to 4.  This is called "winsiz" in the BCSP protocol
specification.

This determines the number of BCSP messages that can be handled by the abcsp
library's transmit path at a time, so it affects the storage requirements for
ABCSP_TXMSG messages. */

#define ABCSP_TXWINSIZE          (4)
#define ABCSP_MAX_MSG_LEN       (300)



/****************************************************************************
NAME
        ABCSP_UART_GETTXBUF  -  obtain buffer for uart output

SYNOPSIS
        char *ABCSP_UART_GETTXBUF(abcsp *_this);

FUNCTION
        Obtains a buffer into which to write UART output bytes.

        When the abcsp library is ready to write (slipped) bytes to
        the UART it calls this function to obtain a buffer.  If this
        function obtains a buffer the library writes to the buffer then
        calls ABCSP_UART_SENDBYTES().  This requires the UART to transmit
        the bytes written to the buffer.  It also returns ownership of
        the buffer to external code.

        The abcsp library guarantees not to hold more than one of these
        buffers at any one time.

RETURNS
        A buffer into which to write UART output bytes, or address zero
        (NULL) if no buffer is available.  The length of any buffer
        returned is written to the location pointed to by "bufsiz".
*/

#define ABCSP_UART_GETTXBUF(t)		abcsp_uart_gettxbuf(t)


/****************************************************************************
NAME
        ABCSP_UART_SENDBYTES  -  pass a block of bytes to the output uart

SYNOPSIS
        void ABCSP_UART_SENDBYTES(abcsp *_this, unsigned n);

FUNCTION
        Tells external code that it that must pass to the output UART the
        "n" bytes in the buffer "buf", obtained by the preceding call
        to ABCSP_UART_GETTXBUF().

        The call returns ownership of the buffer to the external code.
*/
#define ABCSP_UART_SENDBYTES(t,n)	abcsp_uart_sendbytes(t, n)

/****************************************************************************
NAME
        ABCSP_TXMSG_INIT_READ  -  initialise reading a bcsp transmit message

SYNOPSIS
        void ABCSP_TXMSG_INIT_READ(ABCSP_TXMSG *msg);

FUNCTION
	Tells the surrounding code that it wishes to start reading the
	message identified by "msg" from its start.

	The next call to ABCSP_TXMSG_GETBUF() is expected to obtain the first
	raw message bytes from "msg".
*/
#define ABCSP_TXMSG_INIT_READ(m)	abcsp_txmsg_init_read(m)

/****************************************************************************
NAME
        ABCSP_TXMSG_LENGTH  -  how long is a transmit message

SYNOPSIS
        unsigned ABCSP_TXMSG_LENGTH(ABCSP_TXMSG *msg);

RETURNS
        The number of bytes in the message "msg".
*/
#define ABCSP_TXMSG_LENGTH(m)		abcsp_txmsg_length(m)

/****************************************************************************
NAME
        ABCSP_TXMSG_GETBUF  -  access raw message bytes in a message

SYNOPSIS
        char *ABCSP_TXMSG_GETBUF(ABCSP_TXMSG *msg, unsigned *buflen);

RETURNS
	The address of a buffer containing the next raw message bytes to be
	read from "msg", or address zero (NULL) if all of the bytes have been
	read.

        If a buffer is returned its size is written at "buflen".
*/
#define ABCSP_TXMSG_GETBUF(m,l)		abcsp_txmsg_getbuf(m, l)

/****************************************************************************
NAME
        ABCSP_TXMSG_TAKEN  -  tell message how many bytes have been read

SYNOPSIS
        void ABCSP_TXMSG_TAKEN(ABCSP_TXMSG *msg, unsigned ntaken);

FUNCTION
        Tells surrounding code that the abcsp library has read "ntaken"
        bytes from the buffer obtained from the preceding call to
        ABCSP_TXMSG_GETBUF().
*/
#define ABCSP_TXMSG_TAKEN(m,n)	abcsp_txmsg_taken(m, n)

/****************************************************************************
NAME
        ABCSP_TXMSG_DONE  -  signal that message has been delivered

SYNOPSIS
        void ABCSP_TXMSG_DONE(ABCSP_TXMSG *msg);

FUNCTION
	Tells the surrounding code that the abcsp library has finished with
	"msg".  For unreliable messages this means it has been sent to the
	UART.  For reliable messages this means the peer BCSP stack has
	acknowledged reception of the message.
*/
#define ABCSP_TXMSG_DONE(m)	abcsp_txmsg_done(m)

#ifdef __cplusplus
}
#endif 

#endif  /* __CONFIG_TXMSG_H__ */
