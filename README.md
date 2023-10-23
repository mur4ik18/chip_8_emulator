# chip_8_emulator


### Memory
Thqis machine had 4096 (0x1000) memory locations, all of which are 8 bits(1 byte).
Intepreter itself occupies the first 512 (0x200) bytes of the memory space. 
Most programs do not access any of the memory below the location 512.
The uppermost 256 bytes 0xF00-0xFFF are reserved for display refresh
96 bytes below 0xEA0-0xEFF reserved for the call stack, internal use, and other variables

### Registers
CHIP-8 has 16 8-bit data registers named from V0 to VF. 
VF - carry flag (very weird)

### Stack
used to store return addresses when subroutines are called.
