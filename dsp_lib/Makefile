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

# ARM options
AOPT =

# Thumb options
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes

#
# Compiler Settings
##############################################################################

##############################################################################
# User specific defines
#

#
# User specific defines
##############################################################################
C_OBJ := $(DSPSRC:%=%.o)

INCDIR = inc

CFLAGS := -mcpu=$(MCU) -Os -ggdb -I$(INCDIR) $(CWARN) $(TOPT) -mfloat-abi=hard -DARM_MATH_CM4 -D__FPU_PRESENT

default: all

all: libmath_cm4f.a

libmath_cm4f.a: $(C_OBJ)
	$(AR) rcs $@ $(C_OBJ)

%.c.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	rm $(C_OBJ)
