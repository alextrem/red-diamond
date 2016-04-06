# CMSIS DSP Library

ARCH = M4lf
LIB = lib/libmathC$(ARCH).a

# Architectures to build for
ARCHS = M3l M4l M4lf
M3l_FLAGS = -mthumb -mcpu=cortex-m3 -DARM_MATH_CM3
M4l_FLAGS = -mthumb -mcpu=cortex-m4 -DARM_MATH_CM4
M4lf_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DARM_MATH_CM4 -D__FPU_PRESENT

# Directories
DSPLIB = .
DSPDIRS = BasicMathFunctions \
          CommonTables \
          ComplexMathFunctions \
          ControllerFunctions \
          FastMathFunctions \
          FilteringFunctions \
          MatrixFunctions \
          StatisticsFunctions \
          SupportFunctions \
          TransformFunctions

##############################################################################
# Compiler Settings
#

MCU = cortex-m4

TRGT = arm-none-eabi-
CC = $(TRGT)gcc
CXX = $(TRGT)g++
LD = $(TRGT)ld
AR = $(TRGT)ar
AS = $(TRGT)as
CP = $(TRGT)objcopy
OD = $(TRGT)objdump

# ARM options
AOPT =

# Thumb options
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wundef -Wstrict-prototypes

# CFLAGSoptions
INCLUDES = inc
DEBUG = gdb
CFLAGS := -g$(DEBUG) -I$(INCLUDES) $(CWARN) -fomit-frame-pointer -falign-functions=16 \
          -fsingle-precision-constant -ffunction-sections -fdata-sections \
          -fno-common -Os

#
# Compiler Settings
##############################################################################

# Build Paths
BUILD_PATH := lib/build
BUILDDIRS  := $(BUILD_PATH)
BUILDDIRS  += $(foreach dir,$(DSPDIRS),$(BUILD_PATH)/$(dir))

# Targets to build
LIBS := $(foreach tgt, $(ARCHS), libmath_C$(tgt).a)
SRCS := $(wildcard $(DSPLIB)/*/*.c)
OBJS := $(patsubst %.c,%.o, $(SRCS))
OBJS := $(foreach obj, $(OBJS), $(BUILD_PATH)/$(obj))

default: all
all: $(BUILDDIRS) $(OBJS) $(LIB)

.PHONY: clean

clean:
	rm -Rf  $(BUILD_PATH)
	@echo "Clean complete!"

$(BUILD_PATH)/%.o: %.c
	$(CC) -c $(CFLAGS) $($(ARCH)_FLAGS) -o $@ $<

$(LIB): $(OBJS)
	$(AR) rcs $@ $?

$(BUILDDIRS):
	@mkdir -p $@

mrproper: distclean
dist-clean: distclean
distclean: clean
	rm -Rf libmath_cm4f.a
	@echo "Distclean complete!"
