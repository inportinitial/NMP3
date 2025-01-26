#include "lyricsplayer.h"
#include "ui_lyricsplayer.h"
#include<QTimer>
#include<algorithm>
#include<QTextStream>
#include<QFileInfo>
#include<QDateTime>

#include"lyricsnode.h"

#define THIS LyricsPlayer

LyricsPlayer::LyricsPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LyricsPlayer)
{
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    {
        int screen_width, screen_height;
        {
            auto screen = QApplication::primaryScreen();
            screen_width = screen->size().width();
            screen_height = screen->size().height();
        }
        this->move((screen_width - this->width()) / 2,
                   (screen_height - this->height()) - 50);
    }
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);//可以将窗口背景完全透明，键鼠信号优先级最低
    auto timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer,&QTimer::timeout,this,[&](){
        if(this->underMouse()){
            this->activateWindow();
        }
    });
    timer->start();
}

LyricsPlayer::~LyricsPlayer()
{
    delete ui;
}

void THIS::mouseMoveEvent(QMouseEvent *event){
    QPointF now_pos = event->globalPosition();
    if(!pre_mouse_pos){ pre_mouse_pos = new QPointF(now_pos); return;}
    this->move(this->pos()+now_pos.toPoint() - pre_mouse_pos->toPoint());
    *pre_mouse_pos = now_pos;
}

void THIS::SetLyricsFilePath(QString file_path){
    QFileInfo info(file_path);
    if(info.suffix() == "mp3" || info.suffix() == "mp4"){
        QString lrc_path = info.absoluteFilePath();
        lrc_path = lrc_path.sliced(0,lrc_path.lastIndexOf('.'))+".lrc";
        info.setFile(lrc_path);
    }
    lyrics_file.setFileName(info.absoluteFilePath());
    for(int i=0;i<node_pool.size();i++)node_pool[i]->deleteLater();
    node_pool.clear();
    _LoadLyricsFromFile();
}

void THIS::SyncLyrics(qint64 pos){
    if(pos == 0){
        auto t = node_pool.begin();
        if((*t)->isHidden()){
            for(int i=0;i<node_pool.size();i++)node_pool[i]->hide();
            (*t)->show();
            auto timer = new QTimer;
            timer->setInterval(0);
            connect(timer,&QTimer::timeout,this,[&,timer](){
                this->ResizeToMinSize();
                timer->stop();
                timer->deleteLater();
            });
            timer->start();
        }
        return;
    }
    auto t = std::lower_bound(node_pool.begin(),node_pool.end(),pos,[](LyricsNode* node,qint64 pos){
        return node->TimeGet() < pos;
    });
    if(t == node_pool.begin())return;
    --t;
    if((*t)->isHidden()){
        for(int i=0;i<node_pool.size();i++)node_pool[i]->hide();
        (*t)->show();
        this->ResizeToMinSize();
    }
}

void THIS::_LoadLyricsFromFile(){
    if(!lyrics_file.open(QFile::ReadOnly)){
        //虽然返回了node* 但是并没有用到
        // auto newNode = _CreateNewLyricsNode(0,"=====NULL=====");
        _CreateNewLyricsNode(0,"=====NULL=====");
    }else{
        QTextStream ts(&lyrics_file);
        while(!ts.atEnd()){
            QString line = ts.readLine();
            if(line == "")continue;
            qint64 time;
            QString lyrics;
            __ReadTimeANDLyrics(line,time,lyrics);
            if(node_pool.size() == 0 || time != node_pool[node_pool.size()-1]->TimeGet()){
                // auto newNode = _CreateNewLyricsNode(time,lyrics);
                _CreateNewLyricsNode(time,lyrics);
            }else{
                auto node = node_pool[node_pool.size()-1];
                node->AddLyrics(lyrics);
            }
        }
    }
    lyrics_file.close();
    SyncLyrics(0);
}

LyricsNode* THIS::_CreateNewLyricsNode(qint64 time,QString str){
    LyricsNode *newNode = new LyricsNode(time,str,ui->frame);
    newNode->hide();
    ui->frame->layout()->addWidget(newNode);
    node_pool.push_back(newNode);
    return newNode;
}

void THIS::__ReadTimeANDLyrics(const QString& line,qint64 &time,QString &lyrics){
    lyrics = "";
    bool f1,f2,f3;
    int m=0,s=0,ss=0;
    int time_length = 11;
    m = line.sliced(1,2).toInt(&f1);
    if(!f1);
    else{
        s = line.sliced(4,2).toInt(&f2);
        if(!f2);
        else{
            if(line[9] == ']'){
                ss = line.sliced(7,2).toInt(&f3);
                time_length = 10;
            }else{
                ss = line.sliced(7,3).toInt(&f3);
                time_length = 11;
            }
            // if(!f3); 本来有这句，应该是为了逻辑完整
        }
    }
    if(!f1 || !f2 || !f3){ time = 0; lyrics = line;}
    else{ time = m*60+s;time*=1000;time+=ss; lyrics = line.right(line.size()-time_length);}
}

void THIS::ResizeToMinSize(){
    for(int i=0;i<node_pool.size();i++){
        if(node_pool[i]->isHidden());
        else{
            auto size = node_pool[i]->GetMinWidthHeight();
            this->resize(size.width(),size.height());
            break;
        }
    }
    this->resize(MIN_WIDTH,MIN_HEIGHT);
}
