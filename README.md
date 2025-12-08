Feature_1_1 Main update

short for below:
OTA --> OTA Server
GW -- ESP32- GW
Client --ESP32_Client ECU


This setup as Stage1_ sub as 1.1 version

from Stage1, we normally realize the TCP connection between OTA server and client, 
From system test, we detect TCP connection was not stable between OTA and GW, 
Main reason is the tcp rx_buffer not setup correct in every start init phase, 
this cased the OTA json task send to GW, but GW cannot decode correctly, after GW reset, it can batch display the buffer data.

after use ring buffer for json decode, the tcp data can tx/rx with OTA stabely. 

