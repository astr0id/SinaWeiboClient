/*
 * 文件名：view_item_status.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了View_Item_Status类的所有成员函数。
 */

#include "view_item_status.h"
#include "ui_view_item_status.h"

//构造函数
View_Item_Status::View_Item_Status(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::View_Item_Status)
{
    ui->setupUi(this);
    webView = new QWebView;
    pictureList.clear();
    labelList.clear();

    setMouseTracking(true);

    popMenu = new QMenu(this);
    deleteWeibo = new QAction("删除", this);
    popMenu->addAction(deleteWeibo);
    connect(deleteWeibo, SIGNAL(triggered()), this, SLOT(deleteCurWeibo()));

}

//析构函数
View_Item_Status::~View_Item_Status()
{

    delete webView;
    delete ui;
}

//设置表项内容
void View_Item_Status::setItemContent(QString _id_status, QString _id_user_status,
                    QString _text_status, QString _screen_name_user_status,
                    QString _profile_image_url_status, bool _isRepost,
                    QString _text_retweeted, QString _id_retweeted,
                    QString _id_user_retweeted, QString _screen_name_user_retweeted,
                    QString _profile_image_url_retweeted, std::list<QString> _pictureList,
                    bool isNew){
    //设置成员变量
    pictureList=_pictureList;

    id_status=_id_status;
    id_user_status=_id_user_status;

    text_status=_text_status;
    if(isNew)
        text_status=lexicalAnalysis(text_status);
    //ui->label_status_text->setOpenExternalLinks(true);
    ui->label_status_text->setWordWrap(true);
    ui->label_status_text->setText(text_status);
    ui->label_status_text->adjustSize();

    screen_name_user_status=_screen_name_user_status;
    ui->label_status_screen_name->setText(screen_name_user_status);

    profile_image_url_status=_profile_image_url_status;
    //设置头像
    QPixmap head;
    QPixmap mask;
    head.load(profile_image_url_status);
    mask.load(mask_path_status);
    ui->label_avatar->setScaledContents(true);
    ui->label_avatar->setPixmap(head);
    ui->label_avatar->setMask(mask.mask());

    isRepost=_isRepost;

    if(isRepost){//判断是否是转发微博
        text_retweeted=_text_retweeted;
        id_retweeted=_id_retweeted;
        id_user_retweeted=_id_user_retweeted;
        screen_name_user_retweeted=_screen_name_user_retweeted;
        profile_image_url_retweeted=_profile_image_url_retweeted;

        if(isNew)
            text_retweeted=lexicalAnalysis(/*"<font color=blue><b>@"*/"@"+screen_name_user_retweeted+":"/*"</b></font>:"*/+text_retweeted);
        ui->label_retweeted_text->setVisible(true);
        //ui->label_retweeted_text->setOpenExternalLinks(true);
        ui->label_retweeted_text->setWordWrap(true);
        ui->label_retweeted_text->setText(text_retweeted);
        ui->label_retweeted_text->adjustSize();
        ui->label_retweeted_text->setGeometry(ui->label_status_text->x()+10, ui->label_status_text->y()+ui->label_status_text->height()+10,
                                              ui->label_retweeted_text->width(), ui->label_retweeted_text->height());
        ui->label_bar->setVisible(true);
        ui->label_bar->setGeometry(ui->label_status_text->x(), ui->label_retweeted_text->y(),
                                   5, ui->label_retweeted_text->height());
        //picPosX=ui->label_status_text->x();
        //picPosY=ui->label_retweeted_text->y()+ui->label_retweeted_text->height()+10;
    }
    else{
        text_retweeted="";
        id_retweeted="";
        id_user_retweeted="";
        screen_name_user_retweeted="";
        profile_image_url_retweeted="";
        ui->label_retweeted_text->setVisible(false);
        ui->label_bar->setVisible(false);
    }

    setPictures();//设置图片位置
    this->adjustSize();//自动调整大小
    ui->label_divider->setGeometry(0, this->height()-10, ui->label_divider->width(), ui->label_divider->height()-10);
}

//设置图片位置
void View_Item_Status::setPictures(){
    if(pictureList.size()==1){//若只有一个图片
        QLabel * label=new QLabel(this);
        QPixmap pixmap;
        pixmap.load("cache_image/"+id_status+"_"+QString::number(0)+".jpg");
        label->setScaledContents(true);
        label->setPixmap(pixmap);

        if(isRepost){//是转发微博
            //if(pixmap.height()<210){
                label->setGeometry(ui->label_status_text->x(), ui->label_bar->y()+ui->label_bar->height()+10,
                                   ui->label_status_text->width(), ui->label_status_text->width()*pixmap.height()/(pixmap.width()+1));
            //}
            if(label->height()>210){
                label->setGeometry(ui->label_status_text->x(), ui->label_bar->y()+ui->label_bar->height()+10,
                                   210*label->width()/(label->height()+1), 210);
            }
        }
        else{//不是转发微博
            //if(pixmap.height()<210){
                label->setGeometry(ui->label_status_text->x(), ui->label_status_text->y()+ui->label_status_text->height()+10,
                                   ui->label_status_text->width(), ui->label_status_text->width()*pixmap.height()/(pixmap.width()+1));
            //}
            if(label->height()>210){
                label->setGeometry(ui->label_status_text->x(), ui->label_status_text->y()+ui->label_status_text->height()+10,
                                   210*label->width()/(label->height()+1), 210);
            }
        }
        labelList.push_back(label);
    }

    //有2-4个图片
    else if(1<pictureList.size() && pictureList.size()<=4){
        QLabel * label;
        QPixmap pixmap;
        std::list<QString>::iterator itor;
        int i=0;
        for(itor=pictureList.begin(); itor!=pictureList.end(); itor++, i++){
            if(isRepost){//是转发微博
                picPosX=ui->label_status_text->x()+(i%2)*(103+4);
                picPosY=ui->label_bar->y()+ui->label_bar->height()+10+(i/2)*(103+4);
            }
            else{//不是转发微博
                picPosX=ui->label_status_text->x()+(i%2)*(103+4);
                picPosY=ui->label_status_text->y()+ui->label_status_text->height()+10+(i/2)*(103+4);
            }

            label=new QLabel(this);
            pixmap.load("cache_image/"+id_status+"_"+QString::number(i)+".jpg");
            //QImage image;

            label->setScaledContents(true);
            label->setPixmap(pixmap);
            label->setGeometry(picPosX, picPosY, 103, 103);

            labelList.push_back(label);

        }
    }

    //有5-9个图片
    else if(4<pictureList.size() && pictureList.size()<=9){
        QLabel * label;
        QPixmap pixmap;
        std::list<QString>::iterator itor;
        int i=0;
        for(itor=pictureList.begin(); itor!=pictureList.end(); itor++, i++){
            if(isRepost){//是转发微博
                picPosX=ui->label_status_text->x()+(i%3)*(66+3);
                picPosY=ui->label_bar->y()+ui->label_bar->height()+10+(i/3)*(66+3);
            }
            else{//不是转发微博
                picPosX=ui->label_status_text->x()+(i%3)*(66+3);
                picPosY=ui->label_status_text->y()+ui->label_status_text->height()+10+(i/3)*(66+3);
            }

            label=new QLabel(this);
            pixmap.load("cache_image/"+id_status+"_"+QString::number(i)+".jpg");

            label->setScaledContents(true);
            label->setPixmap(pixmap);
            label->setGeometry(picPosX, picPosY, 68, 68);

            labelList.push_back(label);
        }
    }
    else{
        qDebug()<<"did not set the pictures. number is"<<pictureList.size();
    }
}

//返回微博id
QString View_Item_Status::getIdStatus(){
    return this->id_status;
}

//返回用户id
QString View_Item_Status::getIdUserStatus(){
    return this->id_user_status;
}

//返回文本内容
QString View_Item_Status::getTextStatus(){
    return this->text_status;
}

//返回用户名
QString View_Item_Status::getScreenNameUserStatus(){
    return this->screen_name_user_status;
}

//返回头像URL
QString View_Item_Status::getProfileImageUrlStatus(){
    return this->profile_image_url_status;
}

//返回是否是转发
bool View_Item_Status::getIsRepost(){
    return this->isRepost;
}

//返回转发内容
QString View_Item_Status::getTextRetweeted(){
    return this->text_retweeted;
}

//返回微博id
QString View_Item_Status::getIdRetweeted(){
    return this->id_retweeted;
}

//返回uid
QString View_Item_Status::getIdUserRetweeted(){
    return this->id_user_retweeted;
}

//返回用户名
QString View_Item_Status::getScreenNameUserRetweeted(){
    return this->screen_name_user_retweeted;
}

//返回图片URL
QString View_Item_Status::getProfileImageUrlRetweeted(){
    return this->profile_image_url_retweeted;
}

//返回图片list
std::list<QString> View_Item_Status::getPictureList(){
    return this->pictureList;
}

//词法分析
QString View_Item_Status::lexicalAnalysis(QString str){
    QString result="";
    while(str.length()){
        QString strLeft=str.left(1);
        if(strLeft=="@"){//判断是否有@
            for(int i=0; i<str.length() && str.at(i)!=':' && str.at(i)!=' ' && str.mid(i, 1)!="：" && str.mid(i, 1)!="，" && str.mid(i, 1)!="。"; i++){
                strLeft=str.left(i+1);
            }

            str.remove(0, strLeft.length());
            strLeft="<style> a {text-decoration: none} </style>  <a style='color: blue;' href = "+strLeft+"> "+strLeft+"</a>";

        }
        else if(strLeft=="h"){//判断是否有http
            strLeft=str.left(4);
            if(strLeft=="http"){
                for(int i=4; i<str.length() && str.at(i)!=' ' && str.at(i)!='\n'; i++){

                    strLeft=str.left(i+1);
                }

                str.remove(0, strLeft.length());
                strLeft="<style> a {text-decoration: none} </style>  <a style='color: red;' href = "+strLeft+"> 网页链接</a>";
            }
            str.remove(0, strLeft.length());
        }
        else if(strLeft=="#"){//判断是否有话题
            for(int i=1; i<str.length() && str.at(i)!='#'; i++){
                strLeft=str.left(i+1);
            }
            strLeft=str.left(strLeft.length()+1);
            str.remove(0, strLeft.length());
            strLeft="<style> a {text-decoration: none} </style>  <a style='color: green;' href = "+strLeft+"> "+strLeft+"</a>";
        }
        else{
            str.remove(0, strLeft.length());
        }
        result.append(strLeft);
    }

    return result;
}

//外部打开URL
void View_Item_Status::on_label_status_text_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

//外部代开URL
void View_Item_Status::on_label_retweeted_text_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

//鼠标点击事件
void View_Item_Status::mousePressEvent(QMouseEvent *event){
    //qDebug()<<"mousePressEvent";

    if(pictureList.size()==1){//若只有一个图片

        if((*labelList.begin())->x()<event->x() && event->x()<(*labelList.begin())->x()+(*labelList.begin())->width() &&
           (*labelList.begin())->y()<event->y() && event->y()<(*labelList.begin())->y()+(*labelList.begin())->height()){
            QWebView * webView = new QWebView;
            webView->load(QUrl(*pictureList.begin()));

            QPixmap pixmap("cache_image/"+id_status+"_"+QString::number(0)+".jpg");
            webView->resize(pixmap.width(), pixmap.height());
            webView->show();
        }
    }
    else if(pictureList.size()<=9 && pictureList.size()>1){//若有多个图片
        std::list<QLabel *>::iterator itor;
        std::list<QString>::iterator itor1;
        int i=0;
        for(itor=labelList.begin(), itor1=pictureList.begin(); itor!=labelList.end(); itor++, itor1++, i++){
            if(mouseInLabelRect(*itor, event->x(), event->y())){

                webView->load(QUrl(*itor1));
                QPixmap pixmap("cache_image/"+id_status+"_"+QString::number(i)+".jpg");
                webView->resize(pixmap.width(), pixmap.height());

                webView->show();
            }
        }
    }
}

//鼠标双击事件
void View_Item_Status::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug()<<"double click()";
    //emit getStatusesShow(this->id_status);
    emit getCommentsShow(this->id_status, this);

}

//鼠标按压并拖动事件
void View_Item_Status::mouseMoveEvent(QMouseEvent *event){

}

//判断鼠标是否在某一label上
bool View_Item_Status::mouseInLabelRect(QLabel *label, int x, int y){
    return label->x()<x && x<label->x()+label->width() &&
           label->y()<y && y<label->y()+label->height();
}

//Detail按钮被点击
void View_Item_Status::on_pushButton_Details_clicked()
{
    qDebug()<<"statuses/show";
    //emit getStatusesShow(this->id_status);
    emit getCommentsShow(this->id_status, this);
}

//转发按钮被点击
void View_Item_Status::on_pushButton_Retweeted_clicked()
{
    //qDebug()<<"statuses/repost";
    emit postStatusesRepost(this->id_status);
}

//回复按钮被点击
void View_Item_Status::on_pushButton_Reply_clicked()
{
    //qDebug()<<"comments/create";
    emit postCommentsCreate(this->id_status);
}

//显示右键菜单
void View_Item_Status::contextMenuEvent(QContextMenuEvent *event){
    //qDebug()<<"in menu()";
    popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}

//删除当前微博
void View_Item_Status::deleteCurWeibo(){
    qDebug()<<"delete current weibo";
    emit postStatusesDestroy(this->id_status, this->id_user_status);
}

//按钮隐藏
void View_Item_Status::setDetailsDisvisible(){
    ui->pushButton_Details->setVisible(false);
}

//淡化显示按钮
void View_Item_Status::gradientShowButtons(){

}

//淡化隐藏按钮
void View_Item_Status::gradientHideButtons(){

}
