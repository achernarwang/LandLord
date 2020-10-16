#ifndef GAMERULE_H
#define GAMERULE_H

#include <QVector>
#include "card.h"

enum HandType
{
    Hand_Unknown,			// 未知
    Hand_Pass,				// 过

    Hand_Single,			// 单
    Hand_Pair,				// 对

    Hand_Triple,			// 三个
    Hand_Triple_Single,		// 三带一
    Hand_Triple_Pair,		// 三带二

    Hand_Plane,				// 飞机，555_666
    Hand_Plane_Two_Single,	// 飞机带单，555_666_3_4
    Hand_Plane_Two_Pair,	// 飞机带双，555_666_33_44

    Hand_Seq_Pair,			// 连对，33_44_55(_66...)
    Hand_Seq_Single,		// 顺子，34567(8...)

    Hand_Bomb_Two_Single,	// 四带两单
    Hand_Bomb_Pair,			// 四带一对
    Hand_Bomb_Two_pair,     // 四带两对
    Hand_Bomb,				// 炸弹

    Hand_Bomb_Jokers,			// 王炸

};


class GameRule
{
public:
    GameRule(QVector<Card*> &);
    bool is_legal=false;
    bool judgeCard();
    bool canBeat(const GameRule &);
private:
    int handType = Hand_Unknown;
    int basePoint = Card_Begin;
    int length = 0;
    QVector<int> oneCard;
    QVector<int> twoCards;
    QVector<int> threeCards;
    QVector<int> fourCards;
};

#endif // GAMERULE_H
