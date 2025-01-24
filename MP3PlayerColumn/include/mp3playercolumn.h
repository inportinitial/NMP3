#ifndef MP3PLAYERCOLUMN_H
#define MP3PLAYERCOLUMN_H

#include<QWidget>
#include<QString>
#include<QFile>
#include<QDir>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QVideoWidget>
#include<list>
#include"../LyricsPlayer/lyricsplayer.h"

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
    void RemoveSongFromPlayingList(const QString& song_path);
    void Pause();
    void Play();
    QString NowPlayingSongPath();
    std::list<QString> NowPlayingSongPathLists();
    void PlayNextSong();
    void PlayPreSong();
    int Position();
    void ChangeVolumn(float v);
    void JumpToSong(QString song_path);
    QVideoWidget* GetVideoWidget();

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

private:
    std::list<QString>playing_song_path_list_;
    LyricsPlayer* lrc_player;

private:
    void _Play();
    void _Pause();
    void _LoadSong(const QString& song_file_path);
    void __LoadSong(const QString& song_file_path);

private:
    void __init__();
    void __InitConnections();
    void __SetObject();
    bool _AppendSongToPlayingListWithoutEmitListChanged(const QString& song_path);
    float __GetLastPlayingVolume();

private:
    Ui::MP3PlayerColumn *ui;
    QMediaPlayer *mp4_player = nullptr;
    QVideoWidget* video_widget = nullptr;
    QAudioOutput* audio_outputer = nullptr;
};

#endif // MP3PLAYERCOLUMN_H
