import sys
import glob
import serial

def SerialPorts ():
	if sys.platform.startswith ('win'):
		ports = [ 'COM%s' % (i+1) for i in range (256) ]
	elif (sys.platform.startswith ('linux') or sys.platform.startswith ('cygwin')):
		ports = glob.glob ('/dev/tty[A-Za-z]*')
	elif sys.platform.startswith ('darwin'):
		ports = glob.glob ('/dev/tty.*')
		
	result = []
	for port in ports:
		try:
			s = serial.Serial (port)
			s.close ()
			result.append (port)
		except:
			pass
	
	return result

print "Serial Ports:"
print (SerialPorts ())