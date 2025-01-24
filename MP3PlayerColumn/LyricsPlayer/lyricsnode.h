#ifndef LYRICSNODE_H
#define LYRICSNODE_H

#include<QLabel>
#include<QMouseEvent>
#include <QWidget>
#include<QString>
#include<QVBoxLayout>

namespace Ui {
class LyricsNode;
}

class LyricsNode : public QWidget
{
    Q_OBJECT

public:
    explicit LyricsNode(qint64 time,QString str,QWidget *parent = nullptr);
    ~LyricsNode();
    void AddLyrics(QString str);
    void SetTime(qint64 time);
    qint64 TimeGet();
    QSize GetMinWidthHeight();
    QLabel* CreateNewLabel();

private:
    Ui::LyricsNode *ui;
    qint64 time;
    QVBoxLayout* GetLayout();

private:
    void _AddLB2Layout(QLabel* lb);

private:
};

#endif // LYRICSNODE_H
