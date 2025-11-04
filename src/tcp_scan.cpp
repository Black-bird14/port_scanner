#include "tcp_scan.hpp"

bool is_ip_addr(const string& ip_addr){
    if (ip_addr.empty()) {
        return false; // An empty string is not a number
    }
    static const regex ip_pattern(R"((\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b)");
    
    return regex_match(ip_addr, ip_pattern);
}

pair<string, string> dns_resolver(const string& hostname){
    struct addrinfo hints, *result, *rp;
    int status;
    char ipstr_v6[INET6_ADDRSTRLEN];  // enough for both IPv4 and IPv6
    char ipstr_v4[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6 
    hints.ai_socktype = SOCK_STREAM; // stream socket
    hints.ai_flags = AI_PASSIVE;    // For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */

    if ((status = getaddrinfo(hostname.c_str(), NULL, &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
              Try each address until we successfully bind(2).
              If socket(2) (or bind(2)) fails, we (close the socket
              and) try the next address. */

    // Loop through results
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        void *v4_addr, *v6_addr;
        std::string ipver4, ipver6;

        // Get pointer to the address itself
        if (rp->ai_family == AF_INET) { // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)rp->ai_addr;
            v4_addr = &(ipv4->sin_addr);
            ipver4 = "IPv4";
        } else { // IPv6
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)rp->ai_addr;
            v6_addr = &(ipv6->sin6_addr);
            ipver6 = "IPv6";
        }

        // Convert IP to human-readable form
        if(v4_addr){
            inet_ntop(rp->ai_family, v4_addr, ipstr_v4, sizeof ipstr_v4);
            std::cout << "  " << ipver4 << ": " << ipstr_v4 << "\n";
        }
        if(v6_addr){
            inet_ntop(rp->ai_family, v6_addr, ipstr_v6, sizeof ipstr_v6);
            std::cout << "  " << ipver6 << ": " << ipstr_v6 << "\n";
        }

    }
    freeaddrinfo(result); // free the linked list

    return {string(ipstr_v4), string(ipstr_v6)};
}