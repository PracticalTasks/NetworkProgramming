#pragma once
//#include <cassert>
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
#define REQ_BUFFSZ 256

class FtpServer
{
public:
	FtpServer(const uint16_t READ_PORT);
	~FtpServer();

private:
	bool start_server(const uint16_t READ_PORT);
	inline bool cmp_chartostr(const char* buff, const std::string& cmd , const int lenBuff);
	void waiting_request(const boost::system::error_code& ec, std::size_t bytes_transferred);
	void accepted_connection(const boost::system::error_code& ec);

	bool load_file(std::string const& file_path);
	bool send_file(const std::vector<char> buff_bin);
	void insert_sizefile_tobuff(std::vector<char> &buff, int32_t val);

private:
	io_service service;
	ip::tcp::socket* sock = nullptr;
	ip::tcp::endpoint* ep = nullptr;
	ip::tcp::acceptor* serv_accept = nullptr;

	char req_buff[REQ_BUFFSZ]{};
	std::string network_path = "C:\\Netwk\\";
	const std::string CMD_EXT = "exit";
	const uint16_t FILEBUFF_SZ = 4096;

};