#include "probe_result.hpp"


void print_results(const std::vector<ProbeResult>& results) {
    vector<uint16_t> ports;
    cout << "Scan Results" << endl;
    cout << " Target:   " << results[0].target << endl;
    cout  << " Port/Protocol:      " <<  " Status:      " << " RTT:      " << endl;
    for (ProbeResult r: results){
        if(r.status == ProbeResult::Status::CLOSED){
            ports.push_back(r.port);
        }
        else
        cout << r << endl;
    }

       cout << ports.size() << " ports were closed." << endl;

       //check if verbose, then print closed ports

       /*
       << " Port:     " << r.port   << "\n"
       << " Protocol: " << r.protocol << "\n"
       << " Status:   " << r.status   << "\n"
       << " RTT:      " << r.rtt_ms << " ms\n";*/
}

void print_with_closed_ports(const std::vector<ProbeResult>& results){
    vector<uint16_t> ports;
    cout << "Scan Results" << endl;
    cout << " Target:   " << results[0].target << endl;
    cout  << " Port/Protocol      " <<  " Status      " << " RTT      " << endl;
    for (ProbeResult r: results)  cout << r << endl;
    
}