#include "EchoServer.h"


EchoServer::EchoServer(const uint16_t READ_PORT)
{
    read(READ_PORT);
}

EchoServer::~EchoServer()
{
}

//ͥ󯢠򱡢齠char 򳱮릠򬟳tring
inline bool EchoServer::cmpCharToStr(const char* buf, const std::string& cmd, const int lenBuf)
{
    if (cmd.size() != lenBuf)
        return false;

    for (int i{}; i < lenBuf; ++i)
    {
        if (tolower(buf[i]) != cmd[i])
            return false;
    }

    return true;
}

int EchoServer::read(const uint16_t READ_PORT)
{
    socket_wrapper::SocketWrapper sockWrap;
    socket_wrapper::Socket echoSock = { AF_INET, SOCK_DGRAM, IPPROTO_UDP };;

    std::cout << "Starting echo server on the port " << READ_PORT << "...\n";

    if (!echoSock)
    {
        std::cerr << sockWrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in addr =
    {
        .sin_family = PF_INET,
        .sin_port = htons(READ_PORT),
    };

    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(echoSock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        std::cerr << sockWrap.get_last_error_string() << std::endl;
        // Socket will be closed in the Socket destructor.
        return EXIT_FAILURE;
    }

    char buffer[256]{};

    // socket address used to store client address
    struct sockaddr_in clientAddress = {};
    socklen_t clientAddressLen = sizeof(sockaddr_in);
    ssize_t recvLen {};

    std::cout << "Running echo server...\n" << std::endl;
    char clientAddressBuf[INET_ADDRSTRLEN];

    while (true)
    {
        // Read content into buffer from an incoming client.
        recvLen = recvfrom(echoSock, buffer, sizeof(buffer) - 1, 0,
            reinterpret_cast<sockaddr*>(&clientAddress),
            &clientAddressLen);

        if (recvLen > 0)
        {
            buffer[recvLen] = '\0';
            std::cout
                << "Client with address "
                << inet_ntop(AF_INET, &clientAddress.sin_addr, clientAddressBuf, sizeof(clientAddressBuf) / sizeof(clientAddressBuf[0]))
                << ":" << ntohs(clientAddress.sin_port)
                << " sent datagram "
                << "[length = "
                << recvLen
                << "]:\n'''\n"
                << buffer
                << "\n'''"
                << std::endl;

            if (cmpCharToStr(buffer, CMD_EXT, recvLen))
            {
                std::cout << "Stoped echo server ...\n";
                break;
            }
                
            sendto(echoSock, buffer, recvLen, 0, reinterpret_cast<const sockaddr*>(&clientAddress),
                clientAddressLen);
        }

        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
