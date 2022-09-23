#include "DnsResolver.h"

int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <hostname>" << std::endl;
		return EXIT_FAILURE;
	}
	 
	const std::string arg = { argv[1] };
	DnsResolver resolver;
	//Сам определяет прямой или обратный DNS resolve
	if(arg[0] > '0' && arg[0] <= '9')
		resolver.print_hostname(arg);
	else
		resolver.print_ips(arg);
	

}