# SOME_NEW_THING
==============================
* Execution of acf_talker
* Building 

'''shell
cc -I./src -I./include -o acf_talker src/avtp.c src/avtp_tscf.c src/common.c src/avtp_stream.c src/can.c acf_talker.c
'''
Running; 
sudo ./acf_talker -d <destination mac> -i <interface name> -m <maximum transmit time(optional)>
-------------------------------------------------------------------------------------------------
* Execution of acf_listener
Building : 
'''shell
cc -I./src -I./include -o acf_listener src/avtp.c src/avtp_tscf.c src/common.c src/avtp_stream.c src/can.c acf_listener.c
'''
Running : 

 '''shell
sudo ./acf_listener -d <source mac> -i <interface name> 
'''
----
* Execution of ntscf_talker 
Building: 
cc -I./src -I./include -o ntscf_talker src/avtp.c src/avtp_ntscf.c src/common.c src/can.c ntscf_talker.c

Running: 
sudo ./ntscf_talker -d <destination mac> -i <interface name>

---------------------------------------------------------------------
Building: 
cc -I./src -I./include -o ntscf_listener src/avtp.c src/avtp_ntscf.c src/common.c src/can.c ntscf_listener.c

Running: 
sudo ./ntscf_listener -d <source mac> -i <interface name>
================================================================
 
THE END
