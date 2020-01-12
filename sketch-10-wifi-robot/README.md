# The list of files in this repository #

* 8266-robot-round-wifi-v1.ino: just testing the wi-fi connection;
* 8266-robot-round-wifi-v2.ino: test the http server;
* 8266-robot-round-wifi-v3.ino: add the motor controlling code
* 8266-robot-round-wifi-v4.ino: add the http commands handler
* 8266-robot-round-wifi-final.ino: add a html5 interface, to allow mobile phone control;
* 8266-robot-round-wifi-interface.html: the html5 interface code;

## Observation ##

* if you want to run the interface locally on your computer, you need to change line 12, u:'/rpc/', to include the IP address produced by the wi-fi connection like this: u:'http://{ip_address}/rpc/', where {ip_address} is the assigned IP address;