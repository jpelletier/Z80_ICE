Function loadIntelHex 
On linux, works at 115200 bauds if we limit the transfer rate.

On linux, use this command to send an intel hex file through a serial to USB 
converter (change as needed).
	pv -L 800 my_program.hex > /dev/ttyUSB0
	
	