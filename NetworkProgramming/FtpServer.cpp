#include "FtpServer.h"

FtpServer::FtpServer(const uint16_t READ_PORT)
{
    if (!start_server(READ_PORT))
    {
        std::cout << "Can not running echo tcp server on the port " << READ_PORT << std::endl;
        return;
    }

    service.run();
}

FtpServer::~FtpServer()
{
    delete serv_accept;
    delete ep;
    delete sock;
}

bool FtpServer::start_server(const uint16_t READ_PORT)
{   
    sock = new ip::tcp::socket(service);
    ep = new boost::asio::ip::tcp::endpoint(ip::tcp::v4(), READ_PORT);
    serv_accept = new boost::asio::ip::tcp::acceptor(service, *ep);

    serv_accept->listen();
    serv_accept->async_accept(*sock, boost::bind(&FtpServer::accepted_connection,
        this, boost::asio::placeholders::error));

    //serv_accept->async_accept(*sock, [this](const boost::system::error_code& ec)
    //    {
    //        if (ec)
    //            std::cout << "Error async_accept\n";
    //        std::cout
    //            << "Client with address "
    //            << sock->remote_endpoint().address().to_string()
    //            << ":" << ntohs(sock->remote_endpoint().port())
    //            << std::endl;

    //        sock->async_read_some(buffer(req_buff, 256), boost::bind(&FtpServer::waiting_request, 
    //            this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

    //        //sock->async_read_some(buffer(req_buff, 256), [this](const boost::system::error_code& ec, std::size_t bytes_transferred)
    //        //    {
    //        //        if (!ec)
    //        //            waiting_request(ec, bytes_transferred);
    //        //    });

    //    });

    std::cout << "Running ftp server on the port " << READ_PORT << "..." << std::endl;


    return true;
}

void FtpServer::accepted_connection(const boost::system::error_code& ec)
{
    if (!ec)
    {
        std::cout
            << "Client with address "
            << sock->remote_endpoint().address().to_string()
            << ":" << ntohs(sock->remote_endpoint().port())
            << std::endl;

        sock->async_read_some(buffer(req_buff, 256), boost::bind(&FtpServer::waiting_request,
            this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
        std::cout << "Error async_accept\n";
}

void FtpServer::waiting_request(const boost::system::error_code& ec, std::size_t bytes_transferred)
{

    if (bytes_transferred > 0)
    {
        if (cmp_chartostr(req_buff, CMD_EXT, bytes_transferred))
        {
            std::cout << "Echo server has been stopped ...\n";
            return;
        }

        std::string filename(network_path + req_buff);

        if (!load_file(filename))
        {
            std::cout << "Error load file " << filename;
        }
    }
    else if (bytes_transferred == 0)
    {
        std::cout << "Disconnected\n";
        //serv_accept->listen();

        serv_accept->async_accept(*sock, boost::bind(&FtpServer::accepted_connection,
            this, boost::asio::placeholders::error));
        //service.run();
    }
        
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
    
    if (!send_file(buff_bin))
        return false;

    while (file_stream)
    {
        file_stream.read(buff_bin.data(), FILEBUFF_SZ);
        if (!send_file(buff_bin))
            return false;
    }
   
    std::cout << "Done\n";
    sock->async_read_some(buffer(req_buff, 256), boost::bind(&FtpServer::waiting_request,
        this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

    return true;
}

bool FtpServer::send_file(const std::vector<char> buff_bin)
{
    uint32_t packet_size = 0;
    int transmit_cnt = 0;
    int size = buff_bin.size();
    std::vector<char> send_buff;
    write(*sock, buffer(buff_bin));

    return true;
}

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


