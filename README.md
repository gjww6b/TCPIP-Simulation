# TCPIP-Simulation
This is a Tool to simulate RTK over 5G to LabSat GNSS signal capture and replay bencn

1. EnDGUDPMulticastServer.cpp : RTK emulator on Windows based PC: broadcast RTK Data to 192.168.1.255 subnet at UDP 13400 port 
     broadcasting subnet IP address: 192.168.1.255
     UDP Port: 13400
     UDP payload: "RTK from 5G modem"



How to run it:

1. Run RTK emulator:
   On PC1 Windows machine, set Ethernet MAC address:192.168.1.2
on command line:   EnDGUDPMulticastServer.exe 192.168.1.255 13400 192.168.1.2
EnDGUDPMulticastServer Boardcasting IP, UDP port, SIP
  
2. On PC2 Windows, run wireshark, Vehicle Spy Message  





