
#pragma once

#include <cstring>
#include <ctime>
#include <sys/_types/_socklen_t.h>
#include <time.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <netdb.h>
#include <unistd.h>

int sock;
void handle_sig(int signal);

/*
*           struct addrinfo {
*               int              ai_flags;
*               int              ai_family;
*               int              ai_socktype;
*               int              ai_protocol;
*               socklen_t        ai_addrlen;
*               struct sockaddr *ai_addr;
*               char            *ai_canonname;
*               struct addrinfo *ai_next;
*               };
*               
*               struct defined in netdb header
*/
