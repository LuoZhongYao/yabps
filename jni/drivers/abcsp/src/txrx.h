/****************************************************************************
FILE
        txrx.h  -  information used by the transmit and receive paths

CONTAINS
        abcsp_txrx_init  -  initialise the two paths' common information

DESCRIPTION
        The transmit and receive paths of the abcsp library are fairly
        independent, but some data needs to be shared.  This file declares
        the shared information.

IMPLEMENTATION NOTES
        The bcsple_muzzled and rxdemux_bcsple_msg fields really should
        be in bcsple.c and rxmsgdemux.c respectively, but locating the
        flag here saves a little ram.

        Similarly, the txcrc field really should be a global shared
        between txmsg.c and txslip.c (or a function argument), but it
        saves a little ram to site the flag here.

MODIFICATION HISTORY
        1.1   6:jul:00  cjo     Created.
*/

#ifndef __TXRX_H__
#define __TXRX_H__
 
#ifdef RCS_STRINGS
static const char txrx_h_id[]
  = "$Id: txrx.h,v 1.1 2001/07/06 19:15:43 cjo Exp $";
#endif


/* Database of info common to tx and rx paths. */
typedef struct {
        bool      choke:1;              /* Is the choke applied? */
        bool      txsync_req:1;         /* bcsp-le sync requested. */
        bool      txsyncresp_req:1;     /* bcsp-le syncresp requested. */
        bool      txconf_req:1;         /* bcsp-le conf requested. */
        bool      txconfresp_req:1;     /* bcsp-le confresp requested. */
        unsigned  rxseq_txack:3;        /* rxseq == txack. */
        bool      txack_req:1;          /* Request tx rxseq_txack. */
        unsigned  rxack:3;              /* Received acknowledgement. */
        bool      bcsple_muzzled:1;     /* bcsple passive start. */
        bool      rxdemux_bcsple_msg:1; /* rxdemux switch. */
        bool      txcrc:1;              /* Update crc on tx byte. */
        } TXRXINFO;

extern TXRXINFO abcsp_txrx;


/****************************************************************************
NAME
        abcsp_txrx_init  -  initialise the two paths' common information

FUNCTION
        Sets the block of data shared between the transmit and receive
        paths of the abcsp stack to its initial default values.
*/

extern void abcsp_txrx_init(void);


#endif  /* __TXRX_H__ */
