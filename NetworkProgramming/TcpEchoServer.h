#pragma once
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>
#include <socket_wrapper/socket_class.h>

class TcpEchoServer
{
public:
	TcpEchoServer(const uint16_t READ_PORT);
	~TcpEchoServer();

private:
	inline bool cmp_chartostr(const char* buf, const std::string& cmd , const int lenBuf);
	int read(const uint16_t READ_PORT);

private:
	const std::string CMD_EXT = "exit";

};