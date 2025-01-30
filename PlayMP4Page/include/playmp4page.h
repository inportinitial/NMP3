#ifndef PLAYMP4PAGE_H
#define PLAYMP4PAGE_H

#include<QPoint>
#include <QWidget>
#include<QSize>
#include<QDropEvent>
#include<QDragEnterEvent>
#include<QMoveEvent>
#include<QPaintEvent>

class NVideoWidget;

namespace Ui {
class PlayMP4Page;
}

class PlayMP4Page : public QWidget
{
    Q_OBJECT

public:
    explicit PlayMP4Page(QWidget *parent = nullptr);
    ~PlayMP4Page();
    void AddSong();
    void DropEvent(QDropEvent* e){ dropEvent(e); }
    void TakeOffNVideoWidget();
    void AddNVideoWidget(NVideoWidget* widget);

private slots:
    void on_ClearNowPlayingSongList_clicked();

private:
    void __init__();

private:
    Ui::PlayMP4Page *ui;

private:
    void dragEnterEvent(QDragEnterEvent *e)override;
    void dropEvent(QDropEvent* e)override;
    void paintEvent(QPaintEvent* e)override;
};

#endif // PLAYMP4PAGE_H
