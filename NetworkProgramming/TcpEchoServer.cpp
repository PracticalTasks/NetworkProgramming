#include "TcpEchoServer.h"


TcpEchoServer::TcpEchoServer(const uint16_t READ_PORT)
{
    read(READ_PORT);
}

TcpEchoServer::~TcpEchoServer()
{
}

//Метод для сравнения символьной строки с string
inline bool TcpEchoServer::cmp_chartostr(const char* buf, const std::string& cmd, const int lenBuf)
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

int TcpEchoServer::read(const uint16_t READ_PORT)
{
    socket_wrapper::SocketWrapper sock_wrap;
    socket_wrapper::Socket echo_sock = { AF_INET, SOCK_STREAM, NULL };;

    std::cout << "Starting echo server on the port " << READ_PORT << "...\n";

    if (!echo_sock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(READ_PORT),
    };

    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(echo_sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        // Socket will be closed in the Socket destructor.
        return EXIT_FAILURE;
    }

    //if (listen(echo_sock, SOMAXCONN) != SOCKET_ERROR) {
    //    std::cout << "Start listenin at port " << ntohs(addr.sin_port) << std::endl;
    //    //printf("Start listenin at port%u\n", ntohs(addr.sin_port));
    //}

    char buffer[256]{};

    // socket address used to store client address
    struct sockaddr_in client_addr = {};
    socklen_t client_addrlen = sizeof(sockaddr_in);
    ssize_t recv_len {};

    if (listen(echo_sock, SOMAXCONN) != SOCKET_ERROR)
        std::cout << "Running echo tcp server...\n" << std::endl;
        
    char client_addrbuf[INET_ADDRSTRLEN];

    while (true)
    {
        // Read content into buffer from an incoming client.
        recv_len = recvfrom(echo_sock, buffer, sizeof(buffer) - 1, 0,
            reinterpret_cast<sockaddr*>(&client_addr),
            &client_addrlen);

        if (recv_len > 0)
        {
            buffer[recv_len] = '\0';
            std::cout
                << "Client with address "
                << inet_ntop(AF_INET, &client_addr.sin_addr, client_addrbuf, sizeof(client_addrbuf) / sizeof(client_addrbuf[0]))
                << ":" << ntohs(client_addr.sin_port)
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
                
            sendto(echo_sock, buffer, recv_len, 0, reinterpret_cast<const sockaddr*>(&client_addr),
                client_addrlen);
        }

        std::cout << std::endl;
    }

    return EXIT_SUCCESS;

}
