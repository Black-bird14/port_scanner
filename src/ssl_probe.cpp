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
    asio::connect(sock.lowest_layer(), target_ip);//TODO: USE TCP::ENDPOINT TYPE
    sock.lowest_layer().set_option(tcp::no_delay(true));
    sock.set_verify_callback(std::bind(&SslProbe::verify, this, _1, _2));

    connect(target_ip);
}

void handshake(){//handshake handler as a lamda function
    ssl::stream::async_handshake(SSLSocket::client,
        [this](const boost::system::error_code& error) 
        {
          if (!error)
          {
            std::cout << "Handshake was successful" << endl;
          }
          else
          {
            std::cout << "Handshake failed: " << error.message() << endl;
          }
        });

}

//The function object to be used for verifying a certificate
void verify(bool preverified, // True if the certificate passed pre-verification.
      ssl::verify_context& ctx // The peer certificate and other context.
      ){

}

//TODO: SET THINGS UP SO THAT SSL-PROBE DOES NOT USE TARGET_IP
void connect(const ResolvedTarget& target_ip){
    boost::asio::async_connect(sock.lowest_layer(), target_ip,
            boost::bind(&handler,this, boost::asio::placeholders::error));
        /*[this](const boost::system::error_code& error,
          const tcp::endpoint& /*endpoint)*/
        
}

//UNFINISHED AND UNINTEGRATED
  void handler(const boost::system::error_code& error, string htype){
    if (!error)
          {
            switch(htype){
                case("connect") handshake();
                case("handshake") std::cout << "Handshake was successful" << endl;
            }
            
          }
    else
        std::cout << htype << " failed: " << error.message() << "\n";
}