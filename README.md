# TCPIP-Simulation

# TCPIP-Simulation

This is the Criuse Tools for Eric Knutson.
1. SocketServerUDPMulticastLinux1.c: Criuse Emulator.
     local IP address: 169.254.198.81.
     Multicast IP address: 239.255.0.3
     Multicast Port: 4321
     UDP payload: "Radar, Camera, LiDar, GPS, IMU, ultrasonic raw data....."

2. EnDGUDPMulticastClient1=>EnDGTCPClient1.cpp:  UDPtoTCP convert
     TCP socket linked to Tera Term tool:
     Bind to INADDR_LOOPBACK   "127.0.0.1" port: 13401)

     UDP Multicast Receiver:
     local IP address: 169.254.89.149
     Join Multicast IP address: 239.255.0.3

3. Tera Term Tool:
      TCPIP: Host: 127.0.0.1; port:13401


How to run it:

1. Run Criuse emulator:
   On linux machine:  on command line:   SocketServerUDPMulticastLinux1.exe UDPMulticast_IPAddress UDPMulticast_Port Local_IPAddress
    e.x:
   On Windows machine: on command line:   EnDGUDPMulticastServer.exe UDPMulticast_IPAddress UDPMulticast_Port Local_IPAddress
    e.x: EnDGUDPMulticastServer.exe 239.255.0.3 5432 169.254.89.149

2. Run UDP2TCPConverter:
   On linux machine:  ??
   On Windows machine: on command line:   EnDGTCPClient1.exe  UDPMulticast_IPAddress Local_IPAddress UDPMulticast_Port Tool_Port
    e.x: EnDGTCPClient1.exe 239.255.0.3 5432 169.254.89.149 5432 13400

3. Run Tera Term: TCPIP=127.0.0.1  Port=13400





