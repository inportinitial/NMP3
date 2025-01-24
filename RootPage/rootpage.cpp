#include "rootpage.h"
#include "ui_rootpage.h"

#include<QScreen>

#include"../homepage.h"
#include"mp3playercolumn.h"
#include"../DownloadBiliBiliVideoPage/downloadbilibilivideopage.h"
#include"playmp4page.h"

#define THIS RootPage

extern MP3PlayerColumn *mp3_player_column;

RootPage::RootPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RootPage)
{
    ui->setupUi(this);
    __init__();
}

void THIS::__init__(){
    this->resize(1200,730);
    mp3_player_column = ui->mp3_player_column;
    emit ui->GoToHomePage->clicked();
}

RootPage::~RootPage()
{
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

