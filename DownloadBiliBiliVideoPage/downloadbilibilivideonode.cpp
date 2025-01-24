#include "downloadbilibilivideonode.h"
#include "ui_downloadbilibilivideonode.h"

#include<QDateTime>

#include"downloadbilibilivideopage.h"

#define THIS DownloadBilibiliVideoNode

using namespace DownloadBiliBiliVideoPage_PRIVATE;

QString DownloadVideoDevice::download_video_name(){
    return download_video_name_;
}

double DownloadVideoDevice::download_progress(){
    return download_progress_;
}

void DownloadVideoDevice::DownloadBiliVideo(const QString &download_type,
                                            const QString &download_url,
                                            const QString &download_dir_path) {
    _DownloadBiliBiliVideo(download_url, download_type,download_dir_path,DownloadBiliBiliVideoPage::sessdata_,
                           DownloadBiliBiliVideoPage::bili_jct_,
                           DownloadBiliBiliVideoPage::buvid3_);
}

DownloadVideoDevice::DownloadVideoDevice()
{
    __init__();
}

void DownloadVideoDevice::__init__(){
    // connect(this, &QProcess::started, this, [&]() { qDebug() << "start"; });
    connect(this, &QProcess::finished, this, [&]() {emit this->DownloadDone();});
    connect(this, &QProcess::readyReadStandardOutput, this, [&]() {
        QString output = this->readAllStandardOutput();
        QTextStream ts(&output);
        QString op;
        ts >> op;
        QString all = ts.readAll();
        if(op == "begin_download"){
            QTextStream ts3(&all);
            this->download_video_name_ = ts3.readLine();
            emit this->GetDownloadVideoName();
        } else if (op == "warning") {
            qDebug()<<op<<" "<<all;
        } else if (op == "download_begin") {
            emit this->DownloadBegin();
        } else if (op == "download_audio_done") {
            emit this->DownloadAudioDone();
        } else if (op == "download_video_done") {
            emit this->DownloadVideoDone();
        } else if (op == "download_done") {
            // 转为当process::finished的时候激活DownloadDone
            //  emit this->DownloadDone();
        } else if (op == "download_progress") {
            QTextStream ts2(&all);
            QString str1, str2;
            ts2 >> str1 >> str2 >> str2;
            download_progress_ = str1.toDouble() / str2.toDouble();
            emit this->DownloadProgressChanged();
        } else {
            if(is_debug){
            qDebug()<<QDateTime::currentDateTime().toString();
            if(!if_show_normal_message){
                qDebug()<<"normal message ignored";
            } else {
                qDebug() <<qPrintable(op) << " " << qPrintable(all);
            }
            }
        }
    });
    connect(this, &QProcess::finished, this,
            [&]() { emit this->ProgramFinished(); });
    connect(this, &QProcess::readyReadStandardError, this, [&]() {
        QString all = this->readAllStandardError();
        if(is_debug){
        qDebug()<<QDateTime::currentDateTime().toString();
        if (if_show_all_python_error || (all.size() && all[0] == 'T')){
            qDebug() <<qPrintable("python error:\n")<<qPrintable(all);
        } else {
            qDebug()<<"可能不是报错的报错被忽略";
        }
        }
    });
}

void DownloadVideoDevice::DownloadMP3(const QString& url,const QString& download_dir){
    _DownloadBiliBiliVideo(url, "mp3",download_dir,DownloadBiliBiliVideoPage::sessdata_,
                           DownloadBiliBiliVideoPage::bili_jct_,
                           DownloadBiliBiliVideoPage::buvid3_);
}

void DownloadVideoDevice::DownloadMP4(const QString& url,const QString& download_dir){
    _DownloadBiliBiliVideo(url, "mp4",download_dir,DownloadBiliBiliVideoPage::sessdata_,
                           DownloadBiliBiliVideoPage::bili_jct_,
                           DownloadBiliBiliVideoPage::buvid3_);
}

void DownloadVideoDevice::_DownloadBiliBiliVideo(const QString &download_url,
                                                 const QString &download_type,
                                                 const QString &download_dir,
                                                 const QString &sessdata,
                                                 const QString &bili_jct,
                                                 const QString &buvid3) {
    QString command = "python DownloadBilibiliVideo.py";
    QStringList list;
    list << "-download_url" << QString('"'+download_url+'"') << "-download_type"
         << "\""+download_type+"\"" << "-sessdata" << "\""+sessdata+
         "\"" << "-bili_jct" << "\""+bili_jct+"\"" << "-buvid3"
         << "\""+buvid3+"\"" << "-download_dir_path" << "\""+download_dir+
         +"\"";
    for (const QString &t : list) {
        command += " " + t;
    }
    // qDebug()<<qPrintable(command);
    this->start(command);
}

DownloadBilibiliVideoNode::DownloadBilibiliVideoNode(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadBilibiliVideoNode)
{
    ui->setupUi(this);
    __init__();
}

DownloadBilibiliVideoNode::~DownloadBilibiliVideoNode()
{
    device->deleteLater();
    delete ui;
}

void THIS::__init__(){
    device = new DownloadVideoDevice();
    connect(device,&DownloadVideoDevice::DownloadDone,this,[this](){
        emit this->DownloadVideoDone();
    });
    connect(device,&DownloadVideoDevice::GetDownloadVideoName,this,[this](){
        QString str = this->device->download_video_name();
        ui->DownloadVideoTitle->setText(str);
    });
}

void THIS::DownloadVideo(const QString &download_type,
                         const QString &download_url,
                         const QString &download_dir_path) {
    ui->DownloadVideoType->setText(download_type);
    download_type_ = download_type;
    _NextDownloadStep();
    connect(device, &DownloadVideoDevice::DownloadBegin, this,
            [&]() { _NextDownloadStep(); });
    connect(device, &DownloadVideoDevice::DownloadAudioDone, this,
            [&]() { _NextDownloadStep(); });
    connect(device, &DownloadVideoDevice::DownloadVideoDone, this,
            [&]() { _NextDownloadStep(); });
    connect(device, &DownloadVideoDevice::DownloadProgressChanged, this,
            [&]() { _UpdateDownloadProgress(); });

    device->DownloadBiliVideo(download_type,download_url,download_dir_path);
}

void THIS::_NextDownloadStep(){
    QString now_state = ui->DownloadStep->text();
    QString next_state;
    if(now_state == "无"){
        next_state = "准备下载中";
    }else if(now_state == "准备下载中"){
        if(download_type_== "mp3"){
            next_state = "下载音频中";
        }else if(download_type_ == "mp4"){
            next_state = "下载视频中";
        }
    } else if(now_state == "下载音频中"){
        if(download_type_== "mp3"){
            next_state = "生成mp3文件中";
        }else if(download_type_ == "mp4"){
            next_state = "下载视频中";
        }
    }else if(now_state == "下载视频中"){
        next_state = "生成mp4文件中";
    }
    ui->DownloadStep->setText(next_state);
}

void THIS::_UpdateDownloadProgress(){
    double progress = device->download_progress();
    ui->DownloadProgress->setText(QString::number(progress));
}
