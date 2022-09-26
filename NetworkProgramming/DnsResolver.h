#pragma once
#include <cassert>
#include <cstdlib>
#include <iostream>

#include <socket_wrapper/socket_headers.h>
#include <socket_wrapper/socket_wrapper.h>
#include <socket_wrapper/socket_class.h>

class DnsResolver
{
public:
	DnsResolver(const std::string& arg);
	~DnsResolver();

private:
	void printIps(const std::string& host_name);
	void printHostName(const std::string& ip_addr);
	addrinfo* getAddrInfo(const std::string& host_name);
	char* getNameInfo(const std::string& ip_addr);

};