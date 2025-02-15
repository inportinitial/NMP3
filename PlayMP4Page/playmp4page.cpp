#include "playmp4page.h"
#include "ui_playmp4page.h"
#include<QMessageBox>
#include<QProcess>
#include<QCursor>
#include<QTimer>
#include<QUrl>
#include<list>
#include<functional>
#include<QMimeData>
#include<QSqlDatabase>
#include<QFileInfo>
#include<QTextStream>
#include<QDir>
#include<QFile>

#include"mp3playercolumn.h"
#include"../NVideoWidget/nvideowidget.h"

#define THIS PlayMP4Page

extern MP3PlayerColumn *mp3_player_column;

PlayMP4Page::PlayMP4Page(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayMP4Page)
{
    ui->setupUi(this);
    __init__();
}

void THIS::__init__(){
    this->setAcceptDrops(1);
    ui->SimpleEditSongFrame->hide();
    ui->ShowMVOrLrcArea->layout()->addWidget(mp3_player_column->GetVideoWidget());
    mp3_player_column->GetVideoWidget()->setVisible(1);
    mp3_player_column->GetVideoWidget()->SetPlayMP4Page(this);
    ui->frame_2->layout()->takeAt(1);
    ui->display_mp4_node_list->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //display_..._list初始化的一部分
    ui->display_mp4_node_list->SetPlayMP4Page(this);
    ui->display_mp4_node_list->show();
    SetShowMP4NodeListFakeUnvisible(1);
    //===
    //延后一些时间再执行timer2的内容，为了给页面留下绘制的时间
    auto timer2 = new QTimer(this);
    timer2->setInterval(50);
    connect(timer2, &QTimer::timeout, this, [timer2, this]() {
        auto timer = new QTimer(this);
        timer->setInterval(100);
        //控制鼠标靠近和远离歌曲列表 歌曲列表的隐藏和显示逻辑
        connect(timer, &QTimer::timeout, this, [timer, this]() {
            if(!mp3_player_column->isVisible()){
                if(!if_show_mp4_node_list_fake_unvisible()){
                    SetShowMP4NodeListFakeUnvisible(1);
                }
                return;
            }
            QPoint p = QCursor::pos();
            QPoint pos;
            {
                QPoint pp;
                pp.setX(ui->ShowMVOrLrcArea->width() -
                       ui->display_mp4_node_list->width());
                pp.setY(ui->ShowMVOrLrcArea->y());
                pos = this->mapToGlobal(pp);
            }
            // auto pos = this->mapToGlobal(ui->display_mp4_node_list->pos());
            auto size = ui->display_mp4_node_list->size();
            if (p.x() >= pos.x() &&
                p.x() <= pos.x() + size.width() &&
                p.y() >= pos.y()+10 &&
                p.y() <= pos.y() + size.height()-20) {
                if(if_show_mp4_node_list_fake_unvisible()){
                    SetShowMP4NodeListFakeUnvisible(0);
                }
            } else {
                if(!if_show_mp4_node_list_fake_unvisible()){
                    SetShowMP4NodeListFakeUnvisible(1);
                }
            }
        });
        //===更新list大小
        ui->display_mp4_node_list->resize(ui->ShowMVOrLrcArea->width() / 8 * 3,
                                      ui->ShowMVOrLrcArea->height());
        //===
        timer->start();
        timer2->stop();
        timer2->deleteLater();
        //===频繁更新display_..._list的位置
        auto timer4 = new QTimer(this);
        timer4->setInterval(100);
        connect(timer4, &QTimer::timeout, this, [&]() {
            if(if_show_mp4_node_list_fake_unvisible())return;
            if(!mp3_player_column->isVisible())return;
            ui->display_mp4_node_list->resize(ui->ShowMVOrLrcArea->width() / 8 *
                                                  3,
                                              ui->ShowMVOrLrcArea->height());
            QPoint p;
            p.setX(ui->ShowMVOrLrcArea->width() -
                   ui->display_mp4_node_list->width());
            p.setY(ui->ShowMVOrLrcArea->y());
            ui->display_mp4_node_list->move(this->mapToGlobal(p));
        });
        timer4->start();
        //===
    });
    timer2->start();
    //===
}

PlayMP4Page::~PlayMP4Page()
{
    auto widget =  ui->ShowMVOrLrcArea->layout()->takeAt(0)->widget();
    widget->setParent(mp3_player_column);
    widget->setVisible(0);
    delete ui;
}

void THIS::dropEvent(QDropEvent* e){
    QString str = e->mimeData()->text();
    QTextStream ts(&str);
    QString line;
    std::list<QString>ans_list;

    std::function<void(QString)>AppendSongsInDirToMP3PlayerColumn
        = [&](QString str)->void{
        QDir dir(str);
        if(!dir.exists())return;
        dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        auto list = dir.entryInfoList();
        for(const auto& t : list){
            if(t.isFile()){
                ans_list.push_back(t.absoluteFilePath());
            }else if(t.isDir()){
                AppendSongsInDirToMP3PlayerColumn(t.absoluteFilePath());
            }
        }
    };
    while(!ts.atEnd()){
        line = ts.readLine();
        QFileInfo f(QUrl(line).toLocalFile());
        if(f.isFile()){
            ans_list.push_back(f.absoluteFilePath());
        }else if(f.isDir()){
            AppendSongsInDirToMP3PlayerColumn(f.absoluteFilePath());
        }
    }
    mp3_player_column->AppendSongsToPlayingList(ans_list);
}

void THIS::dragEnterEvent(QDragEnterEvent *e){
    e->accept();
}

void THIS::paintEvent(QPaintEvent* e){
    QWidget::paintEvent(e);
    //===当窗口大小变的时候也更改display_mp4_node_list的size和位置
    ui->display_mp4_node_list->resize(ui->ShowMVOrLrcArea->width() / 8 * 3,
                                      ui->ShowMVOrLrcArea->height());
    if(if_show_mp4_node_list_fake_unvisible())return;
    QPoint p;
    p.setX(ui->ShowMVOrLrcArea->width() - ui->display_mp4_node_list->width());
    p.setY(ui->ShowMVOrLrcArea->y());
    ui->display_mp4_node_list->move(this->mapToGlobal(p));
    //===
}

void THIS::TakeOffNVideoWidget(){
    ((QVBoxLayout*)ui->ShowMVOrLrcArea->layout())->takeAt(0);
}

void THIS::AddNVideoWidget(NVideoWidget* widget){
    ui->ShowMVOrLrcArea->layout()->addWidget(widget);
}

void PlayMP4Page::on_ClearNowPlayingSongList_clicked()
{
    ui->display_mp4_node_list->RemoveAllSongFromNowPlayingList();
}

bool THIS::if_show_mp4_node_list_fake_unvisible(){
    return if_show_mp4_node_list_fake_unvisible_;
}

void THIS::SetShowMP4NodeListFakeUnvisible(bool f){
    if(f == if_show_mp4_node_list_fake_unvisible_){
        return;
    }
    if(f){
        ui->display_mp4_node_list->move(0,3000);
        // ui->display_mp4_node_list->move(0,0);
    }else{
        if(mp3_player_column->is_changing_volume())return;
        QPoint p;
        {
            QPoint pp;
            pp.setX(ui->ShowMVOrLrcArea->width() -
                    ui->display_mp4_node_list->width());
            pp.setY(ui->ShowMVOrLrcArea->y());
            p = this->mapToGlobal(pp);
        }
        ui->display_mp4_node_list->move(p);
    }
    if_show_mp4_node_list_fake_unvisible_ = f;
}

void PlayMP4Page::on_ConfirmAdjustSongVolumeBy_dB_clicked()
{
    QString path = mp3_player_column->NowPlayingSongPath();
    ui->display_mp4_node_list->SetNodeUnabled(path,"修改文件中...");
    mp3_player_column->AdjustNowPlayingSongFileVolumeBy_dB(
        ui->Input_dBOfAdjustTheSongVolumn->text().toInt());
    connect(mp3_player_column,&MP3PlayerColumn::AdjustSongFileVolumeDone,this,[&,path](){
        ui->display_mp4_node_list->SetNodeAbled(path);
        QFileInfo f(path);
        auto box = new QMessageBox();
        box->setText(f.fileName()+"音量已调整完毕");
        box->show();
    });
}

void PlayMP4Page::on_EditMVOrMP3_clicked()
{
    ui->SimpleEditSongFrame->setVisible(!ui->SimpleEditSongFrame->isVisible());
}

void PlayMP4Page::on_ConfirmAdjustSongVolumeByPercentage_clicked()
{
    QString path = mp3_player_column->NowPlayingSongPath();
    ui->display_mp4_node_list->SetNodeUnabled(path,"修改文件中...");
    mp3_player_column->AdjustNowPlayingSongFileVolumeByPercentage(
        ui->InputPercentageOfAdjustTheSongVolumn->text().toInt());
    connect(mp3_player_column,&MP3PlayerColumn::AdjustSongFileVolumeDone,this,[&,path](){
        ui->display_mp4_node_list->SetNodeAbled(path);
        QFileInfo f(path);
        auto box = new QMessageBox(this);
        box->setText(f.fileName()+"音量已调整完毕");
        box->show();
    });
}


void PlayMP4Page::on_Confirm_RemoveFirstFewSeconds_clicked()
{
    double second = ui->Input_RemoveFirstFewSeconds->text().toDouble();
    mp3_player_column->DeleteTheFirstSecondsOfNowPlayingSong(second);
}


void PlayMP4Page::on_Confirm_RemoveLastFewSeconds_clicked()
{
    double second = ui->Input_RemovLastFewSeconds->text().toDouble();
    mp3_player_column->DeleteTheLastSecondsOfNowPlayingSong(second);
}

