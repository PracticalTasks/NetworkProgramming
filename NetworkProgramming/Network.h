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

class Network
{
public:
	Network();
	~Network();
	int print_ips(const std::string& host_name);

private:
	int print_ips_with_getaddrinfo(const std::string& host_name);
	int print_ips_with_gethostbyname(const std::string& host_name);
};