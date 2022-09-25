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


class EchoServer
{
public:
	EchoServer(const uint16_t READ_PORT);
	~EchoServer();

private:
	inline std::string& rtrim(std::string& s);
	int read(const uint16_t READ_PORT);

};