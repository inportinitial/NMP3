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
    update_node_list_timer_ = new QTimer(this);
    update_node_list_timer_->setInterval(0);
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
    connect(this, &THIS::GUIUpdateEnded, this, [&]() {
        QString number =  QString::number(mp3_player_column->NowPlayingSongPathLists().size());
        QString text = QString() + "歌\n曲\n数\n量\n";
        for(QChar c : number){
            text+=c;
            text+="\n";
        }
        ui->ShowInfoLabel->setText(text);
    });
    connect(
        mp3_player_column, &MP3PlayerColumn::PlayingSongListChanged, this,
        [&]() {
            ui->ShowInfoLabel->setText(
                QString() + "歌\n曲\n数\n量\n" +
                QString::number(mp3_player_column->NowPlayingSongPathLists().size()));
        });
}

DisplayMP4NodeList::~DisplayMP4NodeList()
{
    delete ui;
}

void THIS::_UpdateLists(std::list<QString> list){
    QTimer *timer = update_node_list_timer_;
    QObject::disconnect(timer,nullptr,nullptr,nullptr);
    __ClearScrollArea();
    std::list<QString> *new_list = new std::list<QString>(list);
    std::list<QString>::iterator* tt = new std::list<QString>::iterator(new_list->begin());
    QString *input_search = new QString(ui->InputSearch->text());
    connect(timer,&QTimer::timeout,this,[this,timer,tt,new_list,input_search](){
        int duration = 2;
        for(int i=0;i<duration;i++){
            if(*tt == new_list->end()){
                timer->stop();
                this->update();
                delete tt;
                delete new_list;
                emit GUIUpdateEnded();
                return;
            }
            bool if_song_valid = 0;
            //计算歌曲名是否满足筛选条件
            {
                if(*input_search == "")if_song_valid = 1;
                else{
                    QFileInfo info(**tt);
                    QString name = info.fileName();
                    int ind = 0;
                    for(QChar c : name){
                        if(ind == input_search->size())break;
                        else if((*input_search)[ind] == c)ind++;
                    }
                    if_song_valid = (ind == input_search->size());
                }
            }
            if(if_song_valid){
                __AppendNode2ScrollArea(**tt);
            }else{
                i--;
            }
            (*tt)++;
        }
        this->update();
    });
    emit GUIBeingUpdated();
    timer->start();
}

void THIS::__ClearScrollArea(){
    setUpdatesEnabled(0);
    auto layout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();
    int count = layout->count();
    count--;
    while(count--){
        layout->takeAt(0)->widget()->deleteLater();
    }
    setUpdatesEnabled(1);
    update();
}

void THIS::__AppendNode2ScrollArea(QString song_path){
    MP4Node* new_node = new MP4Node(song_path);
    connect(new_node,&MP4Node::DeleteButtionClicked,this,[&](){
        deleted_count_++;
    });
    QVBoxLayout* layout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();
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
    _UpdateLists({});
}

void DisplayMP4NodeList::on_ConfirmSeachInput_clicked()
{
    this->_UpdateLists(mp3_player_column->NowPlayingSongPathLists());
}


void DisplayMP4NodeList::on_InputSearch_returnPressed()
{
    this->_UpdateLists(mp3_player_column->NowPlayingSongPathLists());
}

void THIS::SetNodeUnabled(const QString& song_file_path,const QString& displayed_messgae){
    auto layout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();
    int count = layout->count();
    for(int i=0;i<count-1;i++){
        qDebug()<<i;
        auto node = (MP4Node*)layout->itemAt(i)->widget();
        if(node->song_file_path() == song_file_path){
            node->SetDisplayedMessageWhenHandleInputMessageUnabled(displayed_messgae);
            node->SetIfHandleInputMessage(0);
            break;
        }
    }
}
void THIS::SetNodeAbled(const QString& song_file_path){
    auto layout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();
    int count = layout->count();
    for(int i=0;i<count-1;i++){
        auto node = (MP4Node*)layout->itemAt(i)->widget();
        if(node->song_file_path() == song_file_path){
            node->SetIfHandleInputMessage(1);
            break;
        }
    }
}
