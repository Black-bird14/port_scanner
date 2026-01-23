#include "probe.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <vector>
//#include <string>

namespace asio = boost::asio;
namespace ssl = asio::ssl;
using tcp = asio::ip::tcp;
typedef ssl::stream<tcp::socket> SSLSocket;
using generic_endpoint = asio::generic::stream_protocol::endpoint;
using asio::generic::stream_protocol;

enum class HandlerType { CONNECT, HANDSHAKE, VERIFY };


class SslProbe : public Probe {
    private:
        ResolvedTarget target_ip;
        uint16_t port_start;
        uint16_t port_stop;
        int timeout;
    public:
        SslProbe (const ResolvedTarget& ip, uint16_t pstart, uint16_t pstop, int t)
                    : target_ip(ip), port_start(pstart), port_stop(pstop), timeout(t) {}
                            
        vector<ProbeResult> probe_sync(const ResolvedTarget& target_ip,
                                   uint16_t port_start,
                                   uint16_t port_stop,
                                   int timeout, string hostname="") override;
        void perform_handshake(SSLSocket& sock);
        void connect_(const tcp::endpoint& single_ep, SSLSocket& sock);
        void connect_(const tcp::resolver::results_type& multiple_eps, SSLSocket& sock);

        bool verify(bool preverified, ssl::verify_context& ctx);

        void handler(const boost::system::error_code& error, const HandlerType& htype, SSLSocket& sock);//generic handler function
        tcp::endpoint to_tcp_endpoint(ResolvedTarget ip, uint16_t port);
    
};
