#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "pushserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_add_per_notif_clicked();

    void on_pushButton_remove_per_notif_clicked();

    void on_pushButton_add_day_notif_clicked();

    void on_pushButton_remove_day_notif_clicked();

    void on_pushButton_add_client_clicked();

    void on_pushButton_remove_client_clicked();

private:
    Ui::MainWindow *ui;

    PushServer server;

    QLabel *stbar_message;
};

#endif // MAINWINDOW_H
