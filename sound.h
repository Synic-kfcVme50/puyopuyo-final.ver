#ifndef SOUND_H
#define SOUND_H

#include <QSoundEffect>
#include <QUrl>

class Sound
{
public:
    static void init();       // 初始化音效
    static void playClick();  // 鼠标/键盘点击音
    static void playPop();    // 噗哟消除爆炸音
    static void playBgm();    // 循环播放背景音乐
    static void stopBgm();    // 停止背景音乐

private:
    static QSoundEffect clickSound;
    static QSoundEffect popSound;   // 新增爆炸音效
    static QSoundEffect bgmSound;
};

#endif