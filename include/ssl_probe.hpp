#include "probe.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <vector>
//#include <string>

using boost::asio::ip::tcp;
namespace asio = boost::asio;
namespace ssl = asio::ssl;
typedef ssl::stream<tcp::socket> SSLSocket;
using generic_endpoint = asio::generic::stream_protocol::endpoint;

class SslProbe : public Probe {
    private:
        ResolvedTarget target_ip;
        uint16_t port_start;
        uint16_t port_stop;
        int timeout;
    public:
        SslProbe (const ResolvedTarget& ip, uint16_t pstart, uint16_t pstop, int t)
                    : target_ip(ip), port_start(pstart), port_stop(pstop), timeout(t) {}
                            
        vector<ProbeResult> probe_sync(const ResolvedTarget& target_ip, string hostname,
                                   uint16_t port_start,
                                   uint16_t port_stop,
                                   int timeout) override;
        void handshake();
        void connect(const ResolvedTarget& target_ip);//const tcp::resolver::results_type& endpoints);
        bool verify(bool preverified,
      ssl::verify_context& ctx);

        void handler(const boost::system::error_code& error, string htype);//generic handler function
};
