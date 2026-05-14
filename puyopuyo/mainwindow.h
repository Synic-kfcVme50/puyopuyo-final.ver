#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QPainter>
#include<QKeyEvent>
#include<vector>
#include<string>

using namespace std;

const int ROW=15;
const int COL=7;
const int CELL=45;

enum PuyoColor{
    EMPTY=0,
    RED,
    GREEN,
    BLUE,
    YELLOW
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

private:
    int map[ROW][COL];
    int curX,curY;
    int color1,color2;

    QTimer *timer;

    void newPuyo();
    bool checkhere(int x,int y);
    void fixPuyo();

};

#endif // MAINWINDOW_H
