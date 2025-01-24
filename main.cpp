#include <QApplication>

#include"RootPage/rootpage.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<mp3playercolumn.h>
#include<QFileInfo>
#include <QMessageBox>
#include<QDir>
#include<QVariantList>
#include<QTextStream>

#include"DownloadBiliBiliVideoPage/downloadbilibilivideopage.h"

MP3PlayerColumn *mp3_player_column = nullptr;

class DebugPage : public QObject{
public:
    DebugPage(){
        __init__();
    }
    void __init__() {
        DownloadBiliBiliVideoPage *p = new DownloadBiliBiliVideoPage;
        p->show();
    }
};

void InitDatabase();
void Initialize();

int main(int argc, char *argv[])
{
    const int debug_page = 0;
    QApplication a(argc, argv);
    Initialize();
    if(!debug_page){
        RootPage *r = new RootPage;
        r->show();
    }else{
        DebugPage w;
    }
    return a.exec();
}

void InitTable_basic_data(){
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QSqlQuery q(db);
    QString command;
    if (0) {
        command = "DROP TABLE IF EXISTS basic_data";
        if (!q.exec(command)) {
            qDebug() << "command:" + q.lastQuery();
            qDebug() << " error:" << q.lastError();
        }
    }

    command = "CREATE TABLE IF NOT EXISTS basic_data("
              "id TEXT UNIQUE"
              ")";
    if (!q.exec(command)) {
        qDebug() << "command:" + q.lastQuery();
        qDebug() << " error:" << q.lastError();
    } else {
        command = "INSERT INTO basic_data (id) VALUES (:id)";
        q.prepare(command);
        q.bindValue(":id", "nmp3");
        if (!q.exec()) {
            qDebug() << "command:" + q.lastQuery();
            qDebug() << " error:" << q.lastError();
        }
    }

    command = "ALTER TABLE basic_data ADD COLUMN volume REAL";
    if (!q.exec(command)) {
        qDebug() << "command:" + q.lastQuery();
        qDebug() << " error:" << q.lastError();
    } else {
        command = "UPDATE basic_data SET volume = :volume WHERE id = :id";
        q.prepare(command);
        q.bindValue(":volume", 0.5);
        q.bindValue(":id", "nmp3");
        if (!q.exec()) {
            qDebug() << "command:" + q.lastQuery();
            qDebug() << " error:" << q.lastError();
        }
    }

    command = "ALTER TABLE basic_data ADD COLUMN download_dir_path TEXT";
    if(!q.exec(command)){
        qDebug() << "command:" + q.lastQuery();
        qDebug() << " error:" << q.lastError();
    }else{
        command = "UPDATE basic_data SET download_dir_path = :download_dir_path WHERE id = :id";
        q.prepare(command);
        QFileInfo file("download_dir");
        if(!file.exists()){
            QDir dir;
            dir.mkpath(file.absoluteFilePath());
        }
        file.setFile("download_dir");
        q.bindValue(":id","nmp3");
        q.bindValue(":download_dir_path", file.absoluteFilePath());
        if (!q.exec()) {
            qDebug() << "command:" + q.lastQuery();
            qDebug() << " error:" << q.lastError();
        }
    }
    db.commit();
}

void InitTable_now_playing_song_paths(){
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    QString command;
    QSqlQuery q(db);

    command = "CREATE TABEL IF NOT EXISTS now_playing_songpaths("
              "path TEXT UNIQUE,"
              "num INTEGER"
              ")";
    if (!q.exec(command)) {
        qDebug() << "command:" + q.lastQuery();
        qDebug() << " error:" << q.lastError();
    } else {
    }

    db.commit();
}

void InitDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","nmp3");
    db.setDatabaseName("nmp3.db");
    if(!db.open()){
        qDebug()<<"open database failed";
        return;
    }

    InitTable_basic_data();
    InitTable_now_playing_song_paths();
}

void Initialize(){
    InitDatabase();
}
