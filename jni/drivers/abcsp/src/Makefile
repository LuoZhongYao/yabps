#
# Makefile for abcsp.
#
# $Id: Makefile,v 1.2 2001/07/17 19:40:39 cjo Exp $
#

TOP = ../..
SUBDIR = abcsp

include	$(TOP)/abcsp/make/Makefile.config

LIBNAME = abcsp
LIB = lib$(LIBNAME).a

LIB_PUBLIC_HEADERS =
LIB_PRIVATE_HEADERS = abcsp_init.h abcsp_private.h commonc.h crc.h le.h \
		rxbcsp.h rxmsg.h rxmsgdemux.h rxslip.h txmsg.h txrx.h \
		txslip.h

LIB_SOURCES = abcsp_free.c abcsp_init.c abcsp_malloc.c abcsp_zmalloc.c \
		crc.c le.c rxbcsp.c rxmsg.c rxmsgdemux.c rxslip.c \
		txmsg.c txrx.c txslip.c
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)


include $(TOP)/abcsp/make/Makefile.targets


# DO NOT DELETE THIS LINE -- make depend depends on it.

