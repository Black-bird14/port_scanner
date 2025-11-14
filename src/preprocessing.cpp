#include "preprocessing.hpp"


bool is_valid_ip_addr(const string& ip, ResolvedTarget& target){
    char ipstr[INET6_ADDRSTRLEN];
    sockaddr_in sa4{};
    sockaddr_in6 sa6{};

    if (inet_pton(AF_INET, ip.c_str(), &sa4.sin_addr) == 1) {
        sa4.sin_family = AF_INET;
        target.family = AF_INET;
        target.addrlen = sizeof(sa4);
        memcpy(&target.addr, &sa4, sizeof(sa4));
        inet_ntop(AF_INET, &sa4.sin_addr, ipstr, sizeof(ipstr));
        target.printable_ip = ipstr;

        cout << "Valid IPV4 address provided, proceeding with scan." << endl;

        return true;
    }
    if (inet_pton(AF_INET6, ip.c_str(), &sa6.sin6_addr) == 1) {
        sa6.sin6_family = AF_INET6;
        target.family = AF_INET6;
        target.addrlen = sizeof(sa6);
        memcpy(&target.addr, &sa6, sizeof(sa6));
        inet_ntop(AF_INET6, &sa6.sin6_addr, ipstr, sizeof(ipstr));
        target.printable_ip = ipstr;

        cout << "Valid IPV6 address provided, proceeding with scan." << endl;

        return true;
    }

    return false;
}

// Resolve hostname:port, try each result and return the first connectable address.
// Throws std::runtime_error on failure.
ResolvedTarget dns_resolver(const string& hostname, int timeout_ms) {

    ResolvedTarget target{};
    char ipstr[INET6_ADDRSTRLEN];

    // --- Case 1: literal IPv4 or IPv6 ---
    if(is_valid_ip_addr(hostname, target)){
        return target;
    }

    // --- Case 2: DNS Resolution ---

    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;        // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP
    hints.ai_flags = AI_ADDRCONFIG;     // skip families not configured

    addrinfo* result = nullptr;
    int rc = getaddrinfo(hostname.c_str(), nullptr, &hints, &result);
    if (rc != 0) {
        throw runtime_error(string("getaddrinfo: ") + gai_strerror(rc));
    }

    addrinfo* rp;
    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        // Copy the successful address into the return struct
        memcpy(&target.addr, rp->ai_addr, rp->ai_addrlen);
        target.addrlen = rp->ai_addrlen;
        target.family = rp->ai_family;

        if (rp->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)rp->ai_addr;
            inet_ntop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
        } 

        else if (rp->ai_family == AF_INET6) {
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)rp->ai_addr;
            inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipstr, sizeof(ipstr));
        }

        target.printable_ip = ipstr;

        cout << "Resolved IP(s) address for hostname "<< hostname << " : \n"<< target.printable_ip << endl;

        break; // done
    }

    freeaddrinfo(result);

    if (target.addrlen == 0) {
        throw runtime_error("No connectable address found for " + hostname);
    }
    return target;
}