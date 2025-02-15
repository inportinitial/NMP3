#include "nvideowidget.h"
#include"../RootPage/rootpage.h"
#include"../PlayMP4Page/include/playmp4page.h"
#include<QTimer>
#include"mp3playercolumn.h"

extern RootPage* r;
extern MP3PlayerColumn *mp3_player_column;

#define THIS NVideoWidget

NVideoWidget::NVideoWidget(PlayMP4Page* page,QWidget* parent)
    :QVideoWidget(parent)
    ,play_mp4_page(page)
{
    __init__();
}

void THIS::__init__(){
    simple_clicked_timer_ = new QTimer(this);
    simple_clicked_timer_->setInterval(300);
    simple_clicked_timer_->disconnect();
    connect(simple_clicked_timer_,&QTimer::timeout,this,[&](){
        emit this->mouseSimpleClicked();
        simple_clicked_timer_->stop();
    });
    connect(this,&THIS::mouseSimpleClicked,[&](){
        mouseSimpleClickEvent();
    });
}

void THIS::SetPlayMP4Page(PlayMP4Page* page){
    play_mp4_page = page;
}

void THIS::mouseDoubleClickEvent(QMouseEvent* e){
    simple_clicked_timer_->disconnect();
    if(!this->isFullScreen()){
        FullScreenShow();
    }else{
        UnfullScreenShow();
    }
}

void THIS::UnfullScreenShow(){
    if (this->isFullScreen()) {
        this->move(-1000,-1000);
        this->setFullScreen(0);
        play_mp4_page->AddNVideoWidget(this);
        r->setVisible(1);
        this->setVisible(1);
    }
}

void THIS::FullScreenShow(){
    if(!this->isFullScreen()){
        play_mp4_page->TakeOffNVideoWidget();
        this->setParent(nullptr);
        r->setVisible(0);
        this->setVisible(1);
        this->setFullScreen(1);
    }
}

void THIS::mousePressEvent(QMouseEvent* e){
    if(e->button() == Qt::LeftButton){
        simple_clicked_timer_->disconnect();
        connect(simple_clicked_timer_, &QTimer::timeout, this,
                [&]() { emit this->mouseSimpleClicked(); simple_clicked_timer_->stop();});
        simple_clicked_timer_->start();
    }
}

void THIS::mouseSimpleClickEvent(){
    mp3_player_column->ClickPauseOrPlay();
}
