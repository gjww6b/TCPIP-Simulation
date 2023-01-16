========================================================================
       CONSOLE APPLICATION : RTK simulator
========================================================================


AppWizard has created this RTK simulator application for you.  

This file contains a summary of what you will find in each of the files that
make up your EnDGUDPMulticastServer application.

EnDGUDPMulticastServer.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

EnDGUDPMulticastServer.cpp
    This is the main application source file.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named EnDGTCPClient.pch and a precompiled types file named StdAfx.obj.


/////////////////////////////////////////////////////////////////////////////
Other notes:

This is the Tools to simulate the RTK via 5G for GVPM+ test bench. it sends the RTK information via UDP to GIGASTAR, which gating to LABSat via CAN FD frame. 

EnDGUDPMulticastServer.cpp: RTK emulator on Windows based PC: broadcast RTK Data to 192.168.1.255 subnet at UDP 13400 port "RTK from 5G moden".  it takes argument of UDPMulticast_IPAddress UDPMulticast_Port Local_IPAddress




How to run it:

Run RTK emulator: On Windows machine command line: EnDGUDPMulticastServer.exe 192.168.1.255 192.168.1.2 13400  

any PC on the 192.168.1.255 subnet shall receive the "RTK from 5G moden"
/////////////////////////////////////////////////////////////////////////////
