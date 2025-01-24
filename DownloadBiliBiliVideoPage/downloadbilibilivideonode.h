#ifndef DOWNLOADBILIBILIVIDEONODE_H
#define DOWNLOADBILIBILIVIDEONODE_H

#include <QWidget>
#include<QProcess>

namespace Ui {
class DownloadBilibiliVideoNode;
}

namespace DownloadBiliBiliVideoPage_PRIVATE{
//只用来下载一个mp3/mp4就被delete了
class DownloadVideoDevice : public QProcess{

    Q_OBJECT

public:

    static const bool is_debug = 1;
    static const bool if_show_normal_message = 1;
    static const bool if_show_all_python_error = 1;
    DownloadVideoDevice();

    void __init__();

    void DownloadBiliVideo(const QString &download_type,
                           const QString &download_url,
                           const QString &download_dir_path);

    void DownloadMP3(const QString& url,const QString& download_dir);

    void DownloadMP4(const QString& url,const QString& download_dir);

    double download_progress();

    QString download_video_name();

signals:
    void DownloadProgressChanged();
    void DownloadAudioDone();
    void DownloadVideoDone();
    void DownloadDone();
    void DownloadBegin();
    void ProgramFinished();
    void GetDownloadVideoName();

private:
    QString download_video_name_;
    double download_progress_ = 0;
    void _DownloadBiliBiliVideo(const QString &download_url,
                                const QString &download_type,
                                const QString &download_dir,
                                const QString &sessdata, const QString &bili_jct,
                                const QString &buvid3);
};
}

class DownloadBilibiliVideoNode : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadBilibiliVideoNode(QWidget *parent = nullptr);
    ~DownloadBilibiliVideoNode();
    QString download_dir_path();
    void SetDownloadDir(const QString& dir_path);
    void DownloadVideo(const QString &download_type,
                       const QString &download_url,
                       const QString &download_dir_path);

signals:
    void DownloadVideoDone();

private:
    Ui::DownloadBilibiliVideoNode *ui;
    void __init__();
    DownloadBiliBiliVideoPage_PRIVATE::DownloadVideoDevice* device;
    void _NextDownloadStep();
    void _UpdateDownloadProgress();

private:
    QString download_type_;
};

#endif // DOWNLOADBILIBILIVIDEONODE_H
