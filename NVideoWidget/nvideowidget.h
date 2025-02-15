#ifndef NVIDEOWIDGET_H
#define NVIDEOWIDGET_H

#include<QVideoWidget>
#include<QMouseEvent>

class PlayMP4Page;

class NVideoWidget : public QVideoWidget
{

Q_OBJECT

public:
    NVideoWidget(PlayMP4Page* play_mp4_page,QWidget* parent);
    void SetPlayMP4Page(PlayMP4Page* page);
    void UnfullScreenShow();
    void FullScreenShow();

signals:
    void mouseSimpleClicked();

private:
    void __init__();

private:
    void mouseDoubleClickEvent(QMouseEvent* e)override;
    void mousePressEvent(QMouseEvent* e)override;
    void mouseSimpleClickEvent();

private:
    PlayMP4Page *play_mp4_page = nullptr;
    QTimer* simple_clicked_timer_;//用来判断鼠标是否单击

};

#endif // NVIDEOWIDGET_H
