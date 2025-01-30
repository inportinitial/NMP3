#include "nvideowidget.h"
#include"../RootPage/rootpage.h"
#include"../PlayMP4Page/include/playmp4page.h"

extern RootPage* r;

#define THIS NVideoWidget

NVideoWidget::NVideoWidget(PlayMP4Page* page,QWidget* parent)
    :QVideoWidget(parent)
    ,play_mp4_page(page)
{
    __init__();
}

void THIS::__init__(){
}

void THIS::SetPlayMP4Page(PlayMP4Page* page){
    play_mp4_page = page;
}

void THIS::mouseDoubleClickEvent(QMouseEvent* e){
    if(r->isVisible()){
        play_mp4_page->TakeOffNVideoWidget();
        this->setParent(nullptr);
        r->setVisible(0);
        this->setVisible(1);
        this->setFullScreen(1);
    }else{
        r->setVisible(1);
        this->setFullScreen(0);
        play_mp4_page->AddNVideoWidget(this);
    }
}
