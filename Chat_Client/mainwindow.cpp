#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.c"
#include <arpa/inet.h>
#include <QTimer>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <assert.h>

#define POLL_MS 500 //how often pollClient() is called

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    sockfd = -1;

    connected = false;
    hasNickname = false;
    hasChannel = false;

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

}


void MainWindow::on_pb_send_clicked()
{
    // read whats in te_send
    QString message = ui->te_send->toPlainText();
}

void MainWindow::EmitMessage(QString qsError)
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
    if(connected)
        return;

    //read whats in le_server
    //connect to said server
    QString qsServerIP = ui->le_server->text();

    char * cpServerIP = qs_to_cp(qsServerIP);
    if(!isValidIpAddress(qsServerIP))
    {
        EmitMessage("- - - Error: Please enter a valid IP - - -");
        return;
    }

    sockfd = connectToServer(cpServerIP);

    switch(sockfd)
    {
        case -1:
            EmitMessage("- - - Error: Failed to get address info! - - -");
        case -2:
            EmitMessage("- - - Error: Failed to connect! - - -");
        default:
            EmitMessage("- - - Client successfully connected to: " + qsServerIP);
            connected = true;

    }
    UpdateControls();
}

void MainWindow::on_pb_disconnect_clicked()
{
    if(!connected)
        return;
    // disconnect from server
    disconnectFromServer(sockfd);
    EmitMessage("- - - Disconnected from Server - - -");
    sockfd = -1;
    connected = false;
    UpdateControls();
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
    ui->pb_send->setEnabled(connected && hasNickname && hasChannel);
}

void MainWindow::on_le_nickname_textChanged(const QString &arg1)
{
    hasNickname = (arg1.length() > 0);
    UpdateControls();
}

void MainWindow::on_le_channel_textChanged(const QString &arg1)
{
    hasChannel = (arg1.length());
    UpdateControls();
}
