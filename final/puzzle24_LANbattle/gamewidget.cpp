#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());

    ui->modeSelectTabWidget->setCurrentIndex(0);
    ui->onlineModeStackedWidget->setCurrentIndex(0);

    udpSocket=new QUdpSocket(this);
    // solver=new Solver();
    // solver->solveAll();
    // ui->solveModeTextEdit->setPlainText(QString::number(solver->ableCnt)+" solution(s) have loaded.");

    offlinePoints=0;

    port=ui->onlineModePortSelect->currentIndex()+1145;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(hearMessage()));

    changeID();

    IDList.clear();
    pointList.clear();
    onlineTimer=new Timer(100,ui->onlineModeTimer,[this](){
        if(host)
        {
            sendMessage(noticeTimeOut);
        }
    });

    ui->onlineModeRankTabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->onlineModeMemberTabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->onlineModeRankTabel->setDisabled(true);

    setStage(resting);
}

GameWidget::~GameWidget()
{
    delete ui;
}


void GameWidget::sendMessage(MessageType type,QString desID)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out<<type;
    switch (type) {
    case questRoom:
        out<<ID;
        break;
    case ansRoomNameRepeat:
        out<<desID;
        break;
    case ansRoomAccept:
        out<<desID<<IDList;
        break;
    case noticeQuitRoom:
        out<<ID;
        break;
    case noticeNewQuestion:
        out<<randomNums[0]<<randomNums[1]<<randomNums[2]<<randomNums[3];
        break;
    case noticeCorrect:
        out<<ID<<ui->onlineModeTextEdit->toPlainText();
        break;
    case noticeGameQuit:
        out<<ID;
        break;
    default:
        break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}

void GameWidget::hearMessage()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int type;
        in>>type;

        QString mes1,mes2;
        QStringList mesList;

        switch (type) {
        case questRoom:
            in>>mes1;
            if(!host)break;
            if(stage==playingGame)
                sendMessage(ansRoomGaming);
            else if(IDList.contains(mes1))
                sendMessage(ansRoomNameRepeat,mes1);
            else if(stage==waitingStart)
                sendMessage(ansRoomAccept,mes1);
            break;
        case ansRoomGaming:
            if(stage==findingRoom)
            {
                ui->onlineModeDebugLabel->setText("当前有正在进行的游戏");
                setStage(resting);
            }
            break;
        case ansRoomNameRepeat:
            in>>mes1;
            if(mes1==ID&&stage==findingRoom)
            {
                ui->onlineModeDebugLabel->setText("ID被占用");
                setStage(resting);
            }
            break;
        case ansRoomAccept:
            in>>mes1>>mesList;
            if(mes1==ID&&stage==findingRoom)
            {
                host=false;
                IDList=mesList;
                ui->onlineModeDebugLabel->setText("成功加入房间");
                setStage(waitingStart);
            }
            if(stage==waitingStart)
            {
                IDList.append(mes1);
                onlineUpdataMemberTable();
            }
            break;
        case noticeQuitRoom:
            in>>mes1;
            if(stage==waitingStart)
            {
                if(mes1==ID)
                {
                    ui->onlineModeDebugLabel->setText("退出房间");
                    IDList.clear();
                    setStage(resting);
                }
                else
                {
                    IDList.removeAll(mes1);
                    onlineUpdataMemberTable();
                }
            }
            break;
        case noticeKillRoom:
            if(stage==waitingStart)
            {
                host=false;
                ui->onlineModeDebugLabel->setText("房间被解散了");
                IDList.clear();
                setStage(resting);
            }
            break;
        case noticeGameStart:
            if(stage==waitingStart)
            {
                ui->onlineModeTextEdit->clear();
                ui->onlineModeDebugLabel->setText("debug");
                randomNums[0]=randomNums[1]=randomNums[2]=randomNums[3]=0;
                onlineNumShowcaseing();
                setStage(playingGame);
                for(int i=0;i<IDList.length();i++)
                    pointList.push_back(0);
                onlineTimer->reset();
            }
            break;
        case noticeNewQuestion:
            if(stage==playingGame)
            {
                in>>randomNums[0]>>randomNums[1]>>randomNums[2]>>randomNums[3];
                onlineNumShowcaseing();
                ui->onlineModeTextEdit->clear();
                ui->onlineModeCheckButton->setDisabled(false);
                ui->onlineModeCheckButton->setVisible(true);
                ui->onlineModeCheckButton->setText("提交");
                onlineTimer->reset();
                onlineTimer->start();
            }
            break;
        case noticeCorrect:
            if(stage==playingGame)
            {
                in>>mes1>>mes2;
                setStage(playingGame);
                ui->onlineModeTextEdit->setPlainText(mes1+" has got 24!\n"+mes2);
                for(int i=0;i<IDList.length();i++)
                    if(mes1==IDList[i])
                    {
                        pointList[i]++;
                        break;
                    }
                onlineTimer->stop();
            }
            break;
        case noticeTimeOut:
            if(stage==playingGame)
            {
                setStage(playingGame);
                QString ans=solve(randomNums[0],randomNums[1],randomNums[2],randomNums[3]);
                ui->onlineModeTextEdit->setPlainText("TimeOut!\nNobody got the answer!\nA possible answer:"+ans);
            }
            break;
        case noticeGameQuit:
            in>>mes1;
            if(stage==playingGame)
            {
                if(mes1==ID)
                {
                    IDList.clear();
                    pointList.clear();
                    setStage(resting);
                    ui->onlineModeDebugLabel->setText("退出了游戏");
                }
                else
                {
                    // 还没确定要不要把退出的人除名,暂时不除名
                }
            }
            break;
        case noticeGameEnd:
            if(stage==playingGame)
            {
                setStage(lookingRank);
            }
            break;
        default:
            break;
        }
    }
}

void GameWidget::getRandomNums()
{
    do
    {
        for(int i=0;i<4;i++)
            randomNums[i]=QRandomGenerator::global()->bounded(1, 14);
    }
    // while("No solution"==solver->getAns(randomNums[0],randomNums[1],randomNums[2],randomNums[3]));
    while("No solution"==solve(randomNums[0],randomNums[1],randomNums[2],randomNums[3]));
}

void GameWidget::offlinePointShowcasing()
{
    ui->offlineModePointShowcase->setPlainText("得分: "+QString::number(offlinePoints));
}

void GameWidget::offlineNumShowcaseing()
{

    // 伪随机指定花色
    int hashed=(randomNums[0]*1+randomNums[1]*2+randomNums[2]*31+randomNums[3]*1847)%24;
    int repeat[4]={0,0,0,0};
    int seq[4]={0,1,2,3};
    for(int i=0;i<hashed;i++)
        std::next_permutation(seq,seq+4);
    for(int i=0;i<3;i++)
        for(int j=i+1;j<4;j++)
            if(randomNums[i]==randomNums[j])
                repeat[j]++;

    ui->offlineModePoker1->setPixmap(
        QPixmap(QString(":/img/poker/")+QChar(97+seq[(hashed+randomNums[0]+repeat[0])%4])+QString::number(randomNums[0])+QString(".png"))
            .scaled(ui->offlineModePoker1->size()));
    ui->offlineModePoker2->setPixmap(
        QPixmap(QString(":/img/poker/")+QChar(97+seq[(hashed+randomNums[1]+repeat[1])%4])+QString::number(randomNums[1])+QString(".png"))
            .scaled(ui->offlineModePoker2->size()));
    ui->offlineModePoker3->setPixmap(
        QPixmap(QString(":/img/poker/")+QChar(97+seq[(hashed+randomNums[2]+repeat[2])%4])+QString::number(randomNums[2])+QString(".png"))
            .scaled(ui->offlineModePoker3->size()));
    ui->offlineModePoker4->setPixmap(
        QPixmap(QString(":/img/poker/")+QChar(97+seq[(hashed+randomNums[3]+repeat[3])%4])+QString::number(randomNums[3])+QString(".png"))
            .scaled(ui->offlineModePoker4->size()));

    // ui->offlineModeNumShowcase1->setPlainText(QChar(97+seq[(hashed+randomNums[0]+repeat[0])%4])+QString::number(randomNums[0]));
    // ui->offlineModeNumShowcase2->setPlainText(QChar(97+seq[(hashed+randomNums[1]+repeat[1])%4])+QString::number(randomNums[1]));
    // ui->offlineModeNumShowcase3->setPlainText(QChar(97+seq[(hashed+randomNums[2]+repeat[2])%4])+QString::number(randomNums[2]));
    // ui->offlineModeNumShowcase4->setPlainText(QChar(97+seq[(hashed+randomNums[3]+repeat[3])%4])+QString::number(randomNums[3]));
}
void GameWidget::onlineNumShowcaseing()
{
    if(randomNums[0]==0)
    {
        ui->onlineModePoker1->setPixmap(QPixmap(":/img/poker/f.png").scaled(ui->onlineModePoker1->size()));
        ui->onlineModePoker2->setPixmap(QPixmap(":/img/poker/f.png").scaled(ui->onlineModePoker2->size()));
        ui->onlineModePoker3->setPixmap(QPixmap(":/img/poker/f.png").scaled(ui->onlineModePoker3->size()));
        ui->onlineModePoker4->setPixmap(QPixmap(":/img/poker/f.png").scaled(ui->onlineModePoker4->size()));
        return;
    }

    // 伪随机指定花色
    int hashed=(randomNums[0]*1+randomNums[1]*2+randomNums[2]*31+randomNums[3]*1847)%24;
    int repeat[4]={0,0,0,0};
    int seq[4]={0,1,2,3};
    for(int i=0;i<hashed;i++)
        std::next_permutation(seq,seq+4);
    for(int i=0;i<3;i++)
        for(int j=i+1;j<4;j++)
            if(randomNums[i]==randomNums[j])
                repeat[j]++;

    ui->onlineModePoker1->setPixmap(
        QPixmap(QString(":/img/poker/")+QChar(97+seq[(hashed+randomNums[0]+repeat[0])%4])+QString::number(randomNums[0])+QString(".png"))
            .scaled(ui->onlineModePoker1->size()));
    ui->onlineModePoker2->setPixmap(
        QPixmap(QString(":/img/poker/")+QChar(97+seq[(hashed+randomNums[1]+repeat[1])%4])+QString::number(randomNums[1])+QString(".png"))
            .scaled(ui->onlineModePoker2->size()));
    ui->onlineModePoker3->setPixmap(
        QPixmap(QString(":/img/poker/")+QChar(97+seq[(hashed+randomNums[2]+repeat[2])%4])+QString::number(randomNums[2])+QString(".png"))
            .scaled(ui->onlineModePoker3->size()));
    ui->onlineModePoker4->setPixmap(
        QPixmap(QString(":/img/poker/")+QChar(97+seq[(hashed+randomNums[3]+repeat[3])%4])+QString::number(randomNums[3])+QString(".png"))
            .scaled(ui->onlineModePoker4->size()));


    // ui->onlineModeNumShowcase1->setPlainText(QChar(97+seq[(hashed+randomNums[0]+repeat[0])%4])+QString::number(randomNums[0]));
    // ui->onlineModeNumShowcase2->setPlainText(QChar(97+seq[(hashed+randomNums[1]+repeat[1])%4])+QString::number(randomNums[1]));
    // ui->onlineModeNumShowcase3->setPlainText(QChar(97+seq[(hashed+randomNums[2]+repeat[2])%4])+QString::number(randomNums[2]));
    // ui->onlineModeNumShowcase4->setPlainText(QChar(97+seq[(hashed+randomNums[3]+repeat[3])%4])+QString::number(randomNums[3]));
}
void GameWidget::onlineUpdataMemberTable()
{
    ui->onlineModeMemberTabel->clearContents();
    ui->onlineModeMemberTabel->setRowCount(IDList.length());

    for(int i=0;i<IDList.length();i++)
    {
        QTableWidgetItem *IDItem=new QTableWidgetItem(IDList[i]);
        QTableWidgetItem *PointsItem=new QTableWidgetItem(QString(i==0?"房主":"用户")+QString(IDList[i]==ID?"(你)":""));
        ui->onlineModeMemberTabel->setItem(i,0,IDItem);
        ui->onlineModeMemberTabel->setItem(i,1,PointsItem);
    }
}
void GameWidget::changeID()
{
    ID=ui->onlineModeIDEdit->toPlainText().split("\n")[0];
    if(ID=="")
        ID=QString::number(QRandomGenerator::global()->bounded(1, 114514));
}

void GameWidget::setStage(Stage stage)
{
    this->stage=stage;
    switch (stage) {
    case resting:
        ui->modeSelectTabWidget->setTabEnabled(0,true);
        ui->modeSelectTabWidget->setTabEnabled(1,true);
        ui->onlineModeStartButton->setText("寻找房间");
        ui->onlineModeIDEdit->setDisabled(false);
        ui->onlineModePortSelect->setDisabled(false);
        ui->onlineModeStartButton->setDisabled(false);
        ui->onlineModeStartButton->setVisible(true);
        ui->onlineModeQuitRoomButton->setDisabled(true);
        ui->onlineModeQuitRoomButton->setVisible(false);
        ui->onlineModeStackedWidget->setCurrentIndex(0);
        ui->onlineModeMemberTabel->clearContents();
        onlineUpdataMemberTable();
        onlineTimer->reset();
        break;
    case findingRoom:
        ui->modeSelectTabWidget->setTabEnabled(0,false);
        ui->modeSelectTabWidget->setTabEnabled(1,false);
        ui->onlineModeStartButton->setText("寻找中...");
        ui->onlineModeIDEdit->setDisabled(true);
        ui->onlineModePortSelect->setDisabled(true);
        onlineUpdataMemberTable();
        onlineTimer->reset();
        break;
    case waitingStart:
        ui->onlineModeStartButton->setText("开始游戏");
        if(host)
        {
            ui->onlineModeQuitRoomButton->setText("解散房间");
        }
        else
        {
            ui->onlineModeQuitRoomButton->setText("退出房间");
            ui->onlineModeStartButton->setDisabled(true);
            ui->onlineModeStartButton->setVisible(false);
        }
        ui->onlineModeQuitRoomButton->setDisabled(false);
        ui->onlineModeQuitRoomButton->setVisible(true);

        onlineUpdataMemberTable();
        onlineTimer->reset();
        break;
    case playingGame:
        if(host)
        {
            ui->onlineModeCheckButton->setText("加载题目");
        }
        else
        {
            ui->onlineModeCheckButton->setDisabled(true);
            ui->onlineModeCheckButton->setVisible(false);
        }
        ui->onlineModeStackedWidget->setCurrentIndex(1);
        break;
    case lookingRank:
        ui->onlineModeRankTabel->clearContents();
        ui->onlineModeRankTabel->setRowCount(IDList.length());

        for(int i=0;i<IDList.length();i++)
        {
            QTableWidgetItem *IDItem=new QTableWidgetItem(IDList[i]);
            QTableWidgetItem *PointsItem=new QTableWidgetItem(QString::number(pointList[i]));
            ui->onlineModeRankTabel->setItem(i,0,IDItem);
            ui->onlineModeRankTabel->setItem(i,1,PointsItem);
        }
        ui->onlineModeStackedWidget->setCurrentIndex(2);
        onlineTimer->reset();
        break;
    default:
        break;
    }
}

void GameWidget::on_modeSelectTabWidget_tabBarClicked(int index)
{

}


void GameWidget::on_solveModeFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开文件"),"home/rank.txt",tr("TXT(*.txt)"));
    if (fileName.isEmpty())
        return;
    ui->solveModeTextEdit->clear();
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            ui->solveModeTextEdit->insertPlainText(str);
        }
        file.close();
    }
}



void GameWidget::on_solveModeSaveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("保存文件"),"",tr("TXT(*.txt)"));
    QFile file(fileName);
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QString str;
        QTextStream out(&file);
        QStringList questionList=ui->solveModeTextEdit->toPlainText().split("\n");
        QStringList answerList=ui->solveModeTextShowcase->toPlainText().split("\n");
        int count1=0,count2=0;
        for(int i=0;i<questionList.length();i++)
            if(answerList[i]==QString("error"))
            {
                continue;
            }
            else if(answerList[i]==QString("No solution"))
            {
                count2++;
                out<<"-\t"<<questionList[i]<<"\n";
            }
            else
            {
                count1++;
                count2++;
                out<<"+\t"<<questionList[i]<<"\n";
            }

        out<<count1<<"/"<<count2;
        file.close();
    }
}


void GameWidget::on_solveModeSolveButton_clicked()
{
    // qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch()<<"start";
    ui->modeSelectTabWidget->setTabEnabled(1,false);
    ui->modeSelectTabWidget->setTabEnabled(2,false);
    ui->solveModeFileButton->setDisabled(true);
    ui->solveModeSaveButton->setDisabled(true);
    ui->solveModeSolveButton->setDisabled(true);
    ui->solveModeTextEdit->setDisabled(true);

    ui->solveModeTextShowcase->clear();
    int numints[4];
    QStringList numstrs;
    int count=ui->solveModeTextEdit->toPlainText().split("\n").length();
    int solved=0;
    ui->solveModeLoading->setValue(0);
    for(QString line:ui->solveModeTextEdit->toPlainText().split("\n"))
    {
        line.replace(QRegularExpression("[Aa]"),"1");
        line.replace(QRegularExpression("[Jj]"),"11");
        line.replace(QRegularExpression("[Qq]"),"12");
        line.replace(QRegularExpression("[Kk]"),"13");
        numstrs=line.split(QRegularExpression("\\s"));
        numstrs.removeAll("");
        if(numstrs.length()!=4)
        {
            ui->solveModeTextShowcase->append("error");
            // qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch()<<"error1";
            solved++;
            ui->solveModeLoading->setValue(int(1.0*solved/count*100));
            continue;
        }
        bool err=false;
        for(int i=0;i<4;i++)
        {
            numints[i]=numstrs[i].toInt();
            if(numints[i]<=0||numints[i]>=14)
            {
                err=true;
                break;
            }
        }
        if(err)
        {
            ui->solveModeTextShowcase->append("error");
            // qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch()<<"error2";
            solved++;
            ui->solveModeLoading->setValue(int(1.0*solved/count*100));
            continue;
        }
        // QString ans=solver->getAns(numints[0],numints[1],numints[2],numints[3]);
        QString ans=solve(numints[0],numints[1],numints[2],numints[3]);
        // qDebug()<<QDateTime::currentDateTime().toMSecsSinceEpoch()<<"solved";
        ui->solveModeTextShowcase->append(ans);
        solved++;
        ui->solveModeLoading->setValue(int(1.0*solved/count*100));
    }

    ui->modeSelectTabWidget->setTabEnabled(1,true);
    ui->modeSelectTabWidget->setTabEnabled(2,true);
    ui->solveModeFileButton->setDisabled(false);
    ui->solveModeSaveButton->setDisabled(false);
    ui->solveModeSolveButton->setDisabled(false);
    ui->solveModeTextEdit->setDisabled(false);
}


void GameWidget::on_offlineModeGiveupButton_clicked()
{
    offlinePoints-=2;
    offlinePointShowcasing();
    // QString ans=solver->getAns(randomNums[0],randomNums[1],randomNums[2],randomNums[3]);
    QString ans=solve(randomNums[0],randomNums[1],randomNums[2],randomNums[3]);
    ui->offlineModeTextEdit->clear();
    ui->offlineModeTextEdit->setPlainText(ans);
}


void GameWidget::on_offlineModeCheckButton_clicked()
{
    Expression exp;
    exp.setExpression(ui->offlineModeTextEdit->toPlainText());
    exp.calculate();
    Frac res=exp.res;
    ExpressionError err=exp.err;
    if(err==noError)
    {
        if(exp.usedNumCnt<4)
            err=numberTooLessError;
        else if(exp.usedNumCnt>4)
            err=numberTooMuchError;
        else
        {
            bool used[4]={false,false,false,false};
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                    if(!used[j]&&exp.usedNums[i]==randomNums[j])
                    {
                        used[j]=true;
                        break;
                    }
            for(int j=0;j<4;j++)
                if(!used[j])
                    err=numberOutOfRangeError;
        }

    }

    if(err==noError&&res==Frac(24))
    {
        offlinePoints+=1;
        offlinePointShowcasing();
        // ui->offlineModeTextEdit->clear();

        getRandomNums();
        offlineNumShowcaseing();
    }

    ui->offlineModeTextEdit->append(QString::number(res.num)+"/"+QString::number(res.den));
    if(err!=noError)
        ui->offlineModeTextEdit->append(errorText(err));
}

void GameWidget::on_onlineModeQuitRoomButton_clicked()
{
    if(stage==waitingStart)
    {
        if(host)
            sendMessage(noticeKillRoom);
        else
            sendMessage(noticeQuitRoom);
    }
}

void GameWidget::on_onlineModeIDEdit_textChanged()
{
    changeID();
}


void GameWidget::on_onlineModePortSelect_currentIndexChanged(int index)
{
    udpSocket->disconnect();
    udpSocket->abort();
    port=index+1145;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(hearMessage()));
}


void GameWidget::on_onlineModeStartButton_clicked()
{
    // ui->onlineModeStackedWidget->setCurrentIndex(1);
    if(stage==resting)
    {
        setStage(findingRoom);
        QTimer::singleShot(100,[this](){
            if(stage==findingRoom)
            {
                // 请求超时,新建房间
                host=true;
                ui->onlineModeDebugLabel->setText("已创建房间 身份:"+QString(host?"房主":"非房主")+" ID:"+ID);
                IDList.clear();
                IDList.append(ID);
                setStage(waitingStart);
            }
        });
        sendMessage(questRoom);
    }
    if(stage==waitingStart)
        sendMessage(noticeGameStart);
}


void GameWidget::on_onlineModeQuitGameButton_clicked()
{
    if(stage==playingGame)
    {
        if(host)
        {
            host=false;
            sendMessage(noticeGameEnd);
        }
        else
        {
            sendMessage(noticeGameQuit);
        }
    }
}


void GameWidget::on_onlineModeCheckButton_clicked()
{
    if(host&&ui->onlineModeCheckButton->text()=="加载题目")
    {
        getRandomNums();
        sendMessage(noticeNewQuestion);
    }
    else
    {
        Expression exp;
        exp.setExpression(ui->onlineModeTextEdit->toPlainText());
        exp.calculate();
        Frac res=exp.res;
        ExpressionError err=exp.err;
        if(err==noError)
        {
            if(exp.usedNumCnt<4)
                err=numberTooLessError;
            else if(exp.usedNumCnt>4)
                err=numberTooMuchError;
            else
            {
                bool used[4]={false,false,false,false};
                for(int i=0;i<4;i++)
                    for(int j=0;j<4;j++)
                        if(!used[j]&&exp.usedNums[i]==randomNums[j])
                        {
                            used[j]=true;
                            break;
                        }
                for(int j=0;j<4;j++)
                    if(!used[j])
                        err=numberOutOfRangeError;
            }

        }

        if(err==noError&&res==Frac(24))
        {
            sendMessage(noticeCorrect);
        }

        ui->onlineModeTextEdit->append(QString::number(res.num)+"/"+QString::number(res.den));
        if(err!=noError)
            ui->onlineModeTextEdit->append(errorText(err));
    }

}


void GameWidget::on_onlineModeSaveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("保存文件"),"",tr("TXT(*.txt)"));
    QFile file(fileName);
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QString str;
        QTextStream out(&file);
        out<<"用户名"<<'\t'<<"得分"<<'\n';
        for(int i=0;i<IDList.length();i++)
            out<<IDList[i]<<'\t'<<pointList[i]<<'\n';
        file.close();
    }


}


void GameWidget::on_onlineModeExitButton_clicked()
{
    IDList.clear();
    pointList.clear();
    setStage(resting);
}


void GameWidget::on_modeSelectTabWidget_currentChanged(int index)
{
    setWindowIcon(QIcon(QPixmap(":/img/logo"+QString::number(index+1)+".png")));
    if(index==1)
    {
        getRandomNums();
        offlinePointShowcasing();
        offlineNumShowcaseing();
    }
}





