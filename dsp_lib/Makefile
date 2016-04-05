# CMSIS DSP Library

DSPLIB = .

include BasicMathFunctions/files.mk
include CommonTables/files.mk
include ComplexMathFunctions/files.mk
include ControllerFunctions/files.mk
include FastMathFunctions/files.mk
include FilteringFunctions/files.mk
include MatrixFunctions/files.mk
include StatisticsFunctions/files.mk
include SupportFunctions/files.mk
include TransformFunctions/files.mk

##############################################################################
# Compiler Settings
#

MCU = cortex-m4

TRGT = arm-none-eabi-
CC = $(TRGT)gcc
AR = $(TRGT)ar

RM = rm -Rf

# ARM options
AOPT =

# Thumb options
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes

#
# Compiler Settings
##############################################################################

C_OBJ := $(DSPSRC:%=%.o)
ASM_OBJ := $(DSPASM:%=%.o)

default: all
all: libmath_cm4f.a

libmath_cm4f.a: $(C_OBJ) $(ASM_OBJ)
	$(AR) rcs $@ $(C_OBJ) $(ASM_OBJ)

%.S.o: %.S
	$(CC) -c $(CFLAGS) -o $@ $^

%.c.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	@$(RM) $(C_OBJS) $(ASM_OBJS)
	@echo "Clean complete!"

mrproper: distclean
dist-clean: distclean
distclean: clean
	@$(RM) libmath_cm4f.a
	@echo "Distclean complete!"

##############################################################################
# User specific defines
#

UDEFS = -DARM_MATH_CM4 -D__FPU_PRESENT

#
# User specific defines
##############################################################################

PROJECT = math_cm4f

INCDIR = inc
