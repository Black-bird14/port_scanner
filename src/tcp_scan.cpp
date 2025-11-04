#include "tcp_scan.hpp"

bool is_ip_addr(const string& ip_addr){
    if (ip_addr.empty()) {
        return false; // An empty string is not a number
    }
    static const regex ip_pattern(R"((\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b)");
    
    return regex_match(ip_addr, ip_pattern);
}

string dns_resolver(const string& hostname){
    struct addrinfo hints, *result, *rp;
    int status;
    char ipstr[INET6_ADDRSTRLEN];  // enough for both IPv4 and IPv6

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
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
        void* addr;
        std::string ipver;

        // Get pointer to the address itself
        if (rp->ai_family == AF_INET) { // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)rp->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)rp->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Convert IP to human-readable form
        inet_ntop(rp->ai_family, addr, ipstr, sizeof ipstr);
        std::cout << "  " << ipver << ": " << ipstr << "\n";
    }
    freeaddrinfo(result); // free the linked list
    
    return string(ipstr);
}