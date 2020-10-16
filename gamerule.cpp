#include "gamerule.h"

GameRule::GameRule(QVector<Card*> &tmp)
{
    int *cardRecord = new int[Card_End];
    for (int i=0;i<Card_End;i++) {
        cardRecord[i]=0;
    }
    for (int i=0;i<tmp.length();i++) {
        cardRecord[tmp[i]->cardPoint]++;
    }

    oneCard.clear();
    twoCards.clear();
    threeCards.clear();
    fourCards.clear();

    for (int i=0;i<Card_End;i++) {
        if(cardRecord[i]==1)
        {
            oneCard.append(i);
        }
        else if (cardRecord[i]==2) {
            twoCards.append(i);
        }
        else if (cardRecord[i]==3) {
            threeCards.append(i);
        }
        else if (cardRecord[i]==4) {
            fourCards.append(i);
        }
    }
    delete [] cardRecord;

    is_legal=judgeCard();
}

bool GameRule::judgeCard()
{
    handType = Hand_Unknown;
    basePoint = Card_Begin;
    length = 0;
    if(oneCard.isEmpty()&&twoCards.isEmpty()&&threeCards.isEmpty()&&fourCards.isEmpty()){
        handType = Hand_Pass;
    }
    else if (oneCard.length()==1&&twoCards.isEmpty()&&threeCards.isEmpty()&&fourCards.isEmpty()) {
        handType = Hand_Single;
        basePoint = oneCard[0];
    }
    else if (oneCard.isEmpty()&&twoCards.length()==1&&threeCards.isEmpty()&&fourCards.isEmpty()) {
        handType = Hand_Single;
        basePoint = twoCards[0];
    }
    else if (oneCard.isEmpty()&&twoCards.isEmpty()&&threeCards.length()==1&&fourCards.isEmpty()) {
        handType = Hand_Triple;
        basePoint = threeCards[0];
    }
    else if (oneCard.length()==1&&twoCards.isEmpty()&&threeCards.length()==1&&fourCards.isEmpty()) {
        handType = Hand_Triple_Single;
        basePoint = threeCards[0];
    }
    else if (oneCard.isEmpty()&&twoCards.length()==1&&threeCards.length()==1&&fourCards.isEmpty()) {
        handType = Hand_Triple_Pair;
        basePoint = threeCards[0];
    }
    else if (oneCard.isEmpty()&&twoCards.isEmpty()&&threeCards.length()==2&&fourCards.isEmpty()) {
        std::sort(threeCards.begin(),threeCards.end());
        if(threeCards[1] - threeCards[0] == 1 && threeCards[1] < Card_2){
            handType = Hand_Plane;
            basePoint = threeCards[0];
        }
    }
    else if (oneCard.length()==2&&twoCards.isEmpty()&&threeCards.length()==2&&fourCards.isEmpty()) {
        std::sort(threeCards.begin(),threeCards.end());
        std::sort(oneCard.begin(),oneCard.end());
        if(threeCards[1]-threeCards[0]==1&&oneCard[0]!=Card_BJ&&oneCard[1]!=Card_RJ&&threeCards[1]<Card_2){
            handType = Hand_Plane_Two_Single;
            basePoint = threeCards[0];
        }
    }
    else if (oneCard.isEmpty()&&twoCards.length()==2&&threeCards.length()==2&&fourCards.isEmpty()) {
        std::sort(threeCards.begin(),threeCards.end());
        if(threeCards[1]-threeCards[0]==1&&threeCards[1]<Card_2){
            handType = Hand_Plane_Two_Pair;
            basePoint = threeCards[0];
        }
    }
    else if (oneCard.isEmpty()&&twoCards.length()>=3&&threeCards.isEmpty()&&fourCards.isEmpty()) {
        std::sort(twoCards.begin(),twoCards.end());
        if (twoCards.first() >= Card_3&&twoCards.last()<Card_2&&(twoCards.last()-twoCards.first())==(twoCards.length()-1)){
            handType = Hand_Seq_Pair;
            basePoint = twoCards[0];
            length = twoCards.length();
        }
    }
    else if (oneCard.length()>=5&&twoCards.isEmpty()&&threeCards.isEmpty()&&fourCards.isEmpty()) {
        std::sort(oneCard.begin(),oneCard.end());
        if(oneCard.first()>=Card_3&&oneCard.last()<Card_2&&(oneCard.last()-oneCard.first()==(oneCard.length()-1))){
            handType = Hand_Seq_Single;
            basePoint = oneCard[0];
            length = oneCard.length();
        }
    }
    else if (oneCard.isEmpty()&&twoCards.isEmpty()&&threeCards.isEmpty()&&fourCards.length()==1) {
        handType = Hand_Bomb;
        basePoint =fourCards[0];
    }
    else if (oneCard.length()==2&&twoCards.isEmpty()&&threeCards.isEmpty()&&fourCards.length()==1) {
        std::sort(oneCard.begin(),oneCard.end());
        if(oneCard[0]!=Card_BJ&&oneCard[1]!=Card_RJ){
            handType = Hand_Bomb_Two_Single;
            basePoint = fourCards[0];
        }
    }
    else if (oneCard.isEmpty()&&twoCards.length()==1&&threeCards.isEmpty()&&fourCards.length()==1) {
        handType = Hand_Bomb_Pair;
        basePoint = fourCards[0];
    }
    else if (oneCard.isEmpty()&&twoCards.length()==2&&threeCards.isEmpty()&&fourCards.length()==1) {
        handType = Hand_Bomb_Two_pair;
        basePoint = fourCards[0];
    }
    else if (oneCard.length()==2&&twoCards.isEmpty()&&threeCards.isEmpty()&&fourCards.isEmpty()) {
        std::sort(oneCard.begin(),oneCard.end());
        if(oneCard[0] == Card_BJ&&oneCard[1]==Card_RJ){
            handType = Hand_Bomb_Jokers;
        }
    }
    if(handType==Hand_Unknown||handType==Hand_Pass)
        return false;
    else return true;
}

bool GameRule::canBeat(const GameRule &other)
{
    if(handType == Hand_Bomb_Jokers)return true;
    if(other.handType == Hand_Pass)return true;
    if(other.handType >=Hand_Single&&other.handType<=Hand_Bomb_Two_pair&&handType==Hand_Bomb)
        return true;
    if(handType == other.handType)
    {
        if(handType == Hand_Seq_Pair||handType==Hand_Seq_Single){
            return (basePoint>other.basePoint&&length==other.length);
        }
        else {
            return (basePoint>other.basePoint);
        }
    }
    return false;
}
