// include/probe.hpp
#pragma once

#include "probe_result.hpp"
#include "preprocessing.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include <optional>

/*
  Probe - abstract interface for a scan type (TCP connect, SYN, UDP, etc.)

  Design goals:
  - Keep a simple synchronous API for the first iteration (probe_sync).
  - Provide an optional async-ish API (start_probe / poll_probe / cancel_probe)
    so you can later implement event-driven or raw-pcap probes without changing
    the orchestrator's wiring drastically.
  - Implementations should be lightweight to construct; any heavy resources
    (pcap handles, raw sockets) can be managed inside the implementation class.
*/

class Probe {
public:
    virtual ~Probe() = default;

    // Synchronous probe: perform the probe and return a ProbeResult.
    // - timeout_ms: time budget for the probe (ms). Implementations should honor it.
    // This is the easiest to use while building single-threaded versions.
    virtual vector<ProbeResult> probe_sync(const ResolvedTarget& target_ip,
                                   uint16_t port_start, uint16_t port_stop,
                                   int timeout_ms) = 0;

    // Optional asynchronous-ish API (implement later if needed):
    // Start a probe and return an opaque probe id. The caller must later call
    // poll_probe to get a result or cancel_probe to abort.
    //
    // Default implementations return std::nullopt to indicate "not supported".
    virtual optional<int> start_probe(const ResolvedTarget& target_ip,
                                           uint16_t port_start, uint16_t port_stop,
                                           int timeout_ms) {
        return nullopt;
    }

    // Poll the in-flight probe (non-blocking). Returns:
    //  - std::nullopt: still pending
    //  - ProbeResult: probe finished (success/failure)
    // On error, an implementation can return a ProbeResult with Status::ERROR.
    virtual optional<ProbeResult> poll_probe(int probe_id) {
        return nullopt;
    }

    // Cancel an in-flight probe started via start_probe (best-effort).
    // Return true if canceled (or already finished), false on error/not-supported.
    virtual bool cancel_probe(int probe_id) {
        return false;
    }
};
