#include "probe_result.hpp"


void print_results(const std::vector<ProbeResult>& results) {
    vector<uint16_t> ports;
    cout << "Scan Results" << "\n" << endl;
    cout << " Target:   " << results[0].target << "\n" << endl;
    cout << left
         << setw(22) << " Port/Protocol"
         << setw(13) << "Status"
         << setw(13) << "RTT"
         << setw(40) << "Service"
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
         << setw(40) << "Service"
         << "Details"
         << endl;
    for (ProbeResult r: results)  cout << r << endl;
    
}

vector<string> wrap_text(const string &content, int width){
    vector<string> lines;
    stringstream ss(content);
    string current = "";
    string token;
    vector <string> split_content;

    while(ss >> token) split_content.push_back(token);

    if(split_content.size() == 1) return split_content;

    for (string word : split_content){
        if (current.empty()){
            current = word;
        }

        else if (int((current + " " + word).length()) <= width){
            current+= " " + word;
        }

        else{
            lines.push_back(current);
            current = word;
        }

    }
    if (!current.empty())
        lines.push_back(current);

    return lines;
}