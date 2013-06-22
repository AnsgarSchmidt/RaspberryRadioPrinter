#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import os
import xively
import subprocess
import time
import datetime
import requests
import sys

# extract feed_id and api_key from environment variables
FEED_ID = os.environ["FEED_ID"]
API_KEY = os.environ["API_KEY"]
DEBUG   = os.environ["DEBUG"] or false

# initialize api client
api = xively.XivelyAPIClient(API_KEY)

# function to read 1 minute load average from system uptime command
def read_loadavg():
  if DEBUG:
    print "Reading load average"
  return subprocess.check_output(["awk '{print $1}' /proc/loadavg"], shell=True)

# function to read GPU temperature
def read_gputemp():
  if DEBUG:
    print "Reading GPU temperture"
  return subprocess.check_output(["/opt/vc/bin/vcgencmd measure_temp | cut -c6-9"], shell=True)

# function to read CPU temperature
def read_cputemp():
  if DEBUG:
    print "Reading CPU temperture"
  return subprocess.check_output(["echo $((`cat /sys/class/thermal/thermal_zone0/temp|cut -c1-2`)).$((`cat /sys/class/thermal/thermal_zone0/temp|cut -c3-5`))"], shell=True)

# function to read usedspace
def read_usedspace():
  if DEBUG:
    print "Reading usedspace"
  return subprocess.check_output(["df / | tail -n 1 | cut -c 45-46"], shell=True)

# function to read uptime
def read_uptime():
  if DEBUG:
    print "Reading uptime"
  return subprocess.check_output(["cat /proc/uptime | cut -d \" \" -f 1"], shell=True)

# function to read idletime
def read_idletime():
  if DEBUG:
    print "Reading idletime"
  return subprocess.check_output(["cat /proc/uptime |cut -d \" \" -f 2"], shell=True)

# function to return a datastream object. This either creates a new datastream,
# or returns an existing one
def get_datastream(feed,name):
  try:
    datastream = feed.datastreams.get(name)
    if DEBUG:
      print "Found existing datastream"
    return datastream
  except:
    if DEBUG:
      print "Creating new datastream"
    datastream = feed.datastreams.create(name)
    return datastream

if __name__ == '__main__':
  print "Starting Xively tutorial script"

  feed = api.feeds.get(FEED_ID)

  datastreamLoadlevel           = get_datastream(feed,"LoadLevel")
  datastreamLoadlevel.max_value = None
  datastreamLoadlevel.min_value = None

  datastreamGpu           = get_datastream(feed,"GPUTemperature")
  datastreamGpu.max_value = None
  datastreamGpu.min_value = None 

  datastreamCpu           = get_datastream(feed,"CPUTemperature")
  datastreamCpu.max_value = None
  datastreamCpu.min_value = None 

  datastreamUsedspace           = get_datastream(feed,"UsedSpacePercentage")
  datastreamUsedspace.max_value = None
  datastreamUsedspace.min_value = None

  datastreamUptime           = get_datastream(feed,"Uptime")
  datastreamUptime.max_value = None
  datastreamUptime.min_value = None

  datastreamIdletime           = get_datastream(feed,"Idletime")
  datastreamIdletime.max_value = None 
  datastreamIdletime.min_value = None

  while True:
    try:
      load_avg  = read_loadavg()
      gpu_temp  = read_gputemp()
      cpu_temp  = read_cputemp()
      usedspace = read_usedspace()
      uptime    = read_uptime()
      idletime  = read_idletime()

      if DEBUG:
        print "Updating Xively feed with LoadLevel value: %s" % load_avg
        print "Updating Xively feed with GPUTemp   value: %s" % gpu_temp
        print "Updating Xively feed with CPUTemp   value: %s" % cpu_temp
        print "Updating Xively feed with UsedSpace value: %s" % usedspace
        print "Updating Xively feed with Uptime    value: %s" % uptime
        print "Updating Xively feed with Idletime  value: %s" % idletime

      datastreamLoadlevel.current_value = load_avg
      datastreamLoadlevel.at            = datetime.datetime.utcnow()

      datastreamGpu.current_value       = gpu_temp
      datastreamGpu.at                  = datetime.datetime.utcnow()    

      datastreamCpu.current_value       = cpu_temp
      datastreamCpu.at                  = datetime.datetime.utcnow()

      datastreamUsedspace.current_value = usedspace
      datastreamUsedspace.at            = datetime.datetime.utcnow()

      datastreamUptime.current_value    = uptime
      datastreamUptime.at               = datetime.datetime.utcnow()

      datastreamIdletime.current_value  = idletime
      datastreamIdletime.at             = datetime.datetime.utcnow()

      try:
        datastreamLoadlevel.update()
        datastreamGpu.update()
        datastreamCpu.update()
        datastreamUsedspace.update()
        datastreamUptime.update()
        datastreamIdletime.update()
      except requests.HTTPError as e:
        print "HTTPError({0}): {1}".format(e.errno, e.strerror)

      time.sleep(60)
    except (KeyboardInterrupt, SystemExit):
      print "System exit"
      sys.exit(0)
    except:
      print "Error"
