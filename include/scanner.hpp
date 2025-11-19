#ifndef SCANNER_HPP
#define SCANNER_HPP


#include <boost/program_options.hpp>
#include <sstream>
#include <stdexcept>
#include "preprocessing.hpp"
#include "tcp_connect.hpp"

namespace po = boost::program_options;

string PORT_RANGE = "1-1024";
string SCAN = "tC";

string target;
string port_range;
bool verbose = 0;
int ports;
vector<string> scans;
ResolvedTarget target_ip;
vector<ProbeResult> scan_results;
po::options_description get_options_description();
pair<int, int> parse_port_range(const string& range);


/*("include-path,I", po::value< vector<string> >(), 
  "include path")
    ("input-file", po::value< vector<string> >(), "input file")*/
#endif

//src: https://www.boost.org/doc/libs/latest/doc/html/program_options/overview.html#id-1.3.29.5.10