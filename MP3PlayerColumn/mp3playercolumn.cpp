#include "mp3playercolumn.h"
#include "ui_mp3playercolumn.h"

#include<QProcess>
#include<random>
#include<QApplication>
#include<algorithm>
#include<vector>
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
#include<QThread>

#include"volumnslider.h"

using std::list;
using std::vector;

#define THIS MP3PlayerColumn

MP3PlayerColumn::MP3PlayerColumn(QWidget* parent)
    : QWidget(parent)
    ,ui(new Ui::MP3PlayerColumn)
    ,mp4_player(new QMediaPlayer(this))
    ,video_widget(new NVideoWidget(nullptr,this))
    ,audio_outputer(new QAudioOutput(this))
    ,lrc_player(new LyricsPlayer)
    ,pre_song_hotkey(new QHotkey(QApplication::instance()))
    ,next_song_hotkey(new QHotkey(QApplication::instance()))
    ,play_or_pause_hotkey(new QHotkey(QApplication::instance()))
    ,show_or_hide_lrcwindow_hotkey(new QHotkey(QApplication::instance()))
{
    ui->setupUi(this);
    video_widget->setVisible(0);
    __init__();
    lrc_player->show();
}

MP3PlayerColumn::~MP3PlayerColumn(){
    lrc_player->deleteLater();
    delete ui;
}

void MP3PlayerColumn::on_PreSong_clicked()
{
    this->PlayPreSong();
    this->Play();
}

void THIS::__init__() {
    __InitConnections();
    __SetObject();
    __InitNowPlayingSong();
}

void THIS::__InitConnections() {
    __ConnectNextSongHotkey();
    __ConnectPreSongHotkey();
    __ConnectPlayOrPauseHotkey();
    __ConnectShowOrHideLrcWindowHotkey();
    connect(this,&THIS::PlayingSongListChanged,this,[&](){
        playing_song_path_list_ = this->NowPlayingSongPathLists();
    });
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
        if(f.exists()){
            QString file_name = f.fileName();
            ui->SongNameLabel->setText(
                file_name.sliced(0, file_name.lastIndexOf(".")));
        }else{
            ui->SongNameLabel->setText("歌曲名");
        }
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
            this->Play();
        }
    });
    //
    connect(mp4_player,&QMediaPlayer::positionChanged,this,[&](){
        lrc_player->SyncLyrics(mp4_player->position());
    });
    //
}

int THIS::Position(){
    return mp4_player->position();
}

float THIS::__GetLastPlayingVolume() {
    QSqlDatabase db = QSqlDatabase::database("nmp3");

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
    return volume;
}

void THIS::__SetObject(){
    video_widget->setAutoFillBackground(1);
    video_widget->setStyleSheet("background-color : white;");
    audio_outputer->setVolume(__GetLastPlayingVolume());
    mp4_player->setVideoOutput(video_widget);
    mp4_player->setAudioOutput(audio_outputer);
    playing_song_path_list_ = NowPlayingSongPathLists();
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
    QFileInfo f(song_file_path);
    if(!IfSongPlayedAbled(f.absoluteFilePath())){
        return;
    }
    if(mp4_player->isPlaying()){
        mp4_player->pause();
        __LoadSong(song_file_path);
        Play();
    }else{
        mp4_player->pause();
        __LoadSong(song_file_path);
    }
}

void THIS::__LoadSong(const QString& song_file_path){
    mp4_player->setSource(song_file_path);
    QThread::msleep(150);
    lrc_player->SetLyricsFilePath(song_file_path);
    auto db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;
    command = "UPDATE basic_data SET now_playing_songpath_before_app_closed = :path WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    q.bindValue(":path",song_file_path);
    if(!q.exec()){
        qDebug()<<"command:"<<command;
        qDebug()<<"erorr:"<<q.lastError();
    }
}

QString THIS::NowPlayingSongPath(){
    QString path = QFileInfo(mp4_player->source().toString()).absoluteFilePath();

    auto db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command =
        "UPDATE basic_data SET now_playing_songpath_begin_app_closed = :path "
        "WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    q.bindValue(":path",path);
    if(!q.exec()){
        qDebug()<<"command:"<<command;
        qDebug()<<"erorr:"<<q.lastError();
    }
    db.commit();
    return path;
}

void THIS::RemoveSongsFromPlayingList(std::list<QString>list){
    QFileInfo playing_song(mp4_player->source().toString());
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;
    //找到路径为song_path的路径
    q.exec("BEGIN TRANSACTION");
    foreach (QString song_path, list)
    {
        QFileInfo f(song_path);
        int num = -1;
        command = "SELECT num FROM now_playing_songpaths WHERE path = :path";
        q.prepare(command);
        q.bindValue(":path",f.absoluteFilePath());
        if(!q.exec()){
            qDebug()<<"command:"<<command;
            qDebug()<<"error:"<<q.lastError();
        }else{
            while(q.next()){
                num = q.value("num").toInt();
            }
        }
        if(num != -1){
            command = "DELETE FROM now_playing_songpaths WHERE num=:num";
            q.prepare(command);
            q.bindValue(":num",num);
            if (!q.exec()) {
                qDebug() << "command:" << command;
                qDebug() << "error:" << q.lastError();
            }

            int count = __GetTHISTableCount();

            for(int i=0;i<count;i++){
                command = "UPDATE now_playing_songpaths SET num=:target_num WHERE num=:now_num";
                q.prepare(command);
                q.bindValue(":now_num", num + i + 1);
                q.bindValue(":target_num", num + i);
                if (!q.exec()) {
                    qDebug() << "command:" << command;
                    qDebug() << "error:" << q.lastError();
                }
            }
            if (playing_song.absoluteFilePath() == f.absoluteFilePath()) {
                if (__GetTHISTableCount() == 1) {
                    this->_LoadSong("");
                } else {
                    this->PlayNextSong();
                }
            }
        }
    }
    q.exec("COMMIT TRANSACTION");
    emit PlayingSongListChanged();
}

void THIS::PlayNextSong(){
    //不知道source()函数是不是写错了，明明返回的是url，却是文件路径的形式？
    //所以这里不需要转化toLocalFile，反而直接toString就行了
    QFileInfo f(mp4_player->source().toString());

    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;
    int count = 0;
    command = "SELECT COUNT(*) FROM now_playing_songpaths";
    if(!q.exec(command)){
        qDebug() << "command:" << command;
        qDebug() << "error:" << q.lastError();
    }else{
        while(q.next()){
            count = q.value(0).toInt();
        }
    }
    command = "SELECT * FROM now_playing_songpaths WHERE path = :path";
    q.prepare(command);
    q.bindValue(":path",f.absoluteFilePath());
    if(!q.exec()){
        qDebug() << "command:" << command;
        qDebug() << "error:" << q.lastError();
    }else{
        int num = -1;
        while(q.next()){
            num = q.value("num").toInt();
        }
        int target_num = -1;
        if(num == count){
            target_num = 1;
        }else{
            target_num = num+1;
        }
        command = "SELECT * FROM now_playing_songpaths WHERE num = :num";
        q.prepare(command);
        q.bindValue(":num",target_num);
        if (!q.exec()) {
            qDebug() << "command:" << command;
            qDebug() << "error:" << q.lastError();
        } else {
            QString target_path;
            while (q.next()) {
                target_path = q.value("path").toString();
            }
            _LoadSong(target_path);
        }
    }
}

void THIS::PlayPreSong(){
    //不知道source()函数是不是写错了，明明返回的是url，却是文件路径的形式？
    //所以这里不需要转化toLocalFile，反而直接toString就行了
    QFileInfo f(mp4_player->source().toString());

    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;
    int count = 0;
    command = "SELECT COUNT(*) FROM now_playing_songpaths";
    if(!q.exec(command)){
        qDebug() << "command:" << command;
        qDebug() << "error:" << q.lastError();
    }else{
        while(q.next()){
            count = q.value(0).toInt();
        }
    }
    command = "SELECT * FROM now_playing_songpaths WHERE path = :path";
    q.prepare(command);
    q.bindValue(":path",f.absoluteFilePath());
    if(!q.exec()){
        qDebug() << "command:" << command;
        qDebug() << "error:" << q.lastError();
    }else{
        int num = -1;
        while(q.next()){
            num = q.value("num").toInt();
        }
        int target_num = -1;
        if(num == 1){
            target_num = count;
        }else{
            target_num = num-1;
        }
        command = "SELECT * FROM now_playing_songpaths WHERE num = :num";
        q.prepare(command);
        q.bindValue(":num",target_num);
        if (!q.exec()) {
            qDebug() << "command:" << command;
            qDebug() << "error:" << q.lastError();
        } else {
            QString target_path;
            while (q.next()) {
                target_path = q.value("path").toString();
            }
            _LoadSong(target_path);
        }
    }
}

void MP3PlayerColumn::on_NextSong_clicked()
{
    this->PlayNextSong();
    this->Play();
}

void MP3PlayerColumn::on_PauseOrPlay_clicked()
{
    if(mp4_player->isPlaying()){
        Pause();
    }else{
        Play();
    }
}

//使用二分的方法获取now_..._songpaths中一共有多少记录
int THIS::__GetTHISTableCount(){
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;

    //是否有num为num的记录
    auto check = [&](int num)->bool{
        command = "SELECT COUNT(*) FROM now_playing_songpaths WHERE num = :num";
        q.prepare(command);
        q.bindValue(":num",num);
        if(!q.exec()){
            qDebug() << "command:" << command;
            qDebug() << "error:" << q.lastError();
        }else{
            int ans = 0;
            while(q.next()){
                ans =  q.value(0).toInt();
            }
            return ans;
        }
        return 0;
    };
    //在l左边(不包含)一定是有的
    int l = 1,r=1e9;
    while(l<=r){
        int mid = (l+r)/2;
        if(check(mid)){
            l=mid+1;
        }else r= mid-1;
    }
    return l-1;
}

void THIS::AppendSongsToPlayingList(std::list<QString> list){
    int f = 0;
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    q.exec("BEGIN TRANSACTION");
    for (const auto &t : list) {
        f+=_AppendSongToPlayingListWithoutEmitListChanged(t,q);
    }
    q.exec("COMMIT TRANSACTION");
    if(f)emit PlayingSongListChanged();
}

void THIS::AppendSongToPlayingList(const QString& song_path){
    AppendSongToPlayingList({song_path});
}

bool THIS::_AppendSongToPlayingListWithoutEmitListChanged(const QString& song_path,QSqlQuery&q){
    QFileInfo f(song_path);
    if(!(f.isFile() && (f.suffix() == "mp3" || f.suffix() == "mp4" || f.suffix() == "wav")))return 0;

    QString command;

    command = "SELECT COUNT(*) FROM now_playing_songpaths WHERE path = :path";
    q.prepare(command);
    q.bindValue(":path",f.absoluteFilePath());
    if(!q.exec()){
        qDebug() << "command:" << command;
        qDebug() << "error:" << q.lastError();
    }else{
        int flag = 0;
        while(q.next()){
            flag = q.value(0).toInt();
        }
        if(!flag){
            //获取count
            int count = __GetTHISTableCount();
            //添加进数据库
            command = "INSERT INTO now_playing_songpaths (num,path) VALUES "
                      "(:num,:path)";
            q.prepare(command);
            q.bindValue(":num",count+1);
            q.bindValue(":path",f.absoluteFilePath());
            if(!q.exec()){
                qDebug() << "command:" << command;
                qDebug() << "error:" << q.lastError();
            }else{
                return 1;
            }
        }
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
    is_changing_volume_ = 1;
    VolumnSlider *vs = new VolumnSlider();
    vs->show();
    vs->SetVolumn(mp4_player->audioOutput()->volume());
    connect(vs,&VolumnSlider::VolumnChanged,this,[&,vs,this](){
        this->ChangeVolumn(vs->GetVolumn());
    });
    connect(vs,&QWidget::destroyed,this,[&](){
        is_changing_volume_ = 0;
    });
}

void THIS::ChangeVolumn(float v){
    mp4_player->audioOutput()->setVolume(v);
    QSqlDatabase db = QSqlDatabase::database("nmp3");

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
}

std::list<QString> THIS::NowPlayingSongPathLists(){
    list<QString> l;
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;

    command = "SELECT * FROM now_playing_songpaths";
    if(q.exec(command)){
        vector<std::pair<int,QString>>v;
        while(q.next()){
            v.push_back({q.value("num").toInt(),q.value("path").toString()});
        }
        std::sort(v.begin(),v.end());
        int size = v.size();
        for(int i=0;i<size;i++){
            l.push_back(v[i].second);
        }
    }else{
        qDebug()<<"command:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }

    return l;
}

void THIS::JumpToSong(QString song_path){
    for(auto t = playing_song_path_list_.begin();t!=playing_song_path_list_.end();t++){
        if(*t == song_path){
            _LoadSong(*t);
        }
    }
}

NVideoWidget* THIS::GetVideoWidget(){
    return video_widget;
}

void THIS::ClickPauseOrPlay(){
    on_PauseOrPlay_clicked();
}

void THIS::__ConnectPreSongHotkey(){
    pre_song_hotkey->setShortcut(QString()+"Shift+Ctrl+Z",1);
    connect(pre_song_hotkey,&QHotkey::activated,this,[&](){
        this->on_PreSong_clicked();
    });
}

void THIS::__ConnectNextSongHotkey(){
    next_song_hotkey->setShortcut(QString()+"Shift+Ctrl+C",1);
    connect(next_song_hotkey,&QHotkey::activated,this,[&](){
        this->on_NextSong_clicked();
    });
}

void THIS::__ConnectPlayOrPauseHotkey(){
    play_or_pause_hotkey->setShortcut(QString()+"Shift+Ctrl+X",1);
    connect(play_or_pause_hotkey,&QHotkey::activated,this,[&](){
        this->on_PauseOrPlay_clicked();
    });
}

void THIS::__ConnectShowOrHideLrcWindowHotkey(){
    show_or_hide_lrcwindow_hotkey->setShortcut(QString()+"Shift+Ctrl+A",1);
    connect(show_or_hide_lrcwindow_hotkey, &QHotkey::activated, this, [&]() {
        if (lrc_player->isVisible()) {
            lrc_player->setVisible(0);
        } else {
            lrc_player->setVisible(1);
        }
    });
}

void THIS::RemoveAllSongsFromPlayingList(){
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;

    command = "DELETE FROM now_playing_songpaths";
    if(!q.exec(command)){
        qDebug()<<"command:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }

    playing_song_path_list_ = NowPlayingSongPathLists();
}

void THIS::__InitNowPlayingSong(){
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;

    command = "SELECT now_playing_songpath_before_app_closed FROM basic_data WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");

    if(!q.exec()){
        qDebug()<<"command:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }else{
        QString song_path;
        while(q.next()){
            song_path = q.value("now_playing_songpath_before_app_closed").toString();
        }
        command = "SELECT COUNT(*) FROM now_playing_songpaths WHERE path = :path";
        q.prepare(command);
        q.bindValue(":path",song_path);
        if(!q.exec()){
            qDebug() << "command:" << command;
            qDebug() << "error:" << q.lastError();
        }else{
            int flag = 0;
            while(q.next()){
                flag = q.value(0).toInt();
            }
            if(flag){
                _LoadSong(song_path);
            }
        }
    }

}

void MP3PlayerColumn::on_RandomCurrentPlayingList_clicked()
{
    __RandomCurrentPlayingSongPathLists();
}

void THIS::__RandomCurrentPlayingSongPathLists(){

    std::vector<QString>vec;
    foreach(const QString& t,playing_song_path_list_){
        vec.push_back(t);
    }

    std::random_device rd;// 随机数种子
    std::mt19937 g(rd());   // 使用Mersenne Twister算法的随机数生成器
    // 使用std::shuffle随机排列vector中的元素
    std::shuffle(vec.begin(), vec.end(), g);

    auto now_position = this->mp4_player->position();
    auto now_song_path = this->NowPlayingSongPath();

    this->RemoveAllSongsFromPlayingList();
    std::list<QString>l;
    for(const QString& t : vec){ l.push_back(t); }
    this->AppendSongsToPlayingList(l);

    this->JumpToSong(now_song_path);
    mp4_player->setPosition(now_position);
    this->Play();

    emit PlayingSongListChanged();
}

bool THIS::is_changing_volume(){
    return is_changing_volume_;
}

void THIS::AdjustNowPlayingSongFileVolumeByPercentage(int percentage){
    QFileInfo info(this->mp4_player->source().toString());
    if(!info.exists())return;
    if(this->NowPlayingSongPathLists().size() >= 2) this->PlayNextSong();
    else mp4_player->setSource(QUrl());
    _AdjustSongFileVolumeByPercentage(info.absoluteFilePath(),percentage);
}

void THIS::AdjustNowPlayingSongFileVolumeBy_dB(int dB){
    QFileInfo info(this->mp4_player->source().toString());
    if(!info.exists())return;
    if(this->NowPlayingSongPathLists().size() >= 2) this->PlayNextSong();
    else mp4_player->setSource(QUrl());
    _AdjustSongFileVolumeBy_dB(info.absoluteFilePath(),dB);
}

void THIS::_AdjustSongFileVolumeByPercentage(const QString& song_file_path,int percentage){
    QFileInfo f(song_file_path);
    if(!f.exists())return;
    SetSongPlayedAbled(f.absoluteFilePath(),0);
    // 创建QProcess对象
    QProcess *process = new QProcess;

    QString command;
    if(f.suffix() == "mp4"){
        QStringList *list_command = new QStringList;
        //从视频文件中提取音频
        QString original_file_path = f.absoluteFilePath();
        QString output_audio_name = "audio_"+f.fileName().chopped(f.suffix().size()+1)+".mp3";
        QString final_audio_name = "result_"+f.fileName().chopped(f.suffix().size()+1)+".mp3";
        QString final_file_name = "result_"+f.fileName().chopped(f.suffix().size()+1)+".mp4";
        QStringList part_list;
        part_list <<"-i" << "\"" + original_file_path + "\""
                  << "-q:a 0" << "-map a" << "\"" + output_audio_name + "\"";
        command = "ffmpeg";
        for(const auto& t : part_list){
            command+=" "+t;
        }
        *list_command << command;
        //对音频进行音量操作
        part_list.clear();
        part_list << "-i" << "\"" + output_audio_name + "\"" << "-af"
                  << "volume=" + QString::number(percentage*1.0/100)
                  << "\"" + final_audio_name + "\"";
        command = "ffmpeg";
        for(const auto& t : part_list){
            command+=" "+t;
        }
        *list_command << command;
        //将音频结果和原视频融合
        part_list.clear();
        part_list << "-i" << "\"" + original_file_path + "\"" << "-i"
                  << "\"" + final_audio_name + "\""
                  << "-c:v copy -map 0:v:0 -map 1:a:0 -shortest"
                  << "\"" + final_file_name + "\"";
        command = "ffmpeg";
        for(const auto& t : part_list){
            command+=" "+t;
        }
        *list_command << command;
        //执行以上指令
        connect(process, &QProcess::finished, this,
                [&,f,original_file_path, output_audio_name, final_audio_name,
                                                     final_file_name,list_command,process]() {
            if (list_command->size()) {
                auto command = list_command->front();
                qDebug()<<qPrintable(command);
                list_command->pop_front();
                process->startCommand(command);
            } else {
                QFile::remove(output_audio_name);
                QFile::remove(final_audio_name);
                QFileInfo info(final_file_name);
                if(info.exists()){
                    QFile::remove(original_file_path);
                    QFile::copy(info.absoluteFilePath(),original_file_path);
                }
                QFile::remove(final_file_name);
                process->deleteLater();
                SetSongPlayedAbled(f.absoluteFilePath(),1);
                emit this->AdjustSongFileVolumeDone();
            }
        });
        auto command = list_command->front();
        list_command->pop_front();
        process->startCommand(command);
        qDebug()<<qPrintable(command);
    }else if(f.suffix() == "mp3"){
        QString original_file_path = f.absoluteFilePath();
        QString final_audio_name = "result_"+f.fileName().chopped(f.suffix().size()+1)+".mp3";
        command = QString() + "ffmpeg -i" + " " + "\"" + original_file_path +
                  "\"" + " " + "volume=" + QString::number(percentage*1.0/100) + " " +
                  "\"" + final_audio_name + "\"";
        connect(process, &QProcess::finished, this,
                [f,process, this, original_file_path, final_audio_name]() {
            QFile::remove(original_file_path);
            QFile::copy(final_audio_name, original_file_path);
            QFile::remove(final_audio_name);
            process->deleteLater();
            SetSongPlayedAbled(f.absoluteFilePath(),1);
            emit this->AdjustSongFileVolumeDone();
        });
        process->startCommand(command);
    }

    process->startCommand(command);
    connect(process,&QProcess::errorOccurred,this,[&,process](){
        qDebug()<<qPrintable(process->errorString());
    });
    connect(process,&QProcess::readyReadStandardOutput,this,[&,process](){
        qDebug()<<qPrintable(process->readAllStandardOutput());
    });
    connect(process,&QProcess::readyReadStandardError,this,[&,process](){
        qDebug()<<qPrintable(process->readAllStandardError());
    });
}

void THIS::_AdjustSongFileVolumeBy_dB(const QString& song_file_path,int dB){
    QFileInfo f(song_file_path);
    if(!f.exists())return;
    SetSongPlayedAbled(f.absoluteFilePath(),0);
    // 创建QProcess对象
    QProcess *process = new QProcess;

    QString command;
    if(f.suffix() == "mp4"){
        //从视频文件中提取音频
        QStringList *list_command = new QStringList;
        QString original_file_path = f.absoluteFilePath();
        QString output_audio_name = "audio_"+f.fileName().chopped(f.suffix().size()+1)+".mp3";
        QString final_audio_name = "result_"+f.fileName().chopped(f.suffix().size()+1)+".mp3";
        QString final_file_name = "result_"+f.fileName().chopped(f.suffix().size()+1)+".mp4";
        QStringList part_list;
        part_list <<"-i" << "\"" + original_file_path + "\""
                  << "-q:a 0" << "-map a" << "\"" + output_audio_name + "\"";
        command = "ffmpeg";
        for(const auto& t : part_list){
            command+=" "+t;
        }
        *list_command << command;
        //对音频进行音量操作
        part_list.clear();
        part_list << "-i" << "\"" + output_audio_name + "\"" << "-af"
                  << "volume=" + QString::number(dB) + "dB"
                  << "\"" + final_audio_name + "\"";
        command = "ffmpeg";
        for(const auto& t : part_list){
            command+=" "+t;
        }
        *list_command << command;
        //将音频结果和原视频融合
        part_list.clear();
        part_list << "-i" << "\"" + original_file_path + "\"" << "-i"
                  << "\"" + final_audio_name + "\""
                  << "-c:v copy -map 0:v:0 -map 1:a:0 -shortest"
                  << "\"" + final_file_name + "\"";
        command = "ffmpeg";
        for(const auto& t : part_list){
            command+=" "+t;
        }
        *list_command << command;
        //执行以上指令
        connect(process, &QProcess::finished, this,
                [&, f,original_file_path, output_audio_name, final_audio_name,
                                                     final_file_name,list_command,process]() {
            if (list_command->size()) {
                auto command = list_command->front();
                qDebug()<<qPrintable(command);
                list_command->pop_front();
                process->startCommand(command);
            } else {
                QFile::remove(output_audio_name);
                QFile::remove(final_audio_name);
                QFileInfo info(final_file_name);
                if(info.exists()){
                    QFile::remove(original_file_path);
                    QFile::copy(info.absoluteFilePath(),original_file_path);
                }
                QFile::remove(final_file_name);
                process->deleteLater();
                SetSongPlayedAbled(f.absoluteFilePath(),1);
                emit this->AdjustSongFileVolumeDone();
            }
        });
        auto command = list_command->front();
        list_command->pop_front();
        process->startCommand(command);
        qDebug()<<qPrintable(command);
    }else if(f.suffix() == "mp3"){
        QString original_file_path = f.absoluteFilePath();
        QString final_audio_name = "result_"+f.fileName().chopped(f.suffix().size()+1)+".mp3";
        command = QString() + "ffmpeg -i" + " " + "\"" + original_file_path +
                  "\"" + " " + "volume=" + QString::number(dB) + "dB" + " " +
                  "\"" + final_audio_name + "\"";
        connect(process, &QProcess::finished, this,
                [f,process, this, original_file_path, final_audio_name]() {
            QFile::remove(original_file_path);
            QFile::copy(final_audio_name, original_file_path);
            QFile::remove(final_audio_name);
            process->deleteLater();
            SetSongPlayedAbled(f.absoluteFilePath(),1);
            emit this->AdjustSongFileVolumeDone();
        });
        process->startCommand(command);
    }

    process->startCommand(command);
    connect(process,&QProcess::errorOccurred,this,[&,process](){
        qDebug()<<qPrintable(process->errorString());
    });
    connect(process,&QProcess::readyReadStandardOutput,this,[&,process](){
        qDebug()<<qPrintable(process->readAllStandardOutput());
    });
    connect(process,&QProcess::readyReadStandardError,this,[&,process](){
        qDebug()<<qPrintable(process->readAllStandardError());
    });
}

void THIS::SetSongPlayedAbled(const QString& song_file_path,bool f){
    auto& M = map_song_played_unabled_;
    if(f){
        if(M.find(song_file_path) != M.end()){
            M.erase(song_file_path);
        }
    }else{
        M[song_file_path] = 1;
    }
}

bool THIS::IfSongPlayedAbled(const QString&& song_file_path){
    auto& M = map_song_played_unabled_;
    return (M.find(song_file_path) == M.end());
}

void THIS::DeleteTheFirstSecondsOfNowPlayingSong(double second){
    QString song_path = NowPlayingSongPath();
    this->_LoadSong("");
    _DeleteTheFirstSecondsOfSong(song_path,second);
    this->_LoadSong(song_path);
}

void THIS::DeleteTheLastSecondsOfNowPlayingSong(double second){
    QString song_path = NowPlayingSongPath();
    this->_LoadSong("");
    _DeleteTheLastSecondsOfSong(song_path,second);
    this->_LoadSong(song_path);
}

void THIS::_DeleteTheFirstSecondsOfSong(const QString& song_path,double second){
    QFileInfo f(song_path);
    if(!f.exists())return;
    QString final_file_name = QString("result_")+f.fileName();
    QProcess p;
    QString command;
    command = QString() + "ffmpeg -i " + "\"" + f.absoluteFilePath() + "\"" +
              " -ss " + QString::number(second) + " -c copy " + "\""+final_file_name+"\""+" -y";
    connect(&p,&QProcess::readyReadStandardError,this,[&](){
        qDebug()<<qPrintable(p.errorString());
    });
    p.startCommand(command);
    qDebug()<<command;
    p.waitForFinished();
    QFileInfo info(final_file_name);
    if(info.exists()){
        QFile::remove(f.absoluteFilePath());
        QFile::copy(info.absoluteFilePath(),f.absoluteFilePath());
        QFile::remove(info.absoluteFilePath());
    }
}

void THIS::_DeleteTheLastSecondsOfSong(const QString& song_path,double second){
    QFileInfo f(song_path);
    if(!f.exists())return;
    QMediaPlayer m;
    m.setSource(f.absoluteFilePath());
    QEventLoop loop;
    QObject::connect(&m, &QMediaPlayer::mediaStatusChanged, [&loop](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) {
            loop.quit();
        }
    });
    loop.exec();
    int duration = m.duration();
    qDebug()<<duration;
    m.setSource(QUrl(""));
    m.deleteLater();
    QString final_file_name = QString("result_")+f.fileName();
    QProcess p;
    QString command;
    command = QString() + "ffmpeg -i " + "\"" + f.absoluteFilePath() + "\"" +
              " -t " + QString::number(1.0*duration/1000-second) + " -c copy " + "\""+final_file_name+"\""+" -y";
    connect(&p,&QProcess::readyReadStandardError,this,[&](){
        qDebug()<<qPrintable(p.errorString());
    });
    connect(&p,&QProcess::readyReadStandardOutput,this,[&](){
        qDebug()<<qPrintable(p.errorString());
    });
    qDebug()<<qPrintable(command);
    p.startCommand(command);
    p.waitForFinished();
    QFileInfo info(final_file_name);
    if(info.exists()){
        QFile::remove(f.absoluteFilePath());
        QFile::copy(info.absoluteFilePath(),f.absoluteFilePath());
        QFile::remove(info.absoluteFilePath());
    }
}
