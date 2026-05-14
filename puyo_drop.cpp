#include "mainwindow.h"
#include "clear.h"
#include<QRandomGenerator>
#include<QThread>
#include<QCoreApplication>
#include"sound.h"

//检测可否落到此处
bool MainWindow::checkhere(int x, int y){
    if(x < 0 || x >= COL) return false;
    if(y >= ROW) return false;
    if(y<0) return true;
    if(map[y][x] != EMPTY) return false;
    return true;
}

//下落程序
void MainWindow::autoDrop(){

    if(!gameStarted)
        return;

    if(gameOver)
        return;

    if(isChaining)
        return;

    // 每帧计时
    fallTimer++;

    // 到时间才真正下落
    if(fallTimer >= fallInterval){

        fallTimer = 0;

        if(checkhere(curX, curY + 1) &&
            checkhere(curX + offsetX,
                      curY + offsetY + 1))
        {
            curY++;
        }
        else{

            drawY = curY;

            fixPuyo();

            isChaining = true;

            chainStep = 1;

            chainNext();

            update();

            return;
        }
    }

    // 平滑追踪
    drawY += (curY - drawY) * 0.25f;

    // 防止无限抖动
    if(fabs(drawY - curY) < 0.01f)
        drawY = curY;

    update();
}

//------先把能消除的噗哟全消除-----
//AI之力

//标记阶段
std::vector<PuyoToRemove> markedForRemoval;

void MainWindow::chainNext() {
    bool hasMore = true;
    int chainCount=0;//新增！


    while(hasMore) {
        //找出所有可消除噗哟
        std::vector<PuyoToRemove> marked;

        int groupCount = scanAndMark(map, marked);

        if(marked.empty()) {
            hasMore = false;
            break;
        }

        //一次消几种颜色
        std::set<int> colors;

        for(auto &p : marked) {

            int c = map[p.y][p.x];

            if(c != EMPTY)
                colors.insert(c);
        }

        //真正连锁数（下落后再次消除）
        chainCount++;

        //显示连锁数
        //= 当前第几连锁 + 同时消除组数 - 1
        int displayChain = chainCount + groupCount - 1;

        //显示连消贴图
        if(displayChain >= 1 && displayChain <= 5) {

            showChainText = true;

            currentChainText = displayChain;

            chainShowTime = QDateTime::currentMSecsSinceEpoch();

            update();

            QCoreApplication::processEvents();
        }

        // ===== 算分 =====

        int removeCount = marked.size();

        int bonus = 0;

        // 连锁奖励
        if(chainCount == 1) bonus += 0;
        else if(chainCount == 2) bonus += 8;
        else if(chainCount == 3) bonus += 16;
        else if(chainCount == 4) bonus += 32;
        else if(chainCount == 5) bonus += 64;
        else if(chainCount == 6) bonus += 96;
        else if(chainCount == 7) bonus += 128;
        else bonus += 160;

        // 同时消除组奖励
        if(groupCount == 2) bonus += 2;
        else if(groupCount == 3) bonus += 3;
        else if(groupCount >= 4) bonus += 4;

        // 颜色种类奖励
        int colorNum = colors.size();

        if(colorNum == 2) bonus += 1;
        else if(colorNum == 3) bonus += 2;
        else if(colorNum >= 4) bonus += 3;

        if(bonus < 1)
            bonus = 1;

        int addScore = removeCount * 10 * bonus;

        score += addScore;

        update();

        update();


        // 按颜色顺序消除
        for(int c : colors) {
            std::vector<PuyoToRemove> batch;
            for(auto &p : marked) {
                if(map[p.y][p.x] == c)
                    batch.push_back(p);
            }

            // 显示消除动画
            update();
            QThread::msleep(170);

            // 真正删除这一颜色（新增爆炸动画）
            explosions.clear();

            for(auto &p : batch){

                Explosion e;

                e.x = p.x;
                e.y = p.y;

                e.frame = 0;

                // 先记录颜色
                e.color = map[p.y][p.x];

                explosions.push_back(e);
            }

            // 再统一删除
            for(auto &p : batch){
                map[p.y][p.x] = EMPTY;
            }

            Sound::playPop(); // 播放爆炸音效

            // 播放6帧爆炸动画(更新）
            for(int f = 1; f <=6; f++){

                for(auto &e : explosions)
                    e.frame = f;

                update();
                QCoreApplication::processEvents();
                if(f == 4)
                    QThread::msleep(40);
                else
                    QThread::msleep(80);//爆炸动画速度
            }


            explosions.clear();

            // 下落
            dropAll(map);
            update();
            QThread::msleep(150);
        }
    }

    isChaining = false;

    newPuyo();

    // 如果已经 Game Over
    // 立刻结束 chainNext
    if(gameOver)
    {
        update();
        return;
    }
}


//把噗哟固定在桌面上
//4.0新增另一个噗哟可能悬空的情况！
//8.0修复bug！
void MainWindow::fixPuyo()
{
    int x1 = curX;
    int y1 = curY;

    int x2 = curX + offsetX;
    int y2 = curY + offsetY;

    // ========= 竖直状态 =========
    if(offsetX == 0)
    {
        // 主在下 副在上
        if(offsetY == -1)
        {
            while(y1 + 1 < ROW &&
                   map[y1 + 1][x1] == EMPTY)
            {
                y1++;
                y2++;
            }
        }
        // 主在上 副在下
        else if(offsetY == 1)
        {
            while(y2 + 1 < ROW &&
                   map[y2 + 1][x2] == EMPTY)
            {
                y1++;
                y2++;
            }
        }
    }
    // ========= 横向状态 =========
    else
    {
        // 两边分别下落
        while(y1 + 1 < ROW &&
               map[y1 + 1][x1] == EMPTY)
        {
            y1++;
        }

        while(y2 + 1 < ROW &&
               map[y2 + 1][x2] == EMPTY)
        {
            y2++;
        }
    }

    // 放入地图
    map[y1][x1] = color1;
    map[y2][x2] = color2;
}

//实现键盘操控噗哟移动功能
void MainWindow::keyPressEvent(QKeyEvent *e){

    Sound::playClick();

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

    if(e->key() == Qt::Key_Left){
        if(checkhere(curX-1, curY) && checkhere(curX-1 + offsetX, curY + offsetY))
            curX--;
    }
    else if(e->key() == Qt::Key_Right){
        if(checkhere(curX+1, curY) && checkhere(curX+1 + offsetX, curY + offsetY))
            curX++;
    }
    else if(e->key() == Qt::Key_Down){

        fallInterval = 2;
    }
    else if(e->key() == Qt::Key_Up){
        int newOffsetX = -offsetY;
        int newOffsetY = offsetX;

        // 尝试旋转 + 蹭墙
        int kicks[3] = {0, -1, 1};
        bool rotated = false;

        for(int k = 0; k < 3; k++){
            int tryX = curX + kicks[k];
            if(checkhere(tryX, curY) && checkhere(tryX + newOffsetX, curY + newOffsetY)){
                curX = tryX;
                offsetX = newOffsetX;
                offsetY = newOffsetY;
                rotated = true;
                break;
            }
        }
    }
    update();
}

void MainWindow::keyReleaseEvent(QKeyEvent *e){

    if(e->key() == Qt::Key_Down){

        fallInterval = 30;
    }
}

//（新增）计算相连数
// 辅助函数：计算本次消除有几组相连
