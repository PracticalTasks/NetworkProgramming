#include "TcpEchoServer.h"


EchoServer::EchoServer(const uint16_t READ_PORT)
{
    read(READ_PORT);
}

EchoServer::~EchoServer()
{
}

//Метод для сравнения символьной строки с string
inline bool EchoServer::cmp_chartostr(const char* buf, const std::string& cmd, const int lenBuf)
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
    socket_wrapper::SocketWrapper sock_wrap;
    socket_wrapper::Socket echo_sock = { AF_INET, SOCK_DGRAM, IPPROTO_UDP };;

    std::cout << "Starting echo server on the port " << READ_PORT << "...\n";

    if (!echo_sock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in addr =
    {
        .sin_family = PF_INET,
        .sin_port = htons(READ_PORT),
    };

    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(echo_sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        // Socket will be closed in the Socket destructor.
        return EXIT_FAILURE;
    }

    char buffer[256]{};

    // socket address used to store client address
    struct sockaddr_in clientAddress = {};
    socklen_t client_addrlen = sizeof(sockaddr_in);
    ssize_t recv_len {};

    std::cout << "Running echo server...\n" << std::endl;
    char client_addrbuf[INET_ADDRSTRLEN];

    while (true)
    {
        // Read content into buffer from an incoming client.
        recv_len = recvfrom(echo_sock, buffer, sizeof(buffer) - 1, 0,
            reinterpret_cast<sockaddr*>(&clientAddress),
            &client_addrlen);

        if (recv_len > 0)
        {
            buffer[recv_len] = '\0';
            std::cout
                << "Client with address "
                << inet_ntop(AF_INET, &clientAddress.sin_addr, client_addrbuf, sizeof(client_addrbuf) / sizeof(client_addrbuf[0]))
                << ":" << ntohs(clientAddress.sin_port)
                << " sent datagram "
                << "[length = "
                << recv_len
                << "]:\n'''\n"
                << buffer
                << "\n'''"
                << std::endl;

            if (cmp_chartostr(buffer, CMD_EXT, recv_len))
            {
                std::cout << "Stoped echo server ...\n";
                break;
            }
                
            sendto(echo_sock, buffer, recv_len, 0, reinterpret_cast<const sockaddr*>(&clientAddress),
                client_addrlen);
        }

        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
