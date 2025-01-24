#include "mp4node.h"
#include "ui_mp4node.h"

#include<QFileInfo>
#include<QString>
#include"mp3playercolumn.h"

#define THIS MP4Node

extern MP3PlayerColumn* mp3_player_column;

MP4Node::MP4Node(QString song_file_path,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MP4Node)
{
    ui->setupUi(this);
    _SetSongFilePath(song_file_path);
}

MP4Node::~MP4Node()
{
    delete ui;
}

void THIS::_SetSongFilePath(QString song_path){
    song_file_path_ = song_path;
    QString file_name = QFileInfo(song_path).fileName();
    file_name = file_name.sliced(0,file_name.lastIndexOf('.'));
    ui->SongNameLabel->setText(file_name);
}

void THIS::mouseDoubleClickEvent(QMouseEvent* e){
    if(this->underMouse() && e->button() == Qt::LeftButton){
        mp3_player_column->JumpToSong(this->song_file_path());
    }
}

QString THIS::song_file_path(){
    return song_file_path_;
}
