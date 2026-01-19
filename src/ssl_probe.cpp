#include "ssl_probe.hpp"

using namespace SslProbe;

vector<ProbeResult> probe_sync(const ResolvedTarget& target_ip, string hostname="",
                                   uint16_t port_start,
                                   uint16_t port_stop,
                                   int timeout) {
    //will be acting as main
    // Create a context that uses the default paths for
    // finding CA certificates.
    //ssl::context ctx(ssl::context::sslv23);
    ctx.set_default_verify_paths();

    // Open a socket and connect it to the remote host.
    asio::io_context io_context;
    SSLSocket sock(io_context, ctx);
    tcp::resolver resolver(io_context);
    //tcp::resolver::query query("host.name", "https");
    sock.lowest_layer().set_option(tcp::no_delay(true));
    sock.set_verify_callback(std::bind(&SslProbe::verify, this, _1, _2));

    if(!hostname.empty()){
        tcp::resolver::query target_ep(resolver.resolve(target_ep), "https");
        asio::connect(sock.lowest_layer(), query);//TODO: USE TCP::ENDPOINT TYPE
        connect_(target_ep);
    }

    connect_(target_ip);
}

void perform_handshake(){//handshake handler as a lamda function
    ssl::stream::async_handshake(SSLSocket::client,
        boost::bind(&handler,this, boost::asio::placeholders::error));

}

//The function object to be used for verifying a certificate
void verify(bool preverified, // True if the certificate passed pre-verification.
      ssl::verify_context& ctx // The peer certificate and other context.
      ){

}

//TODO: SET THINGS UP SO THAT SSL-PROBE DOES NOT USE TARGET_IP
void connect_(const ResolvedTarget& target_ip=null, tcp::endpoint target_ep=null){

    if(target_ip){
        tcp::endpoint endpoint = to_tcp_endpoint(target_ip.addr);

        boost::asio::async_connect(sock.lowest_layer(), endpoint,
            boost::bind(&handler,this, boost::asio::placeholders::error));
        
        cout << "SSL Probe was successful" << endl; //TODO: ENSURE I WANT THIS MESSAGE HERE
    }
    else
        boost::asio::async_connect(sock.lowest_layer(), target_ep,
            boost::bind(&handler,this, boost::asio::placeholders::error));
        
        cout << "SSL Probe was successful" << endl; //TODO: ENSURE I WANT THIS MESSAGE HERE

}

//TODO: UNFINISHED AND UNINTEGRATED
  void handler(const boost::system::error_code& error, string htype){
    if (!error)
          {
            switch(htype){
                case("Connect") handshake();
                case("Handshake") std::cout << "Handshake was successful" << endl;
            }
            
          }
    else
        std::cout << htype << " failed: " << error.message() << "\n";
}

//ASSUMED TO BE DONE AND FUNCTIONAL
tcp::endpoint to_tcp_endpoint(sockaddr_storage ip){
    // Convert to a generic endpoint first
    generic_endpoint gen_ep(ip, sizeof(ip));

    // Then to a specific endpoint if needed (e.g., TCP)
    if (gen_ep.protocol().type() == tcp::protocol().type()) {
        tcp::endpoint tcp_ep = gen_ep; // Type-safe conversion
        
        return tcp_ep;
    }
}