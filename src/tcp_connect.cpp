#include "tcp_connect.hpp"




vector<ProbeResult> TcpConnectProbe::probe_sync(const ResolvedTarget& target_ip,
                        uint16_t port_start, uint16_t port_stop,
                        int timeout) {
    vector<ProbeResult> results;
    ProbeResult result;
    for (uint16_t port = port_start; port <= port_stop; port++){
        int connect_socket = socket(target_ip.family, SOCK_STREAM, 0);
        if (connect_socket < 0) continue;
        int status;

        if (target_ip.family == AF_INET) {
        // Copy the base IPv4 address, then set the port
        sockaddr_in addr4 = *reinterpret_cast<const sockaddr_in*>(&target_ip.addr);
        addr4.sin_port = htons(port);

        status = connect(connect_socket,
                         reinterpret_cast<const sockaddr*>(&addr4),
                         target_ip.addrlen);
        } 
        else if (target_ip.family == AF_INET6) {
            // Copy the base IPv6 address, then set the port
            sockaddr_in6 addr6 = *reinterpret_cast<const sockaddr_in6*>(&target_ip.addr);
            addr6.sin6_port = htons(port);

            status = connect(connect_socket,
                            reinterpret_cast<const sockaddr*>(&addr6),
                            target_ip.addrlen);
        }
        if(status == 0){
            cout<< "Connect successful for port number: "<< port << endl;
            result.target = target_ip.printable_ip;
            result.port = port;
            result.protocol = ProbeResult::Protocol::TCP;
            result.status = ProbeResult::Status::OPEN;
            results.push_back(result);
        }
        close(connect_socket);

    }
    return results;
}



// potential template for 
// Try to connect with a timeout (milliseconds). Returns true on successful connect.
/*static bool connect_with_timeout(int sockfd, const struct sockaddr* sa, socklen_t sa_len, int timeout_ms) {
    // Set non-blocking
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0) return false;
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    int rc = connect(sockfd, sa, sa_len);
    if (rc == 0) {
        // Connected immediately
        fcntl(sockfd, F_SETFL, flags); // restore flags
        return true;
    }
    if (errno != EINPROGRESS) {
        // real error
        fcntl(sockfd, F_SETFL, flags);
        return false;
    }

    // Wait with select()
    fd_set wf;
    FD_ZERO(&wf);
    FD_SET(sockfd, &wf);
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    rc = select(sockfd + 1, NULL, &wf, NULL, &tv);
    if (rc <= 0) {
        // timeout or select error
        fcntl(sockfd, F_SETFL, flags);
        return false;
    }

    // Check socket error
    int so_error = 0;
    socklen_t len = sizeof(so_error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len) < 0) {
        fcntl(sockfd, F_SETFL, flags);
        return false;
    }
    fcntl(sockfd, F_SETFL, flags);
    return (so_error == 0);
}
*/