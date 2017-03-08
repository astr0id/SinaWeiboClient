/*
 * 文件名：view_oauth2.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了View_Oauth2类的全部成员函数。
 */

#include "view_oauth2.h"
#include "ui_view_oauth2.h"

//构造函数
View_Oauth2::View_Oauth2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View_Oauth2)
{
    ui->setupUi(this);


    counter=0;

    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(setUrlWithCode(QUrl)));

    //setWindowFlags(Qt::FramelessWindowHint);
}

//析构函数
View_Oauth2::~View_Oauth2()
{
    delete ui;
}

//鼠标点击事件
void View_Oauth2::mousePressEvent(QMouseEvent *event){
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
}

//鼠标按压并拖动
void View_Oauth2::mouseMoveEvent(QMouseEvent *event){
    this->move(event->globalPos()-this->dPos);
}

//载入页面
void View_Oauth2::loadUrl(QString url){
    this->ui->webView->setUrl(QUrl(url));
    //this->ui->webView->load(QUrl(url));
}

//设置带有code的URL
void View_Oauth2::setUrlWithCode(QUrl url){
    if(counter%2==1){
        this->urlWithCode = url;
        emit urlChange(url);
    }
    counter++;
}

//获取带有code的URL
QUrl View_Oauth2::getCodeUrl(){
    return urlWithCode;
}

//淡化显示
void View_Oauth2::gradientShow(){
    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(AppearTime);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    this->show();
}

//淡化隐藏
void View_Oauth2::gradientHide(){
    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(DisappearTime);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    this->hide();
}


