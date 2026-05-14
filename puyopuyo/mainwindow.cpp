#include "mainwindow.h"
#include<QRandomGenerator>

MainWindow::MainWindow(QWidget*parent):QMainWindow(parent){
    setFixedSize(COL*CELL+100,ROW*CELL+50);

    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            map[i][j]=EMPTY;
        }
    }
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::autoDrop);
    timer->start(700);

    newPuyo();

}
MainWindow::~MainWindow(){

}
//新的噗哟出生
void MainWindow::newPuyo(){
    curX=COL/2;
    curY=1;
//随机颜色
    color1=QRandomGenerator::global()->bounded(4)+1;
    color2=QRandomGenerator::global()->bounded(4)+1;
}













