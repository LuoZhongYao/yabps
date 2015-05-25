/****************************************************************************
FILE
        crc.c  -  cyclic redundancy check code

CONTAINS
        abcsp_crc_init  -  initialise the crc calculator
        abcsp_crc_update  -  update crc with next data byte
        abcsp_crc_reverse  -  get reverse of generated crc

MODIFICATION HISTORY
        1.1   6:jul:01   cjo    Stolen from Neil.
        1.2   30:jul:01  sm     Removed rev16() and folded its
				functionality into abcsp_crc_reverse().  Bug
				fix to byte swapping in abcsp_crc_reverse().
        1.3   30:jul:01  cjo    Updated comments.
*/

#ifdef RCS_STRINGS
static const char crc_c_id[]
  = "$Id: crc.c,v 1.3 2001/07/30 15:40:06 cjo Exp $";
#endif

#include "abcsp_private.h"


/* Table for calculating CRC for polynomial 0x1021, LSB processed first,
initial value 0xffff, bits shifted in reverse order. */

static const uint16 crc_table[] = {
        0x0000, 0x1081, 0x2102, 0x3183,
        0x4204, 0x5285, 0x6306, 0x7387,
        0x8408, 0x9489, 0xa50a, 0xb58b,
        0xc60c, 0xd68d, 0xe70e, 0xf78f
        };


/****************************************************************************
NAME
        abcsp_crc_init  -  initialise the crc calculator
*/

void abcsp_crc_init(uint16 *crc)
{
        *crc =  0xffff; 
}


/****************************************************************************
NAME
        abcsp_crc_update  -  update crc with next data byte

IMPLEMENTATION NOTE
        The data byte is treated as two nibbles.  The crc is generated
        in reverse, i.e., bits are fed into the register from the top.
*/

void abcsp_crc_update(uint16 *crc, uint8 d)
{
        uint16 reg = *crc;

        reg = (reg >> 4) ^ crc_table[(reg ^ d) & 0x000f];
        reg = (reg >> 4) ^ crc_table[(reg ^ (d >> 4)) & 0x000f];

        *crc = reg;
}


/****************************************************************************
NAME
        abcsp_crc_reverse  -  get reverse of generated crc

IMPLEMENTATION NOTE
        The crc generator (abcsp_crc_init() and abcsp_crc_update())
        creates a reversed crc, so it needs to be swapped back before
        being passed on.
*/

uint16 abcsp_crc_reverse(uint16 crc)
{
        uint16 b, rev;

        for(b = 0, rev=0 ; b < 16 ; b++) {
                rev = rev << 1;
                rev |= (crc & 1);
                crc = crc >> 1;
                }
        return(rev);
}
