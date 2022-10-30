#include "FtpServer.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
{
    std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
    return EXIT_FAILURE;
}

    const int READ_PORT { std::stoi(argv[1]) };
    //const int READ_PORT{ 5555 };
    FtpServer ftp(READ_PORT);
}