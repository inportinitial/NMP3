#include "renamesongpage.h"
#include "ui_renamesongpage.h"
#include<QUrl>
#include<QString>
#include<QFileInfo>
#include<QFile>
#include<QDir>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>

#define THIS RenameSongPage

using namespace RenameSongPage_PRIVATE;

RenameSongPage::RenameSongPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RenameSongPage)
{
    ui->setupUi(this);
    this->setAcceptDrops(1);
    __init__();
}

RenameSongPage::~RenameSongPage()
{
    delete ui;
}

void THIS::__init__(){
    clear_show_area();
    _ReadInputOutputDirPath();
    __InitConnections();
}

void THIS::__InitConnections(){
    connect(ui->ChangeInputOutputStatue,&QPushButton::clicked,this,[&](){
        if(if_auto_inputoutput_){
            ui->ChangeInputOutputStatue->setIcon(QIcon(":/images/arrow-right_02.png"));
        }else{
            ui->ChangeInputOutputStatue->setIcon(QIcon(":/images/arrow-right_01.png"));
        }
        if_auto_inputoutput_ = !if_auto_inputoutput_;
    });
}

void THIS::_Draw(const QString& filePath){
    const QObjectList &list = ui->charContainer->children();
    for(auto& t : list){
        delete t;
    }
    if(filePath == "")return;
    QFileInfo info(filePath);
    QString fileName = info.fileName();
    nchar_pool.clear();
    int spacing = 10;
    int maxx=spacing,maxy=spacing;
    for (QChar cc : fileName) {
        NChar *Nc = new NChar(ui->charContainer, cc);
        NChar &nc = (*Nc);
        nc.move(maxx,maxy);
        nc.x = maxx,nc.y = maxy;
        maxx+=nc.Rsize()+spacing;
        if(maxx+nc.Rsize() >= ui->charContainer->size().width()){
            maxx = spacing;maxy+=nc.Rsize()+spacing;
        }
        nchar_pool.push_back(&nc);
        nc.show();
    }
}

void THIS::Draw(){
    if(!filePath_pool.empty()){
        _Draw(*filePath_pool.begin());
    }else{
        _Draw(QString(""));
    }
}

void THIS::mouseMoveEvent(QMouseEvent *event){
    auto e_pos = this->mapToGlobal(event->position());
    auto pos = ui->charContainer->mapFromGlobal(e_pos);
    int X = pos.x();
    int Y = pos.y();
    for(auto t : nchar_pool){
        if(t->is_clicked(X,Y)){
            t->clicked();
        }
    }
}

QString THIS::GetString(){
    QString res = "";
    for(auto t : nchar_pool){
        if(t->is_choosed()){
            res+=t->RQChar();
        }
    }
    return res;
}


void THIS::clear_show_area(){
    ui->InputAuthor->clear();
    ui->InputSongName->clear();
    ui->InputFileNameResult->clear();
    ui->ConfirmNextSong->setStyleSheet("QPushButton{"
                                       "background : transparent;"
                                       "border : none;"
                                       "}"
                                       "QPushButton:hover{"
                                       "background-color :	#778899;"
                                       "}"
                                       "QPushButton:pressed{"
                                       "background-color :	 #D3D3D3;"
                                       "}");
    ui->InputAuthor->setStyleSheet("background : transparent;"
                                   "border:none;"
                                           "border-bottom: 2px solid black;"
                                           "color : white;");
    ui->InputSongName->setStyleSheet("background : transparent;"
                                   "border:none;"
                                           "border-bottom: 2px solid black;"
                                           "color : white;");
    ui->InputFileNameResult->setStyleSheet("background : transparent;"
                                   "border:none;"
                                           "border-bottom: 2px solid black;"
                                           "color : white;");
}

void THIS::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(state_ == FINISH_NULL){
            ui->InputAuthor->setText(GetString());
            ui->InputAuthor->setStyleSheet("background : transparent;"
                                           "border:none;"
                                           "border-bottom: 2px solid green;"
                                           "border-right: 2px solid green;"
                                           "");
            ui->InputSongName->setStyleSheet("background : transparent;"
                                           "border:none;"
                                           "border-bottom: 2px solid blue;"
                                           "border-right: 2px solid blue;"
                                           "");
            state_ = FINISH_SINGER;
        }else if(state_ == FINISH_SINGER){
            ui->InputSongName->setText(GetString());
            ui->InputSongName->setStyleSheet("background : transparent;"
                                             "border:none;"
                                           "border-bottom: 2px solid green;"
                                           "border-right: 2px solid green;"
                                           "");
            ui->InputFileNameResult->setStyleSheet("background : transparent;"
                                   "border:none;"
                                           "border-bottom: 2px solid blue;"
                                           "border-right: 2px solid blue;"
                                           "");
            ui->InputFileNameResult->setText(ui->InputAuthor->text() + " - " +
                                             ui->InputSongName->text());
            state_ = FINISH_NAME;
        }else if(state_ == FINISH_NAME){
            ui->InputFileNameResult->setStyleSheet("background : transparent;"
                                   "border:none;"
                                           "border-bottom: 2px solid green;"
                                           "border-right: 2px solid green;"
                                           );
            ui->ConfirmNextSong->setStyleSheet("background :transparent;"
                                          "border:none;"
                                           "border: 2px solid blue;"
                                           );
            QString str = ui->InputFileNameResult->text();
            QString invalid = "/\\?:*<>\"|";
            int size = str.size();
            for(int i=0;i<size;i++){
                if(invalid.indexOf(str[i]) != -1){
                    str[i] = '_';
                }
            }
            ui->InputFileNameResult->setText(str);
            state_ = FINISH_ALL;
        }else if(state_ == FINISH_ALL){
            on_ConfirmNextSong_clicked();
        }
        Draw();
    } else if (event->key() == Qt::Key_Backspace) {
    }
}

void THIS::_RenameSong(){
    QString normal_song_path = (*filePath_pool.begin());
    QFileInfo normal_info(normal_song_path);
    normal_song_path = normal_info.absoluteFilePath();
    QString now_song_name = ui->InputFileNameResult->text();
    QFile file(normal_song_path);
    if(file.exists()){
        file.rename(now_song_name+"."+normal_info.suffix());
        (*filePath_pool.begin()) = QFileInfo(file).absoluteFilePath();
    }else{
        qDebug()<<"normal_song_path:"<<normal_song_path;
    }
}

void THIS::_MoveSong(){
    QString target_dir = ui->InputOutputDir->text();
    QFile file(*filePath_pool.begin());
    if(file.copy(target_dir+"/"+QFileInfo(file).fileName())){
        file.remove();
    }
}

void THIS::_GetOneSongFromTargetDir(){
    QDir dir(ui->InputInputDir->text());
    auto list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (QFileInfo url , list) {
        if(url.suffix() == "mp4" || url.suffix() == "mp3"){
            filePath_pool.push_back(url.absoluteFilePath());
            break;
        }
    }
}

void THIS::on_ConfirmNextSong_clicked()
{
    if (filePath_pool.size()) {
        _RenameSong();
        if (if_auto_inputoutput_) {
            _MoveSong();
        }
        filePath_pool.pop_front();
    }
    if(filePath_pool.size() == 0 && if_auto_inputoutput_){
        _GetOneSongFromTargetDir();
    }
    clear_show_area();
    Draw();
    state_ = FINISH_NULL;
}

void THIS::_SaveInputOutputDirPath() {
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;

    command = "UPDATE basic_data SET input_dir_for_rename = :path1";
    q.prepare(command);
    q.bindValue(":path1",ui->InputInputDir->text());
    if(!q.exec()){
        qDebug()<<"commad:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }

    command = "UPDATE basic_data SET output_dir_for_rename = :path2";
    q.prepare(command);
    q.bindValue(":path2",ui->InputOutputDir->text());
    if(!q.exec()){
        qDebug()<<"commad:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }
}

void THIS::_ReadInputOutputDirPath() {
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;

    command = "SELECT input_dir_for_rename FROM basic_data WHERE id=:id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    if(!q.exec()){
        qDebug()<<"commad:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }else{
        QString path;
        while(q.next()){
            path = q.value("input_dir_for_rename").toString();
        }
        ui->InputInputDir->setText(path);
    }

    command = "SELECT output_dir_for_rename FROM basic_data WHERE id=:id";
    q.prepare(command);
    q.bindValue(":id","nmp3");
    if(!q.exec()){
        qDebug()<<"commad:"<<command;
        qDebug()<<"error:"<<q.lastError();
    }else{
        QString path;
        while(q.next()){
            path = q.value("output_dir_for_rename").toString();
        }
        ui->InputOutputDir->setText(path);
    }
}

void RenameSongPage::on_InputInputDir_editingFinished()
{ this->_SaveInputOutputDirPath(); }

void RenameSongPage::on_InputOutputDir_editingFinished()
{ this->_SaveInputOutputDirPath(); }


void RenameSongPage::on_ConfirmRestart_clicked()
{ refresh(); }

