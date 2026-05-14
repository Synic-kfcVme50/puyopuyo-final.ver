#include "mainwindow.h"
#include<QRandomGenerator>
//检测可否落到此处
bool MainWindow::checkhere(int x,int y){
    if(x<0||x>=COL)
        return false;
    if(y>=ROW)
        return false;
    if(y<0)
        return true;
    if(y>=0&&map[x][y]!=EMPTY){
        return false;}
    int y2=y;

    //防止噗哟穿透？
    if(y2>=0){
        if(x<0||x>=COL||y2>=ROW)
            return false;
        if(map[y2][x]!=EMPTY)
            return false;
    }
    return true;
}
//下落程序
void MainWindow::autoDrop(){
    if(checkhere(curX,curY+1)){
        curY++;
    }
    else{
        fixPuyo();
        newPuyo();
    }
    update();//刷新界面
}
//把噗哟固定在桌面上
void MainWindow::fixPuyo(){
    map[curY][curX]=color1;
    map[curY-1][curX]=color2;
}

//实现键盘操控噗哟移动功能
void MainWindow::keyPressEvent(QKeyEvent *e){
    if(e->key()==Qt::Key_Left){
        if(checkhere(curX-1,curY))
            curX--;
    }
    if(e->key()==Qt::Key_Right){
        if(checkhere(curX+1,curY)){
            curX++;
        }
    }
    else if(e->key()==Qt::Key_Down){
        autoDrop();
    }
    //此处的“上”是交换两个噗哟的颜色!
    else if(e->key()==Qt::Key_Up){
        int t=color1;
        color1=color2;
        color2=t;
    }
    update();
}

