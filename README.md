# RED-DIAMOND

Audio DAC based on STM32F4 from STMicroelectronis and [Altera] (http://www.altera.com) FPGA as interface and digital signal processing engine. I know, implementing it with a Raspberry PI or Beagle Bone Black would make things much easier but its just for the pure joy of playing around with electronics and microcontrollers.

The processor is using [ChibiOS] (http://www.chibios.org/dokuwiki/doku.php?id=start) as operating system and in future it is maybe able to decode and encode [MP3] (http://sourceforge.net/projects/mad/files/libmad/) files.

## Interfaces
* AES/EBU and SPDIF
* HDMI RX
* Analog
* Phono
* FM Radio

## FPGA
A Cyclone IV EP4CE22C8N is used to implement all digital(AES/EBU, HDMI) and anlog interfaces.
