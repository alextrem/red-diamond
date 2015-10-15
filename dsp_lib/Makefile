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

UDEFS = -DARM_MATH_CM4 -D__FPU_PRESENT

#
# User specific defines
##############################################################################

PROJECT = math_cm4f

INCDIR = inc

include rules.mk
