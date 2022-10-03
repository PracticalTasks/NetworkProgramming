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
	void print_ips(const std::string& host_name);
	void print_hostname(const std::string& ip_addr);
	addrinfo* get_addrinfo(const std::string& host_name);
	char* get_nameinfo(const std::string& ip_addr);

};