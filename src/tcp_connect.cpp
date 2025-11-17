#include "tcp_connect.hpp"




vector<ProbeResult> TcpConnectProbe::probe_sync(const ResolvedTarget& target_ip,
                        uint16_t port_start, uint16_t port_stop,
                        int timeout) {
    vector<ProbeResult> results;
    ProbeResult result;
    for (uint16_t port = port_start; port <= port_stop; port++){
        int connect_socket = socket(target_ip.family, SOCK_STREAM, 0);
        if (connect_socket < 0) continue;
        // Set non-blocking
        int flags = fcntl(connect_socket, F_GETFL, 0);
        if (flags < 0) continue;
        fcntl(connect_socket, F_SETFL, flags | O_NONBLOCK);

        int status;
        const sockaddr* sa;
        socklen_t sa_len;
        if (target_ip.family == AF_INET) {
            // Copy the base IPv4 address, then set the port
            sockaddr_in addr4 = *reinterpret_cast<const sockaddr_in*>(&target_ip.addr);
            addr4.sin_port = htons(port);
            sa = reinterpret_cast<const sockaddr*>(&addr4);
            sa_len = sizeof(addr4);
        } 
        else if (target_ip.family == AF_INET6) {
            // Copy the base IPv6 address, then set the port
            sockaddr_in6 addr6 = *reinterpret_cast<const sockaddr_in6*>(&target_ip.addr);
            addr6.sin6_port = htons(port);
            sa = reinterpret_cast<const sockaddr*>(&addr6);
            sa_len = sizeof(addr6);
        }

        // |----------------------------|
        // | STAGE A — initial connect()|
        // |----------------------------|
        status = connect(connect_socket, sa, sa_len);
        result.target = target_ip.printable_ip;
        result.port = port;
        result.protocol = ProbeResult::Protocol::TCP;
        if(status == 0){
            cout<< "Connect successful for port number: "<< port << endl;
            result.status = ProbeResult::Status::OPEN;
            results.push_back(result);
            close(connect_socket);
            continue;
        }

        if (errno != EINPROGRESS) {
            // real error
            if (errno == ECONNREFUSED)
                result.status = ProbeResult::Status::CLOSED;
            else
                result.status = ProbeResult::Status::ERROR;
            
            results.push_back(result);
            close(connect_socket);
            continue;
        }
        // |---------------------------|
        // | STAGE B — select() timeout|
        // |---------------------------|
    
        fd_set wf;
        FD_ZERO(&wf);
        FD_SET(connect_socket, &wf);
        struct timeval tv;
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        
        status = select(connect_socket + 1, NULL, &wf, NULL, &tv);
        if (status <= 0) {
            // Timeout => filtered
            result.status = ProbeResult::Status::FILTERED;
            results.push_back(result);
            close(connect_socket);
            continue;
        }
        if (status < 0) {
            result.status = ProbeResult::Status::ERROR;
            results.push_back(result);
            close(connect_socket);
            continue;
        }
        // |---------------------------|
        // | STAGE C — getsockopt()    |
        // |---------------------------|
        
        
        // Check socket error
        int so_error = 0;
        socklen_t len = sizeof(so_error);
        getsockopt(connect_socket, SOL_SOCKET, SO_ERROR, &so_error, &len);

        if (so_error == 0) 
            result.status = ProbeResult::Status::OPEN;
        else if (so_error == ECONNREFUSED)
            result.status = ProbeResult::Status::CLOSED;
        else if (so_error == ETIMEDOUT)
            result.status = ProbeResult::Status::FILTERED;
        else
            result.status = ProbeResult::Status::ERROR;
        
        results.push_back(result);
        close(connect_socket);

    }
    return results;
}
