#include "player.h"

Player::Player()
{

}

void Player::sortCards(QVector<Card*> &cards)
{
    for(int i=cards.length()-1;i>=0;i--)
        for(int j=0;j<i;j++)
            if(cards[j]->id<cards[j+1]->id)
            {
                Card *tmp=cards[j];
                cards[j]=cards[j+1];
                cards[j+1]=tmp;
            }
}
