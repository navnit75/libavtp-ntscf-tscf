## Introduction
- This is mostly way too simple to be on git, or idk. But anyways....
- So when working on CAN protocol in vehicles which is kinda legacy, me in the hopes of building a gateway engine which tunnels CAN to Ethernet, and Ethernet to CAN packets. 
- Needed a serialization layer, with APIs to set the fields for creating the Ethernet packets.
- This repository also contains the serialization layer for CAN packets.
- Hope this helps someone
 
## Was trying to create some kind of Audio transmitter which transmits the audio from one Node to Another
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
