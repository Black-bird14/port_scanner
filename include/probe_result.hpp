#include <string>
#include <vector>
#include <ostream>
#include <iostream>

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

inline std::ostream& operator<<(std::ostream& os, ProbeResult::Protocol p) {
    switch (p) {
        case ProbeResult::Protocol::TCP:   return os << "TCP";
        case ProbeResult::Protocol::UDP:   return os << "UDP";
        case ProbeResult::Protocol::ICMP:  return os << "ICMP";
    }
    return os << "UnknownProtocol";
}

inline std::ostream& operator<<(std::ostream& os, ProbeResult::Status s) {
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

inline std::ostream& operator<<(std::ostream& os, const ProbeResult& r) {
    os << "  " << r.port << "/" << r.protocol << "              "
       << r.status   << "          "
       << r.rtt_ms << " ms\n";

    if (!r.service_banner.empty())
        os << " Banner:   " << r.service_banner << "\n";

    if (!r.details.empty())
        os << " Details:  " << r.details << "\n";

    return os;
}

void print_results(const std::vector<ProbeResult>& results);
void print_with_closed_ports(const std::vector<ProbeResult>& results);