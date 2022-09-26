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
	//Сам определяет прямой или обратный DNS resolve
	if(arg[0] > '0' && arg[0] <= '9')
		resolver.printHostName(arg);
	else
		resolver.printIps(arg);
	

}