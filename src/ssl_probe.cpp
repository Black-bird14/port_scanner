#include "ssl_probe.hpp"

//using namespace SslProbe;

vector<ProbeResult> SslProbe::probe_sync(const ResolvedTarget& target_ip,
                                   uint16_t port_start,
                                   uint16_t port_stop,
                                   int timeout, string hostname) {
    //will be acting as main
    // Create a context that uses the default paths for
    // finding CA certificates.
    ssl::context ctx(ssl::context::sslv3_client);
    ctx.set_default_verify_paths();
    std::vector<ProbeResult> results;

    for (uint16_t port = port_start; port <= port_stop; port++){
        // Open a socket and connect it to the remote host.
        asio::io_context io_context;
        SSLSocket sock(io_context, ctx);
        tcp::resolver resolver(io_context);
        sock.lowest_layer().set_option(tcp::no_delay(true));
        sock.set_verify_callback(
            [this](bool preverified, boost::asio::ssl::verify_context& ctx) {
                return this->verify(preverified, ctx);
            });

        if(!hostname.empty()){
            auto target_eps = resolver.resolve(hostname, std::to_string(port));//TODO: MIGHT USE ERROR_CODE
            SslProbe::connect_(target_eps, sock);
        }

        else {
            /* build from target_ip + port */
            tcp::endpoint ep = to_tcp_endpoint(target_ip, port);
            SslProbe::connect_(ep, sock);
        }

        // run until probe finishes or timeout triggers inside your probe logic
        io_context.run();

        // push whatever result handler recorded for this port
        // results.push_back(...);
    }

    return results;
}

void SslProbe::perform_handshake(SSLSocket& sock){//handshake handler as a lamda function
    sock.async_handshake(ssl::stream_base::client,
        [this, &sock](const boost::system::error_code& ec) {
            this->handler(ec, HandlerType::HANDSHAKE, sock);
        });

}

//The function object to be used for verifying a certificate
bool SslProbe::verify(bool preverified, // True if the certificate passed pre-verification.
      ssl::verify_context& ctx // The peer certificate and other context.
      ){
        // The verify callback can be used to check whether the certificate that is
        // being presented is valid for the peer. For example, RFC 2818 describes
        // the steps involved in doing this for HTTPS. Consult the OpenSSL
        // documentation for more details. Note that the callback is called once
        // for each certificate in the certificate chain, starting from the root
        // certificate authority.

        // For now will simply print the certificate's subject name.
        char subject_name[256];
        X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
        X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
        cout << "Verifying " << subject_name << "\n";

        return preverified;
}

//For direct IP handling 
void SslProbe::connect_(const tcp::endpoint& single_ep, SSLSocket& sock){
    sock.lowest_layer().async_connect(single_ep,
        [this, &sock](const boost::system::error_code& ec) {
             this->handler(ec, HandlerType::CONNECT, sock);
        });
}

//For after hostname was resolved
void SslProbe::connect_(const tcp::resolver::results_type& multiple_eps, SSLSocket& sock){
    boost::asio::async_connect(sock.lowest_layer(), multiple_eps,
        [this, &sock](const boost::system::error_code& ec, const tcp::endpoint& /*ep*/) {
             this->handler(ec, HandlerType::CONNECT, sock);
        });
}



//TODO: UNFINISHED AND UNINTEGRATED
void SslProbe::handler(const boost::system::error_code& error, const HandlerType& htype, SSLSocket& sock){
    if (!error)
          {
            switch(htype){
                case HandlerType::CONNECT:
                    SslProbe::perform_handshake(sock);
                    break;
                case HandlerType::HANDSHAKE:
                    std::cout << "Handshake was successful" << endl;
                    break;
            }
            
          }
    else
        //std::cout << htype << " failed: " << error.message() << "\n";
          cout<< "Operation failed"<<endl;//FOR NOW
}

//ASSUMED TO BE DONE AND FUNCTIONAL
tcp::endpoint SslProbe::to_tcp_endpoint(ResolvedTarget ip, uint16_t port){
    // Use AF_ to determine which to convert to (ipv4 vs ipv6)
    if (ip.family == AF_INET) {
        auto* addr4 = reinterpret_cast<const sockaddr_in*>(&ip.addr);

        boost::asio::ip::address_v4::bytes_type bytes{};
        std::memcpy(bytes.data(), &addr4->sin_addr, bytes.size());

        // Then to a specific endpoint if needed (e.g., TCP)
        return tcp::endpoint(boost::asio::ip::address_v4(bytes), port);
    
    }

    else if(ip.family == AF_INET6){
        // Copy the base IPv6 address
        auto* addr6 = reinterpret_cast<const sockaddr_in6*>(&ip.addr);

        boost::asio::ip::address_v6::bytes_type bytes{};
        std::memcpy(bytes.data(), &addr6->sin6_addr, bytes.size());

        return tcp::endpoint(boost::asio::ip::address_v6(bytes), port);
    }

    throw std::runtime_error("Unsupported address family in sockaddr_storage");
}