  1 0x00000000 0x93000000 IN $a0
  2 0x00000004 0x94000000 IN $a1
  3 0x00000008 0x03300004 ADD $a0 $a0 $a1
  4 0x0000000C 0xA3000000 OUT $a0
  5 0x00000010 0x70000000 HALT
