#include "MainInterface.h"
#include "PreInterface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PreInterface *p=new PreInterface;
    if(p->exec()==QDialog::Accepted)
    {
        MainInterface w;
        w.setWeb(p->getWeb());
        w.iniWeb();
        w.initGame();
        w.show();
        return a.exec();
    }
    else return 0;
}
