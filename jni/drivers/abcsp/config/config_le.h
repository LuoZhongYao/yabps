#ifndef __CONFIG_LE_H__
#define __CONFIG_LE_H__

/****************************************************************************
	    
		     	© Cambridge Silicon Radio Ltd, 2004
 	 			
				All rights reserved

FILE:			config_le.h  -  configure _bcsp _link _establishment

DESCRIPTION:	The BCSP Link Establishment entity is described in CSR document
				bc01-s-010 (aka AN005).  This has a small number of configurable
				items.  These are set by #defines here.

		        The bc01b firmware has a pskey that can be used to disable the use of
				BCSP-LE.  No analogous configuration is provided here.  If a system
				is unwise enough to turn off BCSP-LE, or if a system wishes to use
				its own implementation of the protocol, then the rxmsgdemux.[ch] code
				can be removed and the rxmsgdemux calls in rxbcsp.c can be replaced
				with calls to rxmsg.[ch].

REVISION:		$Revision: #1 $ 

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
 
/* If this is #defined then the host uses the "passive-start" option.  This
is very unlikely to be of value.  There are systems where it makes sense to
use passive-start on the bc01 (e.g., the laptop which crashed if the bc01
sent any messages into its UART while Windows was booting), but the reverse
is unlikely.  A BCSP link must not use passive start on both sides of the
connection. */

/* #define      ABCSP_USE_BCSP_LE_PASSIVE_START */


/* If this is #defined then the host uses the old-style of BCSP Link
Establishment, described in CSR internal document bc01-s-010f.  Otherwise the
host uses the newer protocol, described in bc01-s-010g.  Further notes on the
two versions of the protocol are given in le.c.

This should be left un#defined unless the CSR chip is using a very old build
(before beta-10.X - approx March 2001). */

/* #define      ABCSP_USE_OLD_BCSP_LE */


/* This #define is ignored unless ABCSP_USE_OLD_BCSP_LE is #defined.

The maximum number of times the state machine will ask the peer if it
supports the ability to detect that the peer has restarted.  This can
normally be kept quite low - it only comes into play when the BCSP link is
established and flowing (choke has been removed).  The value is only more
than 1 to overcome the indeterminacy of using an unreliable BCSP channel.
Setting this to zero disables this feature - BCSP-LE never enters state
"garrulous." */

#define ABCSP_USE_BCSP_LE_CONF_CNT_LIMIT (4)

#ifdef __cplusplus
}
#endif 

#endif  /* __CONFIG_LE_H__ */
