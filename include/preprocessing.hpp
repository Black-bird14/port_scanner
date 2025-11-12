#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <cerrno>
#include <chrono>
#include <ostream>
#include <iostream>

extern string PORT_RANGE;
extern string MODE;

struct ResolvedTarget {
    sockaddr_storage addr{};
    socklen_t addrlen = 0;
    int family = AF_UNSPEC;
    string printable_ip;
};


static bool connect_with_timeout(int sockfd, const struct sockaddr* sa, socklen_t sa_len, int timeout_ms);
ResolvedTarget dns_resolver(const string& hostname, int timeout_ms = 500);

#endif