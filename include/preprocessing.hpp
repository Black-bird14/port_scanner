#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

using namespace std;

#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdexcept>
#include <string>
#include <cerrno>
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


ResolvedTarget dns_resolver(const string& hostname, int timeout_ms = 1000);

#endif