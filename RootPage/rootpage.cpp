#include "rootpage.h"
#include "ui_rootpage.h"

#include<QScreen>
#include<QSystemTrayIcon>

#include"../homepage.h"
#include"mp3playercolumn.h"
#include"../DownloadBiliBiliVideoPage/downloadbilibilivideopage.h"
#include"../RenameSongPage/renamesongpage.h"
#include"playmp4page.h"
#include<QSqlDatabase>

#define THIS RootPage

extern MP3PlayerColumn *mp3_player_column;

RootPage::RootPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RootPage)
    ,systemTraySet_hotkey_(new QHotkey(QApplication::instance()))
    ,system_tray_icon_(new QSystemTrayIcon(this))
{
    ui->setupUi(this);
    __init__();
}

void THIS::__init__(){
    __SetHotkey();
    connect(system_tray_icon_, &QSystemTrayIcon::activated, this, [&]() {
        this->show();
        system_tray_icon_->hide();
    });
    this->resize(1200,730);
    mp3_player_column = ui->mp3_player_column;
    this->setAttribute(Qt::WA_DeleteOnClose);
    emit ui->GoToHomePage->clicked();
}

void THIS::__SetHotkey(){
    systemTraySet_hotkey_->setShortcut(QKeySequence("Ctrl+Shift+Q"),1);
    connect(systemTraySet_hotkey_,&QHotkey::activated,this,[&](){
        if(this->isVisible()){
            // systemTrayIcon->setIcon(QIcon(":/images/images/icon.png"));
            this->setVisible(0);
            system_tray_icon_->show();
        }else{
            this->setVisible(1);
            system_tray_icon_->hide();
        }
    });
}

RootPage::~RootPage()
{
    QSqlDatabase db = QSqlDatabase::database("nmp3");
    db.commit();
    db.close();
    QSqlDatabase::removeDatabase("nmp3");
    delete ui;
}

void RootPage::on_GoToHomePage_clicked()
{
    if(pre_clicked_button == ui->GoToHomePage)return;
    _InitialButtonQss(pre_clicked_button);
    _LightButtonQss(ui->GoToHomePage);
    ChangeToPage(HOMEPAGE);
}

void RootPage::on_GoToPlayMP4Page_clicked()
{
    if(pre_clicked_button == ui->GoToPlayMP4Page)return;
    _InitialButtonQss(pre_clicked_button);
    _LightButtonQss(ui->GoToPlayMP4Page);
    ChangeToPage(PLAYMP4PAGE);
}

void THIS::_InitialButtonQss(QPushButton* bt)
{
    if(!bt)return;
}

void THIS::_LightButtonQss(QPushButton* bt)
{
    if(!bt)return;
}

void THIS::ChangeToPage(int page_number){
    if(ui->PageContainer->layout()->count()){
        ui->PageContainer->layout()->takeAt(0)->widget()->deleteLater();
    }
    switch(page_number){
    case HOMEPAGE:
        ui->PageContainer->layout()->addWidget(new HomePage);
        pre_clicked_button = ui->GoToHomePage;
        break;
    case PLAYMP4PAGE:
        ui->PageContainer->layout()->addWidget(new PlayMP4Page(mp3_player_column));
        pre_clicked_button = ui->GoToPlayMP4Page;
        break;
    case DOWNLOAD_BILIBILI_VIDEO_PAGE:
    {
        auto new_page = new DownloadBiliBiliVideoPage;
        connect(new_page, &DownloadBiliBiliVideoPage::NeedReturnToRootPage,
                this, [this,new_page]() {
            this->setVisible(1);
            ui->PageContainer->layout()->addWidget(new_page);
        });
        connect(new_page, &DownloadBiliBiliVideoPage::NeedRootPageUnvisible,
                this, [this, new_page]() {
            ui->PageContainer->layout()->takeAt(0);
            new_page->setParent(nullptr);
            this->setVisible(0);
        });
        ui->PageContainer->layout()->addWidget(new_page);
        pre_clicked_button = ui->DownloadBiliBiliVideoPage;
    }
        break;
    case RENAME_SONG_PAGE:
        ui->PageContainer->layout()->addWidget(new RenameSongPage(this));
        pre_clicked_button = ui->GoToRenameSongPage;
        break;
    };
}


void RootPage::on_DownloadBiliBiliVideoPage_clicked()
{
    if(pre_clicked_button == ui->DownloadBiliBiliVideoPage)return;
    _InitialButtonQss(pre_clicked_button);
    _LightButtonQss(ui->DownloadBiliBiliVideoPage);
    ChangeToPage(DOWNLOAD_BILIBILI_VIDEO_PAGE);
}

void RootPage::on_GoToRenameSongPage_clicked()
{
    if(pre_clicked_button == ui->GoToRenameSongPage)return;
    _InitialButtonQss(pre_clicked_button);
    _LightButtonQss(ui->GoToRenameSongPage);
    ChangeToPage(RENAME_SONG_PAGE);
}

