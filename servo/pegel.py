import serial
import time
import traceback
import sys
import random
import json
import urllib2
import csv
import datetime

#http://www.pegelonline.wsv.de/gast/stammdaten?pegelnr=4910040
url = "http://www.pegelonline.wsv.de/webservices/rest-api/v2/stations/4910040/W/currentmeasurement.json"
pegelMin    = 350
pegelMax    = 850
csvFilename = "Unterweserpegel.csv"

def sendOut(angle):
    try:
        ser = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=1)
        ser.write(str(angle))
    finally:
        ser.close()

def getPegel(url):
    try:
        response = urllib2.urlopen(url)
        data = json.load(response)       
        return float(data["value"])
    except:
        return 500.0

def translate(value, leftMin, leftMax, rightMin, rightMax):
    leftSpan    = leftMax     - leftMin
    rightSpan   = rightMax    - rightMin
    if value < leftMin:
        value = leftMin
    if value > leftMax:
        value = leftMax
    valueScaled = float(value - leftMin) / float(leftSpan)
    return rightMin + (valueScaled * rightSpan)

pegel      = getPegel(url)
normalized = translate(pegel,pegelMin,pegelMax,0,180)
intval     = int(round(normalized))
sendOut(intval) 
with open(csvFilename,"ab") as csvfile:
    csvwriter = csv.writer(csvfile, delimiter=",")
    csvwriter.writerow([datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S'),pegel,"%3.2f"%normalized,intval])

