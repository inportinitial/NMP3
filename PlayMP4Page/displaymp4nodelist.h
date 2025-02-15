#ifndef DISPLAYMP4NODELIST_H
#define DISPLAYMP4NODELIST_H

#include <QWidget>
#include<list>
#include<QString>
#include<QResizeEvent>
#include<QDropEvent>
#include<QDragEnterEvent>
#include<QTimer>

namespace Ui {
class DisplayMP4NodeList;
}

class DisplayMP4NodeList : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayMP4NodeList(QWidget *parent = nullptr);
    ~DisplayMP4NodeList();

    void SetPlayMP4Page(QWidget* play_mp4_page){ play_mp4_page_ = play_mp4_page; }
    void AppendSong(const QString& song_path);
    void RemoveAllSongFromNowPlayingList();
    void SetNodeUnabled(const QString& song_file_path,const QString& displayed_messgae);
    void SetNodeAbled(const QString& song_file_path);

signals:
    void GUIBeingUpdated();
    void GUIUpdateEnded();

private slots:
    void on_ConfirmSeachInput_clicked();

    void on_InputSearch_returnPressed();

private:
    Ui::DisplayMP4NodeList *ui;
    QWidget* play_mp4_page_;
    QTimer* update_node_list_timer_;

private:
    void __InitConnections();
    void _UpdateLists(std::list<QString> list);
    void __ClearScrollArea();
    void __AppendNode2ScrollArea(QString song_path);

private:
    void resizeEvent(QResizeEvent* e)override;
    void dropEvent(QDropEvent* e)override;
    void dragEnterEvent(QDragEnterEvent* e)override;

private:
    //用来记录被删掉的歌曲的数量，用来防止每一次删除都重新获取播放队列把页面重新刷新一遍
    int deleted_count_ = 0;
    //同上
    int append_count_ = 0;
};

#endif // DISPLAYMP4NODELIST_H
