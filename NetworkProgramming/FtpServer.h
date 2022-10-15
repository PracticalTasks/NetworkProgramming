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

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>
#include <socket_wrapper/socket_class.h>

#define BUFF_SIZE 256

class FtpServer
{
public:
	FtpServer(const uint16_t READ_PORT);
	~FtpServer();

private:
	bool start_server(const uint16_t READ_PORT);
	inline bool cmp_chartostr(const char* buff, const std::string& cmd , const int lenBuff);
	int waiting_request();
	bool load_file(std::string const& file_path);
	bool send_file(const std::vector<char> buff_bin);
	void insert_sizefile_tobuff(std::vector<char> &buff, int32_t val);

private:
	socket_wrapper::SocketWrapper sock_wrap;
	socket_wrapper::Socket* ftpserv_sock = nullptr;
	socket_wrapper::Socket* client_sock = nullptr;
	std::string network_path = "C:\\Netwk\\";
	const std::string CMD_EXT = "exit";
	const uint16_t FILEBUFF_SZ = 4096;

};