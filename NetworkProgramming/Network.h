#pragma once
#include <cassert>
#include <cstdlib>
#include <iostream>

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>

class Network
{
public:
	Network();
	~Network();
	void print_ips(const std::string& host_name);
	void print_hostname(const std::string& ip_addr);

private:
	addrinfo* getAddrInfo(const std::string& host_name);
	char* getNameInfo(const std::string& ip_addr);

};