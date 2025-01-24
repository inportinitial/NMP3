#include "lyricsnode.h"
#include "ui_lyricsnode.h"
#include<algorithm>
#include<QLabel>
#include<QFont>
#include<QFontMetrics>

#define THIS LyricsNode

THIS::LyricsNode(qint64 time,QString str,QWidget *parent)
    : QWidget(parent)
    ,time(time)
    , ui(new Ui::LyricsNode)
{
    ui->setupUi(this);
    AddLyrics(str);
}

LyricsNode::~LyricsNode()
{
    delete ui;
}

QSize THIS::GetMinWidthHeight(){
    QSize size(0,0);
    auto layout = GetLayout();
    for(int i=0;i<layout->count();i++){
        auto font = layout->itemAt(i)->widget()->font();
        QFontMetrics fm(font);
        QString text = ((QLabel*)layout->itemAt(i)->widget())->text();
        size.setWidth(std::max(size.width(),fm.horizontalAdvance(text)));
        size.setHeight(size.height()+fm.height());
    }
    return size;
}

QLabel* THIS::CreateNewLabel(){
    QLabel *newlb = new QLabel(ui->frame);
    QFont font;
    font.setPointSize(12);
    newlb->setFont(font);
    return newlb;
}

void THIS::AddLyrics(QString str){
    QLabel *newlb = CreateNewLabel();
    newlb->setText(str);
    _AddLB2Layout(newlb);
}

void THIS::_AddLB2Layout(QLabel* lb){
    auto layout = GetLayout();
    int count = layout->count();
    layout->insertWidget(count,lb);
}

QVBoxLayout* THIS::GetLayout(){
    return (QVBoxLayout*)ui->frame->layout();
}


void THIS::SetTime(qint64 time){ this->time = time; }
qint64 THIS::TimeGet(){ return this->time; }
