# Smart Card Redirection Ubuntu 18.04 WebSocket Server

## Prerequisites

- Install USB/IP client on Ubuntu 18.04

```
sudo apt-get remove --purge usbip* libusbip*
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install linux-tools-5.4.0-42-generic -y
```
The `usbip` binary will be placed in `/usr/lib/linux-tools/5.4.0-42-generic/` directory

For manual configuration of the client see:
https://developer.ridgerun.com/wiki/index.php?title=How_to_setup_and_use_USB/IP

## The Code

The project is based on the following:
- https://github.com/adamrehn/websocket-server-demo
- https://github.com/vakuum/tcptunnel

## Building the server
Building the server requires a recent version of the [CMake](https://cmake.org/) build system. To build the server, invoke the following commands in the `server` directory:

```
cmake .
cmake --build .
```
## Networking setup
The testing JavaScript accesses the server by 'sc_server.com' domain name on port 10522
So, make sure you have TCP 10522 port open for inbound access on the Ubuntu PC/VM
In the test setup add the following line to the 'hosts' file on the Windows machine

```
ip_address         sc_server.com .
```

"ip_address" here is the Ubuntu PC/VM static IP
