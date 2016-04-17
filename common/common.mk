#
COMMONDIR = .

# Source files
COMMONSRC = $(COMMONDIR)/common/common.c

# Include directory
COMMONINC = $(COMMONDIR)/common

vpath %.c $(COMMONSRC)
vpath %.h $(COMMONINC)
