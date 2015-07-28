#ifndef __CONFIG_TIMER_H__
#define __CONFIG_TIMER_H__

/****************************************************************************

     			? Cambridge Silicon Radio Ltd, 2005
 	 			
				All rights reserved

FILE:			config_timer.h  -  wire to the environment's timer functions

CONTAINS:		ABCSP_START_BCSP_TIMER  -  start the bcsp ack timeout timer
				ABCSP_START_TSHY_TIMER  -  start the bcsp-le tshy timer
				ABCSP_START_TCONF_TIMER  -  start the bcsp-le tconf timer
				ABCSP_CANCEL_BCSP_TIMER  -  cancel the bcsp ack timeout timer
		        ABCSP_CANCEL_TSHY_TIMER  -  cancel the bcsp-le tshy timer
		        ABCSP_CANCEL_TCONF_TIMER  -  cancel the bcsp-le tconf timer

DESCRIPTION:	The environment is required to provide a set of timers to support:

                bcsp's retransmission mechanism,
                bcsp link-establishment's Tshy timer
                bcsp link-establishment's Tconf timer

				External code must map to local timed event functions.  The three
				timers' periods are all bcsp configurable-items; these are also set
				in the external environment.  For example, a common value for the
				Tshy timer is 2 seconds, when the function supporting
				ABCSP_START_TSHY_TIMER() is called this should start a timer that
				calls abcsp_tshy_timed_event() after 2 seconds, unless
				ABCSP_CANCEL_TSHY_TIMER() is called first.

				The timers' accuracy requirements are lax; the only danger is the
				peer bcsp stack may think this stack is dead if events take too long
				to occur.  Accuracy to the nearest 0.1s is more than adequate.

REVISION:		$Revision: #1 $ 

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
NAMES
        ABCSP_START_BCSP_TIMER  -  start the bcsp ack timeout timer
        ABCSP_START_TSHY_TIMER  -  start the bcsp-le tshy timer
        ABCSP_START_TCONF_TIMER  -  start the bcsp-le tconf timer

SYNOPSES
        void ABCSP_START_BCSP_TIMER(void);
        void ABCSP_START_TSHY_TIMER(void);
        void ABCSP_START_TCONF_TIMER(void);

FUNCTIONS
        These three functions each schedule a timed event.  Each
        event catcher is "void fn(void)".

            The BCSP timer requires a call to abcsp_bcsp_timed_event();

            The TSHY timer requires a call to abcsp_tshy_timed_event();

            The TCONF timer requires a call to abcsp_tconf_timed_event();

        The timers' periods are configurable items, and are set in the
        external environment:

            The BCSP (Ttimeout) timer is normally set to 0.25s.

            The BCSP Link Establishment Tshy timer is normally set to 0.25s.

            The BCSP Link Establishment Tconf timer is normally set to 0.25s.

        The ABCSP_CANCEL_*_TIMER() functions each cancel the corresponding
        timer.

	The ABCSP_START_TSHY_TIMER() and ABCSP_START_TCONF_TIMER() timers are
	not used at the same time.
*/
extern void abcsp_start_bcsp_timer(void);
extern void abcsp_start_tshy_timer(void);
extern void abcsp_start_tconf_timer(void);
#define ABCSP_START_BCSP_TIMER(t)	ABCSP_LOGD("ABCSP_START_BCSP_TIMER"#t)//abcsp_start_bcsp_timer()
#define ABCSP_START_TSHY_TIMER(t)	ABCSP_LOGD("ABCSP_START_TSHY_TIMER"#t)//abcsp_start_tshy_timer()
#define ABCSP_START_TCONF_TIMER(t)	ABCSP_LOGD("ABCSP_START_TCONF_TIMER"#t)//abcsp_start_tconf_timer()

/****************************************************************************
NAMES
        ABCSP_CANCEL_BCSP_TIMER  -  cancel the bcsp ack timeout timer
        ABCSP_CANCEL_TSHY_TIMER  -  cancel the bcsp-le tshy timer
        ABCSP_CANCEL_TCONF_TIMER  -  cancel the bcsp-le tconf timer

SYNOPSES
        void ABCSP_CANCEL_BCSP_TIMER(void);
        void ABCSP_CANCEL_TSHY_TIMER(void);
        void ABCSP_CANCEL_TCONF_TIMER(void);

FUNCTIONS
        Each function prevents its timed event from occurring, if possible.

        It is acceptable to call one of these cancel functions if the
        corresponding timer isn't running.
*/
extern void abcsp_cancel_bcsp_timer(void);
extern void abcsp_cancel_tshy_timer(void);
extern void abcsp_cancel_tconf_timer(void);
#define ABCSP_CANCEL_BCSP_TIMER(t)	ABCSP_LOGD("ABCSP_CANCEL_BCSP_TIMER" #t)//abcsp_cancel_bcsp_timer()
#define ABCSP_CANCEL_TSHY_TIMER(t)	ABCSP_LOGD("ABCSP_CANCEL_TSHY_TIMER"#t)//abcsp_cancel_tshy_timer()
#define ABCSP_CANCEL_TCONF_TIMER(t)	ABCSP_LOGD("ABCSP_CANCEL_TCONF_TIMER"#t)//abcsp_cancel_tconf_timer()

#ifdef __cplusplus
}
#endif 

#endif  /* __CONFIG_TIMER_H__ */
