# Smart Card Redirection Ubuntu 18.04 WebSocket Server

The project is based on https://github.com/SC-Develop/SCD_SMCAuthServer

## The Project Compilation
This is a Qt 5 project and can be compiled with Qt Creator
For installation of the tool on Ubuntu 18.04 see https://lucidar.me/en/dev-c-cpp/how-to-install-qt-creator-on-ubuntu-18-04/

## Networking setup
The testing JavaScript accesses the server by 'sc_server.com' domain name on port 10522
So, make sure you have TCP 10522 port open for inbound access on the Ubuntu PC/VM
In the test setup add the following line to the 'hosts' file on the Windows machine

<code>
<ip address>         sc_server.com
</code>

<ip address> here is the Ubuntu PC/VM static IP

## Cryptography
The project utilizes OpenSSL for all cryptographic operations. It is supposed to be installed on Ubuntu 18.04 by default. Check it by running the following command on the terminal

<code>
~$ openssl version
OpenSSL 1.1.1  11 Sep 2018
</code>
