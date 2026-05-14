#include "sound.h"

// 静态成员定义
QSoundEffect Sound::clickSound;
QSoundEffect Sound::popSound;   // 新增
QSoundEffect Sound::bgmSound;

void Sound::init()
{
    // 点击音效
    clickSound.setSource(QUrl("qrc:/new/prefix1/sound/decide.wav"));
    clickSound.setVolume(0.05f);

    // 爆炸音效
    popSound.setSource(QUrl("qrc:/new/prefix1/sound/pop.wav"));
    popSound.setVolume(0.08f);

    // 背景音乐
    bgmSound.setSource(QUrl("qrc:/new/prefix1/sound/bgm.wav"));
    bgmSound.setVolume(0.02f);
    bgmSound.setLoopCount(QSoundEffect::Infinite); // 循环
}

void Sound::playClick()
{
    if(clickSound.isPlaying())
        clickSound.stop();
    clickSound.play();
}

void Sound::playPop()
{
    if(popSound.isPlaying())
        popSound.stop(); // 避免重叠
    popSound.play();
}

void Sound::playBgm()
{
    bgmSound.play();
}

void Sound::stopBgm()
{
    bgmSound.stop();
}