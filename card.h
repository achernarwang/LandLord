#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

enum CardPoint
{
    Card_Begin,

    Card_3,
    Card_4,
    Card_5,
    Card_6,
    Card_7,
    Card_8,
    Card_9,
    Card_10,
    Card_J,
    Card_Q,
    Card_K,
    Card_A,
    Card_2,

    Card_BJ,
    Card_RJ,

    Card_End
};

enum CardSuit
{
    Suit_Begin,

    Diamond,	// 方块
    Club,		// 梅花
    Heart,		// 红桃
    Spade,		// 黑桃

    Suit_End
};

class Card : public QWidget
{
    Q_OBJECT
public:
    explicit Card(QWidget *parent = nullptr);
    void setInfo(int suit,int point);
    void setPic();
    bool is_frontSide=true;
    bool is_selected=false;
    int id=0;
    int cardPoint=0;
    int cardSuit=0;

protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *);

signals:
    void mouseClicked(Qt::MouseButton);

public slots:

private:
    QPixmap picture;
};

#endif // CARD_H
