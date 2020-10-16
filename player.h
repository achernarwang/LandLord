#ifndef PLAYER_H
#define PLAYER_H

#include <QVector>
#include "card.h"
class Player
{
public:
    Player();
    void sortCards(QVector<Card*> &);
    QVector<Card*> handCards;
    QVector<Card*> selectedCards;
    int role=-1;
    enum Role{lord,farmer};
    int decision=-1;
    enum Decision{pass,act};
    int cardsNum=17;
};

#endif // PLAYER_H
