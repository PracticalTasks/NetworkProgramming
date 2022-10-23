#pragma once
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

#pragma warning(disable : 4996)

#define BUFF_SIZE 256

using boost::asio::ip::tcp;
const int PORT = 5555;

char buff[256];

std::string make_daytime_string()
{
	time_t now = time(0);
	return ctime(&now);
}

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::shared_ptr<TcpConnection> pointer;

	static pointer create(boost::asio::io_context& io_context)
	{
		return pointer(new TcpConnection(io_context));
	}

	tcp::socket& socket()
	{
		return socket_;
	}

    void waiting_request(const boost::system::error_code& err, std::size_t bytes_transferred)
    {
        if (bytes_transferred == 0)
            return;
        std::cout << "Hello, world!\n";
    }

	void start()
	{

        //char client_addrbuf[INET_ADDRSTRLEN];

        //sockaddr_in addr_c;
        //int addrlen = sizeof(addr_c);

        std::array <char, BUFF_SIZE> buff{};
        uint32_t packet_size = 0;

        std::cout
            << "Client with address "
            << socket().remote_endpoint().address().to_string()
            << ":" << ntohs(socket().remote_endpoint().port())
            << std::endl;

        boost::asio::async_read(socket_, boost::asio::buffer(buff),
            [&](const boost::system::error_code& error, size_t bytes_transferred)
                {
                    waiting_request(error, bytes_transferred);

                });

        //auto readHandler = std::bind(&waiting_request,
        //    shared_from_this(),
        //    std::placeholders::_1,
        //    std::placeholders::_2);

        //boost::asio::async_write(socket_, boost::asio::buffer(message_),
        //    [&](const boost::system::error_code& error, size_t bytes_transferred)
        //    {
        //        waiting_request(error, bytes_transferred);
        //    });

        //boost::asio::async_read(socket_, boost::asio::buffer(buff), std::move(readHandler));
    

        

        //while (true)
        //{
        //    //std::string sClientIp = socket().remote_endpoint().address().to_string();
        //    //unsigned short uiClientPort = socket().remote_endpoint().port();


        //    //while (true)
        //    //{
        //    //    packet_size = recv(*client_sock, buff.data(), buff.size(), 0);

        //    //    if (packet_size > 0)
        //    //    {
        //    //        if (cmp_chartostr(buff.data(), CMD_EXT, packet_size))
        //    //        {
        //    //            std::cout << "Echo server has been stopped ...\n";
        //    //            return EXIT_SUCCESS;
        //    //        }

        //    //        std::string filename(network_path + buff.data());

        //    //        if (!load_file(filename))
        //    //        {
        //    //            std::cout << "Error load file " << filename;
        //    //        }
        //    //    }
        //    //    else if (packet_size == 0)
        //    //    {
        //    //        std::cout << "disconnect\n";
        //    //        break;
        //    //    }
        //    //        
        //    //    buff.fill(0);
        //    //    std::cout << std::endl;
        //    //}
        //    //delete client_sock;
        //}
	    	//message_ = make_daytime_string();
	    	//auto s = shared_from_this();

	}

private:
	TcpConnection(boost::asio::io_context& io_context)
		: socket_(io_context)
	{
	}

	void handle_write(const boost::system::error_code&, size_t bytes_transferred)
	{
		std::cout << "Bytes transferred: " << bytes_transferred << std::endl;
	}

private:
    const uint16_t FILEBUFF_SZ = 4096;
    //std::shared_ptr<char> req_buff;
    //char buff[256];

	tcp::socket socket_;
	std::string message_;
};
