#ifndef DOWNLOADBILIBILIVIDEOPAGE_H
#define DOWNLOADBILIBILIVIDEOPAGE_H

#include<QTimer>
#include <QWidget>
#include<QString>
#include<QProcess>
#include<QSqlDatabase>
#include<QDropEvent>
#include<QDragEnterEvent>

class DownloadBilibiliVideoNode;

namespace Ui {
class DownloadBiliBiliVideoPage;
}


class DownloadBiliBiliVideoPage : public QWidget
{
    Q_OBJECT

public:
    //video_type
    static const int MP4 = 1;
    static const int MP3 = 2;
    //download_video_quality
    static const int BEST_QUALITY = 1;

public:
    explicit DownloadBiliBiliVideoPage(QWidget *parent = nullptr);
    ~DownloadBiliBiliVideoPage();
    QString download_dir_path();
    void SetDownloadDir(const QString& dir_path);

public:
    static QString sessdata_;
    static QString bili_jct_;
    static QString buvid3_;

signals:
    //当此页面缩小化的时候，相当于把此页面单独拿出来
    //然后隐藏除这个页面以外的所有页面
    //所以在回复原样的时候，需要把它塞回rootpage
    void NeedReturnToRootPage();
    void NeedRootPageUnvisible();

private slots:
    void on_ConfirmInputSessdata_clicked();

    void on_ConfirmInputBiliJCT_clicked();

    void on_ConfirmInputBuvid3_clicked();

    void on_ConfirmInputVideoUrl_clicked();

    void on_ConfirmDownloadDir_clicked();

    void on_StartOrParseListening_clicked();

    void on_SmallOrBigWidget_clicked();

private:
    Ui::DownloadBiliBiliVideoPage *ui;

private:
    QString pre_clipboard_content_ = "";
    QTimer download_listener;
    DownloadBilibiliVideoNode* _GetNewDownloadVideoNode();
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    void __ConnectNewNode(DownloadBilibiliVideoNode *new_node);
    void __AddNewNode(DownloadBilibiliVideoNode *new_node);
    void _DownloadBiliVideo(const QString& download_url);
    void __DownloadBiliVideo(int video_type,const QString& video_url,const QString& target_dir);
    void __init__();

private:
    QSize pre_widget_size_;
    int pre_widget_max_width;

private:
    void dropEvent(QDropEvent* e)override;
    void dragEnterEvent(QDragEnterEvent *e)override;
};

#endif // DOWNLOADBILIBILIVIDEOPAGE_H
