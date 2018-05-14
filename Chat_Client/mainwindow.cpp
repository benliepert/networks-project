#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.c"
#include <arpa/inet.h>
#include <QTimer>
#include <QDebug>
#include <QString>
#include <QByteArray>

#define POLL_MS 500 //how often pollClient() is called

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connected = false;
    sockfd = -1;

    UpdateControls();

    // timer to call pollClient every however many ms
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(pollClient()));
    timer->start(POLL_MS); //time specified in ms
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pollClient()
{
   qDebug() << "POLL CLIENT" << endl;
}


void MainWindow::on_pb_send_clicked()
{
    // read whats in te_send
    QString message = ui->te_send->toPlainText();
}

void MainWindow::EmitError(QString qsError)
{
    ui->tb_chat->append(qsError);
}
bool MainWindow::isValidIpAddress(QString qs)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, qs_to_cp(qs), &(sa.sin_addr));
    return result != 0;
}

char * MainWindow::qs_to_cp(QString qs)
{
    QByteArray ba = qs.toLatin1();
    return ba.data();
}

void MainWindow::on_pb_connect_clicked()
{
    //read whats in le_server
    //connect to said server
    QString qsServerIP = ui->le_server->text();

    char * cpServerIP = qs_to_cp(qsServerIP);
    if(!isValidIpAddress(qsServerIP))
    {
        EmitError("- - - Error: Please enter a valid IP - - -");
        return;
    }

    sockfd = connectToServer(cpServerIP);

    switch(sockfd)
    {
        case -2:
            EmitError("- - - Error: Failed to get address info! - - -");
        case -3:
            EmitError("- - - Error: Failed to connect! - - -");
    }

//    if(sockfd == -3)
//    {
//        EmitError("- - - Error: Failed to connect! - - -");
//    }
//    else


}

void MainWindow::on_pb_disconnect_clicked()
{
    // disconnect from server
}

void MainWindow::on_pb_join_clicked()
{
    // read whats in le_channel
    // attempt to connect to said channel
    QString channel = ui->le_channel->text();
}

void MainWindow::UpdateControls()
{
    // call this boi to update button states and such
    ui->pb_connect->setEnabled(!connected);
    ui->pb_disconnect->setEnabled(connected);
    ui->pb_join->setEnabled(connected);
    ui->pb_send->setEnabled(connected);
}
