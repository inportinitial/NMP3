#include "displaymp4nodelist.h"
#include "ui_displaymp4nodelist.h"

#include<QTimer>
#include<QVBoxLayout>

#include"playmp4page.h"
#include"mp3playercolumn.h"
#include"mp4node.h"

extern MP3PlayerColumn* mp3_player_column;

#define THIS DisplayMP4NodeList

DisplayMP4NodeList::DisplayMP4NodeList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DisplayMP4NodeList)
{
    ui->setupUi(this);
    this->setAcceptDrops(1);
    __InitConnections();
    QTimer *timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer,&QTimer::timeout,this,[&,timer](){
        ui->scrollArea->setMinimumWidth(this->width());
        ui->scrollAreaWidgetContents->setMaximumWidth(this->width()-10);
        _UpdateLists(mp3_player_column->NowPlayingSongPathLists());
        timer->stop();
        timer->deleteLater();
    });
    timer->start();
}

void THIS::__InitConnections(){
    connect(mp3_player_column,&MP3PlayerColumn::PlayingSongListChanged,this,[this](){
        if(deleted_count_){
            deleted_count_--;
            return;
        }
        auto list = mp3_player_column->NowPlayingSongPathLists();
        this->_UpdateLists(list);
    });
    connect(this,&THIS::GUIBeingUpdated,this,[&](){
        ui->ShowInfoLabel->setVisible(1);
        ui->ShowInfoLabel->setText("更\n新\n歌\n曲\n列\n表\n中");
    });
    connect(this,&THIS::GUIUpdateEnded,this,[&](){
        ui->ShowInfoLabel->setVisible(0);
    });
}

DisplayMP4NodeList::~DisplayMP4NodeList()
{
    delete ui;
}

void THIS::_UpdateLists(std::list<QString> list){
    __ClearScrollArea();
    QTimer *timer = new QTimer(this);
    timer->setInterval(0);
    std::list<QString> *new_list = new std::list<QString>(list);
    std::list<QString>::iterator* tt = new std::list<QString>::iterator(new_list->begin());
    connect(timer,&QTimer::timeout,this,[this,timer,tt,new_list](){
        int duration = 3;
        for(int i=0;i<duration;i++){
            if(*tt == new_list->end()){
                timer->stop();
                timer->deleteLater();
                delete tt;
                delete new_list;
                emit GUIUpdateEnded();
                return;
            }
            __AppendNode2ScrollArea(**tt);
            (*tt)++;
        }
    });
    timer->start();
    emit GUIBeingUpdated();
}

void THIS::__ClearScrollArea(){
    auto layout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();
    int count = layout->count();
    count--;
    while(count--){
        layout->takeAt(0)->widget()->deleteLater();
    }
}

void THIS::__AppendNode2ScrollArea(QString song_path){
    auto new_node = new MP4Node(song_path);
    connect(new_node,&MP4Node::DeleteButtionClicked,this,[&](){
        deleted_count_++;
    });
    auto layout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();
    layout->insertWidget(layout->count()-1,new_node);
}

void THIS::resizeEvent(QResizeEvent* e){
    ui->scrollAreaWidgetContents->setMaximumWidth(ui->scrollArea->width());
}

void THIS::dropEvent(QDropEvent* e){
    ((PlayMP4Page*)play_mp4_page_)->DropEvent(e);
}

void THIS::dragEnterEvent(QDragEnterEvent* e){
    e->accept();
}

void THIS::AppendSong(const QString& song_path){
    this->__AppendNode2ScrollArea(song_path);
}

void THIS::RemoveAllSongFromNowPlayingList(){
    mp3_player_column->_LoadSong("");
    mp3_player_column->RemoveAllSongsFromPlayingList();
    __ClearScrollArea();
}
