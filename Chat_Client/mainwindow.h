#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_pb_send_clicked();

    void on_pb_connect_clicked();

    void on_pb_disconnect_clicked();

    void on_pb_join_clicked();

    void UpdateControls();

private:
    Ui::MainWindow *ui;

    QTimer *timer; //so that pollClient can be called every second

    // button state members
    bool connected;
};

#endif // MAINWINDOW_H
