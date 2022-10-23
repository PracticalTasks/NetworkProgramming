#pragma once
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <array>
#include <fstream>
#include <filesystem>


#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/system/error_code.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

//#define BUFF_SIZE 256

class FtpServer : public std::enable_shared_from_this<FtpServer>
{
public:
	FtpServer(const uint16_t READ_PORT);
	~FtpServer();

private:
	bool start_server(const uint16_t READ_PORT);
	inline bool cmp_chartostr(const char* buff, const std::string& cmd , const int lenBuff);
	int waiting_request();
	bool load_file(std::string const& file_path);
	//bool send_file(const std::vector<char> buff_bin);
	void insert_sizefile_tobuff(std::vector<char> &buff, int32_t val);
	//void handle_accept(socket_ptr sock, const boost::system::error_code& err);
	//void start_accept(socket_ptr sock);
	//size_t read_complete(char* buff, const error_code& err, size_t bytes);

private:
	io_service service;
	ip::tcp::socket* sock = nullptr;
	ip::tcp::endpoint* ep = nullptr;
	ip::tcp::acceptor* acc = nullptr;

	//socket_wrapper::SocketWrapper sock_wrap;
	//socket_wrapper::Socket* ftpserv_sock = nullptr;
	//socket_wrapper::Socket* client_sock = nullptr;
	std::string network_path = "C:\\Netwk\\";
	const std::string CMD_EXT = "exit";
	const uint16_t FILEBUFF_SZ = 4096;

};