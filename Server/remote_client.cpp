#include "remote_client.h"

Remote_Client::Remote_Client(std::string s_ip)
{
    set_ip( s_ip );
}

bool Remote_Client::set_ip(std::string s_ip)
{
    struct in_addr cip;
    if( inet_aton( s_ip.c_str(), &cip) != 1){
        qDebug() << "set_ip: Неправильно задан адрес" << s_ip.c_str();
        return 0;
    }
    str_ip = s_ip;
    return 1;
}

void Remote_Client::send_msg(std::string msg)
{
    int client_socket, sz;
    struct sockaddr_in ssa;
    struct sockaddr *sp;
    struct in_addr client_ip;

    sp = (struct sockaddr *)&ssa;
    sz = sizeof(ssa);

    if( inet_aton( str_ip.c_str(), &client_ip) != 1){
        qDebug() << "send_msg: Неправильно задан адрес удаленного клиента";
        return;
    }

    // Создаём сокет
    client_socket = socket( AF_INET, SOCK_STREAM, 0);
    //client_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(client_socket == -1){
        qDebug() << "Невозможно создать сокет";
        return;
    }

    // Разрешить сокету повторное использование локальных адресов
    /*int yes = 1;
    //setsockopt(client_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof (yes) ) < 0 )
    {
        perror("setsockopt");
        close(listenSocket);
        exit(1);
    }*/

    // Задаём адрес клиента
    ssa.sin_family = AF_INET;
    ssa.sin_port = htons(PORT);
    ssa.sin_addr = client_ip;

    // Устанавливаем соединение
    if( connect( client_socket, sp, sz) == -1){
        qDebug() << "Не удалось установить соединение c" << str_ip.c_str();
        return;
    }

    // Отправляем
    qDebug() << send(client_socket, msg.c_str(), MIN( msg.size(), BUF_SIZE), 0);

    close(client_socket);
}

/*void MultiSend(std::string msg)
{
    int client_socket;
    struct sockaddr_in ssa;
    struct sockaddr *sp;

    sp = (struct sockaddr *)&ssa;

    // Создаём сокет
    client_socket = socket( AF_INET, SOCK_STREAM, 0);
    //client_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(client_socket == -1){
        qDebug() << "Невозможно создать сокет";
        return;
    }

    // Разрешить сокету повторное использование локальных адресов
    /*int yes = 1;
    //setsockopt(socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof (yes) ) < 0 )
    {
        perror("setsockopt");
        close(listenSocket);
        exit(1);
    }*/

  /*  // Задаём адрес клиента
    ssa.sin_family = AF_INET;
    ssa.sin_port = htons(PORT);
    ssa.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind( client_socket, sp, sizeof(ssa));

    // Устанавливаем соединение
    if( connect( client_socket, sp, sizeof(ssa)) == -1){
        qDebug() << "Не удалось установить соединение";
        return;
    }

    // Отправляем
    send(client_socket, msg.c_str(), MIN( msg.size(), BUF_SIZE), 0);

    close(client_socket);
}*/
