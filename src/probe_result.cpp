#include "probe_result.hpp"


void print_results(const std::vector<ProbeResult>& results) {
    vector<uint16_t> ports;
    cout << "Scan Results" << endl;
    cout << " Target:   " << results[0].target << endl;
    cout << left
         << setw(22) << " Port/Protocol"
         << setw(15) << "Status"
         << setw(18) << "RTT"
         << setw(30) << "Service"
         << "Details"
         << endl;
    
    for (ProbeResult r: results){
        if(r.status == ProbeResult::Status::CLOSED){
            ports.push_back(r.port);
        }
        else
        cout << r << endl;
    }

       cout << "Found " << ports.size() << " closed port(s)." << endl;
}

void print_with_closed_ports(const std::vector<ProbeResult>& results){
    vector<uint16_t> ports;
    cout << "Scan Results" << endl;
    cout << " Target:   " << results[0].target << endl;
    cout << left
         << setw(22) << " Port/Protocol"
         << setw(15) << "Status"
         << setw(18) << "RTT"
         << setw(30) << "Service"
         << "Details"
         << endl;
    for (ProbeResult r: results)  cout << r << endl;
    
}