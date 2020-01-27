#!/usr/bin/python
import serial

port = "/dev/ttyACM0"
s1 = serial.Serial(port,9600)
s1.flush()   

while True:
    if s1.inWaiting()>0:
        s1.write("a")
