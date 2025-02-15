#ifndef MP4NODE_H
#define MP4NODE_H

#include <QWidget>
#include<QString>
#include<QMouseEvent>
#include<QLabel>

namespace Ui {
class MP4Node;
}

class MP4Node : public QWidget
{
    Q_OBJECT

public:
    explicit MP4Node(QString song_file_path,QWidget *parent = nullptr);
    ~MP4Node();
    QString song_file_path();
    void DeleteThis();
    void SetIfHandleInputMessage(bool f);
    void SetDisplayedMessageWhenHandleInputMessageUnabled(const QString& s);

signals:
    void DeleteButtionClicked();

private slots:
    void on_RemoveSongFromLists_clicked();

private:
    Ui::MP4Node *ui;
    QString song_file_path_;
    bool if_handle_input_message_ = 1;
    QLabel* show_message_when_handle_input_message_unabled_;

private:
    void _SetSongFilePath(QString song_path);

private:
    void mouseDoubleClickEvent(QMouseEvent* e)override;
};


#endif // MP4NODE_H
