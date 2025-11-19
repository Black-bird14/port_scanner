#include "probe_result.hpp"


void print_results(const std::vector<ProbeResult>& results) {
    vector<uint16_t> ports;
    cout << "SCan Results" << endl;
       for (ProbeResult r: results){
        if(r.status == ProbeResult::Status::CLOSED){
            ports.push_back(r.port);
        }
        else
        cout << r << endl;
       }

       //check if verbose, then print closed ports

       /*
       << " Port:     " << r.port   << "\n"
       << " Protocol: " << r.protocol << "\n"
       << " Status:   " << r.status   << "\n"
       << " RTT:      " << r.rtt_ms << " ms\n";*/
}