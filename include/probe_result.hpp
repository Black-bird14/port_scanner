#include <string>
#include <vector>
#include <ostream>
#include <iostream>
#include <iomanip>


using namespace std;


struct ProbeResult {
    string target;        // IPv[4|6] string
    uint16_t    port = 0;
    enum class Protocol { TCP, UDP, ICMP } protocol = Protocol::TCP;
    enum class Status { OPEN, CLOSED, FILTERED, TIMEOUT, ERROR, UNKNOWN } status = Status::UNKNOWN;
    string service_banner; // optional, e.g. "HTTP/1.1 200 OK"
    double     rtt_ms = 0.0;   // optional round-trip time in milliseconds
    string details;          // human-readable diagnostics or errno string
};

//
// =====================
//  Printing enums
// =====================
//

inline ostream& operator<<(ostream& os, ProbeResult::Protocol p) {
    switch (p) {
        case ProbeResult::Protocol::TCP:   return os << "TCP";
        case ProbeResult::Protocol::UDP:   return os << "UDP";
        case ProbeResult::Protocol::ICMP:  return os << "ICMP";
    }
    return os << "UnknownProtocol";
}

inline ostream& operator<<(ostream& os, ProbeResult::Status s) {
    switch (s) {
        case ProbeResult::Status::OPEN:     return os << "OPEN";
        case ProbeResult::Status::CLOSED:   return os << "CLOSED";
        case ProbeResult::Status::FILTERED: return os << "FILTERED";
        case ProbeResult::Status::TIMEOUT:  return os << "TIMEOUT";
        case ProbeResult::Status::ERROR:    return os << "ERROR";
        case ProbeResult::Status::UNKNOWN:  return os << "UNKNOWN";
    }
    return os << "InvalidStatus";
}

//
// =====================================
//  Pretty-print a full ProbeResult
// =====================================
//
vector<string> wrap_text(const string &content, int width);

inline ostream& operator<<(ostream& os, const ProbeResult& r) {
    // --- Column 1: Port/Protocol ---
    ostringstream col1;
    col1 << "  " << r.port << "/" << r.protocol;

    // --- Column 3: RTT + unit ---
    ostringstream col3;
    col3 << r.rtt_ms << " ms";

    os << left
       << setw(23) << col1.str()
       << setw(12) << r.status
       << setw(12) << col3.str();

    if(!r.service_banner.empty()){
        auto banner_lines = wrap_text(r.service_banner, 20);

        if(banner_lines.size() == 1) os << setw(30) << banner_lines[0];

        else{
            os << setw(30) << banner_lines[0] << "\n";
            for (size_t i = 1; i < banner_lines.size(); i++) {
                os << setw(23) << ""       // indent to align columns
                    << setw(12) << ""       // empty for status column
                    << setw(12) << ""       // empty for RTT column
                    << setw(30) << banner_lines[i];
            }
        }
    }
    else os << setw(30) << "No service info";

    if (!r.details.empty()) {
        auto detail_lines = wrap_text(r.details, 28);

        // First details line printed normally
        os << setw(23) << ""
           << setw(12) << ""
           << setw(12) << ""
           << setw(30) << detail_lines[0]
           << "\n";

        // Remaining detail lines
        for (size_t i = 1; i < detail_lines.size(); i++) {
            os << setw(23) << ""
               << setw(12) << ""
               << setw(12) << ""
               << setw(30) << detail_lines[i]
               << "\n";
        }
    }
    else {
        os << setw(15) << ""
           << setw(30) << "No details" << "\n";
    }

    return os;
}

void print_results(const vector<ProbeResult>& results);
void print_with_closed_ports(const vector<ProbeResult>& results);