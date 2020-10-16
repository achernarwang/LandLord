#ifndef PREINTERFACE_H
#define PREINTERFACE_H

#include <QDialog>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include "web.h"

namespace Ui {
class PreInterface;
}

class PreInterface : public QDialog
{
    Q_OBJECT

public:
    explicit PreInterface(QWidget *parent = nullptr);
    ~PreInterface();

    Web* getWeb();

private slots:
    void on_btnStart_clicked();

    void on_newConnection();

    void on_connected();

    void on_timeout();

private:
    Ui::PreInterface *ui;
    Web *web=nullptr;
    QEventLoop *loop=nullptr;
    QTimer *timer;
};

#endif // PREINTERFACE_H
