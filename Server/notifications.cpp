#include "notifications.h"


quint32 Notification::id_counter = 0;

Notification::Notification()
{
    id = ++id_counter;
    days_of_week << 1 << 2 << 3 << 4 << 5 << 6 << 7; // Monday - Sunday
}

void Notification::setText(std::string s)
{
    notif_text = s;
}

std::string Notification::text()
{
    return notif_text;
}

quint32 Notification::get_id()
{
    return id;
}


PeriodicNotification::PeriodicNotification()
{

}

PeriodicNotification::PeriodicNotification(quint16 t_begin, quint16 t_end, quint16 t_period, std::string p_text)
{
    setup(t_begin, t_end, t_period);
    setText(p_text);
}

void PeriodicNotification::init(quint16 cur_minute)
{
    if( in_range(cur_minute) ){
        int per_count = (cur_minute - time_begin)/time_period;
        time_next = time_begin+(per_count + 1)*time_period;
        if(time_next>time_end) time_next = time_begin + time_period;
    }else
        time_next = time_begin + time_period;
}

bool PeriodicNotification::check_time(quint16 cur_minute)
{
    if( !in_range(cur_minute) ){
        time_next = time_begin + time_period;
        return false;
    }
    if( cur_minute >= time_next ){
        time_next += time_period;
        return true;
    }

    return false;
}

void PeriodicNotification::setup(quint16 t_begin, quint16 t_end, quint16 t_period)
{
    time_begin = t_begin;
    time_end = t_end;
    time_period = t_period;

    if(time_end <= time_begin) time_end += 24*60;
}

bool PeriodicNotification::in_range(quint16 &cur_minute)
{
    QDate d = QDate::currentDate();
    if( cur_minute > 24*60 ) d.addDays(-1);
    if( !days_of_week.contains( d.dayOfWeek() ) ) return false;

    if( cur_minute < time_begin ) cur_minute+=24*60;

    if( IS_BETWEEN(cur_minute, time_begin, time_end) ) return true;
    return false;
}


EverydayNotification::EverydayNotification()
{

}

EverydayNotification::EverydayNotification(quint16 t_daytime)
{
    setup(t_daytime);
}

void EverydayNotification::init(quint16 cur_minute)
{
    last_day = QDate::currentDate();
    if( cur_minute <= day_time )
        last_day = last_day.addDays(-1);
}

bool EverydayNotification::check_time(quint16 cur_minute)
{
    if( !in_range() )
        return false;
    if( cur_minute>=day_time && last_day!=QDate::currentDate()){
        last_day = last_day.addDays(1);
        return true;
    }
    return false;
}

void EverydayNotification::setup(quint16 t_daytime)
{
    day_time = t_daytime;
}

bool EverydayNotification::in_range()
{
    if( days_of_week.contains( QDate::currentDate().dayOfWeek() ) ) return true;
    return false;
}
