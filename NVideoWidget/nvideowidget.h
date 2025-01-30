#ifndef NVIDEOWIDGET_H
#define NVIDEOWIDGET_H

#include<QVideoWidget>
#include<QMouseEvent>

class PlayMP4Page;

class NVideoWidget : public QVideoWidget
{
public:
    NVideoWidget(PlayMP4Page* play_mp4_page,QWidget* parent);
    void SetPlayMP4Page(PlayMP4Page* page);

private:
    void __init__();

private:
    void mouseDoubleClickEvent(QMouseEvent* e)override;

private:
    PlayMP4Page *play_mp4_page = nullptr;

};

#endif // NVIDEOWIDGET_H
