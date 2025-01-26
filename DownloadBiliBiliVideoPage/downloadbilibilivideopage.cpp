#include "downloadbilibilivideopage.h"
#include "ui_downloadbilibilivideopage.h"

#include"downloadbilibilivideonode.h"

#include<QMimeData>
#include<QDateTime>
#include<QApplication>
#include<QClipboard>
#include<QVBoxLayout>
#include<QDir>
#include<QFileInfo>
#include<QProcess>
#include<QTextStream>
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>

#define THIS DownloadBiliBiliVideoPage

QString THIS::sessdata_ = "";
QString THIS::bili_jct_ = "";
QString THIS::buvid3_ = "";

DownloadBiliBiliVideoPage::DownloadBiliBiliVideoPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DownloadBiliBiliVideoPage)
{
    ui->setupUi(this);
    __init__();
}

void THIS::__init__(){
    this->setAcceptDrops(1);
    database.setDatabaseName("nmp3.db");
    if(!database.open()){
        qDebug()<<database.lastError();
    }
    QString dir_path = download_dir_path();
    ui->InputDownloadDir->setText(dir_path);
    download_listener.setInterval(300);
    connect(&download_listener,&QTimer::timeout,this,[&](){
        QString now_content = QApplication::clipboard()->text();
        if(now_content != pre_clipboard_content_){
            pre_clipboard_content_ = now_content;
            this->_DownloadBiliVideo(now_content);
        }
    });
    _GetDownloadDirPath();
    _GetSessdata();
    _GetBili_jct();
    _GetBuvid3();
    this->on_ConfirmInputSessdata_clicked();
    this->on_ConfirmInputBiliJCT_clicked();
    this->on_ConfirmInputBuvid3_clicked();
}

void THIS::_GetDownloadDirPath(){
    ui->InputDownloadDir->setText(download_dir_path());
}

void THIS::_GetSessdata(){
    QString ans;
    auto& db = database;
    QSqlQuery q(db);
    QString command = "SELECT sessdata FROM basic_data WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    if(!q.exec()){
        qDebug()<<"command:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }else{
        while(q.next()){
            ans = q.value("sessdata").toString();
        }
    }
    ui->InputSessdata->setText(ans);
    this->sessdata_ = ans;
}

void THIS::_GetBili_jct(){
    QString ans;
    auto& db = database;
    QSqlQuery q(db);
    QString command = "SELECT bili_jct FROM basic_data WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    if(!q.exec()){
        qDebug()<<"command:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }else{
        while(q.next()){
            ans = q.value("bili_jct").toString();
        }
    }
    ui->InputBiliJCT->setText(ans);
    this->bili_jct_ = ans;
}

void THIS::_GetBuvid3(){
    QString ans;
    auto& db = database;
    QSqlQuery q(db);
    QString command = "SELECT buvid3 FROM basic_data WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    if(!q.exec()){
        qDebug()<<"command:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }else{
        while(q.next()){
            ans = q.value("buvid3").toString();
        }
    }
    ui->InputBuvid3->setText(ans);
    this->buvid3_ = ans;
}

void THIS::_SaveSessdata(){
    QString ans = ui->InputSessdata->text();
    QString command;
    auto& db = database;
    QSqlQuery q(db);
    command = "UPDATE basic_data SET sessdata = :sessdata "
              "WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    q.bindValue(":sessdata", ans);
    if (!q.exec()) {
        qDebug() << "command:" + q.lastQuery();
        qDebug() << " error:" << q.lastError();
    }
    this->sessdata_ = ans;
}

void THIS::_SaveBili_jct(){
    QString ans = ui->InputBiliJCT->text();
    QString command;
    auto& db = database;
    QSqlQuery q(db);
    command = "UPDATE basic_data SET bili_jct = :bili_jct "
              "WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    q.bindValue(":bili_jct", ans);
    if (!q.exec()) {
        qDebug() << "command:" + q.lastQuery();
        qDebug() << " error:" << q.lastError();
    }
    this->bili_jct_ = ans;
}

void THIS::_SaveBuvid3(){
    QString ans = ui->InputBuvid3->text();
    QString command;
    auto& db = database;
    QSqlQuery q(db);
    command = "UPDATE basic_data SET buvid3 = :buvid3 "
              "WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    q.bindValue(":buvid3", ans);
    if (!q.exec()) {
        qDebug() << "command:" + q.lastQuery();
        qDebug() << " error:" << q.lastError();
    }
    this->buvid3_ = ans;
}

DownloadBiliBiliVideoPage::~DownloadBiliBiliVideoPage()
{
    database.close();
    delete ui;
}

void DownloadBiliBiliVideoPage::on_ConfirmInputSessdata_clicked()
{
    sessdata_ = ui->InputSessdata->text();
    _SaveSessdata();
}

void DownloadBiliBiliVideoPage::on_ConfirmInputBiliJCT_clicked()
{
    bili_jct_ = ui->InputBiliJCT->text();
    _SaveBili_jct();
}

void DownloadBiliBiliVideoPage::on_ConfirmInputBuvid3_clicked()
{
    buvid3_ = ui->InputBuvid3->text();
    _SaveBuvid3();
}

void THIS::__DownloadBiliVideo(int video_type,const QString& video_url,const QString &target_dir){
    DownloadBilibiliVideoNode* new_node = _GetNewDownloadVideoNode();
    if(video_type == MP4){
        new_node->DownloadVideo("mp4",video_url,target_dir);
    }else if(video_type == MP3){
        new_node->DownloadVideo("mp3",video_url,target_dir);
    }else{
        qDebug()<<"error video type";
    }
}

void THIS::_DownloadBiliVideo(const QString& download_url){
    QString download_type = ui->ChooseDownloadType->text();
    if(download_type == "MP4"){
        __DownloadBiliVideo(THIS::MP4,download_url,ui->InputDownloadDir->text());
    }else if(download_type == "MP3"){
        __DownloadBiliVideo(THIS::MP3,download_url,ui->InputDownloadDir->text());
    }
}

void DownloadBiliBiliVideoPage::on_ConfirmInputVideoUrl_clicked()
{
    _DownloadBiliVideo(ui->InputDownloadVideoUrl->text());
}


QString THIS::download_dir_path(){
    QString ans;
    auto& db = database;
    QSqlQuery q(db);
    QString command = "SELECT download_dir_path FROM basic_data WHERE id = :id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    if(!q.exec()){
        qDebug()<<"command:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }else{
        while(q.next()){
            ans = q.value("download_dir_path").toString();
        }
    }
    return ans;
}

void THIS::SetDownloadDir(const QString& dir_path){
    QString command;
    auto& db = database;
    QSqlQuery q(db);
    command = "UPDATE basic_data SET download_dir_path = :download_dir_path "
              "WHERE id = :id";
    QFileInfo info(dir_path);
    if(!info.exists()){
        QDir dir;
        dir.mkpath(dir_path);
    }
    q.prepare(command);
    q.bindValue(":id","nmp3");
    q.bindValue(":download_dir_path", info.absoluteFilePath());
    if (!q.exec()) {
        qDebug() << "command:" + q.lastQuery();
        qDebug() << " error:" << q.lastError();
    }
    db.commit();
}

DownloadBilibiliVideoNode* THIS::_GetNewDownloadVideoNode(){
    DownloadBilibiliVideoNode *new_node = new DownloadBilibiliVideoNode;
    __ConnectNewNode(new_node);
    __AddNewNode(new_node);
    return new_node;
}

void THIS::__ConnectNewNode(DownloadBilibiliVideoNode *new_node){
    connect(new_node,&DownloadBilibiliVideoNode::DownloadVideoDone,this,[this,new_node](){
        new_node->deleteLater();
    });
}

void THIS::__AddNewNode(DownloadBilibiliVideoNode *new_node){
    auto layout = (QVBoxLayout*)ui->scrollAreaWidgetContents->layout();
    int count = layout->count();
    layout->insertWidget(count-1,new_node);
}

void DownloadBiliBiliVideoPage::on_ConfirmDownloadDir_clicked()
{
    this->SetDownloadDir(ui->InputDownloadDir->text());
}


void DownloadBiliBiliVideoPage::on_StartOrParseListening_clicked()
{
    if(download_listener.isActive()){
        download_listener.stop();
        ui->StartOrParseListening->setText("启动监听");
        ui->ShowListeningStatueLabel->setText("未监听");
    }else{
        download_listener.start();
        ui->StartOrParseListening->setText("暂停监听");
        ui->ShowListeningStatueLabel->setText("正在监听");
    }
}


void DownloadBiliBiliVideoPage::on_SmallOrBigWidget_clicked()
{
    //通过暴力更改屏幕大小和隐藏左边的框架来起到缩小或者放大的效果
    //因为英文命名不方便，所以不起函数，直接在这里堆史山
    if(ui->LeftFrame->isVisible()){
        emit this->NeedRootPageUnvisible();
        pre_widget_size_ = this->size();
        pre_widget_max_width = this->maximumWidth();
        ui->LeftFrame->setVisible(0);
        this->setMaximumWidth(this->size().width()/3);
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
        this->show();
        ui->SmallOrBigWidget->setText("放大界面");
        QTimer* timer = new QTimer;
        timer->setInterval(1);
        connect(timer,&QTimer::timeout,this,[this,timer](){
            this->resize(this->maximumWidth(),this->height());
            this->move(0,0);
            timer->stop();
            timer->deleteLater();
        });
        timer->start();
    }else{
        ui->LeftFrame->setVisible(1);
        this->setMaximumWidth(pre_widget_max_width);
        this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
        this->resize(pre_widget_size_);
        this->show();
        ui->SmallOrBigWidget->setText("缩小界面");
        emit NeedReturnToRootPage();
    }
}


void THIS::dropEvent(QDropEvent* e){
    this->_DownloadBiliVideo(e->mimeData()->text());
}

void THIS::dragEnterEvent(QDragEnterEvent *e){
    e->accept();
}

void DownloadBiliBiliVideoPage::on_ChooseDownloadType_clicked()
{
    if(ui->ChooseDownloadType->text() == "MP4"){
        ui->ChooseDownloadType->setText("MP3");
    }else{
        ui->ChooseDownloadType->setText("MP4");
    }
}

