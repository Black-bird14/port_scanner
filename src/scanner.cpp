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

po::options_description get_options_description() {
    po::options_description desc("Options:");
    desc.add_options()
        ("help,h", "Display help message")
        ("ports,p", po::value<string>(&port_range)->default_value(PORT_RANGE), 
      "Set port range to scan, e.g. -p 1000-5000, if no port range is provided, the default is 1-1024.")
        ("verbose,v", "Displays a more detailed output during the scan.")
        ("scans", po::value< vector<string> >(&scans), 
        "Run multiple scan types.\nNOTE: Can't run two TCP scans at once, so only a TCP scan of choice and UDP scan can be run together.") 
        ("connect,C", "TCP connect Scan")
        ("syn,S", "TCP SYN Scan (stealth mode)")
        ("udp,U", "UDP Scan (stealth mode)");
    return desc;
}

pair<int, int> parse_port_range(const string& range) {
    int start, stop;
    char dash;
    istringstream iss(range);

    if (!(iss >> start >> dash >> stop) || dash != '-') {
        throw invalid_argument("Invalid port range format. Expected format: start-stop (e.g. 20-80)");
    }
    if (start < 1 || stop > 65535 || start > stop) {
        throw out_of_range("Invalid port range values (must be 1–65535 and start <= stop)");
    }
    return {start, stop};
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

    po::options_description hidden("Hidden");
    hidden.add_options()
        ("target", po::value<string>(&target), "Target host or IP");
    po::options_description all("All options");
    all.add(desc).add(hidden);
    po::positional_options_description pos;
    pos.add("target", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(all).positional(pos).run(), vm);
    po::notify(vm);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [options] [hostname/IP address]\n", argv[0]);
        cout << desc << endl;
        exit(EXIT_FAILURE);
    }

    if(vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    //Check IP or Resolve Hostname        
    target_ip = dns_resolver(target);
    // first turn port ranges to int
    auto [port_start, port_stop] = parse_port_range(port_range);

    if(vm.count("connect")){ 
        cout << "TCP Connect scan selected" << endl;
        //start scan
        TcpConnectProbe connectprobe = TcpConnectProbe(target_ip, port_start, port_stop, 500);
        scan_results = connectprobe.probe_sync(target_ip, port_start, port_stop, 500);

        print_results(scan_results);
    }
    else if (vm.count("syn")){
        cout << "TCP SYN scan selected (stealth mode)" << endl;
    }

    else if (vm.count("udp")){
        cout << "UDP scan selected" << endl;
    }


    return 0;
    

}