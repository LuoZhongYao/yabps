/****************************************************************************
FILE
        crc.h  -  cyclic redundancy check code

CONTAINS
        abcsp_crc_init  -  initialise the crc calculator
        abcsp_crc_update  -  update crc with next data byte
        abcsp_crc_reverse  -  translate crc into big-endian number

DESCRIPTION
        Cyclic redundancy check calculator.

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Heavily based on Neil's crc.h.
*/

#ifndef __CRC_H__
#define __CRC_H__
 
#ifdef RCS_STRINGS
static const char crc_h_id[]
  = "$Id: crc.h,v 1.1 2001/07/06 19:15:34 cjo Exp $";
#endif

#include "abcsp_private.h"


/****************************************************************************
NAME
        abcsp_crc_init  -  initialise the crc calculator

FUNCTION
        Write an initial value (0xffff) into *crc.
*/

extern void abcsp_crc_init(uint16 *crc);


/****************************************************************************
NAME
        abcsp_crc_update  -  update crc with next data byte

FUNCTION
        Updates the cyclic redundancy check value held in *crc with the
        next data byte in the current sequence, "d".
*/

extern void abcsp_crc_update(uint16 *crc, uint8 d);


/****************************************************************************
NAME
        abcsp_crc_reverse  -  translate crc into big-endian number

RETURNS
        A bit reversed version of crc.
*/

extern uint16 abcsp_crc_reverse(uint16 crc);


#endif /* __CRC_H__ */
