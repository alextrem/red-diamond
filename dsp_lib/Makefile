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

MCFLAGS = -mcpu=$(MCU)

CFLAGS = $(MCFLAGS) -Os -ggdb $(CWARN) $(TOPT) -Wa,-alms=$(LISTDIR)/$(notdir $(<:.c=.lst)) -mfloat-abi=hard $(DEFS)


# Output directories
BUILDDIR = build
OUTFILES = $(BUILDDIR)/$(PROJECT).lst

# various directories
OBJDIR = $(BUILDDIR)/obj
LISTDIR = $(BUILDDIR)/lst

TCSRC = $(DSPSRC)

ASRC = $(ACSRC)$(ACPPSRC)
TSRC = $(TCSRC)$(TCPPSRC)
SRCPATHS = $(sort $(dir $(ASRC)) $(dir $(TSRC)))

# Object files
ACOBJS   = $(addprefix $(OBJDIR)/, $(notdir $(ACSRC:.c=.o)))
ACPPOBJS = $(addprefix $(OBJDIR)/, $(notdir $(ACPPSRC:.cpp=.o)))
TCOBJS   = $(addprefix $(OBJDIR)/, $(notdir $(TCSRC:.c=.o)))
TCPPOBJS = $(addprefix $(OBJDIR)/, $(notdir $(TCPPSRC:.cpp=.o)))
ASMOBJS  = $(addprefix $(OBJDIR)/, $(notdir $(ASMSRC:.S=.o)))
ASMXOBJS = $(addprefix $(OBJDIR)/, $(notdir $(ASMXSRC:.S=.o)))
OBJS     = $(ACOBJS) $(ACPPOBJS) $(TCOBJS) $(TCPPOBJS) $(ASMOBJS) $(ASMXOBJS)

# Paths
IINCDIR = $(patsubst %,-I%,$(INCDIR))
LLIBDIR = $(patsubst %,-L%,$(LIBDIR))

# Macros
DEFS = $(DDEFS) $(UDEFS)
MCFLAGS = -mcpu=$(MCU)

CFLAGS = $(MCFLAGS) -Os -ggdb $(CWARN) $(TOPT) -Wa,-alms=$(LISTDIR)/$(notdir $(<:.c=.lst)) -mfloat-abi=hard $(DEFS)

# Generate dependency information
ASFLAGS  += -MD -MP -MF .dep/$(@F).d
CFLAGS   += -MD -MP -MF .dep/$(@F).d
CPPFLAGS += -MD -MP -MF .dep/$(@F).d

VPATH = $(SRCPATHS)

$(OBJS): | $(BUILDDIR) $(OBJDIR) $(LISTDIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LISTDIR):
	@mkdir -p $(LISTDIR)

$(ACOBJS):$(OBJDIR)/%.o : %.c Makefile
	@echo Comiling $(<F)
	$(CC) -c $(CFLAGS) -I. $(IINCDIR) $< -o $@

$(ACPPOBJS):$(OBJDIR)/%.o : %.c Makefile
	@echo Comiling $(<F)
	$(CCP) -c $(CFLAGS) -I. $(IINCDIR) $< -o $@

$(TCOBJS):$(OBJDIR)/%.o : %.c Makefile
	@echo Comiling $(<F)
	$(CC) -c $(CFLAGS) -I. $(IINCDIR) $< -o $@

$(TCPPOBJS):$(OBJDIR)/%.o : %.c Makefile
	@echo Comiling $(<F)
	$(CCP) -c $(CFLAGS) -I. $(IINCDIR) $< -o $@

lib: $(OBJS) $(BUILDDIR)/lib$(PROJECT).a

$(BUILDDIR)/lib$(PROJECT).a: $(OBJS)
	$(AR) -r $@ $^

clean:
	@echo Cleaning
	rm -Rf .dep $(BUILDDIR)
	@echo
	@echo Done

print-%:
	@echo $* = $($*)

-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
