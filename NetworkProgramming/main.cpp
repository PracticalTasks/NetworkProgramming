
#include "Network.h"

int main(int argc, const char* argv[])
{
	const std::string host_name = "google.com";

	//if (argc < 2)
	//{
	//	std::cout << "Usage: " << argv[0] << " <hostname>" << std::endl;
	//	return EXIT_FAILURE;
	//}
	// 
	//const std::string host_name = { argv[1] };
	Network net;
	net.print_ips(host_name);


}