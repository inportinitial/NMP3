#ifndef ROOTPAGE_H
#define ROOTPAGE_H

#include <QWidget>
#include<QPushButton>
#include<QString>
#include<QDir>
#include<QFile>

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

public:
    explicit RootPage(QWidget *parent = nullptr);
    ~RootPage();

private slots:
    void on_GoToHomePage_clicked();

    void on_GoToPlayMP4Page_clicked();

    void on_DownloadBiliBiliVideoPage_clicked();

private:
    Ui::RootPage *ui;
    QPushButton* pre_clicked_button = nullptr;

private:
    void __init__();
    void ChangeToPage(int page_number);
    void _InitialButtonQss(QPushButton* bt);
    void _LightButtonQss(QPushButton* bt);
};

#endif // ROOTPAGE_H
