# TCPIP-Simulation
This is the Criuse Tools for Eric Knutson.

1. EnDGUDPServer.cpp : Cruise emulator on Windows based PC: broadcast Cruise Data to 192.168.1.255 subnet at UDP 1534 port 
     broadcasting subnet IP address: 192.168.1.255
     UDP Port: 1534
     UDP payload: "Cruise Data....."

2. EnDGUDPReceiver.cpp : UDPtoTCPconverter to bridge the cruise data to acquisition tool
     TCP socket linked to Tera Term tool via "127.0.0.1" at TCP port: 13400)
     UDP Receiver listening on local IP address at UDP 1534 port
     TCP Converter transforing on TCP socket
 
3. Tera Term Tool:
      TCPIP: Host: 127.0.0.1; port:13400


How to run it:

1. Run Criuse emulator:
   On Windows machine command line:   EnDGUDPServer.exe
  
2. Run UDP2TCPConverter:
   On Windows machine command line:   EnDGUDPReceiver.exe

3. Run Tera Term: TCPIP=127.0.0.1  Port=13400



