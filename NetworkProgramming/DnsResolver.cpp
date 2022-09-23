#include "DnsResolver.h"

#pragma warning(disable : 4996)

DnsResolver::DnsResolver()
{
}

DnsResolver::~DnsResolver()
{
}

//Метод для печати на экран ip адреса по имени хоста
void DnsResolver::print_ips(const std::string& host_name)
{
    addrinfo* servinfo = getAddrInfo(host_name);

    if (servinfo)
    {
        std::cout
            << "Getting name for \"" << host_name << "\"...\n"
            << "Using getaddrinfo() function." << std::endl;

        for (auto const* s = servinfo; s != nullptr; s = s->ai_next)
        {
            std::cout << "Canonical name: ";
            if (s->ai_canonname)
                std::cout << s->ai_canonname;
            std::cout << "\n";

            assert(s->ai_family == s->ai_addr->sa_family);
            std::cout << "Address type: ";

            if (AF_INET == s->ai_family)
            {
                char ip[INET_ADDRSTRLEN];
                std::cout << "AF_INET\n";
                sockaddr_in const* const sin = reinterpret_cast<const sockaddr_in* const>(s->ai_addr);
                std::cout << "Address length: " << sizeof(sin->sin_addr) << "\n";
                std::cout << "IP Address: " << inet_ntop(AF_INET, &(sin->sin_addr), ip, INET_ADDRSTRLEN) << "\n";
            }
            else if (AF_INET6 == s->ai_family)
            {
                char ip6[INET6_ADDRSTRLEN];

                std::cout << "AF_INET6\n";
                const sockaddr_in6* const sin = reinterpret_cast<const sockaddr_in6* const>(s->ai_addr);
                std::cout << "Address length: " << sizeof(sin->sin6_addr) << "\n";
                std::cout << "IP Address: " << inet_ntop(AF_INET6, &(sin->sin6_addr), ip6, INET6_ADDRSTRLEN) << "\n";
            }
            else
            {
                std::cout << s->ai_family << "\n";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    freeaddrinfo(servinfo);
}

//Вывод на экран имени хоста по ip адресу
void DnsResolver::print_hostname(const std::string& ip_addr)
{
    std::cout
        << "Getting name for \"" << ip_addr << "\"...\n"
        << "Using getnameinfo() function." << std::endl;

    std::cout<<"Host name: " << getNameInfo(ip_addr) << std::endl;
}

//Метод для трансяции имени хоста в адресс
//Возвращает указатель на связанный список структур addrinfo содержащую информацию о хосте
addrinfo* DnsResolver::getAddrInfo(const std::string& host_name)
{
    // Need for Windows initialization.
    socket_wrapper::SocketWrapper sock_wrap;

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

    int status = 0;

    if ((status = getaddrinfo(host_name.c_str(), nullptr, &hints, &servinfo)) != 0)
    {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return nullptr;
    }

    return servinfo;
}

//Метод для получения от сервера имени хоста по ip адресу
//Возвращает строку с именем хоста
char* DnsResolver::getNameInfo(const std::string& ip_addr)
{
    // Need for Windows initialization.
    socket_wrapper::SocketWrapper sock_wrap;
    sockaddr_in pSockaddr;

    pSockaddr.sin_family = AF_INET;
    pSockaddr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

    char hostname[NI_MAXHOST]{};

    //std::string hostName;
    int status = 0;
    if (status = getnameinfo((sockaddr*)&pSockaddr, sizeof(sockaddr), hostname, NI_MAXHOST, NULL, NULL, NULL) != 0)
    {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return nullptr;
    }

    return hostname;
}
