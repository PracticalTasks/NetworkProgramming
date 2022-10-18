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
	inline bool cmp_chartostr(const char* buff, const std::string& cmd, const int buff_len);
	int waiting_request();
	bool load_file(std::string const& str_cmd);
	bool send_file(const std::vector<char> file_buff);
	void insert_sizefile_inbuff(std::vector<char>& buff, int32_t val);
	void strcmd_parsing(std::string str_cmd);

private:
	//constants
	const uint8_t SERVINFO_SZ = 4;
	const std::string EXT_CMD = "exit";
	const uint16_t FILEBUFF_SZ = 4096;

	socket_wrapper::SocketWrapper sock_wrap;
	socket_wrapper::Socket* ftpserv_sock = nullptr;
	socket_wrapper::Socket* client_sock = nullptr;
	//Папка в которой будет искать файлы сервер
	std::string network_folder = "C:\\Netwk\\";
	std::string file_path;
	int32_t arg_1{};
	int32_t arg_2{};

};