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
    bool if_show_mp4_node_list_fake_unvisible();
    void SetShowMP4NodeListFakeUnvisible(bool f);

private slots:
    void on_ClearNowPlayingSongList_clicked();

    void on_ConfirmAdjustSongVolumeBy_dB_clicked();

    void on_EditMVOrMP3_clicked();

    void on_ConfirmAdjustSongVolumeByPercentage_clicked();

    void on_Confirm_RemoveFirstFewSeconds_clicked();

    void on_Confirm_RemoveLastFewSeconds_clicked();

private:
    void __init__();

private:
    Ui::PlayMP4Page *ui;
    bool if_show_mp4_node_list_fake_unvisible_ = 0;

private:
    void dragEnterEvent(QDragEnterEvent *e)override;
    void dropEvent(QDropEvent* e)override;
    void paintEvent(QPaintEvent* e)override;
};

#endif // PLAYMP4PAGE_H
