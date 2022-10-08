#include "TcpEchoServer.h"
#pragma warning(disable : 4996)

TcpEchoServer::TcpEchoServer(const uint16_t READ_PORT)
{
    if (!start_server(READ_PORT))
    {
        std::cout << "Can not running echo tcp server on the port " << READ_PORT << std::endl;
        return;
    }
       
    read();
}

TcpEchoServer::~TcpEchoServer()
{
    delete tcp_echosock;
}

//Метод для трансяции имени хоста в адресс
//Возвращает указатель на связанный список структур addrinfo содержащую информацию о хосте
addrinfo* TcpEchoServer::get_addrinfo(const std::string& host_name)
{
    // Need for Windows initialization.
    //socket_wrapper::SocketWrapper sock_wrap;

    addrinfo* servinfo = nullptr;

    addrinfo hints =
    {
        .ai_flags = AI_CANONNAME,
        // Неважно, IPv4 или IPv6.
        .ai_family = AF_UNSPEC,
        // TCP stream-sockets.
        .ai_socktype = SOCK_STREAM,
        // Any protocol.
        .ai_protocol = 0
    };

    int status{};

    if ((status = getaddrinfo(host_name.c_str(), nullptr, &hints, &servinfo)) != 0)
    {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return nullptr;
    }

    return servinfo;
}

////Метод для получения от сервера имени хоста по ip адресу
////Возвращает строку с именем хоста
//char* TcpEchoServer::get_nameinfo(const std::string& ip_addr)
//{
//    // Need for Windows initialization.
//    socket_wrapper::SocketWrapper sock_wrap;
//    sockaddr_in pSockaddr;
//
//    pSockaddr.sin_family = AF_INET;
//    pSockaddr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
//
//    char host_name[NI_MAXHOST]{};
//
//    //std::string hostName;
//    int status{};
//    if (status = getnameinfo((sockaddr*)&pSockaddr, sizeof(sockaddr), host_name, NI_MAXHOST, NULL, NULL, NULL) != 0)
//    {
//        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
//        return nullptr;
//    }
//
//    return host_name;
//}

bool TcpEchoServer::start_server(const uint16_t READ_PORT)
{
    //addrinfo* servinfo = get_addrinfo("localhost");


    //tcp_echosock = new socket_wrapper::Socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    //tcp_echosock6 = new socket_wrapper::Socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    tcp_echosock = new socket_wrapper::Socket(AF_INET, SOCK_STREAM, NULL);

   // std::cout << "Starting echo server on the port " << READ_PORT << "...\n";


    if (!*tcp_echosock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return false;
    }

    sockaddr_in addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(READ_PORT),
    };

    addr.sin_addr.s_addr = INADDR_ANY;


    if (bind(*tcp_echosock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        // Socket will be closed in the Socket destructor.
        return false;
    }
    //freeaddrinfo(servinfo);

    if (listen(*tcp_echosock, SOMAXCONN) != SOCKET_ERROR)
    {
        std::cout << "Running echo tcp server on the port " << READ_PORT << "...\n";
        return true;
    }
        
    return false;
}

int TcpEchoServer::read()
{
    char client_addrbuf[INET_ADDRSTRLEN];

    while (true)
    {
        sockaddr_storage addr_c;
        //sockaddr_in addr_c;
        int addrlen = sizeof(addr_c);
        socket_wrapper::Socket client_sock = accept(*tcp_echosock, reinterpret_cast<sockaddr*>(&addr_c), &addrlen);

        if (!client_sock)
        {
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            return EXIT_FAILURE;
        }

        std::array <char, BUFF_SIZE> buff{};
        short packet_size = 0;

        while (true)
        {

            packet_size = recv(client_sock, buff.data(), buff.size(), 0);

            if (packet_size > 0)
            {
                std::cout
                    << "Client with address "
                    //              << inet_ntop(AF_INET, &addr_c.sin_addr, client_addrbuf, sizeof(client_addrbuf) / sizeof(client_addrbuf[0]))
                    //              << ":" << ntohs(addr_c.sin_port)
                    << " sent datagram "
                    << "[length = "
                    << packet_size
                    << "]:\n'''\n"
                    << buff.data()
                    << "\n'''"
                    << std::endl;

                if (cmp_chartostr(buff.data(), CMD_EXT, packet_size))
                {
                    std::cout << "Echo server has been stopped ...\n";
                    return EXIT_SUCCESS;
                }

                packet_size = send(client_sock, buff.data(), packet_size, 0);

                if (packet_size == SOCKET_ERROR)
                {
                    std::cerr << "Can't send message to Client. Error #" << sock_wrap.get_last_error_string() << std::endl;
                    return EXIT_FAILURE;
                }
            }
            break;
        }
        buff.fill(0);
        std::cout << std::endl;
        Sleep(50);
    }

   // return EXIT_SUCCESS;
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