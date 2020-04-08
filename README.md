# PjTelemetry
The goal of this projet is to create a linux UART/UDP bridge with low CPU comsuption in order to receive telemetric data from drones over a 4G-LTE network

The bridge is now working, this program use a POSIX select to check file descriptor on both  UDP socket & Serial stream

The bridge is based on three thread :
  -the main thread who check if data are available on serial stream or udp socket , notify Serial or UDP thread and go for sleep until their finish theirs work
  -the Serial Thread who procces data if available and then go for sleep
  -the UDP Thread who procces data if available and then go for sleep
  
  Both serial & udp were implemented in POSIX with standard libs (#include <termios.h> + ioctl)  &  <sys/socket.h>)
  
  This implementation is probably not the most optimized for reduce CPU usage but it seems to work...any suggestions are welcome.
  
  
