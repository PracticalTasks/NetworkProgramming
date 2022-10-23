#include "FtpServer.h"

size_t read_complete(char* buff, const error_code& err, size_t bytes) {
    if (err) return 0;
    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

FtpServer::FtpServer(const uint16_t READ_PORT)
    : sock(new ip::tcp::socket(service))
{
    if (!start_server(READ_PORT))
    {
        std::cout << "Can not running echo tcp server on the port " << READ_PORT << std::endl;
        return;
    }
       
    waiting_request();
}

FtpServer::~FtpServer()
{
    delete ep;
    delete sock;
}

bool FtpServer::start_server(const uint16_t READ_PORT)
{   
    ep = new boost::asio::ip::tcp::endpoint(ip::tcp::v4(), 5555); // listen on 5555



    //ftpserv_sock = new socket_wrapper::Socket(AF_INET, SOCK_STREAM, NULL);


    //if (!*ftpserv_sock)
    //{
    //    std::cerr << sock_wrap.get_last_error_string() << std::endl;
    //    return false;
    //}

    //sockaddr_in addr =
    //{
    //    .sin_family = AF_INET,
    //    .sin_port = htons(READ_PORT),
    //};


    //addr.sin_addr.s_addr = INADDR_ANY;

    ////if (bind(*ftpserv_sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0)
    ////{
    ////    std::cerr << sock_wrap.get_last_error_string() << std::endl;
    ////    // Socket will be closed in the Socket destructor.
    ////    return false;
    ////}

    //if (listen(*ftpserv_sock, SOMAXCONN) != SOCKET_ERROR)
    //{
    //    std::cout << "Running ftp server on the port " << READ_PORT << "...\n";
    //    return true;
    //}
        
    return true;
}

int FtpServer::waiting_request()
{
    acc = new boost::asio::ip::tcp::acceptor(service, *ep);

    char client_addrbuf[INET_ADDRSTRLEN];

    //sockaddr_in addr_c;
    //int addrlen = sizeof(addr_c);

    char buff[1024];
    uint32_t packet_size = 0;



    while (true)
    {
        //client_sock = new socket_wrapper::Socket(accept(*ftpserv_sock, reinterpret_cast<sockaddr*>(&addr_c), &addrlen));

        //if (!client_sock)
        //{
        //    std::cerr << sock_wrap.get_last_error_string() << std::endl;
        //    return EXIT_FAILURE;
        //}

        acc->accept(*sock);
        
        std::cout
            << "Client with address "
            << sock->remote_endpoint().address().to_string()
            << ":" << ntohs(sock->remote_endpoint().port())
            << std::endl;

        //auto read_complete = [](char* buff, const error_code& err, size_t bytes) -> size_t{
        //    if (err) return 0;
        //    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
        //    // we read one-by-one until we get to enter, no buffering
        //    return found ? 0 : 1;
        //};

        int bytes = read(*sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));

        sock->close();
    //    while (true)
    //    {
    //        packet_size = recv(*client_sock, buff.data(), buff.size(), 0);

    //        if (packet_size > 0)
    //        {
    //            if (cmp_chartostr(buff.data(), CMD_EXT, packet_size))
    //            {
    //                std::cout << "Echo server has been stopped ...\n";
    //                return EXIT_SUCCESS;
    //            }

    //            std::string filename(network_path + buff.data());

    //            if (!load_file(filename))
    //            {
    //                std::cout << "Error load file " << filename;
    //            }
    //        }
    //        else if (packet_size == 0)
    //        {
    //            std::cout << "disconnect\n";
    //            break;
    //        }
    //            
    //        buff.fill(0);
    //        std::cout << std::endl;
    //    }
    //    delete client_sock;
    }

    return EXIT_SUCCESS;
}

void FtpServer::insert_sizefile_tobuff(std::vector<char> &buff, int32_t val)
{
    uint16_t hw_val{}; //старшее слово
    uint16_t lw_val{}; //младшее слово

    hw_val = val >> 16;
    lw_val = val & 0xFFFF;

    auto it = buff.begin();
    it[0] = lw_val & 0xFF;
    it[1] = lw_val >> 8;
    it[2] = hw_val & 0xFF;
    it[3] = hw_val >> 8;
}

//void FtpServer::handle_accept(socket_ptr sock, const boost::system::error_code& err)
//{
//    if (err) return;
//    // at this point, you can read/write to the socket
//    socket_ptr sock(new boost::asio::ip::tcp::socket(service));
//    start_accept(sock);
//}
//
//void FtpServer::start_accept(socket_ptr sock)
//{
//    acc->async_accept(*sock, boost::asio::bind(handle_accept, sock, _1));
//}

bool FtpServer::load_file(std::string const& file_path)
{
    std::vector<char> buff_bin(FILEBUFF_SZ);
    //Должна быть папка "C:\Netwk" в которой сервер будет искать файл 
    std::ifstream file_stream(file_path, std::ifstream::binary);
    
    if (!file_stream)
        return false;

    //get length of file:
    file_stream.seekg(0, file_stream.end);
    int32_t length = file_stream.tellg();
    file_stream.seekg(0, file_stream.beg);

    std::cout << "Sending file " << file_path << "...\n";

    //Заносить в первые четыре байта длину файла в big endian
    insert_sizefile_tobuff(buff_bin, length);
    file_stream.read(buff_bin.data() + 4, FILEBUFF_SZ - 4);
    
    //if (!send_file(buff_bin))
    //    return false;

    //while (file_stream)
    //{
    //    file_stream.read(buff_bin.data(), FILEBUFF_SZ);
    //    if (!send_file(buff_bin))
    //        return false;
    //}
   
    return true;
}

//bool FtpServer::send_file(const std::vector<char> buff_bin)
//{
//    uint32_t packet_size = 0;
//    int transmit_cnt = 0;
//    int size = buff_bin.size();
//    std::vector<char> send_buff;
//
//    while (transmit_cnt != size)
//    {
//        packet_size = send(*client_sock, &(buff_bin.data()[0]) + transmit_cnt, size  - transmit_cnt, 0);
//
//        if (packet_size == SOCKET_ERROR)
//        {
//            std::cerr << "Can't send file to Client. Error #" << sock_wrap.get_last_error_string() << std::endl;
//            return false;
//        }
//        transmit_cnt += packet_size;
//    }
//
//    return true;
//}

//Метод для сравнения символьной строки с string
inline bool FtpServer::cmp_chartostr(const char* buff, const std::string& cmd, const int lenBuff)
{
    if (cmd.size() != lenBuff)
        return false;

    for (int i{}; i < lenBuff; ++i)
    {
        if (tolower(buff[i]) != cmd[i])
            return false;
    }

    return true;
}

