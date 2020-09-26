#ifndef TCPTUNNEL_H
#define TCPTUNNEL_H

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifdef __MINGW32__
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#define VERSION "0.8"

#define LOCAL_PORT_OPTION     'a'
#define REMOTE_PORT_OPTION    'b'
#define REMOTE_HOST_OPTION    'c'
#define BIND_ADDRESS_OPTION   'd'
#define CLIENT_ADDRESS_OPTION 'e'
#define BUFFER_SIZE_OPTION    'f'
#define FORK_OPTION           'g'
#define LOG_OPTION            'h'
#define STAY_ALIVE_OPTION     'i'
#define HELP_OPTION           'j'
#define VERSION_OPTION        'k'

#define PATH_SEPARATOR '/'

int build_server(void);
int wait_for_clients(void);
void handle_client(void);
void handle_tunnel(void);
int build_tunnel(void);
int use_tunnel(void);
int fd(void);

//void set_options(int argc, char *argv[]);
void set_option(char option, const char *value);

char *get_current_timestamp(void);

void print_help(void);
void print_helpinfo(void);
void print_usage(void);
void print_version(void);
void print_missing(const char *message);

bool stay_alive();

struct struct_settings {
	unsigned int local_port     : 1;
	unsigned int remote_host    : 1;
	unsigned int remote_port    : 1;
	unsigned int bind_address   : 1;
	unsigned int client_address : 1;
	unsigned int buffer_size    : 1;
	unsigned int fork           : 1;
	unsigned int log            : 1;
	unsigned int stay_alive     : 1;
};

struct struct_options {
	const char *local_port;
	const char *remote_host;
	const char *remote_port;
	const char *bind_address;
	const char *client_address;
	unsigned int buffer_size;
};

struct struct_rc {
	int server_socket;
	int client_socket;
	int remote_socket;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	struct sockaddr_in remote_addr;
	struct hostent *remote_host;
};

#endif

