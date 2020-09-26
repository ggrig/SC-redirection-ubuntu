#ifndef TCPTUNNEL_H
#define TCPTUNNEL_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#ifdef __MINGW32__
#define required_argument 0
#define no_argument 0
#include <winsock2.h>
#else
#include <getopt.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#ifdef __cplusplus
extern "C" {
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

#define OPTIONS_BUFFER_SIZE 4096

int build_server(void);
int wait_for_clients(void);
void handle_client(void);
void handle_tunnel(void);
int build_tunnel(void);
int use_tunnel(void);
int fd(void);

#ifdef __MINGW32__
void set_option(char option, const char *optarg);
#else
void set_options(int argc, char *argv[]);
void set_option(char **option, char *value);
#endif

char *get_current_timestamp(void);

void print_help(void);
void print_helpinfo(void);
void print_usage(void);
void print_version(void);
void print_missing(const char *message);

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
#ifndef __MINGW32__
	unsigned int buffer_size;
#endif
};

struct struct_rc {
#ifdef __MINGW32__
	SOCKET server_socket;
	SOCKET client_socket;
	SOCKET remote_socket;
#else
	int server_socket;
	int client_socket;
	int remote_socket;
#endif

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	struct sockaddr_in remote_addr;
	struct hostent *remote_host;
};

#endif

int stay_alive();

#ifdef __cplusplus
}
#endif
