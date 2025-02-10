reset halt
flash probe 0
flash write_image erase firmware.bin 0x08000000
reset run
exit