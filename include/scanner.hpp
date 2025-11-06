#ifndef SCANNER_HPP
#define SCANNER_HPP
#include <boost/program_options.hpp>
#include "preprocessing.hpp"

using namespace std;
namespace po = boost::program_options;

string PORT_RANGE = "1-1024";
string SCAN = "tC";


string ports;
vector<string> scans;
po::options_description desc("Options:");
desc.add_options()
    ("help,h", "Display help message")
    ("ports,p", po::value<string>(&ports)->default_value(PORT_RANGES), 
  "Set port range to scan, e.g. -p 1000-5000, if no port range is provided, the default is 1-1024.")
    ("verbose,v", "Displays a more detailed output during the scan.")
    ("scans", po::value< vector<string> >(scans), 
    "Run multiple scan types.\nNOTE: Can't run two TCP scans at once, so only a TCP scan of choice and UDP scan can be run together.") 
    ("tC", "TCP connect Scan")
    ("tS", "TCP SYN Scan (stealth mode)")
;


/*("include-path,I", po::value< vector<string> >(), 
  "include path")
    ("input-file", po::value< vector<string> >(), "input file")*/
#endif