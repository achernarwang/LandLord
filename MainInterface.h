#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QDialog>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include "web.h"
#include "card.h"
#include "player.h"
#include "gamerule.h"

namespace Ui {
class MainInterface;
}

class MainInterface : public QDialog
{
    Q_OBJECT

public:
    explicit MainInterface(QWidget *parent = nullptr);
    ~MainInterface();

    void setWeb(Web *);
    void iniWeb();
    void initGame();
    void displayPlayerCards();
    void displayRestThree();
    void displayOnTableCards();
    void send();

    void callLord();
    void declareLord();
    void playerIsLord();
    void playerAIsLord();
    void playerBIsLord();

    void startPlay();
    void playCard();

    void gameOver();


private slots:
    void receive();

    void on_btnLeft_clicked();

    void on_btnRight_clicked();

    void on_mouseClicked(Qt::MouseButton);

    void on_timeout();

private:
    Ui::MainInterface *ui;
    Web *web;
    QVector<Card*> cards;
    QVector<Card*> shuffledCards;
    QVector<Card*> restThree;
    QVector<Card*> restThreeDisplay;
    QVector<Card*> onTableCards;
    Player *playerA=nullptr;
    Player *playerB=nullptr;
    Player *player=nullptr;
    QTimer *timer = nullptr;
    QEventLoop *loop=nullptr;
    enum gameStatus{start,call,declare,play,over,again};
    int status=start;
    int firstPlayer=-1;
    int nextPlayer=-1;
    bool AFirst=false;
    int mustPlay=0; //牌权 记录收到的不出的个数，如果个数为2说明player有牌权
    int replayFlag=0;
};

#endif // MAININTERFACE_H
