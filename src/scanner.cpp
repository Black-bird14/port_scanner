#include "scanner.hpp"

// 1st Iteration
// Start simple: args will be target IP/domain + optional port range (default port range is 1-1024)
// For a start this will be single threaded
// Since there is domain/hostname option, then there should be domain resolution
// Decide on how to display results
// Add progress bar

// 2nd Iteration
// Add verbose mode, although got to decide what verbose & non-verbose modes should look like
//Add different scan types (UDP, SYN, maybe try to replicate the scan types nmap provides)


// 3rd Iteration
// Add multithreading (1 thread = subrange)

string ports;
vector<string> scans;

po::options_description get_options_description() {
    po::options_description desc("Options:");
    desc.add_options()
        ("help,h", "Display help message")
        ("ports,p", po::value<string>(&ports)->default_value(PORT_RANGE), 
      "Set port range to scan, e.g. -p 1000-5000, if no port range is provided, the default is 1-1024.")
        ("verbose,v", "Displays a more detailed output during the scan.")
        ("scans", po::value< vector<string> >(&scans), 
        "Run multiple scan types.\nNOTE: Can't run two TCP scans at once, so only a TCP scan of choice and UDP scan can be run together.") 
        ("tC", "TCP connect Scan")
        ("tS", "TCP SYN Scan (stealth mode)")
    ;
    return desc;
}

int main(int argc, char* argv[]) {
    // 1. Parse arguments: target IP/domain + optional port range [x]
    // 2. Resolve hostname to IP [x]
    // 3. Try to connect to port
    //    - If connected, report as open
    // 4. Spawn threads to scan different port chunks
    // 5. Each thread:
    //    - Try to connect to port
    //    - If connected, report as open
    // 6. Join all threads
    // 7. Display results

    po::options_description desc = get_options_description();
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hostname/IP address>\n", argv[0]);
        cout << desc << endl;
        exit(EXIT_FAILURE);
    }

    if(vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    if (is_valid_ip_addr(argv[1])) {
        cout << "Valid IP Provided, proceeding with scan." << endl;
        const string target_ip = argv[1];
    }
    //TODO: Implement IPV6 regex-based validator
    else {
        //DNS Resolution here
        const auto [target_ipv4, target_ipv6] = dns_resolver(argv[1]);
        cout << "Resolved IP(s) address for hostname "<< argv[1]<< " : \n"
        << "IPV4: " << target_ipv4 << "\n IPV6: "
        << target_ipv6 << endl;
    }

    if(vm.count("tC")) cout << "TCP connect scan selected" << endl;

    //start scan
    // first turn port ranges in port list
    /*
    for ( variables_map::iterator i = vm.begin() ; i != vm.end() ; ++ i )
    {
        
    }

    */


    return 0;
    

}