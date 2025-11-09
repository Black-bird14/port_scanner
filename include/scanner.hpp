#ifndef SCANNER_HPP
#define SCANNER_HPP
#include <boost/program_options.hpp>
#include "preprocessing.hpp"
#include <sstream>
#include <stdexcept>

using namespace std;
namespace po = boost::program_options;

string PORT_RANGE = "1-1024";
string SCAN = "tC";

string target;
string port_range;
int ports;
vector<string> scans;
po::options_description get_options_description();
pair<int, int> parse_port_range(const string& range);


/*("include-path,I", po::value< vector<string> >(), 
  "include path")
    ("input-file", po::value< vector<string> >(), "input file")*/
#endif

//src: https://www.boost.org/doc/libs/latest/doc/html/program_options/overview.html#id-1.3.29.5.10