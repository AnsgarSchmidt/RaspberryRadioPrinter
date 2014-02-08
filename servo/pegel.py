import serial
import time
import traceback

def sendOut(angle):
  try:
    ser = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=1)
    ser.close()
  exept:
    ser.close()
