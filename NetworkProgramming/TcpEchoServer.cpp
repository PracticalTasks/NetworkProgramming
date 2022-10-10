#include "TcpEchoServer.h"


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

bool TcpEchoServer::start_server(const uint16_t READ_PORT)
{
    tcp_echosock = new socket_wrapper::Socket(AF_INET, SOCK_STREAM, NULL);

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

    if (listen(*tcp_echosock, SOMAXCONN) != SOCKET_ERROR)
    {
        std::cout << "Running ftp server on the port " << READ_PORT << "...\n";
        return true;
    }
        
    return false;
}

int TcpEchoServer::read()
{
    char client_addrbuf[INET_ADDRSTRLEN];

    sockaddr_in addr_c;
    int addrlen = sizeof(addr_c);

    std::array <char, BUFF_SIZE> buff{};
    uint32_t packet_size = 0;

    while (true)
    {
        socket_wrapper::Socket client_sock = accept(*tcp_echosock, reinterpret_cast<sockaddr*>(&addr_c), &addrlen);

        if (!client_sock)
        {
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            return EXIT_FAILURE;
        }



        while (true)
        {
            packet_size = recv(client_sock, buff.data(), buff.size(), 0);

            if (packet_size > 0)
            {
                std::string filename(buff.data());
                //Должна быть папка "C:\Netwk" в которой сервер будет искать файл 
                std::ifstream file("C:\\Netwk\\" + filename, std::ifstream::binary);

                if (!file)
                {
                    std::cout << "Cannot open file " << filename << std::endl;
                    continue;
                }

                // get length of file:
                file.seekg(0, file.end);
                int length = file.tellg();
                file.seekg(0, file.beg);

                std::vector<char> buff_bin(length);
                file.read(buff_bin.data(), length);

                packet_size = send(client_sock, buff_bin.data(), length, 0);

                if (packet_size == SOCKET_ERROR)
                {
                    std::cerr << "Can't send message to Client. Error #" << sock_wrap.get_last_error_string() << std::endl;
                    return EXIT_FAILURE;
                }
            }
            else if (packet_size == -1)
                break;

            buff.fill(0);
            std::cout << std::endl;
        }

    }


    return EXIT_SUCCESS;
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