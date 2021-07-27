#!/bin/bash
openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program pico-solar.elf verify reset exit"
