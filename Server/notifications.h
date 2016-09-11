#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QTime>
#include <QDebug>
#include <utility.h>

class Notification
{
public:
    Notification();
    virtual ~Notification(){}
    virtual void init(quint16 cur_minute) = 0;
    virtual bool check_time(quint16 cur_minute) = 0;
    void setText(std::string s);
    std::string text();
    quint32 get_id();    

private:
    std::string notif_text;
    quint32 id;
    static quint32 id_counter;

protected:
    QSet<quint8> days_of_week; //todo: manipulations
};


class PeriodicNotification: public Notification
{
public:
    PeriodicNotification();
    PeriodicNotification(quint16 t_begin, quint16 t_end, quint16 t_period, std::string p_text);
    ~PeriodicNotification(){}
    void init(quint16 cur_minute);
    bool check_time(quint16 cur_minute);
    void setup(quint16 t_begin, quint16 t_end, quint16 t_period);

private:
    quint16 time_begin, time_end, time_period, time_next; // in minutes
    bool in_range(quint16 &cur_minute);
};


class EverydayNotification: public Notification
{
public:
    EverydayNotification();
    EverydayNotification(quint16 t_daytime);
    ~EverydayNotification(){}
    void init(quint16 cur_minute);
    bool check_time(quint16 cur_minute);
    void setup(quint16 t_daytime);

private:
    QDate last_day;
    quint16 day_time;
    bool in_range();
};


#endif // NOTIFICATIONS_H
