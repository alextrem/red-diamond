# RED-DIAMOND

Audio preamplifier based on STM32F4 from STMicroelectronis and [Altera] (http://www.altera.com) FPGA as interface and digital signal processing engine. I know, implementing it with a Raspberry PI or Beagle Bone Black would make things much easier but its just for the pure joy of playing around with electronics and microcontrollers.

The processor is using [ChibiOS] (http://www.chibios.org/dokuwiki/doku.php?id=start) as operating system and in future it is maybe able to decode and encode [MP3] (http://sourceforge.net/projects/mad/files/libmad/) files. The first steps will be to be able to read and configure the different devices

To build the software clone the repo, init and update the submodules and type:

    $ make all

If there is a [Toolchain] (https://launchpad.net/gcc-arm-embedded) available you should get a working binary.

### File Tree: ###

|-- board
    |-- schematic
    |-- pcb
|-- ai
|-- ChibiOS
|-- dspi_lib
|-- fpga
|-- gui
|-- drivers
|   |-- adc
|   |-- dac
|   |-- hdmi
|-- libmad
|-- LICENSE
|-- Makefile
|-- README.md

## Interfaces
* AES/EBU and SPDIF
* HDMI RX
* Analog
* Phono
* FM/DAB Radio

## FPGA
A Cyclone IV EP4CE22C8N is used to implement all digital(AES/EBU, HDMI) and anlog interfaces. Filters will be implemented in VHDL

### HDMI
This interface will be realized by a dedicated HDMI receiver device - ADV7612.

### AES/EBU and SPDIF
This interface will be connected differentially to the FPGA. No external IC will be used.

The decoding will be handled with FPGA resources.

### A/D and D/A converter
This amp handles all incoming data and produces a PWM signal

## Microcontroller
To be able to handle the use of the alternate functions of the I/O pads the following table will help:

|LQFP100 | Alternate Function | DAC | FPGA | LEDs | ADC | USB | SDIO | HDMI |
|--------|--------------------|-----|------|------|-----|-----|------|------|
|PA4     | I2S3_WS            |     |   x  |      |     |     |      |      |
|PA5     | SPI1_SCK           |  x  |      |      |  x  |     |      |      |
|PA6     | SPI1_MISO          |  x  |      |      |  x  |     |      |      |
|PA7     | SPI1_MOSI          |  x  |      |      |  x  |     |      |      |
|PA9     | OTG_FS_VBUS        |     |      |      |     |  x  |      |      |
|PA10    | OTG_FS_ID          |     |      |      |     |  x  |      |      |
|PA11    | OTG_FS_DM          |     |      |      |     |  x  |      |      |
|PA11    | OTG_FS_DP          |     |      |      |     |  x  |      |      |
|PB0     | ETH_MII_RXD2       |     |      |      |     |     |      |      |
|PB1     | ETH_MII_RXD3       |     |      |      |     |     |      |      |
|PB6     | I2C1_SCL           |  x  |      |      |     |     |      |   x  |
|PB9     | I2C1_SDA           |  x  |      |      |     |     |      |   x  |
|PB12    | SPI2_NSS           | CS  |      |      |     |     |      |      |
|PC7     | I2S3_MCK           |     |   x  |      |     |     |      |      |
|PC10    | I2S3_CK            |     |   x  |      |     |     |      |      |
|PC12    | I2S3_SD            |     |   x  |      |     |     |      |      |

## F.A.Q.
**Everything builds. I get a binary but it does not seem to work. What's wrong ?**

So far I had this problem only once. I built the software with hardware floating point support but the option was not compiled in the toolchain. Neither the compiler or the linker mentioned any errors. When you debug it you will end up in the initialization phase of the processor jumping right into nirvana.

**All the files are compiled successfully. But I get an error while linking. WTF ... ?**

* I observed this problem only on the cross toolchain for OSX. The error was caused by a symbolic link to libc. The library 'libg' was a symbolic link to libc and somehow the compiler could not handle it.
Just remove the symbolic link and create a copy named 'libg'. There you go !

This command tells you where the compiler takes a look for librarys:

    $command

* With the compilers -B option you can add a directory to the compilers search path. This is a good option if you dont't want to hack your toolchain. Just add the option into the Makefile. There you go ! Rock on !
