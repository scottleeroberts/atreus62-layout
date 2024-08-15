# Atreus62 Keyboard Build Instructions
$ git clone git@github.com:qmk/qmk_firmware
$ cd qmk_firmware
$ make git-submodule
$ git clone git@github.com:scottleeroberts/atreus62-layout.git \
            keyboards/atreus62/keymaps/scottleeroberts
$ ./util/docker_build.sh atreus62:scottleeroberts

# Flash the firmware
$ unplug and replug the keyboard
$ push the reset button on the keyboard
$ `sudo su`
$ 'avrdude -v -p atmega32u4 -c avr109 -U flash:w:atreus62_default.hex -P /dev/ttyACM0'
