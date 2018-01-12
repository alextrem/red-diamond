# Source directory
DRIVERPATH = drivers

# All subdirectories end up here
DRIVERDIRS = dac \
             hdmi \
             sensors \

# Source files

DRIVERSRC = $(DRIVERPATH)/dac/cs43l22.c \
            $(DRIVERPATH)/dac/pcm1792a.c \
            $(DRIVERPATH)/hdmi/adv7612.c \
            $(DRIVERPATH)/sensors/max30101.c

# Include files for drivers
DRIVERINC = $(foreach inc,$(DRIVERDIRS),$(DRIVERPATH)/$(inc)/inc)
