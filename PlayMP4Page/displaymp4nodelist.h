#ifndef DISPLAYMP4NODELIST_H
#define DISPLAYMP4NODELIST_H

#include <QWidget>
#include<list>
#include<QString>
#include<QResizeEvent>
#include<QDropEvent>
#include<QDragEnterEvent>

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

private:
    Ui::DisplayMP4NodeList *ui;
    QWidget* play_mp4_page_;

private:
    void __InitConnections();
    void _UpdateLists(std::list<QString> list);
    void __ClearScrollArea();
    void __AppendNode2ScrollArea(QString song_path);

private:
    void resizeEvent(QResizeEvent* e)override;
    void dropEvent(QDropEvent* e)override;
    void dragEnterEvent(QDragEnterEvent* e)override;
};

#endif // DISPLAYMP4NODELIST_H
