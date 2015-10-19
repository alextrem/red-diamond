# RED-DIAMOND

Audio preamplifier based on STM32F4 from STMicroelectronis and [Altera] (http://www.altera.com) FPGA as interface and digital signal processing engine. I know, implementing it with a Raspberry PI or Beagle Bone Black would make things much easier but its just for the pure joy of playing around with electronics and microcontrollers.

The processor is using [ChibiOS] (http://www.chibios.org/dokuwiki/doku.php?id=start) as operating system and in future it is maybe able to decode and encode [MP3] (http://sourceforge.net/projects/mad/files/libmad/) files. The first steps will be to be able to read and configure the different devices

### File Tree: ###
**|-- board**
**|   |-- schematic**
**|   |-- pcb**
**|-- ai** *Audio interface*
**|-- dsp_lib** *CMSIS DSP library*
**|-- fpga** *Firmware for FPGA project*
**|-- drivers**
**|   |-- adc**
**|   |-- dac**
**|   |-- hdmi**
**|-- libmad** *MP3 decoder source and library*
**|-- LICENSE** *GPL License file*
**|-- Makefile** *Makefile for this project*
**|-- README.md** *This Readme file*

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

## Microcontroller
To be able to handle the use of the alternate functions of the I/O pads the following table will help:

|LQFP100 | Alternate Function | DAC | FPGA | LEDs | ADC | USB | SDIO | HDMI |
|--------|--------------------|-----|------|------|-----|-----|------|------|
|PA4     | SPI1_NSS           |  x  |      |      |     |     |      |      |
|PA5     | SPI1_SCK           |  x  |      |      |  x  |     |      |      |
|PA6     | SPI1_MISO          |  x  |      |      |  x  |     |      |      |
|PA7     | SPI1_MOSI          |  x  |      |      |  x  |     |      |      |
|PB0     | ETH_MII_RXD2       |     |      |      |     |     |      |      |
|PB1     | ETH_MII_RXD3       |     |      |      |     |     |      |      |
|PB6     | I2C1_SCL           |     |      |      |     |     |      |   x  |
|PB7     | I2C1_SDA           |     |      |      |     |     |      |   x  |
|PB12    | SPI2_NSS           | CS  |      |      |     |     |      |      |
