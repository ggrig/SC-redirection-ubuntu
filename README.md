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

The project is based on https://github.com/SC-Develop/SCD_SMCAuthServer

## The Project Compilation
This is a Qt 5 project and can be compiled with Qt Creator
For installation of the tool on Ubuntu 18.04 see https://lucidar.me/en/dev-c-cpp/how-to-install-qt-creator-on-ubuntu-18-04/

## Networking setup
The testing JavaScript accesses the server by 'sc_server.com' domain name on port 10522
So, make sure you have TCP 10522 port open for inbound access on the Ubuntu PC/VM
In the test setup add the following line to the 'hosts' file on the Windows machine

```
ip_address         sc_server.com .
```

"ip_address" here is the Ubuntu PC/VM static IP

## Cryptography
The project utilizes OpenSSL for all cryptographic operations. It is supposed to be installed on Ubuntu 18.04 by default. Check it by running the following command on the terminal

```
~$ openssl version .
OpenSSL 1.1.1  11 Sep 2018
```