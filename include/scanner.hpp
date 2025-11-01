#ifndef SCANNER_HPP
#define SCANNER_HPP

using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <regex>
#include <cctype> // For isdigit()
#include <algorithm> // For std::all_of

extern string PORT_RANGE;
extern string MODE;

bool is_ip_addr(const string& target);


#endif