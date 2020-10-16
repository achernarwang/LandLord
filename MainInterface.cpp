#include "MainInterface.h"
#include "ui_MainInterface.h"

MainInterface::MainInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainInterface)
{
    ui->setupUi(this);
    setFixedSize(1000,650);
    timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timeout()));
}

MainInterface::~MainInterface()
{
    delete ui;
}

void MainInterface::setWeb(Web *web)
{
    this->web=web;
}

void MainInterface::iniWeb()//connect与web有关的函数
{
    if(web->Id==web->A)ui->label->setText(tr("客户端：A"));
    if(web->Id==web->B)ui->label->setText(tr("客户端：B"));
    if(web->Id==web->C)ui->label->setText(tr("客户端：C"));
    connect(web->socketA,SIGNAL(readyRead()),this,SLOT(receive()));
    connect(web->socketB,SIGNAL(readyRead()),this,SLOT(receive()));

}

void MainInterface::send()
{
    QByteArray *array=new QByteArray;
    array->clear();
    if(status==start){
        for(int i=0;i<54;i++){
            array->append(QString::number(shuffledCards[i]->id)+"\n");
        }
        array->append(QString::number(nextPlayer)+"\n");
        web->socketA->write(array->data());
        web->socketB->write(array->data());
    }
    else if (status==call) {
        array->append(QString::number(player->decision)+"\n");
        nextPlayer=(nextPlayer+1)%3;
        if(nextPlayer==firstPlayer)status=declare;
        array->append(QString::number(nextPlayer)+"\n");
        web->socketA->write(array->data());
        web->socketB->write(array->data());
    }
    else if (status==declare) {
        array->append(QString::number(player->role)+"\n");
        array->append(QString::number(playerA->role)+"\n");
        web->socketA->write(array->data());
        array->clear();
        array->append(QString::number(player->role)+"\n");
        array->append(QString::number(playerB->role)+"\n");
        web->socketB->write(array->data());
        status=play;
    }
    else if (status==play) {
        nextPlayer=(nextPlayer+1)%3;
        array->append(QString::number(nextPlayer)+"\n");
        array->append(QString::number(mustPlay)+"\n");
        array->append(QString::number(player->decision)+"\n");
        if(player->decision==Player::act){
            array->append(QString::number(player->cardsNum)+"\n");
            array->append(QString::number(onTableCards.length())+"\n");
            for(int i=0;i<onTableCards.length();i++)
                array->append(QString::number(onTableCards[i]->id)+"\n");
        }
        web->socketA->write(array->data());
        web->socketB->write(array->data());
    }
    else if (status==over) {
        array->append(QString::number(player->role)+"\n");
        web->socketA->write(array->data());
        web->socketB->write(array->data());
    }
    else if (status==again) {
        array->append(QString::number(status)+"\n");
        web->socketA->write(array->data());
        web->socketB->write(array->data());
    }
}

void MainInterface::receive()
{
    QTcpSocket* tmpSocket=qobject_cast<QTcpSocket*>(sender());
    QVector<QString> str;
    if(status==start){
        while (tmpSocket->canReadLine()) {
            str.append(tmpSocket->readLine());
        }
        for (int i=0;i<54;i++) {
            int tmpId=str[i].toInt();
            for(int i=0;i<54;i++){
                if(tmpId==cards[i]->id){
                    shuffledCards.append(cards[i]);
                }
            }
        }
        if(web->Id==web->B)
            for(int i=17;i<34;i++)
                player->handCards.append(shuffledCards[i]);
        else if(web->Id==web->C)
            for(int i=34;i<51;i++)
                player->handCards.append(shuffledCards[i]);
        displayRestThree();
        displayPlayerCards();
        nextPlayer=str[54].toInt();
        firstPlayer=nextPlayer;
        status=call;
        callLord();
    }
    else if (status==call) {
        while(tmpSocket->canReadLine())
            str.append(tmpSocket->readLine());
        int tmpDecision=str[0].toInt();
        nextPlayer=str[1].toInt();
        if (tmpSocket==web->socketA){
            playerA->decision=tmpDecision;
            if (playerB->decision==-1)AFirst=true;//若B状态仍未变说明A先到
            if (tmpDecision==Player::pass)
                ui->labelAInfo->setText(tr("不叫"));
            else if (tmpDecision==Player::act) {
                ui->labelAInfo->setText(tr("叫地主"));
            }
        }
        else if (tmpSocket==web->socketB){
            playerB->decision=tmpDecision;
            if (tmpDecision==Player::pass)
                ui->labelBInfo->setText(tr("不叫"));
            else if (tmpDecision==Player::act) {
                ui->labelBInfo->setText(tr("叫地主"));
            }
        }
        if (firstPlayer==nextPlayer&&player->decision!=-1&&playerA->decision!=-1&&playerB->decision!=-1){
            if(web->Id==firstPlayer)
                declareLord();
            else status=declare;
        }
        else callLord();
    }
    else if (status==declare){
        while(tmpSocket->canReadLine())
            str.append(tmpSocket->readLine());
        //qDebug()<<str[0];
        //qDebug()<<str[1];
        if(tmpSocket==web->socketA){
            if(str[0].toInt()==Player::lord)
                playerAIsLord();
            else if (str[1].toInt()==Player::lord) {
                playerIsLord();
            }
            else {
                playerBIsLord();
            }
        }
        else if (tmpSocket==web->socketB) {
            if(str[0].toInt()==Player::lord)
                playerBIsLord();
            else if (str[1].toInt()==Player::lord) {
                playerIsLord();
            }
            else {
                playerAIsLord();
            }
        }
        startPlay();
    }
    else if (status==play) {
        while (tmpSocket->canReadLine()) {
            str.append(tmpSocket->readLine());
        }
        nextPlayer=str[0].toInt();
        mustPlay=str[1].toInt();
        if(tmpSocket==web->socketA){
            playerA->decision=str[2].toInt();
            if(playerA->decision==Player::act){
                ui->labelANum->setNum(str[3].toInt());
                ui->labelInfo->clear();
                ui->labelAInfo->clear();
                ui->labelBInfo->clear();
            }
            else if (playerA->decision==Player::pass) {
                ui->labelAInfo->setText(tr("不出"));
            }
        }
        else if(tmpSocket==web->socketB){
            playerB->decision=str[2].toInt();
            if(playerB->decision==Player::act){
                ui->labelBNum->setNum(str[3].toInt());
                ui->labelInfo->clear();
                ui->labelAInfo->clear();
                ui->labelBInfo->clear();
            }
            else if (playerB->decision==Player::pass) {
                ui->labelBInfo->setText(tr("不出"));
            }
        }
        if(str[2].toInt()==Player::act){
            for(int i=0;i<onTableCards.length();i++)
                onTableCards[i]->hide();
            onTableCards.clear();
            int tmpNum=str[4].toInt();
            for(int i=0;i<tmpNum;i++){
                for(int j=0;j<cards.length();j++)
                    if(str[i+5].toInt()==cards[j]->id)
                        onTableCards.append(cards[j]);
            }
            displayOnTableCards();
            if(str[3].toInt()==0){
                status=over;
                if(str.length()>onTableCards.length()+5){//粘包
                    gameOver();
                    //qDebug()<<str.last().toInt();
                    if(str.last().toInt()==Player::lord)QMessageBox::information(this,"游戏结束","地主赢了！");
                    else if(str.last().toInt()==Player::farmer)QMessageBox::information(this,"游戏结束","农民赢了！");
                }
                return;
            }
        }
        playCard();
    }
    else if (status==over) {
        while (tmpSocket->canReadLine()) {
            str.append(tmpSocket->readLine());
        }
        if(str[0].toInt()==again){
            replayFlag+=1;
            if(replayFlag==3)initGame();
        }
        else {
            gameOver();
            if(str[0].toInt()==Player::lord)QMessageBox::information(this,"游戏结束","地主赢了！");
            else if(str[0].toInt()==Player::farmer)QMessageBox::information(this,"游戏结束","农民赢了！");
        }
    }
    else if (status == again) {
        while (tmpSocket->canReadLine()) {
            str.append(tmpSocket->readLine());
        }
        if(str[0].toInt()==again){
            replayFlag+=1;
            if(replayFlag==3)initGame();
        }
    }
}

void MainInterface::initGame()
{
    status=start;
    ui->labelAName->setText(tr("玩家"));
    ui->labelBName->setText(tr("玩家"));
    ui->labelName->setText(tr("玩家"));
    ui->labelAInfo->clear();
    ui->labelBInfo->clear();
    ui->labelInfo->clear();
    ui->labelANum->setNum(17);
    ui->labelBNum->setNum(17);
    ui->btnLeft->hide();
    ui->btnRight->hide();
    for(int i=0;i<cards.length();i++)
        delete  cards[i];
    cards.clear();
    shuffledCards.clear();
    restThree.clear();
    onTableCards.clear();
    for(int i=0;i<restThreeDisplay.length();i++)
        delete restThreeDisplay[i];
    restThreeDisplay.clear();
    playerA=new Player;
    playerB=new Player;
    player=new Player;

    for(int i=CardPoint::Card_3;i<=CardPoint::Card_2;i++)
        for(int j=CardSuit::Diamond;j<=CardSuit::Spade;j++)
        {
            Card *item=new Card(this);
            item->setInfo(j,i);
            item->setPic();
            item->hide();
            cards.append(item);
        }

    Card *item=new Card(this);
    item->setInfo(CardSuit::Suit_End,CardPoint::Card_BJ);
    item->setPic();
    item->hide();
    cards.append(item);

    item = new Card(this);
    item->setInfo(CardSuit::Suit_End,CardPoint::Card_RJ);
    item->setPic();
    item->hide();
    cards.append(item);
    if(web->Id==web->A){
        QVector<Card*> tmpCards;
        for(int i=0;i<54;i++)
            tmpCards.append(cards[i]);
        for (int i=54;i>0;i--) {
            qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
            int rand = qrand()%i;
            shuffledCards.append(tmpCards.at(rand));
            tmpCards.remove(rand);
        }
        for (int i=0;i<17;i++)
            player->handCards.append(shuffledCards[i]);
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        nextPlayer = qrand()%3;
        //nextPlayer = web->A;
        firstPlayer = nextPlayer;
        send();
        displayRestThree();
        displayPlayerCards();
        status=call;
        callLord();
    }
}

void MainInterface::displayPlayerCards()
{
    player->sortCards(player->handCards);
    for(int i=0;i<player->handCards.length();i++)
    {
        player->handCards[i]->move(200+i*30,510);
        player->handCards[i]->raise();
        player->handCards[i]->show();
    }
}

void MainInterface::displayRestThree()
{
    if(status==start)
        for(int i=0;i<3;i++){
            restThree.append(shuffledCards[i+51]);
            Card* item = new Card(this);
            item->setInfo(shuffledCards[i+51]->cardSuit,shuffledCards[i+51]->cardPoint);
            restThreeDisplay.append(item);
            restThreeDisplay[i]->is_frontSide=false;
            restThreeDisplay[i]->setPic();
            restThreeDisplay[i]->move(375+i*87,20);
            restThreeDisplay[i]->raise();
            restThreeDisplay[i]->show();
        }
}

void MainInterface::displayOnTableCards()
{
    player->sortCards(onTableCards);
    int Left = (this->width()-77-(onTableCards.length()-1)*30)/2;
    for(int i=0;i<onTableCards.length();i++){
        onTableCards[i]->move(Left+i*30,180);
        onTableCards[i]->raise();
        onTableCards[i]->show();
    }
}

void MainInterface::callLord()
{
    if(nextPlayer==web->Id){
        ui->btnLeft->setText(tr("不叫"));
        ui->btnRight->setText(tr("叫地主"));
        ui->btnLeft->show();
        ui->btnRight->show();
    }
}

void MainInterface::declareLord()
{
    status=declare;

    if (AFirst) {
        if(playerB->decision==Player::act){
            playerBIsLord();
        }
        else if (playerA->decision==Player::act) {
            playerAIsLord();
        }
        else {
            playerIsLord();
        }
    }
    else if (!AFirst) {
        if(playerA->decision==Player::act){
            playerAIsLord();
        }
        else if (playerB->decision==Player::act) {
            playerBIsLord();
        }
        else {
            playerIsLord();
        }
    }

    send();
    startPlay();
}

void MainInterface::playerIsLord()
{
    player->role=Player::lord;
    playerA->role=Player::farmer;
    playerB->role=Player::farmer;
    ui->labelName->setText(tr("地主"));
    ui->labelAName->setText(tr("农民"));
    ui->labelBName->setText(tr("农民"));
    for(int i=0;i<3;i++){
        player->handCards.append(restThree[i]);
    }
    firstPlayer=web->Id;
    nextPlayer=web->Id;
    player->cardsNum=player->handCards.length();
    displayPlayerCards();
    mustPlay=2;//地主有牌权
}

void MainInterface::playerAIsLord()
{
    playerA->role=Player::lord;
    player->role=Player::farmer;
    playerB->role=Player::farmer;
    ui->labelAName->setText(tr("地主"));
    ui->labelName->setText(tr("农民"));
    ui->labelBName->setText(tr("农民"));
    playerA->cardsNum+=3;
}

void MainInterface::playerBIsLord()
{
    playerB->role=Player::lord;
    playerA->role=Player::farmer;
    player->role=Player::farmer;
    ui->labelBName->setText(tr("地主"));
    ui->labelAName->setText(tr("农民"));
    ui->labelName->setText(tr("农民"));
    playerB->cardsNum+=3;
}

void MainInterface::startPlay()
{
    ui->labelAInfo->clear();
    ui->labelBInfo->clear();
    ui->labelInfo->clear();

    for (int i=0;i<3;i++) {
        restThreeDisplay[i]->is_frontSide=true;
        restThreeDisplay[i]->setPic();
    }

    ui->labelANum->setNum(playerA->cardsNum);
    ui->labelBNum->setNum(playerB->cardsNum);

    player->decision=-1;
    playerA->decision=-1;
    playerB->decision=-1;

    for(int i=0;i<player->handCards.length();i++)
        connect(player->handCards[i],SIGNAL(mouseClicked(Qt::MouseButton)),this,SLOT(on_mouseClicked(Qt::MouseButton)));
    status = play;
    if(player->role==Player::lord)playCard();
}

void MainInterface::playCard()
{
    if(nextPlayer==web->Id){
        ui->labelInfo->clear();
        ui->btnLeft->setText(tr("不出"));
        ui->btnRight->setText(tr("出牌"));
        ui->btnLeft->show();
        ui->btnRight->show();
        if(mustPlay==2)ui->btnLeft->setEnabled(false);
    }
}

void MainInterface::gameOver()
{
    if(status==over){
        ui->btnLeft->setEnabled(true);
        ui->btnLeft->setText(tr("再来一局"));
        ui->btnRight->setText(tr("退出"));
        ui->btnLeft->show();
        ui->btnRight->show();
    }
}

void MainInterface::on_mouseClicked(Qt::MouseButton button)
{
    if(status==play&&nextPlayer==web->Id){
        if(button==Qt::LeftButton){
            Card *tmp =qobject_cast<Card*>(sender());
            if(!tmp->is_selected){
                player->selectedCards.append(tmp);
                tmp->move(tmp->x(),tmp->y()-10);
                tmp->is_selected=true;
            }
            else if (tmp->is_selected) {
                tmp->move(tmp->x(),tmp->y()+10);
                tmp->is_selected=false;
                for(int i=0;i<player->selectedCards.length();i++){
                    if(tmp==player->selectedCards[i]){
                        player->selectedCards.remove(i);
                    }
                }
            }
        }
        else if (button==Qt::RightButton) {
            on_btnRight_clicked();
        }
    }
}

void MainInterface::on_btnLeft_clicked()
{
    if(status==call){
        player->decision=Player::pass;

        ui->btnLeft->hide();
        ui->btnRight->hide();
        send();
        ui->labelInfo->setText(tr("不叫"));
    }
    if(status==play){
        player->decision=Player::pass;
        ui->labelInfo->setText(tr("不出"));
        mustPlay+=1;
        ui->btnLeft->hide();
        ui->btnRight->hide();
        send();
    }
    if(status==over){
        replayFlag+=1;
        status=again;
        ui->labelInfo->setText(tr("正在重连"));

        loop =new QEventLoop;
        send();
        timer->start(20);
        loop->exec();
        if(replayFlag==3)initGame();
    }
}

void MainInterface::on_btnRight_clicked()
{
    if(status==call){
        player->decision=Player::act;

        ui->btnLeft->hide();
        ui->btnRight->hide();
        send();
        ui->labelInfo->setText(tr("叫地主"));
    }
    else if(status==play){
        if(player->selectedCards.isEmpty())return;
        GameRule selected(player->selectedCards);//注意牌权问题，若有牌权则onTabelCards不影响
        GameRule onTable(onTableCards);
        if(selected.is_legal&&(mustPlay==2||selected.canBeat(onTable))){//
            ui->labelAInfo->clear();
            ui->labelBInfo->clear();

            player->decision=Player::act;
            for(int i=0;i<onTableCards.length();i++)
                onTableCards[i]->hide();
            onTableCards.clear();
            for(int i=0;i<player->selectedCards.length();i++)
                onTableCards.append(player->selectedCards[i]);
            for(int i=0;i<onTableCards.length();i++)
                disconnect(onTableCards[i],SIGNAL(mouseClicked(Qt::MouseButton)),this,SLOT(on_mouseClicked(Qt::MouseButton)));


            for(int i=0;i<player->selectedCards.length();i++)
                for(int j=0;j<player->handCards.length();j++)
                    if(player->handCards[j]==player->selectedCards[i]){
                        player->handCards.remove(j);
                        break;
                    }
            player->cardsNum=player->handCards.length();
            player->selectedCards.clear();
            mustPlay=0;//如果出牌成功就取消牌权
            displayOnTableCards();
            displayPlayerCards();
            ui->btnLeft->hide();
            ui->btnLeft->setEnabled(true);
            ui->btnRight->hide();
            send();
            if(player->handCards.isEmpty()){
                status=over;
                send();
                gameOver();
                QMessageBox::information(this,tr("游戏结束"),tr("恭喜你，你赢了！"));
            }
        }
        else {
            if(!selected.is_legal)QMessageBox::information(this,tr("游戏提示"),tr("出牌不合法！"));
            else QMessageBox::information(this,tr("游戏提示"),tr("压不住上家！"));
            for(int i=0;i<player->selectedCards.length();i++){
                player->selectedCards[i]->move(player->selectedCards[i]->x(),player->selectedCards[i]->y()+10);
                player->selectedCards[i]->is_selected=false;
            }
            player->selectedCards.clear();
        }
    }
    else if (status==over) {
        web->socketA->disconnectFromHost();
        web->socketB->disconnectFromHost();
        MainInterface::close();
    }
}

void MainInterface::on_timeout()
{
    timer->stop();
    loop->exit();
}
