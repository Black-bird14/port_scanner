#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

using namespace std;

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <regex>
//DNS Resolution
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
//

extern string PORT_RANGE;
extern string MODE;

bool is_valid_ip_addr(const string& target);
pair<string, string> dns_resolver(const string& hostname);

#endif