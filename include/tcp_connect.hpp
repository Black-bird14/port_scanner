#ifndef TCP_CONNECT_HPP
#define TCP_CONNECT_HPP

using namespace std;


#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "probe.hpp"

class TcpConnectProbe : public Probe {
    private:
        string target_ip;
        uint16_t port;
        int timeout;
    public:
        TcpConnectProbe (const string& ip, uint16_t p, int t)
                                   : target_ip(ip), port(p), timeout(t){}
                            
        ProbeResult probe_sync(const string& target_ip,
                                   uint16_t port,
                                   int timeout) override {
            //init socket here
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            
        }
}

#endif