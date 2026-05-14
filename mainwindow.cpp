#include "mainwindow.h"
#include<QRandomGenerator>
#include<QMessageBox>
#include"sound.h"

MainWindow::MainWindow(QWidget*parent):QMainWindow(parent){
    setFixedSize(510,870);

    startBg.load(":/new/prefix1/res/start_bg.jpg");

    puyoImg[1].load(":/new/prefix1/res/red.png");
    puyoImg[2].load(":/new/prefix1/res/puyo-green.png");
    puyoImg[3].load(":/new/prefix1/res/blue.png");
    puyoImg[4].load(":/new/prefix1/res/puyo-yellow.png");
    puyoImg[5].load(":/new/prefix1/res/puyo-purple.png");

    boomImg[1][1] = QPixmap(":/new/prefix1/res/red1.png");
    boomImg[1][2] = QPixmap(":/new/prefix1/res/red.png");
    boomImg[1][3] = QPixmap(":/new/prefix1/res/red1.png");
    boomImg[1][4] = QPixmap(":/new/prefix1/res/red2.png");
    boomImg[1][5] = QPixmap(":/new/prefix1/res/red3.png");
    boomImg[1][6] = QPixmap(":/new/prefix1/res/red4.png");
    boomImg[1][7] = QPixmap(":/new/prefix1/res/red5.png");

    boomImg[3][1] = QPixmap(":/new/prefix1/res/blue1.png");
    boomImg[3][2] = QPixmap(":/new/prefix1/res/blue.png");
    boomImg[3][3] = QPixmap(":/new/prefix1/res/blue1.png");
    boomImg[3][4] = QPixmap(":/new/prefix1/res/blue2.png");
    boomImg[3][5] = QPixmap(":/new/prefix1/res/blue3.png");
    boomImg[3][6] = QPixmap(":/new/prefix1/res/blue4.png");
    boomImg[3][7] = QPixmap(":/new/prefix1/res/blue5.png");

    boomImg[2][1] = QPixmap(":/new/prefix1/res/green1.png");
    boomImg[2][2] = QPixmap(":/new/prefix1/res/green.png");
    boomImg[2][3] = QPixmap(":/new/prefix1/res/green1.png");
    boomImg[2][4] = QPixmap(":/new/prefix1/res/green2.png");
    boomImg[2][5] = QPixmap(":/new/prefix1/res/green3.png");
    boomImg[2][6] = QPixmap(":/new/prefix1/res/green4.png");
    boomImg[2][7] = QPixmap(":/new/prefix1/res/green5.png");

    boomImg[4][1] = QPixmap(":/new/prefix1/res/yellow1.png");
    boomImg[4][2] = QPixmap(":/new/prefix1/res/yellow.png");
    boomImg[4][3] = QPixmap(":/new/prefix1/res/yellow1.png");
    boomImg[4][4] = QPixmap(":/new/prefix1/res/yellow2.png");
    boomImg[4][5] = QPixmap(":/new/prefix1/res/yellow3.png");
    boomImg[4][6] = QPixmap(":/new/prefix1/res/yellow4.png");
    boomImg[4][7] = QPixmap(":/new/prefix1/res/yellow5.png");

    boomImg[5][1] = QPixmap(":/new/prefix1/res/purple1.png");
    boomImg[5][2] = QPixmap(":/new/prefix1/res/purple.png");
    boomImg[5][3] = QPixmap(":/new/prefix1/res/purple1.png");
    boomImg[5][4] = QPixmap(":/new/prefix1/res/purple2.png");
    boomImg[5][5] = QPixmap(":/new/prefix1/res/purple3.png");
    boomImg[5][6] = QPixmap(":/new/prefix1/res/purple4.png");
    boomImg[5][7] = QPixmap(":/new/prefix1/res/purple5.png");

    chainImg[1]=QPixmap(":/new/prefix1/chain/chain1.png");
    chainImg[2]=QPixmap(":/new/prefix1/chain/chain2.png");
    chainImg[3]=QPixmap(":/new/prefix1/chain/chain3.png");
    chainImg[4]=QPixmap(":/new/prefix1/chain/chain4.png");
    chainImg[5]=QPixmap(":/new/prefix1/chain/chain5.png");

    endBg.load(":/new/prefix1/res/end_bg.jpg");



    bg.load(":/new/prefix1/res/bg2.0.png");

    qDebug() << puyoImg[1].isNull();

    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            map[i][j]=EMPTY;
        }
    }
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::autoDrop);
    timer->start(16);//已修改（8.0版本）

    //停顿计时器
    chainTimer=new QTimer(this);
    chainTimer->setSingleShot(true);
    connect(chainTimer,&QTimer::timeout,this,&MainWindow::chainNext);

    isChaining=false;

    Sound::init();
    Sound::playBgm();



    newPuyo();

}
MainWindow::~MainWindow(){

}
//新的噗哟出生
void MainWindow::newPuyo(){
    curX=COL/2;
    curY=-1;
    drawY=curY;
//随机颜色
    color1=QRandomGenerator::global()->bounded(5)+1;
    color2=QRandomGenerator::global()->bounded(5)+1;

    offsetX=0;
    offsetY=-1;//保证新噗哟都是以上下两个为一组生成的

    // 游戏结束判断
    if(map[0][curX] != EMPTY)
    {
        gameOver = true;

        timer->stop();

        update();

        return;
    }

}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    Sound::playClick();

    // 开始游戏
    if(!gameStarted)
    {
        gameStarted = true;

        gameOver = false;

        memset(map, 0, sizeof(map));

        score = 0;

        newPuyo();

        timer->start(16);

        update();

        return;
    }

    // Game Over 后点击重新开始
    if(gameOver)
    {
        gameOver = false;

        memset(map, 0, sizeof(map));

        score = 0;

        newPuyo();

        timer->start(16);

        update();
    }
}






