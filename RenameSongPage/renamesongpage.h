#ifndef RENAMESONGPAGE_H
#define RENAMESONGPAGE_H

#include<QFileInfo>
#include<vector>
#include<list>
#include<QMimeData>
#include<QDragEnterEvent>
#include<QDropEvent>
#include <QWidget>
#include<QFrame>
#include<QLabel>
#include<QHBoxLayout>
#include<QPaintEvent>

using std::vector;
using std::list;

namespace Ui {
class RenameSongPage;
}

namespace RenameSongPage_PRIVATE{
class NChar : public QFrame{
public:
    NChar(QWidget *parent, QChar C = QChar(0))
        : QFrame(parent), c(C), parent(parent) {
        this->setLayout(&HBL);
        this->setFrameShape(QFrame::NoFrame);
        HBL.addWidget(&lb);
        _set();
        setColor();
        this->resize(_size);
    }
    void SetChar(char C) { c = C; }
    void SetChar(QChar C) { c = C; }
    QChar RQChar() { return c; }
    bool is_choosed() { return choosed; }
    int Rsize() { return _size; }

    bool already_clicked;
    int x=0,y=0;

    bool is_clicked(int X,int Y){
        if(x <= X
            && X <= x+_size
            && y <= Y
            && Y <= y+_size){
            return 1;
        }return 0;
    }
    void clicked(){
        if(already_clicked)return;
        already_clicked = 1;
        choosed = !choosed;
        setColor();
    }

private:
    QChar c;
    bool choosed = 0;
    QHBoxLayout HBL;
    QLabel lb;
    QFont font;
    QWidget *parent;
    int _size = 30;
    void _set(){
        HBL.setContentsMargins(0,0,0,0);
        lb.setText(c);
        lb.setAlignment(Qt::AlignCenter);
        font.setPointSize(18);
        lb.setFont(font);
    }
    void setColor(){
        if(choosed){
            this->setStyleSheet("color : black;"
                                "border-bottom: 1px solid black;"
                                "background : transparent;");
        }else{
            this->setStyleSheet("color : white;"
                                "border : none;"
                                "background : transparent;");
        }
        // parent->update();
    }
    void resize(int n) { QWidget::resize(n, n); }
};
}

using namespace RenameSongPage_PRIVATE;

class RenameSongPage : public QWidget
{
    Q_OBJECT

public:
    static const int FINISH_NULL = 0;
    static const int FINISH_SINGER = 1;
    static const int FINISH_NAME = 2;
    static const int FINISH_ALL = 3;
    explicit RenameSongPage(QWidget *parent = nullptr);
    ~RenameSongPage();

private slots:
    void on_ConfirmNextSong_clicked();

    void on_InputInputDir_editingFinished();

    void on_InputOutputDir_editingFinished();

    void on_ConfirmRestart_clicked();

private:
    void _RenameSong();

private:
    Ui::RenameSongPage *ui;
    bool if_auto_inputoutput_ = 0;

private:
    void __init__();
    void __InitConnections();
    void _ReadInputOutputDirPath();
    void _SaveInputOutputDirPath();

private:
    int state_ = FINISH_NULL;
    int button_size = 50;
    vector<NChar*>nchar_pool;
    list<QString>filePath_pool;
    void _Draw(const QString& file_path);
    void Draw();
    QString GetString();
    void refresh() { clear_show_area();_Draw(*filePath_pool.begin()); state_ = FINISH_NULL;}
    void _MoveSong();
    void NextStep();
    void clear_show_area();
    void _GetOneSongFromTargetDir();

private:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if(event->mimeData()->hasUrls()){
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event) override{
        QList<QUrl> tp = event->mimeData()->urls();
        for(const auto& t : tp){
            filePath_pool.push_back(QFileInfo(t.toLocalFile()).absoluteFilePath());
        }
        Draw();
    }
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override{
        for(auto t : nchar_pool){
            t->already_clicked = 0;
        }
    }
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // RENAMESONGPAGE_H
