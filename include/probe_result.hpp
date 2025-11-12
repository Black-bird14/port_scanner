#pragma once
#include <string>

struct ProbeResult {
    string target;        // IPv[4|6] string
    uint16_t    port = 0;
    enum class Protocol { TCP, UDP, ICMP } protocol = Protocol::TCP;
    enum class Status { OPEN, CLOSED, FILTERED, TIMEOUT, ERROR, UNKNOWN } status = Status::UNKNOWN;
    string service_banner; // optional, e.g. "HTTP/1.1 200 OK"
    double     rtt_ms = 0.0;   // optional round-trip time in milliseconds
    string details;          // human-readable diagnostics or errno string
};
