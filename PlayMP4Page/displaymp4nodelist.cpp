#include "displaymp4nodelist.h"
#include "ui_displaymp4nodelist.h"

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
    ui->scrollAreaWidgetContents->setMaximumWidth(ui->scrollArea->width());
    this->setAcceptDrops(1);
    _UpdateLists(mp3_player_column->NowPlayingSongPathLists());
    __InitConnections();
}

void THIS::__InitConnections(){
    connect(mp3_player_column,&MP3PlayerColumn::PlayingSongListChanged,this,[this](){
        auto list = mp3_player_column->NowPlayingSongPathLists();
        this->_UpdateLists(list);
    });
}

DisplayMP4NodeList::~DisplayMP4NodeList()
{
    delete ui;
}

void THIS::_UpdateLists(std::list<QString> list){
    __ClearScrollArea();
    for(const auto& t : list){
        __AppendNode2ScrollArea(t);
    }
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
