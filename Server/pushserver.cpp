#include "pushserver.h"

PushServer::PushServer():loop_running(false)
{

}

PushServer::~PushServer()
{
    stop();
    b_thread.join();

    foreach (Notification *notif, notifications_map) {
        delete notif;
    }
    foreach (Remote_Client* client, clients_map) {
        delete client;
    }
}

bool PushServer::run()
{
    b_thread = boost::thread(boost::bind(&PushServer::loop, this));
    if(!b_thread.joinable()) return false;
    return true;
}

void PushServer::stop()
{
    boost::mutex::scoped_lock lock(mutex_loop);
    loop_running = false;
}

bool PushServer::is_running()
{
    boost::mutex::scoped_lock lock(mutex_loop);
    return loop_running;
}

bool PushServer::add_notification(Notification *p_notif)
{
    if(is_running())
        p_notif->init( QTime::currentTime().msecsSinceStartOfDay()/60000 );

    boost::mutex::scoped_lock lock(mutex_notif);
    notifications_map[p_notif->get_id()] = p_notif;

    //todo: check parameters
    return true;
}

quint32 PushServer::add_periodic_notification(quint16 t_begin, quint16 t_end, quint16 t_period, std::string text)
{
    Notification* new_notif = new PeriodicNotification( t_begin, t_end, t_period, text);

    if(is_running())
        new_notif->init( QTime::currentTime().msecsSinceStartOfDay()/60000 );

    {
        boost::mutex::scoped_lock lock(mutex_notif);
        notifications_map[new_notif->get_id()] = new_notif;
    }
    return new_notif->get_id();
}

bool PushServer::remove_notification(int id)
{
    boost::mutex::scoped_lock lock(mutex_notif);
    if( notifications_map.contains(id) ){
        delete notifications_map[id];
        notifications_map.remove(id);
        return true;
    }
    return false;
}

bool PushServer::set_notification_text(int id, std::string text)
{
    boost::mutex::scoped_lock lock(mutex_notif);
    if(notifications_map.contains(id)){
        notifications_map[id]->setText(text);
        return true;
    }
    return false;
}

bool PushServer::init_notification(int id)
{
    boost::mutex::scoped_lock lock(mutex_notif);
    if(notifications_map.contains(id)){
        notifications_map[id]->init( QTime::currentTime().msecsSinceStartOfDay()/60000 );
        return true;
    }
    return false;
}

void PushServer::send_notification(std::string text)
{
    boost::mutex::scoped_lock lock(mutex_client);
    foreach (Remote_Client* client, clients_map) {
        client->send_msg(text);
    }
}

int PushServer::notifications_count()
{
    boost::mutex::scoped_lock lock(mutex_notif);
    return notifications_map.size();
}

bool PushServer::add_client(std::string ip)
{
    struct in_addr client_ip;
    if( inet_aton( ip.c_str(), &client_ip) != 1){
        qDebug() << "Server: Неправильно задан адрес удаленного клиента";
        return false;
    }
    boost::mutex::scoped_lock lock(mutex_client);
    if(clients_map.contains(ip)){
        qDebug() << "Server: Клиент с таким адресом уже существует";
        return false;
    }
    clients_map[ip] = new Remote_Client(ip);
    return true;
}

bool PushServer::remove_client(std::string ip)
{
    boost::mutex::scoped_lock lock(mutex_client);  
    if( clients_map.contains(ip) ){
        delete clients_map[ip];
        clients_map.remove(ip);
        return true;
    }
    return false;
}

void PushServer::loop()
{
    {
        boost::mutex::scoped_lock lock(mutex_loop);
        loop_running = true;
    }

    quint16 cur_minute = QTime::currentTime().msecsSinceStartOfDay()/60000;
    foreach (Notification* notif, notifications_map) {
        notif->init(cur_minute);
    }

    while( is_running() )
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(SERVER_REVIEW_TIME_MS));        
        check_notifications();
    }
}

void PushServer::check_notifications()
{
    quint16 cur_minute = QTime::currentTime().msecsSinceStartOfDay()/60000;
    boost::mutex::scoped_lock lock(mutex_notif);
    foreach (Notification* notif, notifications_map) {

        if( notif->check_time(cur_minute) ){

            qDebug() << QString::fromUtf8( notif->text().c_str() ) << cur_minute;
            send_notification( notif->text() );
        }

    }
}

