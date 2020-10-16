#include "PreInterface.h"
#include "ui_PreInterface.h"

PreInterface::PreInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreInterface)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    web=new Web();
}

PreInterface::~PreInterface()
{
    delete ui;
}

void PreInterface::on_btnStart_clicked()
{
    ui->labelTip->setText(tr("正在连接中，请稍候"));
    timer = new QTimer;
    web->socketA = new QTcpSocket;
    web->socketA->connectToHost(QHostAddress("127.0.0.1"),8888);
    /*if(web->socketA->waitForConnected(100))
    {
        qDebug("1");
        web->connectNum+=1;
    }*/
    loop=new QEventLoop;
    connect(web->socketA,SIGNAL(connected()),this,SLOT(on_connected()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timeout()));
    timer->start(100);
    loop->exec();

    web->socketB = new QTcpSocket;
    web->socketB->connectToHost(QHostAddress("127.0.0.1"),8899);
    /*if(web->socketB->waitForConnected(100))
    {
        qDebug("2");
        web->connectNum+=1;
    }*/
    loop=new QEventLoop;
    connect(web->socketB,SIGNAL(connected()),this,SLOT(on_connected()));
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timeout()));
    timer->start(100);
    loop->exec();

    if(web->connectNum==2){
        web->Id=web->C;
        this->accept();
    }
    else if(web->connectNum==1)
    {
        web->Id=web->B;
        delete web->socketB;
        web->socketB=nullptr;
        web->server=new QTcpServer;
        web->server->listen(QHostAddress("127.0.0.1"),8899);
        connect(web->server,SIGNAL(newConnection()),this,SLOT(on_newConnection()));
    }
    else if(web->connectNum==0)
    {
        web->Id=web->A;
        delete web->socketA;
        delete web->socketB;
        web->socketA=nullptr;
        web->socketB=nullptr;
        web->server=new QTcpServer;
        web->server->listen(QHostAddress("127.0.0.1"),8888);
        connect(web->server,SIGNAL(newConnection()),this,SLOT(on_newConnection()));
    }
}

void PreInterface::on_connected()
{
    web->connectNum+=1;
    loop->exit();

}

void PreInterface::on_timeout()
{
    timer->stop();
    loop->exit();
}

void PreInterface::on_newConnection()
{
    QTcpSocket *tmp=new QTcpSocket;
    tmp=web->server->nextPendingConnection();
    if(web->Id==web->A)
    {
        if(web->socketA==nullptr)web->socketA=tmp;
        else web->socketB=tmp;
    }
    else if (web->Id==web->B)
    {
        web->socketB=tmp;
    }
    web->connectNum+=1;
    if(web->connectNum==2)this->accept();
}

Web* PreInterface::getWeb()
{
    return web;
}
