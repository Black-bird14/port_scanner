#include "tcp_connect.hpp"

// potential template for 
// Try to connect with a timeout (milliseconds). Returns true on successful connect.
static bool connect_with_timeout(int sockfd, const struct sockaddr* sa, socklen_t sa_len, int timeout_ms) {
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