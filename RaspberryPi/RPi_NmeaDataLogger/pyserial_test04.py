import serial.tools.list_ports
# print [port for port in serial.tools.list_ports.comports() if port[2] != 'n/a']
objPorts = serial.tools.list_ports.comports()
for p in objPorts :
    print(p)
##  COM3 - Intel(R) Active Management Technology - SOL (COM3)
