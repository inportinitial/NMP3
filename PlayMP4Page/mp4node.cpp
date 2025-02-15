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
    ui->verticalLayout->takeAt(1);
    show_message_when_handle_input_message_unabled_ = ui->show_message_when_handle_input_message_unabled_;
    show_message_when_handle_input_message_unabled_->move(0,0);
    show_message_when_handle_input_message_unabled_->hide();
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
    if(!if_handle_input_message_)return;
    if(this->underMouse() && e->button() == Qt::LeftButton){
        mp3_player_column->JumpToSong(this->song_file_path());
        mp3_player_column->Play();
    }
}

QString THIS::song_file_path(){
    return song_file_path_;
}

void MP4Node::on_RemoveSongFromLists_clicked() {
    if(!if_handle_input_message_)return;
    emit DeleteButtionClicked();
    mp3_player_column->RemoveSongsFromPlayingList({this->song_file_path()});
    this->deleteLater();
}

void MP4Node::DeleteThis(){
    on_RemoveSongFromLists_clicked();
}

void THIS::SetIfHandleInputMessage(bool f){
    if(f == if_handle_input_message_)return;
    if_handle_input_message_ = f;
    if(f){
        show_message_when_handle_input_message_unabled_->hide();
    }else{
        show_message_when_handle_input_message_unabled_->resize(this->size());
        show_message_when_handle_input_message_unabled_->move(0,0);
        show_message_when_handle_input_message_unabled_->show();
    }
}
void THIS::SetDisplayedMessageWhenHandleInputMessageUnabled(const QString& s){
    show_message_when_handle_input_message_unabled_->setText(s);
}
