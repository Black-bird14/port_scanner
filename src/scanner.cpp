#include "preprocessing.hpp"

string PORT_RANGE = "1-1024";
string MODE = "TCP";

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


    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hostname/IP address>\n", argv[0]);
        cout << "Options:" << endl;
        cout << "   -p <port range>: Example -p 1000-5000, if no port range is provided, the default is 1-1024." << endl;
        cout << "   -v: Verbose output, displays a more detailed output during the scan." << endl;
        exit(EXIT_FAILURE);
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
    //start scan
    // first split port subranges among threads (2nd iter)


    return 0;
    

}