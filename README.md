# RED-DIAMOND

Amplifier based on STM32F4 from STMicroelectronis and [Altera] (http://www.altera.com) FPGA as interface and digital signal processing engine. I know, implementing it with a Raspberry PI or Beagle Bone Black would make things much easier but its just for the fun of playing around with electronics and Microcontrollers.

The processor is using ChibiOS as operating system and in future it is maybe able to decode and encode MP3 files.

## Interfaces
* AES/EBU and SPDIF
* HDMI RX
* Analog XLR
* Radio

## Amplifier
* Class A Design

## FPGA
A Cyclone IV EP4CE22C8N is used to implement all digital(AES/EBU, HDMI) and anlog interfaces
