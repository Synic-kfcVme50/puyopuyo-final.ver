#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QPainter>
#include<QKeyEvent>
#include<vector>
#include<string>
#include<QCoreApplication>
#include<map>
#include<set>
#include<QDateTime>
#include<QMouseEvent>
#include <QMediaPlayer>

using namespace std;

const int ROW=12;
const int COL=6;
const int CELL_W=52;
const int CELL_H=50;

const int BOARD_X=95;
const int BOARD_Y=120;

enum PuyoColor{
    EMPTY=0,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    PURPLE
};

//新增
struct PuyoToRemove {
    int x;
    int y;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected://游戏画面
    void paintEvent(QPaintEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;

private slots:
    void autoDrop();
    void chainNext();

private:
    int map[ROW][COL];
    int curX,curY;

    //
    float drawY;
    int color1,color2;

    QTimer *timer;

    void newPuyo();
    bool checkhere(int x,int y);
    void fixPuyo();

    QTimer*chainTimer;
    bool isChaining;
    int chainStep;

    int offsetX=0;
    int offsetY=-1;

    int score=0;
    int chainCount=0;

    QPixmap puyoImg[6];

    QPixmap bg;

    struct Explosion{
        int x;
        int y;
        int frame;
        int color;
    };

    std::vector<Explosion> explosions;

    QPixmap boomImg[6][8];

    int countGroups(const std::vector<PuyoToRemove>&list);

    //（新增）连锁贴图
    QPixmap chainImg[6]; // 1~5连消，索引0不用
    bool showChainText = false;


    int currentChainText = 0;
    qint64 chainShowTime = 0;   // 记录贴图弹出时刻时间戳
    const int chainShowMs = 1200;


    int fallTimer = 0;
    int fallInterval = 30;

    void keyReleaseEvent(QKeyEvent *event) override;

    bool gameStarted=false;
    bool gameOver=false;

    QPixmap startBg;
    QPixmap endBg;

protected:
    void mousePressEvent(QMouseEvent *event) override;

};


#endif // MAINWINDOW_H
