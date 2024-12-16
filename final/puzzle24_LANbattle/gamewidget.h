#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QTimer>
#include <QEventLoop>
#include <QWidget>
#include <QUdpSocket>
#include <QHostInfo>
#include <QtNetwork>
#include <QNetworkInterface>
#include <QFileDialog>
#include <QRandomGenerator>
#include <vector>

#include "solver.h"
#include "expression.h"
#include "frac.h"
#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWidget;
}
QT_END_NAMESPACE

class QUdpSocket;
enum MessageType{
    // 功能 =内容 发送者 -> 接收者

    // 创建/询问/加入房间
    questRoom,// 询问 是否存在房间 =QString(ID) rest -> host
    ansRoomGaming,// 回答 正在游戏中 =null host -> quest
    ansRoomNameRepeat,//回答 名称重复 =QString(questID) host -> quest
    ansRoomAccept,// 回答 同意加入房间 =QString(questID)+QStringList(ID) host -> all/quest
    // 退出/解散房间
    noticeQuitRoom,//通知 退出房间 =QString(ID) gamer -> all
    noticeKillRoom,//通知 解散房间 =null host -> all

    // 游戏相关
    noticeGameStart,// 通知 游戏开始 =null host -> gamer
    noticeNewQuestion,// 通知 新题目 =int[4](randomNums[4]) host -> gamer

    noticeCorrect,//通知 找到正确答案 =QString(ID)+QString(ans) gamer -> host
    noticeTimeOut,//通知 答题超时 =null host -> all

    noticeGameQuit,// 通知 退出游戏 =QString(ID) gamer -> all
    noticeGameEnd,// 通知 游戏结束 =null host -> gamer
};
enum Stage{
    resting,// 无活动
    findingRoom,// 寻找/创建房间中,等待响应
    waitingStart,// 已经找到/创建准备开始中
    playingGame,// 游戏中
    lookingRank,//在看排行榜
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private slots:
    void hearMessage();

    void on_solveModeFileButton_clicked();

    void on_solveModeSolveButton_clicked();

    void on_modeSelectTabWidget_tabBarClicked(int index);

    void on_offlineModeGiveupButton_clicked();

    void on_offlineModeCheckButton_clicked();

    void on_onlineModeQuitRoomButton_clicked();

    void on_onlineModeIDEdit_textChanged();

    void on_onlineModePortSelect_currentIndexChanged(int index);

    void on_onlineModeStartButton_clicked();

    void on_onlineModeQuitGameButton_clicked();

    void on_onlineModeCheckButton_clicked();

    void on_onlineModeSaveButton_clicked();

    void on_onlineModeExitButton_clicked();

    void on_modeSelectTabWidget_currentChanged(int index);



private:
    Ui::GameWidget *ui;
    QUdpSocket *udpSocket;
    Solver *solver;
    Timer *onlineTimer;
    qint16 port;
    int randomNums[4];
    int offlinePoints;
    void sendMessage(MessageType type,QString desID="");
    void getRandomNums();
    void offlinePointShowcasing();
    void offlineNumShowcaseing();

    void onlineNumShowcaseing();

    void onlineUpdataMemberTable();

    QString ID;
    void changeID();
    QStringList IDList;
    int onlinePloints;
    std::vector<int> pointList;

    bool host;
    Stage stage;
    void setStage(Stage stage);
};
#endif // GAMEWIDGET_H
