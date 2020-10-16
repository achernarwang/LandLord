#include "card.h"

Card::Card(QWidget *parent) : QWidget(parent)
{
}

void Card::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),picture);
    QWidget::paintEvent(event);
}

void Card::mousePressEvent(QMouseEvent *event)
{
    emit mouseClicked(event->button());
    QWidget::mousePressEvent(event);
}

void Card::setInfo(int suit, int point)
{
    cardSuit=suit;
    cardPoint=point;
    id=cardPoint*100+cardSuit;
}

void Card::setPic()
{
    if(!is_frontSide){
        picture=QPixmap(":/images/PADDING.png");
        picture=picture.scaledToHeight(105);
    }
    else if(cardPoint){
        switch (cardPoint) {
        case CardPoint::Card_3:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D3.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S3.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H3.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C3.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_4:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D4.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S4.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H4.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C4.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_5:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D5.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S5.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H5.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C5.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_6:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D6.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S6.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H6.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C6.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_7:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D7.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S7.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H7.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C7.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_8:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D8.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S8.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H8.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C8.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_9:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D9.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S9.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H9.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C9.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_10:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D10.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S10.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H10.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C10.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_J:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/DJ.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/SJ.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/HJ.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/CJ.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_Q:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/DQ.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/SQ.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/HQ.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/CQ.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_K:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/DK.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/SK.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/HK.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/CK.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_A:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/DA.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/SA.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/HA.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/CA.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_2:
            switch (cardSuit) {
            case CardSuit::Diamond:
                picture=QPixmap(":/images/D2.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Spade:
                picture=QPixmap(":/images/S2.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Heart:
                picture=QPixmap(":/images/H2.png");
                picture=picture.scaledToHeight(105);
                break;
            case CardSuit::Club:
                picture=QPixmap(":/images/C2.png");
                picture=picture.scaledToHeight(105);
                break;
            }
            break;
        case CardPoint::Card_BJ:
            picture=QPixmap(":/images/BLACK JOKER.png");
            picture=picture.scaledToHeight(105);
            break;
        case CardPoint::Card_RJ:
            picture=QPixmap(":/images/RED JOKER.png");
            picture=picture.scaledToHeight(105);
            break;
        }
    }
    resize(picture.size());
    update();
}
