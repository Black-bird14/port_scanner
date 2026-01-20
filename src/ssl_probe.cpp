#include "ssl_probe.hpp"

//using namespace SslProbe;

vector<ProbeResult> probe_sync(const ResolvedTarget& target_ip,
                                   uint16_t port_start,
                                   uint16_t port_stop,
                                   int timeout, string hostname="") {
    //will be acting as main
    // Create a context that uses the default paths for
    // finding CA certificates.
    ssl::context ctx(ssl::context::sslv23);
    ctx.set_default_verify_paths();

    // Open a socket and connect it to the remote host.
    asio::io_context io_context;
    SSLSocket sock(io_context, ctx);
    tcp::resolver resolver(io_context);
    //tcp::resolver::query query("host.name", "https");
    sock.lowest_layer().set_option(tcp::no_delay(true));
    sock.set_verify_callback(std::bind(&SslProbe::verify, std::placeholders::_1, std::placeholders::_2));

    if(!hostname.empty()){
        auto target_ep = resolver.resolve(target_ep, "https");//TODO: MIGHT USE ERROR_CODE
        asio::connect(sock.lowest_layer(), target_ep);
        SslProbe::connect_(target_ep, sock);
    }

    else SslProbe::connect_(target_ip, sock);
}

void perform_handshake(){//handshake handler as a lamda function
    ssl::stream::async_handshake(SSLSocket::client,
        std::bind(&SslProbe::handler, boost::asio::placeholders::error));

}

//The function object to be used for verifying a certificate
void verify(bool preverified, // True if the certificate passed pre-verification.
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

void connect_(std::optional<ResolvedTarget> target_ip,
              std::optional<tcp::resolver::results_type> target_ep,
              SSLSocket& sock)
{
    if (target_ip) {
        tcp::endpoint ep = /* build from target_ip + port */; //TODO

        sock.lowest_layer().async_connect(ep,
            [this](const boost::system::error_code& ec) {
                handler(ec, HandlerType::CONNECT);
            });

        return;
    }

    if (target_ep) {
        boost::asio::async_connect(sock.lowest_layer(), *target_ep,
            [this](const boost::system::error_code& ec, const tcp::endpoint& /*ep*/) {
                handler(ec, HandlerType::CONNECT);
            });

        return;
    }
}


//TODO: UNFINISHED AND UNINTEGRATED
void handler(const boost::system::error_code& error, const HandlerType& htype){
    if (!error)
          {
            switch(htype){
                case HandlerType::CONNECT:
                    SslProbe::handshake();
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
tcp::resolver::results_type to_tcp_endpoint(sockaddr_storage ip){
    // Convert to a generic endpoint first
    stream_protocol::endpoint gen_ep(ip, sizeof(ip));

    // Then to a specific endpoint if needed (e.g., TCP)
    tcp::resolver::results_type tcp_ep(gen_ep); 

    return tcp_ep;
}