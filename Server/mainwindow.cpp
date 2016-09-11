#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stbar_message = new QLabel(this);
    statusBar()->addWidget( stbar_message, true);

    ui->timeEdit_daytime->setTime( QTime::currentTime().addSecs(60) );

    ui->treeWidget_per_notifs->setColumnCount(2);
    ui->treeWidget_per_notifs->setColumnHidden(1, true);
    ui->treeWidget_per_notifs->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    ui->treeWidget_day_notifs->setColumnCount(2);
    ui->treeWidget_day_notifs->setColumnHidden(1, true);
    ui->treeWidget_day_notifs->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit_ip->setValidator(ipValidator);
    ui->lineEdit_ip->setText("127.0.0.1");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    if( server.is_running() ){
        server.stop();
        ui->pushButton_start->setText("Start server");
        stbar_message->setText("Server is stoped");
    }else{
        if( server.run() ){
            ui->pushButton_start->setText("Stop server");
            stbar_message->setText("Server is running");
        }
    }
}

void MainWindow::on_pushButton_exit_clicked()
{
    close();
}

void MainWindow::on_pushButton_add_per_notif_clicked()
{
    quint32 id;
    if(( id = server.add_periodic_notification( ui->timeEdit_begin->time().msecsSinceStartOfDay()/60000,
                                          ui->timeEdit_end->time().msecsSinceStartOfDay()/60000,
                                          ui->timeEdit_period->time().msecsSinceStartOfDay()/60000,
                                          ui->lineEdit_per_text->text().toUtf8().constData() ) ))
    {
        QString s = "from " + ui->timeEdit_begin->text() + " to " + ui->timeEdit_end->text() +
                ", " + ui->timeEdit_period->text();
        QTreeWidgetItem *item = new QTreeWidgetItem( QStringList() << s << QString::number(id) );
        ui->treeWidget_per_notifs->addTopLevelItem( item );

        qDebug() << "server.size" << server.notifications_count();
    }
}

void MainWindow::on_pushButton_remove_per_notif_clicked()
{
    if(ui->treeWidget_per_notifs->selectedItems().size()==0) return;

    int id = ui->treeWidget_per_notifs->currentItem()->data(1,0).toInt();
    if( id >=0 ){
        delete ui->treeWidget_per_notifs->currentItem();
        server.remove_notification(id);
        qDebug() << "server.size" << server.notifications_count();
    }
}

void MainWindow::on_pushButton_add_day_notif_clicked()
{
    Notification* new_notif = new EverydayNotification(
                ui->timeEdit_daytime->time().msecsSinceStartOfDay()/60000 );
    if( server.add_notification( new_notif ) )
    {
        server.set_notification_text(new_notif->get_id(), ui->lineEdit_day_text->text().toUtf8().constData() );

        QString s = ui->timeEdit_daytime->text();
        QTreeWidgetItem *item = new QTreeWidgetItem( QStringList() << s << QString::number(new_notif->get_id()) );
        ui->treeWidget_day_notifs->addTopLevelItem( item );
        qDebug() << "server.size" << server.notifications_count();
    }
}

void MainWindow::on_pushButton_remove_day_notif_clicked()
{
    if(ui->treeWidget_day_notifs->selectedItems().size()==0) return;

    int id = ui->treeWidget_day_notifs->currentItem()->data(1,0).toInt();
    if( id >=0 ){
        delete ui->treeWidget_day_notifs->currentItem();
        server.remove_notification(id);
        qDebug() << "server.size" << server.notifications_count();
    }
}

void MainWindow::on_pushButton_add_client_clicked()
{
    if( server.add_client( ui->lineEdit_ip->text().toStdString() ) )
    {
        QTreeWidgetItem *item = new QTreeWidgetItem( QStringList() << ui->lineEdit_ip->text() );
        ui->treeWidget_clients->addTopLevelItem( item );
    }
}

void MainWindow::on_pushButton_remove_client_clicked()
{
    if(ui->treeWidget_clients->selectedItems().size()==0) return;
    std::string s_ip = ui->treeWidget_clients->currentItem()->data(0,0).toString().toStdString();
    if( s_ip.size()>0 ){
        delete ui->treeWidget_clients->currentItem();
        server.remove_client(s_ip);
    }
}
