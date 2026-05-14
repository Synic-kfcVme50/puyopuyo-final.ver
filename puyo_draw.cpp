#include"mainwindow.h"
#include<QRandomGenerator>
//绘制地图和噗哟
void MainWindow::paintEvent(QPaintEvent*event){
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(rect(),bg);

    //puyo生成范围改这里！！！
    p.setClipRect(
        BOARD_X,
        BOARD_Y - CELL_H/18,
        COL * CELL_W,
        ROW * CELL_H + CELL_H
        );

    //颜色表（删去）


    //画网格(删去）


    //画已经存在的噗哟
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            int c=map[i][j];
            if(c != EMPTY){
                p.drawPixmap(
                    BOARD_X + j * CELL_W,//???
                    BOARD_Y + i * CELL_H,
                    CELL_W,
                    CELL_H,
                    puyoImg[c]
                    );
            }
        }
    }

    //画正在落下的噗哟
    // 只有非连锁状态才绘制当前下落噗哟(新增）
    if(!isChaining){

        p.drawPixmap(
            BOARD_X + curX * CELL_W,
            BOARD_Y + drawY * CELL_H,
            CELL_W,
            CELL_H,
            puyoImg[color1]
            );

        p.drawPixmap(
            BOARD_X + (curX + offsetX) * CELL_W,
            BOARD_Y + (drawY + offsetY) * CELL_H,
            CELL_W,
            CELL_H,
            puyoImg[color2]
            );
    }

    p.setClipping(false);

    //（新增）显示分数
    //已修改

    int scoreX = BOARD_X+20;
    int scoreY = BOARD_Y + ROW * CELL_H + 70;

    QFont scoreFont = p.font();
    scoreFont.setPointSize(50);
    scoreFont.setBold(true);
    scoreFont.setFamily("Consolas");
    p.setFont(scoreFont);

    QString scoreText = QString("%1").arg(score, 8, 10, QChar('0'));

    // 1. 绘制灰色描边（上下左右各偏移2像素）
    p.setPen(QColor(120,120,120));
    p.drawText(scoreX - 2, scoreY - 2, scoreText);
    p.drawText(scoreX - 2, scoreY + 2, scoreText);
    p.drawText(scoreX + 2, scoreY - 2, scoreText);
    p.drawText(scoreX + 4, scoreY + 4, scoreText);

    // 2. 绘制白色主体（居中覆盖）
    p.setPen(Qt::white);
    p.drawText(scoreX, scoreY, scoreText);

    // (新增）绘制爆炸动画
    for(auto &e : explosions){

        QPixmap &img = boomImg[e.color][e.frame];

        if(!img.isNull()){
            p.drawPixmap(
                BOARD_X + e.x * CELL_W,
                BOARD_Y + e.y * CELL_H,
                CELL_W,
                CELL_H,
                img
                );
        }
    }

    //（新增）播放连锁提示的贴图（放在噗哟贴图之后）
    // 连锁贴图 固定秒数显示 放在最后绘制 置顶不被遮挡
    if(showChainText)
    {
        // 计算已经显示了多久
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        // 超过设定时间，直接隐藏
        if(now - chainShowTime >= chainShowMs)
        {
            showChainText = false;
            currentChainText = 0;
        }
        else
        {
            // 没到时间就绘制
            QPixmap &img = chainImg[currentChainText];
            if(!img.isNull())
            {
                float scale = 0.045;
                int w = img.width() * scale;
                int h = img.height() * scale;

                int x = BOARD_X + (COL * CELL_W - w) / 2;
                int y = BOARD_Y + 100;

                // 可选淡出，不要可以删掉透明度两行
                p.setOpacity(1.0);
                p.drawPixmap(x, y, w, h, img);
                p.setOpacity(1.0);
            }
        }
    }

    // ===== 开始界面 =====
    if(!gameStarted)
    {
        // 开始背景
        p.drawPixmap(rect(), startBg);


        return;
    }

    // ===== 游戏结束界面 =====(精致版！）

    if(gameOver)
    {
        // 背景
        p.drawPixmap(rect(), endBg);

        // 暗化层
        p.fillRect(rect(), QColor(0,0,0,120));

        // ===== GAME OVER 标题 =====

        QFont titleFont;
        titleFont.setPointSize(38);
        titleFont.setBold(true);
        titleFont.setFamily("Consolas");

        p.setFont(titleFont);

        QString gameOverText = "GAME OVER";

        int textX = width()/2 - 125;
        int textY = 145;

        // 灰色描边
        p.setPen(QColor(120,120,120));

        p.drawText(textX - 3, textY - 3, gameOverText);
        p.drawText(textX - 3, textY + 3, gameOverText);
        p.drawText(textX + 3, textY - 3, gameOverText);
        p.drawText(textX + 3, textY + 3, gameOverText);

        // 白色主体
        p.setPen(Qt::white);

        p.drawText(textX, textY, gameOverText);

        // ===== 分数框 =====

        QRect scoreBox(width()/2 - 170, 240, 340, 110);

        p.setPen(Qt::NoPen);

        p.setBrush(QColor(255,255,255,200));

        p.drawRoundedRect(scoreBox, 25, 25);

        // Score 标题
        QFont scoreTitle;
        scoreTitle.setPointSize(16);
        scoreTitle.setBold(true);

        p.setFont(scoreTitle);

        p.setPen(QColor(80,80,80));

        p.drawText(
            QRect(scoreBox.x(),
                  scoreBox.y() + 12,
                  scoreBox.width(),
                  30),
            Qt::AlignCenter,
            "FINAL SCORE"
            );

        // 分数数字
        QFont scoreFont;
        scoreFont.setPointSize(28);
        scoreFont.setBold(true);
        scoreFont.setFamily("Consolas");

        p.setFont(scoreFont);

        p.setPen(QColor(255,180,40));

        p.drawText(
            QRect(scoreBox.x(),
                  scoreBox.y() + 38,
                  scoreBox.width(),
                  50),
            Qt::AlignCenter,
            QString::number(score)
            );

        // ===== 重新开始按钮 =====

        QRect btn(width()/2 - 140, 410, 280, 75);

        // ===== 按钮阴影 =====
        p.setPen(Qt::NoPen);

        p.setBrush(QColor(0,0,0,70));

        p.drawRoundedRect(
            btn.adjusted(0,8,0,8),
            28,
            28
            );

        // ===== 按钮渐变主体 =====
        QLinearGradient grad(
            btn.topLeft(),
            btn.bottomLeft()
            );

        // 上亮下深（果冻感）
        grad.setColorAt(0.0, QColor(255,170,170));
        grad.setColorAt(0.45, QColor(255,120,120));
        grad.setColorAt(1.0, QColor(235,70,70));

        p.setBrush(grad);

        // 外描边
        QPen border(QColor(255,255,255,180));
        border.setWidth(4);

        p.setPen(border);

        p.drawRoundedRect(btn, 28, 28);

        // ===== 顶部高光 =====
        QRect highlight = btn.adjusted(12, 8, -12, -38);

        QLinearGradient shine(
            highlight.topLeft(),
            highlight.bottomLeft()
            );

        shine.setColorAt(0.0, QColor(255,255,255,180));
        shine.setColorAt(1.0, QColor(255,255,255,30));

        p.setBrush(shine);
        p.setPen(Qt::NoPen);

        p.drawRoundedRect(highlight, 20, 20);

        // ===== 按钮文字 =====
        QFont btnFont;
        btnFont.setPointSize(22);
        btnFont.setBold(true);
        btnFont.setFamily("Arial Black");

        p.setFont(btnFont);

        // 文字描边（灰边）
        p.setPen(QColor(120,120,120));

        p.drawText(btn.adjusted(-2,-2,-2,-2),
                   Qt::AlignCenter,
                   "RESTART");

        p.drawText(btn.adjusted(2,2,2,2),
                   Qt::AlignCenter,
                   "RESTART");

        // 白字主体
        p.setPen(Qt::white);

        p.drawText(
            btn,
            Qt::AlignCenter,
            "RESTART"
            );
    }
}
