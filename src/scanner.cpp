#include "scanner.hpp"

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
// Add multithreading (1 thread = subrange)

// 3rd Iteration
bool is_ip_addr(const string& target){
    if (target.empty()) {
        return false; // An empty string is not a number
    }
    static const regex ip_pattern(R"((\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b)");
    
    return regex_match(target, ip_pattern);
}
//Add different scan types (UDP, SYN, maybe try to replicate the scan types nmap provides)
int main(int argc, char* argv[]) {
    // 1. Parse arguments: target IP/domain + optional port range
    // 2. Resolve hostname to IP
    // 3. Spawn threads to scan different port chunks
    // 4. Each thread:
    //    - Try to connect to port
    //    - If connected, report as open
    // 5. Join all threads
    // 6. Display results


    if (argc < 2) {
        cout << "Usage: scanner <hostname/IP address>" << endl;
        cout << "Options:" << endl;
        cout << "   -p <port range>: Example -p 1000-5000, if no port range is provided, the default is 1-1024." << endl;
        cout << "   -v: Verbose output, displays a more detailed output during the scan." << endl;
        return 0;
    }

    if (is_ip_addr(argv[1])) cout << "Valid IP" << endl;
    else cout << "Invalid IP" << endl;
    return 0;
    

}