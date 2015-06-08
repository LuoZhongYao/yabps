#
# Makefile for the abcsp library.
#
# $Id: Makefile,v 1.1 2001/07/06 19:13:30 cjo Exp $
#

TOP = ..
SUBDIR = 

include	$(TOP)/abcsp/make/Makefile.config

all clean nuke depend lint::
	for d in src ; do \
		( cd $$d ; $(MAKE) $@ ) || exit $$? ; \
		done

# DO NOT DELETE THIS LINE -- make depend depends on it.

