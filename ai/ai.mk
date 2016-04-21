#
AIDIR = ai

# Source files
AISRC = $(AIDIR)/audioctrl.c \
        $(AIDIR)/audionav.c \
        $(AIDIR)/disknav.c

# Include directory
AIINC = $(AIDIR)/inc

vpath %.c $(AISRC)
vpath %.h $(AIINC)
