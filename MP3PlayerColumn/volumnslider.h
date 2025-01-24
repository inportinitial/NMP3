#ifndef VOLUMNSLIDER_H
#define VOLUMNSLIDER_H

#include<QWheelEvent>
#include <QWidget>
#include<QTimer>
#include<QSlider>
#include<QSize>
#include<QSizePolicy>

namespace Ui {
class VolumnSlider;
}

class VolumnSlider_Private : public QSlider{
public:
    VolumnSlider_Private(QWidget* parent = nullptr)
        :QSlider(parent){
        this->setMaximum(999);
        this->setValue(0);
        this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    }
private:
    void wheelEvent(QWheelEvent* e)override{
        float value = 1.0*this->value()/this->maximum();
        int change_value = e->angleDelta().y()/120;
        value+=1.0*change_value*0.1;
        if(value < 0)value = 0;
        else if(value > 1)value = 1;
        this->setValue(value*this->maximum());
        emit this->sliderReleased();
    }
};

class VolumnSlider : public QWidget
{
    Q_OBJECT

public:
    explicit VolumnSlider(QWidget *parent = nullptr);
    ~VolumnSlider();
    void SetVolumn(double volume);
    double GetVolumn();

signals:
    void VolumnChanged();

private:
    Ui::VolumnSlider *ui;
    VolumnSlider_Private* slider_;
};

#endif // VOLUMNSLIDER_H
