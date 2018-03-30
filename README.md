# RED-DIAMOND

[![Build Status](https://travis-ci.org/alextrem/red-diamond.svg?branch=master)](https://travis-ci.org/alextrem/red-diamond)
[![Coverity Scan](https://scan.coverity.com/projects/8494/badge.svg)](https://scan.coverity.com/projects/alextrem-red-diamond)

This project is all about an audio preamplifier based on STM32F4 from STMicroelectronis and [Altera](http://www.altera.com) FPGA as interface and digital signal processing engine.
I know, implementing it with a Raspberry PI or Beagle Bone Black would make things much easier but its just for the pure joy of playing around with electronics and microcontrollers.

The processor is using [ChibiOS](http://www.chibios.org/dokuwiki/doku.php?id=start) as operating system and in future it is maybe able to decode and encode [MP3](http://sourceforge.net/projects/mad/files/libmad/) files. The first steps will be to be able to read and configure the different external devices.

## Development environment setup

I'm using the GCC ARM launchpad [cross compiler](https://launchpad.net/gcc-arm-embedded)

    $ cd ~/dev
    $ wget https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update/+download/gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
    $ tar xvfj gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
    $ rm gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2
    $ echo 'export PATH=$PATH:~/dev/gcc-arm-none-eabi-4_9-2015q3/bin:$PATH' >> ~/.bashrc

To build the software clone the repo, init and update the submodules and type:

    $ git submodule update --init

    $ make extract
    $ cd dsp_lib; make all; cd ..
    $ make all

### Code Coverage ###

Code coverage option has been included in Makefile with the USE_GCOV switch. Set it according to your needs. It is deactivated per default.
This [website](http://mcuoneclipse.com/2014/12/26/code-coverage-for-embedded-target-with-eclipse-gcc-and-gcov) gets you started if you are not familiar
with gcc and gcov.

### CMSIS DSP_LIB ###

So far I don't use this library. If so

    $ cd dsp_lib
    $ make all
    $ cd ..

## Interfaces
* AES/EBU and SPDIF
* HDMI RX
* Analog
* Phono
* FM/DAB Radio
* SD-CARD
* USB-OTG

## FPGA
A Cyclone IV EP4CE22C8N is used to implement all digital(AES/EBU, HDMI) and anlog interfaces. Filters will be implemented in VHDL

### HDMI
This interface will be realized by a dedicated HDMI receiver device - ADV7612.

### AES/EBU and SPDIF
This interface will be connected differentially to the FPGA. No external IC will be used.

The decoding will be handled with FPGA resources.

### A/D and D/A converter
This amp handles all incoming data and produces a PWM signal.

## Microcontroller
To be able to handle the use of the alternate functions of the I/O pads the following table will help:

|LQFP100 | Alternate Function | DAC | FPGA | LEDs | ADC | USB | SDIO | HDMI |
|--------|--------------------|-----|------|------|-----|-----|------|------|
|PA1     |                    |     |      |      |     |     |      |      |
|PA2     |                    |     |      |      |     |     |      |      |
|PA3     |                    |     |      |      |     |     |      |      |
|PA4     | I2S3_WS            |     |   x  |      |     |     |      |      |
|PA5     | SPI1_SCK           |  x  |      |      |  x  |     |      |      |
|PA6     | SPI1_MISO          |  x  |      |      |  x  |     |      |      |
|PA7     | SPI1_MOSI          |  x  |      |      |  x  |     |      |      |
|PA8     |                    |     |      |      |     |     |      |      |
|PA9     | OTG_FS_VBUS        |     |      |      |     |  x  |      |      |
|PA10    | OTG_FS_ID          |     |      |      |     |  x  |      |      |
|PA11    | OTG_FS_DM          |     |      |      |     |  x  |      |      |
|PA12    | OTG_FS_DM          |     |      |      |     |  x  |      |      |
|PB0     | ETH_MII_RXD2       |     |      |      |     |     |      |      |
|PB1     | ETH_MII_RXD3       |     |      |      |     |     |      |      |
|PB2     |                    |     |      |      |     |     |      |      |
|PB3     |                    |     |      |      |     |     |      |      |
|PB4     |                    |     |      |      |     |     |      |      |
|PB5     |                    |     |      |      |     |     |      |      |
|PB6     | I2C1_SCL           |  x  |      |      |     |     |      |   x  |
|PB7     |                    |     |      |      |     |     |      |      |
|PB8     |                    |     |      |      |     |     |      |      |
|PB9     | I2C1_SDA           |  x  |      |      |     |     |      |   x  |
|PB10    |                    |     |      |      |     |     |      |      |
|PB11    |                    |     |      |      |     |     |      |      |
|PB12    | SPI2_NSS           | CS  |      |      |     |     |      |      |
|PC0     |                    |     |      |      |     |     |      |      |
|PC1     |                    |     |      |      |     |     |      |      |
|PC2     |                    |     |      |      |     |     |      |      |
|PC3     |                    |     |      |      |     |     |      |      |
|PC4     |                    |     |      |      |     |     |      |      |
|PC5     |                    |     |      |      |     |     |      |      |
|PC6     |                    |     |      |      |     |     |      |      |
|PC7     | I2S3_MCK           |     |   x  |      |     |     |      |      |
|PC8     |                    |     |      |      |     |     |      |      |
|PC9     |                    |     |      |      |     |     |      |      |
|PC10    | I2S3_CK            |     |   x  |      |     |     |      |      |
|PC11    |                    |     |      |      |     |     |      |      |
|PC12    | I2S3_SD            |     |   x  |      |     |     |      |      |
|PC13    |                    |     |      |      |     |     |      |      |
|PC14    |                    |     |      |      |     |     |      |      |
|PC15    |                    |     |      |      |     |     |      |      |


## F.A.Q.
**Everything builds. I get a binary but it does not seem to work. What's wrong ?**

So far I had this problem only once. I built the software with hardware floating point support but the option was not compiled in the toolchain. Neither the compiler or the linker mentioned any errors. When you debug it you will end up in the initialization phase of the processor jumping right into nirvana.

**All the files are compiled successfully. But I get an error while linking. WTF ... ?**

* I observed this problem only on the cross toolchain for OSX. The error was caused by a symbolic link to libc. The library 'libg' was a symbolic link to libc and somehow the compiler could not handle it.
Just remove the symbolic link and create a copy named 'libg'. There you go !

This command tells you where the compiler takes a look for librarys:

    $command

* With the compilers -B option you can add a directory to the compilers search path. This is a good option if you dont't want to hack your toolchain. Just add the option into the Makefile. There you go ! Rock on !
