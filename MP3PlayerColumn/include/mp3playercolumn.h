#ifndef MP3PLAYERCOLUMN_H
#define MP3PLAYERCOLUMN_H

#include<qhotkey.h>
#include<QWidget>
#include<QString>
#include<QFile>
#include<QDir>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QVideoWidget>
#include<list>
#include<QSqlQuery>
#include"../LyricsPlayer/lyricsplayer.h"
#include"../../NVideoWidget/nvideowidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MP3PlayerColumn;
}
QT_END_NAMESPACE

class MP3PlayerColumn : public QWidget
{
    Q_OBJECT
public:
    MP3PlayerColumn(QWidget *parent = nullptr);
    ~MP3PlayerColumn();
    void AppendSongToPlayingList(const QString& song_path);
    void AppendSongsToPlayingList(std::list<QString> list);
    void RemoveSongsFromPlayingList(std::list<QString>list);
    void RemoveAllSongsFromPlayingList();
    void Pause();
    void Play();
    QString NowPlayingSongPath();
    std::list<QString> NowPlayingSongPathLists();
    void PlayNextSong();
    void PlayPreSong();
    int Position();
    void ChangeVolumn(float v);
    void JumpToSong(QString song_path);
    NVideoWidget* GetVideoWidget();
    void ClickPauseOrPlay();
    void _LoadSong(const QString& song_file_path);

signals:
    void NowPlayingSongChanged();
    void SongPlayingProgressChanged();
    void PlayingSongListChanged();

private slots:
    void on_PreSong_clicked();

    void on_NextSong_clicked();

    void on_PauseOrPlay_clicked();

    void on_ProgressBar_sliderReleased();

    void on_AdjustVolume_clicked();

    void on_RandomCurrentPlayingList_clicked();

private:
    std::list<QString>playing_song_path_list_;
    LyricsPlayer* lrc_player;
    QHotkey *pre_song_hotkey;
    QHotkey *next_song_hotkey;
    QHotkey *play_or_pause_hotkey;
    QHotkey *show_or_hide_lrcwindow_hotkey;

private:
    void _Play();
    void _Pause();
    void __LoadSong(const QString& song_file_path);
    void __ConnectPreSongHotkey();
    void __ConnectNextSongHotkey();
    void __ConnectPlayOrPauseHotkey();
    void __ConnectShowOrHideLrcWindowHotkey();

private:
    int __GetTHISTableCount();
    void __init__();
    void __InitConnections();
    void __SetObject();
    void __InitNowPlayingSong();
    bool _AppendSongToPlayingListWithoutEmitListChanged(const QString& song_path,QSqlQuery&q);
    float __GetLastPlayingVolume();
    void __RandomCurrentPlayingSongPathLists();

private:
    Ui::MP3PlayerColumn *ui;
    QMediaPlayer *mp4_player = nullptr;
    NVideoWidget* video_widget = nullptr;
    QAudioOutput* audio_outputer = nullptr;
};

#endif // MP3PLAYERCOLUMN_H
