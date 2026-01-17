## Introduction
- This is mostly way too simple to be on git, or idk. But anyways....
- So when working on CAN protocol in vehicles which is kinda legacy, me in the hopes of building a gateway engine which tunnels CAN to Ethernet(AVTP), and Ethernet(AVTP) to CAN packets. 
- Needed a serialization layer, with APIs to set the fields for creating the Ethernet packets and CAN packets.
- This repository also contains the serialization layer for CAN packets.
- Hope this helps someone
- Few of things which can be checked out is :-
   - So, you can checkout IEEE - 1722 specification, for AVTP from which AVTP pdu formats are defined.
        - A more detailed preview of the papers include https://1.ieee802.org/tsn/
   - You can also checkout open source implementation (at the time I didn't know, and could have saved some time) , [this](https://github.com/COVESA/Open1722).
   - All this lies under a larger umbrella of TSN (Time Sensisitive Networking), which can be checked out [here](https://tsn.readthedocs.io/avb.html).

## To Do :-
- So now the data is being transmitted in terms of Audio File captured through the microphone of the talker
- Coz this was supposed to work for Real Time Data in LAN, so audio it was at the time
- I need to create something more CLI based which gives the overview of the process to user
- maybe try to use the
## Was trying to create some kind of Audio transmitter which transmits the audio from one Node to another by using the AUDIO as the data file
### Execution of acf_talker
Building 
```shell
cc -I./src -I./include -o acf_talker src/avtp.c src/avtp_tscf.c src/common.c src/avtp_stream.c src/can.c acf_talker.c
```
* Running
```shell
sudo ./acf_talker -d <destination mac> -i <interface name> -m <maximum transmit time(optional)>
```
----
### Execution of acf_listener
* Building : 
```shell
cc -I./src -I./include -o acf_listener src/avtp.c src/avtp_tscf.c src/common.c src/avtp_stream.c src/can.c acf_listener.c
```
* Running : 
```shell
sudo ./acf_listener -d <source mac> -i <interface name> 
```
-------------------------------------------------------------------------------------------------------
### Execution of ntscf_talker 
* Building: 
```shell
cc -I./src -I./include -o ntscf_talker src/avtp.c src/avtp_ntscf.c src/common.c src/can.c ntscf_talker.c
```
* Running:
```shell 
sudo ./ntscf_talker -d <destination mac> -i <interface name>
```
---------------------------------------------------------------------
### Execution of ntscf_listener
* Building: 
```shell
cc -I./src -I./include -o ntscf_listener src/avtp.c src/avtp_ntscf.c src/common.c src/can.c ntscf_listener.c
```
* Running: 
```shell
sudo ./ntscf_listener -d <source mac> -i <interface name>
```
