/****************************************************************************
FILE
        abcsp.h  -  another bcsp implementation

CONTAINS
        abcsp_init  -  initialise abcsp library
        abcsp_sendmsg  -  set up message for sending to the uart
        abcsp_pumptxmsgs  -  send message to the uart
        abcsp_uart_deliverbytes  -  push received uart bytes into the library
        abcsp_bcsp_timed_event  -  transmit path's timed event
        abcsp_tshy_timed_event  -  report a tshy timeout event to the fsm
        abcsp_tconf_timed_event  -  report a tconf timeout event to the fsm

DESCRIPTION
        The abcsp library provides an implementation of the BCSP protocol,
        described in bc01-s-006e (aka AN004) and an implementation of
        the associated BCSP Link Establishment protocol, described in
        bc01-s-010e (aka AN005).

        This file describes the abcsp library's main external interfaces.

        The code's primary task is to translate between BCSP wire (UART)
        format messages and the corresponding higher layer format messages
        such as HCI ACL, SCO and CMD/EVT:
                        
                abcsp_sendmsg()
                abcsp_pumptxmsgs()        ABCSP_DELIVERMSG()
                        |                       ^
                        v                       |
                ------------------------------------------
                |                                        |
                |               abcsp library            | <- abcsp_init()
                |                                        |
                ------------------------------------------
                        |                       ^
                        v                       |
             ABCSP_UART_SENDBYTES()   abcsp_uart_deliverbytes()

        The library must first be initialised by a call to abcsp_init().

        Transmit path:
	    To send a message, higher layer code calls abcsp_sendmsg(); this
	    places the message into a queue within the library.  The higher
	    layer code then repeatedly calls abcsp_pumptxmsgs() to translate
	    the message into its BCSP wire format and push these bytes out of
	    the bottom of the library via ABCSP_UART_SENDBYTES().

        Receive path:
	    For inbound messages the UART driver code passes BCSP wire format
	    bytes into the library via calls to abcsp_uart_deliverbytes().
	    When the library has all of the bytes to form a complete higher
	    layer message it calls ABCSP_DELIVERMSG() to pass this to higher
	    layer code.

        Except for the library's initialisation call, abcsp_init(), the code
        blocks implementing the transmit and receive paths are largely
        independent.

	In the above diagram, function names in lower case are part of the
	library code.  Function names in upper case are macros within the
	code - the external environment must implement these according to the
	descriptions given in this file.

	The library contains no internal scheduler; it depends entirely on
	the above function calls to drive the code.  The transmit path is
	driven ("down") by calls to abcsp_sendmsg() and abcsp_pumptxmsgs();
	these result in calls to ABCSP_UART_SENDBYTES().  Similarly, the
	receive path is driven ("up") entirely by calls to
	abcsp_uart_deliverbytes(), resulting in calls to ABCSP_DELIVERMSG().

	A key point about scheduling: the paths' two output functions,
	ABCSP_UART_SENDBYTES() and ABCSP_DELIVERMSG(), have no means of
	detecting failure to deliver their payloads.  Consequently the
	surrounding code must not call the input functions, abcsp_sendmsg(),
	abcsp_pumptxmsgs() and abcsp_uart_deliverbytes(), unless they know
	the surrounding code can accept the payload of one call to the
	corresponding output function.

	An associated point: a call to one of the input functions makes, at
	most, one call to the corresponding output function, thus the
	surrounding code only needs to be sure that the corresponding output
	function can accept one payload.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Created.
        1.2   29:jan:03  cjo    Corrected comment.
*/

#ifndef __ABCSP_H__
#define __ABCSP_H__
 
#ifdef RCS_STRINGS
static const char abcsp_h_id[]
  = "$Id: abcsp.h,v 1.2 2003/01/29 21:54:17 cjo Exp $";
#endif

typedef struct {
    char *buf;
    unsigned   buflen;
    unsigned   dex;
}MessageStructure;


/* Use #defines rather than typedefs to aid portability.  The abcsp code
treats these types as opaque references, so void* is appropriate. */

#define ABCSP_TXMSG             void
#define ABCSP_RXMSG             void


/****************************************************************************
NAME
        abcsp_init  -  initialise abcsp library

FUNCTION
	Initialises the state of the abcsp library.  This resets all of its
	internal state, including FREE()ing held memory, cancelling any
	pending timed event timers and aborting the transfer of any messages
	in progress.

	This must be called before all other functions in the abcsp library,
	including all of the abcsp functions described in this file.

        This may be called at any time to reinitialise the state of the
        library.
*/

extern void abcsp_init(void);


/****************************************************************************
NAME
        abcsp_sendmsg  -  set up message for sending to the uart

FUNCTION
	Passes the message "msg" into the library ready for transmission.
	The actual message translation and transmission is performed by
	abcsp_pumptxmsgs(), so this call really just parks "msg" within the
	library.

	The message is sent on BCSP channel "chan".  If "rel" is zero then it
	is sent as an unreliable datagram, else it is sent as a reliable
	datagram.

	Ownership of the memory referenced via "msg" is retained by the
	caller.  The caller must not alter the contents of "msg" until the
	abcsp library signals that it has completed its transmission (by
	calling ABCSP_TXMSG_DONE()) or until the library has been
	reinitialised.

RETURNS
        1 if the message was accepted for transmission, else 0.

	The function returns 0 if the initialisation function has not been
	called, if message transmission is currently being prevented (choke),
	if any argument is unacceptable or if the queue that would accept the
	message is full.
*/

extern unsigned abcsp_sendmsg(ABCSP_TXMSG *msg, unsigned chan, unsigned rel);


/****************************************************************************
NAME
        abcsp_pumptxmsgs  -  send message to the uart

FUNCTION
	Processes messages posted into the abcsp library by abcsp_sendmsg(),
	translating them to their wire SLIP form and sending this to the
	UART.  One or more calls to this function will be required to send
	each ABCSP_TXMSG.

	This function must not be called unless it is known that
	ABCSP_UART_SENDBYTES() can accept a block of bytes.

	One call to abcsp_pumptxmsgs() makes, at most, one call to
	ABCSP_UART_SENDBYTES().

	The function's return value can be used to decide on when next to
	call the function - it is an aid to the library's scheduling.  If it
	returns 0 then all immediate work on the library's transmit path is
	finished.  It returns 1 to indicate that there is work to be done as
	soon as possible.

RETURNS
	1 to indicate that the transmit path of the abcsp library still has
	work to do immediately, else 0.
*/

extern unsigned abcsp_pumptxmsgs(void);


/****************************************************************************
NAME
        abcsp_uart_deliverbytes  -  push received uart bytes into the library

FUNCTION
        Pushes slipped bytes received from the UART into the abcsp library.

	The "n" bytes in the buffer "buf" are passed into the library.  This
	may provoke a single call to ABCSP_DELIVERMSG().

        The caller retains ownership of "buf".

RETURNS
        The number of bytes taken from "buf".

	If this does not equal "n" it is normally reasonable to call the
	function again to try to deliver the remaining bytes.  However, if
	the function returns zero it implies the library failed to obtain a
	required resource (ABCSP_RXMSG or heap memory).
*/

extern unsigned abcsp_uart_deliverbytes(char *buf, unsigned n); 


/****************************************************************************
NAME
        abcsp_bcsp_timed_event  -  transmit path's timed event

FUNCTION
	The abcsp library's transmit path uses a single timed event
	controlled with ABCSP_START_BCSP_TIMER() and
	ABCSP_CANCEL_BCSP_TIMER().  If the timer fires it must call this
	function.

	The timer event provokes a retransmission of all unacknowledged
	transmitted BCSP messages.
*/

extern void abcsp_bcsp_timed_event(void);


/****************************************************************************
NAME
        abcsp_tshy_timed_event  -  report a tshy timeout event to the fsm

FUNCTION
        The bcsp link establishment engine uses two timers.  The first is
        controlled by ABCSP_START_TSHY_TIMER() and ABCSP_CANCEL_TSHY_TIMER().
        If this timer fires it must call this function.

        This function sends sends a Tshy timeout event into the bcsp-le
        state machine.
*/

extern void abcsp_tshy_timed_event(void);


/****************************************************************************
NAME
        abcsp_tconf_timed_event  -  report a tconf timeout event to the fsm

FUNCTION
	The bcsp link establishment engine uses two timers.  The second is
	controlled by ABCSP_START_TCONF_TIMER() and
	ABCSP_CANCEL_TCONF_TIMER().  If this timer fires it must call this
	function.

        This function sends sends a Tconf timeout event into the bcsp-le
        state machine.
*/

extern void abcsp_tconf_timed_event(void);


/****************************************************************************
NAME
        abcsp_bcsp_timed_event  -  transmit path's timed event

FUNCTION
	The abcsp library's transmit path uses a singled timed event
	controlled via ABCSP_START_BCSP_TIMER() and
	ABCSP_CANCEL_BCSP_TIMER().  If the timer fires it must call this
	function.

	The timer event provokes a retransmission of all unacknowledged
	transmitted BCSP messages.  */

extern void abcsp_bcsp_timed_event(void);
#endif  /* __ABCSP_H__ */
