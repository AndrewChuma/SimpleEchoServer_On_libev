#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHostAddress>

#include "mytextedit.h"
#include "C_TcpClient.h"

class QTextEdit;
class QTextBrowser;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

  QPushButton *m_connectBtn;
  QTextEdit   *m_msgView;
  QLineEdit   *m_host;
  QLineEdit   *m_port;

  C_TcpClient *m_client;

  C_MyTextEdit *m_msgEdit;
  QHostAddress m_hostAddr;
  quint16 m_portNum;

public slots:

  void onPortEdit();
  void onHostEdit();
  void onEnteredMsg();
  void onBtnConnect();

};
#endif // MAINWINDOW_H
