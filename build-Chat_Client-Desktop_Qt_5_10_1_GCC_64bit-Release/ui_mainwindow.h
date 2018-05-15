/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QTextEdit *te_send;
    QPushButton *pb_send;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *le_server;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *le_nickname;
    QTextBrowser *tb_chat;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *le_channel;
    QPushButton *pb_join;
    QSpacerItem *verticalSpacer;
    QPushButton *pb_connect;
    QPushButton *pb_disconnect;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(938, 651);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_5 = new QHBoxLayout(centralWidget);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        te_send = new QTextEdit(splitter);
        te_send->setObjectName(QStringLiteral("te_send"));
        splitter->addWidget(te_send);
        pb_send = new QPushButton(splitter);
        pb_send->setObjectName(QStringLiteral("pb_send"));
        splitter->addWidget(pb_send);

        gridLayout->addWidget(splitter, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_3->addWidget(label_4);

        le_server = new QLineEdit(centralWidget);
        le_server->setObjectName(QStringLiteral("le_server"));

        horizontalLayout_3->addWidget(le_server);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        le_nickname = new QLineEdit(centralWidget);
        le_nickname->setObjectName(QStringLiteral("le_nickname"));

        horizontalLayout_2->addWidget(le_nickname);


        horizontalLayout_4->addLayout(horizontalLayout_2);


        gridLayout->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        tb_chat = new QTextBrowser(centralWidget);
        tb_chat->setObjectName(QStringLiteral("tb_chat"));

        gridLayout->addWidget(tb_chat, 1, 0, 1, 1);


        horizontalLayout_5->addLayout(gridLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        le_channel = new QLineEdit(centralWidget);
        le_channel->setObjectName(QStringLiteral("le_channel"));

        horizontalLayout->addWidget(le_channel);

        pb_join = new QPushButton(centralWidget);
        pb_join->setObjectName(QStringLiteral("pb_join"));

        horizontalLayout->addWidget(pb_join);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pb_connect = new QPushButton(centralWidget);
        pb_connect->setObjectName(QStringLiteral("pb_connect"));
        pb_connect->setMinimumSize(QSize(100, 40));
        pb_connect->setStyleSheet(QLatin1String("color: rgb(138, 226, 52);\n"
"font: 57 15pt \"Ubuntu\";"));

        verticalLayout->addWidget(pb_connect);

        pb_disconnect = new QPushButton(centralWidget);
        pb_disconnect->setObjectName(QStringLiteral("pb_disconnect"));
        pb_disconnect->setMinimumSize(QSize(100, 40));
        pb_disconnect->setStyleSheet(QLatin1String("color: rgb(239, 41, 41);\n"
"font: 57 11pt \"Ubuntu\";"));

        verticalLayout->addWidget(pb_disconnect);


        horizontalLayout_5->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "I bought an '83 cutlass for the weekend", nullptr));
        pb_send->setText(QApplication::translate("MainWindow", "Send", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Server:", nullptr));
        label->setText(QApplication::translate("MainWindow", "Nickname:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Channel:", nullptr));
        pb_join->setText(QApplication::translate("MainWindow", "Join", nullptr));
        pb_connect->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        pb_disconnect->setText(QApplication::translate("MainWindow", "Disconnect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
