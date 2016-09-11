#ifndef PUSHSERVER_H
#define PUSHSERVER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <QDebug>
#include <boost/thread.hpp>
#include "notifications.h"
#include "remote_client.h"
#include "utility.h"

class PushServer
{
public:
    PushServer();
    ~PushServer();
    bool run();
    void stop();
    bool is_running();
    bool add_notification(Notification *p_notif);
    quint32 add_periodic_notification(quint16 t_begin, quint16 t_end, quint16 t_period, std::string text);
    bool remove_notification(int);
    bool set_notification_text(int, std::string);
    bool init_notification(int);
    void send_notification(std::string text);
    int notifications_count();
    bool add_client(std::string ip);
    bool remove_client(std::string ip);

private:
    boost::thread b_thread;
    boost::mutex mutex_loop, mutex_notif, mutex_client;
    void loop();
    bool loop_running;

    QMap<quint32, Notification*> notifications_map;     // id, Notification*
    QMap<std::string, Remote_Client*> clients_map;      // ip, Remote_Client*

    void check_notifications();
};

#endif // PUSHSERVER_H
