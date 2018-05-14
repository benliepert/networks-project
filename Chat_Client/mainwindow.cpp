#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

#define POLL_MS 500 //how often pollClient() is called

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connected = false;

    UpdateControls();

    // timer to call pollClient every however many ms
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), myWidget, SLOT(showGPS()));
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

void MainWindow::on_pb_connect_clicked()
{
    //read whats in le_server
    //connect to said server
    QString server = ui->le_server->text();
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
