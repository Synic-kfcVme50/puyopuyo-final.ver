#include"mainwindow.h"
#include<QRandomGenerator>
//绘制地图和噗哟
void MainWindow::paintEvent(QPaintEvent*event){
    Q_UNUSED(event);
    QPainter p(this);

    //颜色表
    QColor colors[]={
        Qt::white,
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::yellow,
    };

    //画网格
    p.setPen(Qt::lightGray);
    for(int i=0;i<=ROW;i++){
        p.drawLine(0,i*CELL,COL*CELL,i*CELL);
    }
    for(int j=0;j<=COL;j++){
        p.drawLine(j*CELL,0,j*CELL,ROW*CELL);
    }


    //画已经存在的噗哟
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            int c=map[i][j];
            if(c!=EMPTY){//意思是 这里应该有噗哟，所以要画
                p.setBrush(colors[c]);
                p.drawEllipse(j*CELL+2,i*CELL+2,CELL-4,CELL-4);
            }
        }
    }

    //画正在落下的噗哟
    p.setBrush(colors[color1]);
    p.drawEllipse(curX*CELL+2,curY*CELL+2,CELL-4,CELL-4);
    p.setBrush(colors[color2]);
    p.drawEllipse(curX*CELL+2,(curY-1)*CELL+2,CELL-4,CELL-4);
}
