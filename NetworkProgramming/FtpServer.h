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
	inline bool cmp_chartostr(const char* buf, const std::string& cmd , const int lenBuf);
	int waiting_request();


private:
	socket_wrapper::SocketWrapper sock_wrap;
	socket_wrapper::Socket* ftpserv_sock = nullptr;
	const std::string CMD_EXT = "exit";

};