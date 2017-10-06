# FPGA

The FPGA is the heart of this amplifier. All interfaces to ADCs, AES/EBU and other analog interfaces are managed and controled by this device. A low phase noise oscillator will be used for clocking digital logic and mixed signal devices.

The HDL related sources are located in this structure

| Directory       | Description                             |
| :-------------- | :-------------------------------------- |
| vdhl            |                                         |
| vhdl/simulation | all simulation files are located here   |
| vhdl/tb         | all modul testbench files               |
| constraints     | constraints files like sdc or qsf files |

## I2S

The I2S interface shall have following features:

* MSB first data transfer
* Data word length 8, 16, 20, 24 and 32 bits
* AXI-4 interface
* Testmodes like sawtooth, sinus e.g.

### Tasks
- [x] Testbench running
- [ ] Develop more sophisticated testbench
- [ ] AXI-4 interfaceing
- [ ] Implement test modes
- [ ] Define register interface
