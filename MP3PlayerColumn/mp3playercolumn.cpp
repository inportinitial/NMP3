#include "mp3playercolumn.h"
#include "ui_mp3playercolumn.h"

#include<QPalette>
#include<QTimer>
#include<QAudioFormat>
#include<QMediaDevices>
#include<QAudioDevice>
#include<functional>
#include<QSlider>
#include<list>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>

#include"volumnslider.h"

using std::list;

#define THIS MP3PlayerColumn

MP3PlayerColumn::MP3PlayerColumn(QWidget* parent)
    : QWidget(parent)
    ,ui(new Ui::MP3PlayerColumn)
    ,mp4_player(new QMediaPlayer(this))
    ,video_widget(new QVideoWidget(this))
    ,audio_outputer(new QAudioOutput(this))
    ,lrc_player(new LyricsPlayer(this))
{
    ui->setupUi(this);
    video_widget->setVisible(0);
    __init__();
    lrc_player->show();
}

MP3PlayerColumn::~MP3PlayerColumn(){
    delete ui;
}

void MP3PlayerColumn::on_PreSong_clicked()
{
    this->PlayPreSong();
}

void THIS::__init__() {
    __InitConnections();
    __SetObject();
}

void THIS::__InitConnections() {
    connect(mp4_player,&QMediaPlayer::positionChanged,this,[this](){
        emit SongPlayingProgressChanged();
    });
    connect(mp4_player,&QMediaPlayer::sourceChanged,this,[this](){
        emit NowPlayingSongChanged();
    });
    connect(mp4_player,&QMediaPlayer::playingChanged,this,[this](){
        if(mp4_player->isPlaying())ui->PauseOrPlay->setIcon(QPixmap(":/images/pause.png"));
        else ui->PauseOrPlay->setIcon(QPixmap(":/images/play.png"));
    });
    connect(this,&THIS::NowPlayingSongChanged,this,[this](){
        QFileInfo f(mp4_player->source().toString());
        QString file_name = f.fileName();
        ui->SongNameLabel->setText(file_name.sliced(0,file_name.lastIndexOf(".")));
    });
    connect(this, &THIS::SongPlayingProgressChanged, this, [&]() {
        auto num2time = [](int num) -> QString {
        QString ans;
        for(int i=0;i<3;i++){
            num /= 10;
        }
        ans = QString::number(num % 60).rightJustified(2,'0') + ans;
        num /= 60;
        ans = ":"+ans;
        ans = QString::number(num%60)+ans;
        return ans;
        };
        int position = mp4_player->position();
        int duration = mp4_player->duration();
        ui->ProgressLabel->setText(num2time(position) + " / " +
                                   num2time(duration));
    });
    //绑定歌曲进度条
    connect(this,&THIS::SongPlayingProgressChanged,this,[&](){
        if(!ui->ProgressBar->isSliderDown()){
            ui->ProgressBar->setValue(1.0 * mp4_player->position() /
                                      mp4_player->duration() *
                                      ui->ProgressBar->maximum());
        }
    });
    //当播放轨道变的时候，更换过去
    auto media = new QMediaDevices(this);
    connect(media, &QMediaDevices::audioOutputsChanged, this, [media, this]() {
        this->Pause();
        audio_outputer->setDevice(media->defaultAudioOutput());
        this->Play();
    });
    connect(this->audio_outputer, &QAudioOutput::deviceChanged, this, [&]() {
    });
    //当这一首播放完后，播放下一首
    connect(mp4_player,&QMediaPlayer::mediaStatusChanged,this,[this](){
        if(mp4_player->mediaStatus() == QMediaPlayer::EndOfMedia){
            this->PlayNextSong();
        }
    });
    //
    connect(mp4_player,&QMediaPlayer::positionChanged,this,[&](){
        lrc_player->SyncLyrics(mp4_player->position());
    });
    //
    connect(mp4_player,&QMediaPlayer::sourceChanged,this,[&](){
        QString str = mp4_player->source().toString();
        QFileInfo info(str);
        lrc_player->SetLyricsFilePath(info.absoluteFilePath());
    });
}

int THIS::Position(){
    return mp4_player->position();
}

float THIS::__GetLastPlayingVolume() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "nmp3");
    db.setDatabaseName("nmp3.db");
    db.open();

    float volume = 1;
    QSqlQuery q(db);
    QString command = "SELECT volume FROM basic_data WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id", "nmp3");
    if (!q.exec()) {
        qDebug() << "command:" << q.lastQuery();
        qDebug() << "error:" << q.lastError();
    } else {
        while (q.next()) {
            volume = q.value("volume").toDouble();
        }
    }

    q.exec("SELECT * FROM basic_data");
    db.close();
    return volume;
}

void THIS::__SetObject(){
    video_widget->setAutoFillBackground(1);
    video_widget->setStyleSheet("background-color : white;");
    audio_outputer->setVolume(__GetLastPlayingVolume());
    mp4_player->setVideoOutput(video_widget);
    mp4_player->setAudioOutput(audio_outputer);
}

void THIS::_Play(){
    mp4_player->play();
}

void THIS::_Pause(){
    mp4_player->pause();
}

void THIS::Pause(){
    _Pause();
}

void THIS::Play(){
    if(mp4_player->source().isEmpty() && playing_song_path_list_.size()){
        _LoadSong(*playing_song_path_list_.begin());
    }
    _Play();
}

void THIS::_LoadSong(const QString& song_file_path){
    mp4_player->pause();
    __LoadSong(song_file_path);
    Play();
}

void THIS::__LoadSong(const QString& song_file_path){
    mp4_player->setSource(song_file_path);
}

QString THIS::NowPlayingSongPath(){
    return QFileInfo(mp4_player->source().toLocalFile()).absoluteFilePath();
}

void THIS::RemoveSongFromPlayingList(const QString& song_path){
    QFileInfo f(song_path);
    for(auto t = playing_song_path_list_.begin();t != playing_song_path_list_.end();t++){
        QFileInfo f2(*t);
        if(f.absoluteFilePath() == f2.absoluteFilePath()){
            playing_song_path_list_.erase(t);
            break;
            emit PlayingSongListChanged();
        }
    }
}

void THIS::PlayNextSong(){
    //不知道source()函数是不是写错了，明明返回的是url，却是文件路径的形式？
    //所以这里不需要转化toLocalFile，反而直接toString就行了
    QFileInfo f(mp4_player->source().toString());
    for(auto t = playing_song_path_list_.begin();t != playing_song_path_list_.end();t++){
        QFileInfo f2(*t);
        if(f2.absoluteFilePath() == f.absoluteFilePath()){
            ++t;
            if(t != playing_song_path_list_.end()){
                _LoadSong(*t);
            }
            break;
        }
    }
}

void THIS::PlayPreSong(){
    //不知道source()函数是不是写错了，明明返回的是url，却是文件路径的形式？
    //所以这里不需要转化toLocalFile，反而直接toString就行了
    QFileInfo f(mp4_player->source().toString());
    for(auto t = playing_song_path_list_.begin();t != playing_song_path_list_.end();t++){
        QFileInfo f2(*t);
        if(f2.absoluteFilePath() == f.absoluteFilePath()){
            if(t != playing_song_path_list_.begin()){
                _LoadSong(*(--t));
            }
            break;
        }
    }
}

void MP3PlayerColumn::on_NextSong_clicked()
{
    this->PlayNextSong();
}

void MP3PlayerColumn::on_PauseOrPlay_clicked()
{
    if(mp4_player->isPlaying()){
        Pause();
    }else{
        Play();
    }
}

void THIS::AppendSongsToPlayingList(std::list<QString> list){
    int f = 0;
    for (const auto &t : list) {
        f+=_AppendSongToPlayingListWithoutEmitListChanged(t);
    }
    if(f)emit PlayingSongListChanged();
}

void THIS::AppendSongToPlayingList(const QString& song_path){
    if(_AppendSongToPlayingListWithoutEmitListChanged(song_path)){
        emit PlayingSongListChanged();
    }
}

bool THIS::_AppendSongToPlayingListWithoutEmitListChanged(const QString& song_path){
    QFileInfo f(song_path);
    bool flag = 0;
    for(auto t = playing_song_path_list_.begin();t != playing_song_path_list_.end();t++){
        QFileInfo f2(*t);
        if(f.absoluteFilePath() == f2.absoluteFilePath()){
            flag = 1;
        }
    }
    if(!flag && f.isFile() &&
            (f.suffix() == "mp3" || f.suffix() == "mp4" || f.suffix() == "wav")){
        playing_song_path_list_.push_back(song_path);
        return 1;
    }
    return 0;
}

void MP3PlayerColumn::on_ProgressBar_sliderReleased()
{
    int position = ui->ProgressBar->value();
    int ans = 1.0*position/ui->ProgressBar->maximum()*mp4_player->duration();
    mp4_player->setPosition(ans);
}


void MP3PlayerColumn::on_AdjustVolume_clicked()
{
    VolumnSlider *vs = new VolumnSlider();
    vs->show();
    vs->SetVolumn(mp4_player->audioOutput()->volume());
    connect(vs,&VolumnSlider::VolumnChanged,this,[&,vs,this](){
        this->ChangeVolumn(vs->GetVolumn());
    });
}

void THIS::ChangeVolumn(float v){
    mp4_player->audioOutput()->setVolume(v);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","nmp3");
    db.setDatabaseName("nmp3.db");
    if(!db.open()){
        qDebug()<<"open db failed";
        return;
    }

    QSqlQuery q(db);
    QString command;

    command = "UPDATE basic_data SET volume = :volume WHERE id = :id";
    q.prepare(command);
    q.bindValue(":volume",v);
    q.bindValue(":id","nmp3");
    if(!q.exec()){
        qDebug()<<"command:"<<q.lastQuery();
        qDebug()<<"error:"<<q.lastError();
    }
    db.commit();
    db.close();
    QSqlDatabase::removeDatabase("nmp3");
}

std::list<QString> THIS::NowPlayingSongPathLists(){
    return playing_song_path_list_;
}

void THIS::JumpToSong(QString song_path){
    for(auto t = playing_song_path_list_.begin();t!=playing_song_path_list_.end();t++){
        if(*t == song_path){
            _LoadSong(*t);
        }
    }
}

QVideoWidget* THIS::GetVideoWidget(){
    return video_widget;
}
