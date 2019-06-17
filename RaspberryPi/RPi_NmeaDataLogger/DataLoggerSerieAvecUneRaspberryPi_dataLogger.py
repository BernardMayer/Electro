#!/usr/bin/python

import serial
import time
import os
import glob
import thread
from evdev import InputDevice

mountDirPath = '/media/usb1'
screenState = 1
shutDownTO = 0

class PyTouch:
	def __init__(self):
		self.dev = InputDevice('/dev/input/event0') 	


	def poll(self):
		event = self.dev.read_one()			
		if (event != None):
			while (event != None):
				event = self.dev.read_one()	
			return 1
		else :
			return 0		


def touchDetect():
	global screenState
	global shutDownTO
	touch = PyTouch()
	timeout = 0

	while True:
		if screenState :
			time.sleep(1)
			timeout = timeout +1
			if timeout == 13 :
				os.system('echo "0" > /sys/class/backlight/soc\:backlight/brightness')
				timeout = 0
				screenState = 0
		else:
			if touch.poll() == 1:
				shutDownTO = 0
				screenState = 1
				os.system('echo "1" > /sys/class/backlight/soc\:backlight/brightness')
				time.sleep(2)
				touch.poll()

# Turn OFF ACT and PWR leds
os.system('echo "0" > /sys/class/leds/led0/brightness')
os.system('echo "0" > /sys/class/leds/led1/brightness')
# Disable HDMI
os.system('/usr/bin/tvservice -o')

try:
   thread.start_new_thread( touchDetect, ( ) )
except:
   print "Error: unable to start thread"

# Wait serial screen 
while True: 
	try:  
		serDisplay = serial.Serial('/dev/tty1', 115200, timeout=0.5)
	except:
		time.sleep(1)
	else:
		break
serDisplay = serial.Serial('/dev/tty1', 115200, timeout=0.5)
time.sleep(5)
serDisplay.write('\n\rDATA LOGGER START\n\r')

while True:
	# Clear screen
	#os.system("setsid sh -c 'exec clear <> /dev/tty1 >&0 2>&1'")

	# Wait serial probe 
	test = 0
	shutDownTO = 0
	while not  glob.glob('/dev/ttyUSB*'):
		if not test:
			test = 1
			serDisplay.write('Waiting serial probe...\n\r')
		time.sleep(1)
		shutDownTO = shutDownTO + 1
		if shutDownTO == 60:
			# Turn OFF ACT and PWR leds
			os.system('echo "cpu0" > /sys/class/leds/led0/trigger')
			os.system('echo "input" > /sys/class/leds/led1/trigger')
			os.system('echo "1" > /sys/class/backlight/soc\:backlight/brightness')
			serDisplay.write('Idle during 60s: shutting down system\n\r')
			serDisplay.write('Idle during 60s: shutting down system\n\r')
			serDisplay.write('Idle during 60s: shutting down system\n\r')
			serDisplay.write('Idle during 60s: shutting down system\n\r')
			time.sleep(4)
			os.system("shutdown now -h")
			exit()

	probePath = glob.glob('/dev/ttyUSB*')[0]
	serProbe = serial.Serial( probePath, 115200, timeout=0.5)
	serDisplay.write('Serial probe '+ probePath+' OK\n\r')

	# Check is USB device is connected
	test = 0
	while not os.path.ismount(mountDirPath):
		if not test:
			test = 1
			serDisplay.write('Waiting USB key...\n\r')
	serDisplay.write('USB key OK\n\r')

	# Create data file
	fileNumber = 0
	while True:
		if os.path.exists(mountDirPath+"/data_"+str(fileNumber)+".log"):
			fileNumber = fileNumber + 1
		else:
			f = open(mountDirPath+"/data_"+str(fileNumber)+".log", "w+")
			f.close() 
			break
	serDisplay.write('Log file created: data_'+str(fileNumber)+'.log\n\r')

	# Read, print and store lines
	f = open(mountDirPath+"/data_"+str(fileNumber)+".log", "a+")
	while True:
		# Read a line
		try:
			line = serProbe.readline()
		except:
			pass
		else:
			if line:  # If it isn't a blank line
				f.write(line)
				serDisplay.write(line)
		if not os.path.exists(probePath):
			f.close()
			break

	serDisplay.write('\n\rData logging over, you can remove USB key.\n\r')
serDisplay.write('Restarting Data logger...\n\r')