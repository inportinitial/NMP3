#ifndef ROOTPAGE_H
#define ROOTPAGE_H

#include<QSystemTrayIcon>
#include <QWidget>
#include<QPushButton>
#include<QString>
#include<QDir>
#include<QFile>
#include<qhotkey.h>
#include"../NVideoWidget/nvideowidget.h"

namespace Ui {
class RootPage;
}

class RootPage : public QWidget
{
    Q_OBJECT

public:
    static const int HOMEPAGE = 1;
    static const int PLAYMP4PAGE = 2;
    static const int DOWNLOAD_BILIBILI_VIDEO_PAGE = 4;
    static const int RENAME_SONG_PAGE = 5;

public:
    explicit RootPage(QWidget *parent = nullptr);
    ~RootPage();
    void show();
    void hide();
    void setVisible(bool f);


private slots:
    void on_GoToHomePage_clicked();

    void on_GoToPlayMP4Page_clicked();

    void on_DownloadBiliBiliVideoPage_clicked();

    void on_GoToRenameSongPage_clicked();

private:
    Ui::RootPage *ui;
    QPushButton* pre_clicked_button = nullptr;
    QHotkey* systemTraySet_hotkey_;
    QSystemTrayIcon *system_tray_icon_;

private:
    void __init__();
    void __SetHotkey();
    void ChangeToPage(int page_number);
    void _InitialButtonQss(QPushButton* bt);
    void _LightButtonQss(QPushButton* bt);
};

#endif // ROOTPAGE_H
