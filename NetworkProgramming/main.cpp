#include "EchoServer.h"

int main(int argc, char argv[])
{
    //if (argc != 2)
//{
//    std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
//    return EXIT_FAILURE;
//}

    //const int port { std::stoi(argv[1]) };
    const uint16_t READ_PORT = 5555;
    EchoServer echo(READ_PORT);
}