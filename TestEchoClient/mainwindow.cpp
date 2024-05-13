#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QValidator>
#include <QMessageBox>
#include <QLabel>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setGeometry(20, 20, 600, 400);

    QWidget *mainWidget = new QWidget;


    m_connectBtn = new QPushButton("Connect");
    m_connectBtn->setCheckable(true);
    m_connectBtn->setDisabled(true);

    m_host = new QLineEdit("Address");
    m_port = new QLineEdit;

    m_host->setInputMask("000.000.000.000");

    QIntValidator *portValidator = new QIntValidator(0, 65535, this);
    m_port->setValidator(portValidator);

    QVBoxLayout *controlLayout = new QVBoxLayout;

    m_msgView = new QTextEdit;
    m_msgView->setReadOnly(true);
    m_client = new C_TcpClient(*m_msgView, this);

    m_msgEdit = new C_MyTextEdit;

    QVBoxLayout *chatLayout = new QVBoxLayout;

    QHBoxLayout *mainLayout = new QHBoxLayout;

    controlLayout->addWidget(m_host);
    controlLayout->addWidget(m_port);
    controlLayout->addWidget(m_connectBtn);

    chatLayout->addWidget(m_msgView, 7);

    chatLayout->addWidget(m_msgEdit, 1);

    mainLayout->addLayout(chatLayout, 7);
    mainLayout->addLayout(controlLayout, 2);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    m_host->setFocus(Qt::OtherFocusReason);
    m_port->setDisabled(true);

    connect(m_port, &QLineEdit::returnPressed, this, &MainWindow::onPortEdit);
    connect(m_host, &QLineEdit::returnPressed, this, &MainWindow::onHostEdit);
    connect(m_msgEdit, &C_MyTextEdit::returnPressed, this, &MainWindow::onEnteredMsg);
    connect(m_connectBtn, &QPushButton::clicked, this, &MainWindow::onBtnConnect);

}

MainWindow::~MainWindow()
{

}

void MainWindow::onHostEdit()
{
    if ( m_hostAddr.setAddress(m_host->text())) {
        m_port->setEnabled(true);
        m_port->setFocus(Qt::OtherFocusReason);
        m_msgView->setTextColor(QColor(Qt::black));
    }
    else {
        m_msgView->setTextColor(QColor(Qt::red));
        m_msgView->append("Error: nvalid host address");
    }
}

void MainWindow::onPortEdit()
{
    m_portNum = m_port->text().toUShort();
    m_connectBtn->setEnabled(true);
    m_connectBtn->setFocus(Qt::OtherFocusReason);
}

void MainWindow::onEnteredMsg()
{
     m_client->sendMessage(m_msgEdit->toPlainText());
     m_msgEdit->clear();
}

void MainWindow::onBtnConnect()
{
    m_client->connectHost(m_hostAddr, m_portNum);
}
