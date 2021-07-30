#!/usr/bin/env python3
import json
import os
import serial
import time

# '/dev/ttyACM0' # USB - linux/rpi
# '/dev/serial0' # GPIO - linux/rpi
serial_port = '/dev/tty.usbmodem14101'  # mac
serial_port = '/dev/ttyACM0'

with serial.Serial(serial_port, 115200) as ser:
    while(True):
        line = ser.readline().strip().decode('ascii')
        print(line)
