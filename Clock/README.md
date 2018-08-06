## SIMPLE CLOCK ON AVR
I implement easy but useful clock that could be used in different projects. I used arduino leonardo with atmega32u4 on board.
Before compiling and uploading to the mcu, be sure that you've changed *Makefile* on your mcu.
# Some commands to compile
- *make hex* -compiles C code.
- *make clean*  deleting everything except C and h files.
- *make flash*  uploading the hex to your mcu.
