# Implementation of popular game FlappyBird on AVR
 In that project i used ssd1306 128x64 OLED display,arduino Leonardo, 4-bit LED Digital Tube Module and one button.
 The main idea of the game is to make bird fly through the window. You can do that by pushing the button. If you don't - the     bird will fall.
 ## Uploading to your mcu
Be sure that you change mcu in the *Makefile* to yours.
- *make hex* compiles C code.
- *make clean*  deleting everything except C and h files.
- *make flash*  uploading the hex to your mcu.
### Demonstration of the project
https://youtu.be/JCXHE8eQRZ8
