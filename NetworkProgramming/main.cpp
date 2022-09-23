
#include "Network.h"

int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <hostname>" << std::endl;
		return EXIT_FAILURE;
	}
	 
	const std::string arg = { argv[1] };
	Network net;
	if(arg[0] > '0' && arg[0] <= '9')
		net.print_hostname(arg);
	else
		net.print_ips(arg);
	

}