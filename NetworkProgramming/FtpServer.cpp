#include "FtpServer.h"

FtpServer::FtpServer(const uint16_t READ_PORT)
{
    if (!start_server(READ_PORT))
    {
        std::cout << "Can not running echo tcp server on the port " << READ_PORT << std::endl;
        return;
    }

    waiting_request();
}

FtpServer::~FtpServer()
{
    delete ftpserv_sock;
}

bool FtpServer::start_server(const uint16_t READ_PORT)
{
    ftpserv_sock = new socket_wrapper::Socket(AF_INET, SOCK_STREAM, NULL);

    if (!*ftpserv_sock)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        return false;
    }

    sockaddr_in addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(READ_PORT),
    };

    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(*ftpserv_sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        std::cerr << sock_wrap.get_last_error_string() << std::endl;
        // Socket will be closed in the Socket destructor.
        return false;
    }

    if (listen(*ftpserv_sock, SOMAXCONN) != SOCKET_ERROR)
    {
        std::cout << "Running ftp server on the port " << READ_PORT << "...\n";
        return true;
    }

    return false;
}

int FtpServer::waiting_request()
{
    char client_addrbuf[INET_ADDRSTRLEN];

    sockaddr_in addr_c;
    int addrlen = sizeof(addr_c);

    std::array <char, BUFF_SIZE> name_file{};
    uint32_t packet_size = 0;

    while (true)
    {
        client_sock = new socket_wrapper::Socket(accept(*ftpserv_sock, reinterpret_cast<sockaddr*>(&addr_c), &addrlen));

        if (!client_sock)
        {
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            return EXIT_FAILURE;
        }

        std::cout
            << "Client with address "
            << inet_ntop(AF_INET, &addr_c.sin_addr, client_addrbuf, sizeof(client_addrbuf) / sizeof(client_addrbuf[0]))
            << ":" << ntohs(addr_c.sin_port)
            << "\n\n";

        while (true)
        {
            packet_size = recv(*client_sock, name_file.data(), name_file.size(), 0);

            if (packet_size > 0)
            {
                if (cmp_chartostr(name_file.data(), EXT_CMD, packet_size))
                {
                    std::cout << "Echo server has been stopped ...\n";
                    return EXIT_SUCCESS;
                }

                if (!load_file(network_folder + name_file.data()))
                    continue;
            }
            else if (packet_size == 0)
            {
                std::cout << "disconnect " << inet_ntop(AF_INET, &addr_c.sin_addr, client_addrbuf,
                    sizeof(client_addrbuf) / sizeof(client_addrbuf[0])) << ":" << ntohs(addr_c.sin_port) << std::endl;
                break;
            }

            name_file.fill(0);
            std::cout << std::endl;
        }
        delete client_sock;
    }

    return EXIT_SUCCESS;
}

void FtpServer::insert_sizefile_inbuff(std::vector<char>& buff, int32_t val)
{
    uint16_t hw_val{}; //старшее слово
    uint16_t lw_val{}; //младшее слово

    hw_val = val >> 16;
    lw_val = val & 0xFFFF;

    auto it = buff.begin();
    it[0] = lw_val & 0xFF;
    it[1] = lw_val >> 8;
    it[2] = hw_val & 0xFF;
    it[3] = hw_val >> 8;
}

void FtpServer::strcmd_parsing(std::string str_cmd)
{
    int item_pos{};
    int prev_pos{};

    //Получаем имя файла из принятой команды
    if ((item_pos = str_cmd.find(' ')) == -1)
    {
        std::cout << "No arguments";
        file_path = str_cmd;
        return;
    }
    
    file_path = str_cmd.substr(0, item_pos);
    prev_pos = item_pos + 1;
    
    //Первый аргумент
    std::string str_arg1;
    if ((item_pos = str_cmd.find(' ', prev_pos)) == -1)
    {
        str_arg1 = str_cmd.substr(prev_pos + 1);
        try
        {
            arg_1 = stoi(str_arg1);
        }
        catch (std::invalid_argument e)
        {
            std::cout << "Caught Invalid Argument_1 Exception\n";
        }
        return;
    } 
    
    str_arg1 = str_cmd.substr(prev_pos + 1, item_pos - (prev_pos + 1));      //Прибовляем к позиции разделитель что бы его не записывать
    try
    {
        arg_1 = stoi(str_arg1);
    }
    catch (std::invalid_argument e)
    {
        std::cout << "Caught Invalid Argument_1 Exception\n";
    }

    prev_pos = item_pos + 1;

    //Второй аргумент
    std::string str_arg2;
    str_arg2 = str_cmd.substr(prev_pos + 1);      //Прибовляем к позиции разделитель что бы его не записывать

    try
    {
        arg_2 = stoi(str_arg2);
    }
    catch(std::invalid_argument e)
    {
        std::cout << "Caught Invalid Argument_2 Exception\n";
    }  
}

bool FtpServer::load_file(std::string const& str_cmd)
{
    std::vector<char> file_buff(FILEBUFF_SZ);

    //Парсим команду пришедшую от клиента
    strcmd_parsing(str_cmd);

    std::ifstream file_stream(file_path, std::ifstream::binary);

    if (!file_stream)
    {
        std::cout << "Error load file " << file_path;
        return false;
    }

    int32_t file_length{};
    //Количество считываемых байт
    int reading_file_sz{};
    //Если есть второй аргумент указывающий сколько байт передовать
    if (arg_2)
        file_length = arg_2;
    else
    {
        //get length of file:
        file_stream.seekg(0, file_stream.end);
        //Переменная обратный счётчик для определения конца файла
        file_length = file_stream.tellg();
        //Отнимаем от общего размера смещения с которого читается файл
        file_length -= arg_1;
        file_stream.seekg(arg_1, file_stream.beg);
    }

    //Если размер файла меньше буфера за минусом служебной информации
    if (file_length < (FILEBUFF_SZ - SERVINFO_SZ))
        reading_file_sz = file_length;
    else
        reading_file_sz = FILEBUFF_SZ - SERVINFO_SZ;

    std::cout << "Sending file " << file_path << "...\n";

    //Заносить в первые четыре байта длину файла в big endian
    insert_sizefile_inbuff(file_buff, file_length);
    //Если есть первый аргумент 'смещения от начала файла'
    //Устанавливаем текущую позицию чтения из file_stream на него
    //if (arg_1)
    //    file_stream.seekg(arg_1, file_stream.cur);

    file_stream.read(file_buff.data() + SERVINFO_SZ, reading_file_sz);

    //Проверяем ни моследняя ли часть файла
    if (file_length <= FILEBUFF_SZ)
        file_buff.resize(file_length + SERVINFO_SZ);                //Изменяем размер буфера равный оставшимся байтам

    //Часть файла уже загрузили
    file_length -= FILEBUFF_SZ - SERVINFO_SZ;
    if (!send_file(file_buff))
        return false;

    //Снова делаем размер считываемых данных равный буферу
    reading_file_sz += SERVINFO_SZ;

    while (file_length > 0)
    {
        file_stream.read(file_buff.data(), reading_file_sz);

        //Проверяем ни моследняя ли часть файла
        if (file_length <= FILEBUFF_SZ)
            file_buff.resize(file_length);                //Изменяем размер буфера равный оставшимся байтам

        file_length -= FILEBUFF_SZ;

        if (!send_file(file_buff))
            return false;
        file_buff.assign(FILEBUFF_SZ, 0);
    }

    std::cout << "done!\n";

    return true;
}

bool FtpServer::send_file(const std::vector<char> file_buff)
{
    uint32_t packet_size = 0;
    int transmit_cnt = 0;
    int size = file_buff.size();

    while (transmit_cnt != size)
    {
        packet_size = send(*client_sock, &(file_buff.data()[0]) + transmit_cnt, size - transmit_cnt, 0);

        if (packet_size == SOCKET_ERROR)
        {
            std::cerr << "Can't send file to Client. Error #" << sock_wrap.get_last_error_string() << std::endl;
            return false;
        }
        transmit_cnt += packet_size;
    }

    return true;
}

//Метод для сравнения символьной строки с string
inline bool FtpServer::cmp_chartostr(const char* buff, const std::string& cmd, const int buff_len)
{
    if (cmd.size() != buff_len)
        return false;

    for (int i{}; i < buff_len; ++i)
    {
        if (tolower(buff[i]) != cmd[i])
            return false;
    }

    return true;
}