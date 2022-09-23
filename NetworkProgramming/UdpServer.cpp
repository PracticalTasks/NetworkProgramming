#include "UdpServer.h"
#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>

UdpServer::UdpServer()
{
}

UdpServer::~UdpServer()
{
}

// Trim from end (in place).
inline std::string& UdpServer::rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) { return !std::isspace(c); }).base());
    return s;
}

std::string UdpServer::readData()
{
    socket_wrapper::SocketWrapper sock_wrap;
    //const int port { std::stoi(argv[1]) };

    const int port = 5555;

    socket_wrapper::Socket sock = { AF_INET, SOCK_DGRAM, IPPROTO_UDP };

    std::cout << "Starting echo server on the port " << port << "...\n";

    if (!sock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in addr =
    {
        .sin_family = PF_INET,
        .sin_port = htons(port),
    };
    return std::string();
}

void UdpServer::writeData(std::string data)
{

}
