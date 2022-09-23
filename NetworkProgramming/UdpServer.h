#pragma once
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>


class UdpServer
{
public:
	UdpServer();
	~UdpServer();

private:
	inline std::string& rtrim(std::string& s);
	std::string readData();
	void writeData(std::string data);

private:
	const uint16_t READ_PORT = 5555;

};