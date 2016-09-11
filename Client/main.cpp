#include <QApplication>
//#include <QMessageBox>
#include <pthread.h>

#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define BUF_SIZE    512
#define PORT        3333

/*void *create_msgbox(void *arg)
{
    std::string msg = (char*)arg;
    msg = "notify-send 'Notification' '" + msg + "' -t 500 -i gtk-info";
    //printf("thread: %s\n",msg.c_str());
    system(msg.c_str());
    return NULL;
}*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    int listenSocket, acceptSocket, get_l;
    socklen_t sz, cz;
    struct sockaddr_in ssa, csa;
    struct sockaddr *sp, *cp;
    char buf[BUF_SIZE];
    //char *host;

    sp = (struct sockaddr *)&ssa;
    cp = (struct sockaddr *)&csa;
    sz = sizeof(ssa);
    cz = sizeof(csa);

    // Создаём сокет
    listenSocket = socket( AF_INET, SOCK_STREAM, 0);
    //listenSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(listenSocket == -1){
        perror("Невозможно создать сокет");
        exit(1);
    }

    // Разрешить сокету повторное использование локальных адресов
    int yes = 1;
    if( setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof (yes) ) < 0 )
    {
        perror("setsockopt");
        close(listenSocket);
        exit(1);
    }

    // Резервируем порт
    ssa.sin_family = AF_INET;
    ssa.sin_port = htons( PORT );
    ssa.sin_addr.s_addr = INADDR_ANY;

    if( bind( listenSocket, sp, sz) == -1){
        perror("Невозможно занять порт");
        exit(1);
    }
    //struct hostent *rhost;
    // Переводим сокет в режим ожидания соединения
    if( listen( listenSocket, 10) == -1 ){
        perror("Невозможно перейти в режим ожидания");
        close(listenSocket);
        exit(1);
    }
    printf("Waiting notifications...\n");

    fd_set read_set;
    FD_ZERO(&read_set);str_

    while(1){

        FD_SET( listenSocket, &read_set);

        // Проверяем состояние не блокирующего сокета
        if( select( FD_SETSIZE, &read_set, NULL, NULL, NULL ) == -1 ) {
            perror("Server-select() error!");
            close(listenSocket);
            exit(1);
        }

        if( FD_ISSET( listenSocket, &read_set) )
        {
            // Принимаем соединение, блокирование других запросов связи с данным сервером
            if( (acceptSocket = accept( listenSocket, cp, &cz) ) == -1) {
                perror("Ошибка при выполнении accept");
                close(listenSocket);
                exit(1);
            }

            // Преобразуем адрес хоста отправителя в его имя
            /*rhost = gethostbyaddr( (char*)(&csa.sin_addr), sizeof(csa.sin_addr), AF_INET);
            if(h_errno){
                printf("gethostbyaddr error: %d\n", h_errno);
                host = inet_ntoa(csa.sin_addr);
            } else {
                host = rhost->h_name;
            }*/

            memset(&buf, 0, sizeof(buf));

            /*if( recvfrom(listenSocket, buf, BUF_SIZE, 0, cp, &cz)==-1 ){
                perror("recvfrom error");
                exit(1);
            }*/


            // Получаем данные от сервера
            if( recv(acceptSocket, buf, BUF_SIZE, 0)==-1 ){
                perror("recvfrom error");
                close(acceptSocket);
                close(listenSocket);
                exit(1);
            }


            // Получаем данные от сервера
            /*while( (get_l = recv( acceptSocket, buf, BUF_SIZE, 0)) > 0 ){
                //write(1, buf, get_l);
                printf("%s\n", buf);
            }*/

            printf("%s - from %s:%d\n", buf, inet_ntoa(csa.sin_addr), htons(csa.sin_port) );

            system( std::string("notify-send 'Notification' '" + std::string(buf) + "' -i gtk-info -t 1000").c_str() );

            //sleep(1);

            //char *tbuf;
            //strcpy(tbuf, buf);
            /*pthread_t *new_thread  = new pthread_t();
            if( pthread_create(new_thread, NULL, create_msgbox, (void*)buf) != 0 ){
                perror("Creating thread");
                exit(1);
            }*/
            //pthread_join( *new_thread, NULL);

            // Закрываем соединение
            close(acceptSocket);

        } // FD_ISSET
    } // while(1)

    close(listenSocket);

    //system( "notify-send 'Notification' 'close(listenSocket)' -i gtk-info -t 1000" );

    return a.exec();
}
