  1 0x00000000 0xD3400008 loop: BGT $a0 $a1 after
  2 0x00000004 0x23300001 ADDI $a0 $a0 1
  3 0x00000008 0x500FFFF4 BEQ $zero $zero loop
  4 0x0000000C 0x03300004 after: ADD $a0 $a0 $a1
  5 0x00000010 0x03300003 ADD $a0 $a0 $a0
  6 0x00000014 0x03300003 ADD $a0 $a0 $a0
  7 0x00000018 0x03300003 ADD $a0 $a0 $a0
  8 0x0000001C 0x03300003 ADD $a0 $a0 $a0
  9 0x00000020 0x03300003 ADD $a0 $a0 $a0
 10 0x00000024 0x70000000 HALT