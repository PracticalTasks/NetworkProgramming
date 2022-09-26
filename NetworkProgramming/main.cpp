#include "DnsResolver.h"

int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <hostname>" << std::endl;
		return EXIT_FAILURE;
	}
	 
	const std::string arg = { argv[1] };
	DnsResolver resolver(arg);
}