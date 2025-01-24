#include "volumnslider.h"
#include "ui_volumnslider.h"

#include<QCursor>
#include<QPoint>

#define THIS VolumnSlider

VolumnSlider::VolumnSlider(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VolumnSlider)
{
    ui->setupUi(this);
    QPoint global_pos = QCursor::pos();
    QPoint localPos = mapFromGlobal(global_pos);
    this->move(localPos.x()-this->width()/2,localPos.y()-this->height()+10);

    this->setWindowFlags(Qt::FramelessWindowHint);

    QTimer *timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer,&QTimer::timeout,this,[&,timer](){
        if(!this->underMouse()){
            this->deleteLater();
            timer->stop();
        }
    });
    timer->start();

    slider_ = new VolumnSlider_Private(this);
    ui->verticalLayout->addWidget(slider_);
    connect(slider_,&VolumnSlider_Private::sliderReleased,this,[this](){
        emit this->VolumnChanged();
    });
}

VolumnSlider::~VolumnSlider()
{ delete ui; }

void THIS::SetVolumn(double volume){
    slider_->setValue(volume*slider_->maximum());
}

double THIS::GetVolumn(){
    return 1.0*slider_->value()/slider_->maximum();
}
