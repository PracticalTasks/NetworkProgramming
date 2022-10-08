#pragma once
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <array>

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>
#include <socket_wrapper/socket_class.h>

#define BUFF_SIZE 256

class TcpEchoServer
{
public:
	TcpEchoServer(const uint16_t READ_PORT);
	~TcpEchoServer();

private:
	bool start_server(const uint16_t READ_PORT);
	inline bool cmp_chartostr(const char* buf, const std::string& cmd , const int lenBuf);
	int read();
	addrinfo* get_addrinfo(const std::string& host_name);
	//char* get_nameinfo(const std::string& ip_addr);

private:
	socket_wrapper::SocketWrapper sock_wrap;
	socket_wrapper::Socket* tcp_echosock = nullptr;
	//socket_wrapper::Socket* tcp_echosock6 = nullptr;
	const std::string CMD_EXT = "exit";
	//const uint16_t BUFF_SIZE = 256;

};