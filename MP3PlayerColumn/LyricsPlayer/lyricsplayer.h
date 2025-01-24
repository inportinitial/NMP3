#ifndef LYRICSPLAYER_H
#define LYRICSPLAYER_H

#include <QWidget>
#include<QMouseEvent>
#include<QString>
#include<QFile>
#include<vector>

class LyricsNode;

using std::vector;

namespace Ui {
class LyricsPlayer;
}

class LyricsPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit LyricsPlayer(QWidget *parent = nullptr);
    ~LyricsPlayer();
    void SetLyricsFilePath(QString file_path);
    void SyncLyrics(qint64 pos);
    void ShowDesktopLyrics(bool flag);
    void ResizeToMinSize();
    const int MIN_WIDTH = 200;
    const int MIN_HEIGHT = 60;

private:
    Ui::LyricsPlayer *ui;
    QFile lyrics_file;
    vector<LyricsNode*>node_pool;
    QPointF *pre_mouse_pos = nullptr;

private:
    void _LoadLyricsFromFile();
    LyricsNode* _CreateNewLyricsNode(qint64 time,QString str);
    void __ReadTimeANDLyrics(const QString& line,qint64 &time,QString &lyrics);

private:
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override{
        if(pre_mouse_pos){ delete pre_mouse_pos; pre_mouse_pos = nullptr; }
    }

/*=====patch for key press on show lrc window=====*/
private:
    void keyPressEvent(QKeyEvent* event)override{
        emit KeyPress(event);
    }
signals:
    void KeyPress(QKeyEvent *event);
};

#endif // LYRICSPLAYER_H
