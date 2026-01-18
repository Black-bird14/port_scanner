#ifndef TCP_CONNECT_HPP
#define TCP_CONNECT_HPP

using namespace std;


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <cstring>
#include <stdexcept>
#include <cerrno>
#include <chrono>
#include <ostream>
#include <iostream>
#include <netinet/in.h>
#include "probe.hpp"

class TcpConnectProbe : public Probe {
    private:
        ResolvedTarget target_ip;
        uint16_t port_start;
        uint16_t port_stop;
        int timeout;
    public:
        TcpConnectProbe (const ResolvedTarget& ip, string hostname, uint16_t pstart, uint16_t pstop, int t)
                    : target_ip(ip), port_start(pstart), port_stop(pstop), timeout(t) {}
                            
        vector<ProbeResult> probe_sync(const ResolvedTarget& target_ip,
                                   uint16_t port_start,
                                   uint16_t port_stop,
                                   int timeout) override;
};

#endif