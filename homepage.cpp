#include "homepage.h"
#include "./ui_homepage.h"

#include<QFile>
#include<QDir>
#include<QFileInfo>
#include<mp3playercolumn.h>

#include"mp3playercolumn.h"

extern MP3PlayerColumn *mp3_player_column;

HomePage::HomePage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HomePage)
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::on_pushButton_clicked()
{
    QString song_path = ui->lineEdit->text();
    QFileInfo f(song_path);
    if(f.exists() && f.isFile()){
        mp3_player_column->AppendSongToPlayingList(f.absoluteFilePath());
    }
}
