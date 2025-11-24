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
    // --- MAIN ROW (row 0) ---
    ostringstream col1;
    col1 << "  " << r.port << "/" << r.protocol;
    ostringstream col3;
    col3 << r.rtt_ms << " ms";

    // Prepare wrapped text (each becomes its own vector of lines)
    auto banner_lines = wrap_text(r.service_banner.empty() ?
                                  "No service info" : r.service_banner, 30);

    auto detail_lines = wrap_text(r.details.empty() ?
                                  "No details" : r.details, 30);

    size_t max_rows = std::max(banner_lines.size(), detail_lines.size());

    os << left
       << setw(23) << col1.str()
       << setw(12) << r.status
       << setw(12) << col3.str()
       << setw(40) << banner_lines[0]
       << setw(40) << detail_lines[0]
       << "\n";

    // --- WRAPPED FOLLOW-UP ROWS ---
    for (size_t i = 1; i < max_rows; i++) {
        os << setw(23) << ""          // empty Port/Protocol
           << setw(12) << ""          // empty Status
           << setw(12) << ""          // empty RTT
           << setw(40) << (i < banner_lines.size() ? banner_lines[i] : "")
           << setw(40) << (i < detail_lines.size() ? detail_lines[i] : "")
           << "\n";
    }
    return os;
}

void print_results(const vector<ProbeResult>& results);
void print_with_closed_ports(const vector<ProbeResult>& results);