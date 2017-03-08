/*
 * 文件名：view_item_comment.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了View_Item_Comment类的全部成员函数。
 */

#include "view_item_comment.h"
#include "ui_view_item_comment.h"

//构造函数
View_Item_Comment::View_Item_Comment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::View_Item_Comment)
{
    ui->setupUi(this);

    popMenu = new QMenu(this);
    deleteWeibo = new QAction("删除", this);
    popMenu->addAction(deleteWeibo);
    connect(deleteWeibo, SIGNAL(triggered()), this, SLOT(deleteCurComment()));
}

//析构函数
View_Item_Comment::~View_Item_Comment()
{
    delete ui;
}

//设置item的内容
void View_Item_Comment::setItemContent(QString _id_comment, QString _id_status,
                                       QString _id_user, QString _screen_name_comment,
                                       QString _screen_name_status, QString _profile_image_url,
                                       QString _text_comment, QString _text_status,
                                       bool _isReply, QString _text_reply,
                                       QString _screen_name_reply){
    //设置成员变量内容
    id_comment=_id_comment;
    id_status=_id_status;
    id_user=_id_user;
    screen_name_comment=_screen_name_comment;//
    screen_name_status=_screen_name_status;
    profile_image_url=_profile_image_url;
    text_comment=_text_comment;//
    text_status=_text_status;
    isReply=_isReply;
    text_reply=_text_reply;
    screen_name_reply=_screen_name_reply;

    //设置头像
    QPixmap head;
    QPixmap mask;
    head.load(profile_image_url);
    mask.load(":/image/image/mask");
    ui->label_avatar->setScaledContents(true);
    ui->label_avatar->setPixmap(head);
    ui->label_avatar->setMask(mask.mask());

    text_comment=lexicalAnalysis(text_comment);//原始文本进行词法分析，找出用户名和网页链接
    ui->label_textComment->setWordWrap(true);
    ui->label_textComment->setText(text_comment);
    ui->label_textComment->adjustSize();

    ui->label_screenNameComment->setText(screen_name_comment);

    if(isReply){//若是含有回复的评论
        ui->label_reply->setVisible(true);
        ui->label_bar->setVisible(true);
        ui->label_reply->setWordWrap(true);
        ui->label_reply->setText(this->lexicalAnalysis("@"+screen_name_reply+"："+text_reply));
        ui->label_reply->adjustSize();
        ui->label_reply->setGeometry(ui->label_textComment->x()+10, ui->label_textComment->y()+ui->label_textComment->height()+10,
                                     ui->label_reply->width(), ui->label_reply->height());
        ui->label_bar->setGeometry(ui->label_textComment->x(), ui->label_reply->y(),
                                   5, ui->label_reply->height());
    }
    else{//不是回复的评论，隐藏控件
        text_reply="";
        screen_name_reply="";
        ui->label_bar->setVisible(false);
        ui->label_reply->setVisible(false);
    }

    //调整大小
    this->adjustSize();
    ui->label_divider->setGeometry(0, this->height()-10, ui->label_divider->width(), ui->label_divider->height()-10);
}

//词法分析，识别话题、用户名和网页链接
QString View_Item_Comment::lexicalAnalysis(QString str){
    QString result="";
    while(str.length()){
        QString strLeft=str.left(1);
        if(strLeft=="@"){//是否有@
            for(int i=0; i<str.length() && str.at(i)!=':' && str.at(i)!=' ' && str.mid(i, 1)!="：" && str.mid(i, 1)!="，" && str.mid(i, 1)!="。"; i++){
                strLeft=str.left(i+1);
            }
            str.remove(0, strLeft.length());
            strLeft="<style> a {text-decoration: none} </style>  <a style='color: blue;' href = "+strLeft+"> "+strLeft+"</a>";
        }
        else if(strLeft=="h"){//是否有http
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
        else if(strLeft=="#"){//是否有话题
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

//鼠标点击事件
void View_Item_Comment::mousePressEvent(QMouseEvent *event){

}

//要回复本条评论
void View_Item_Comment::on_pushButton_Reply_clicked()
{
    emit postCommentsReply(this->id_comment, this->id_status);
}

//设置右键菜单
void View_Item_Comment::contextMenuEvent(QContextMenuEvent *event){
    popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}

//删除当前评论
void View_Item_Comment::deleteCurComment(){
    emit postCommentsDestroy(this->id_comment, this->id_user);
}

//点击了回复评论中的网页链接
void View_Item_Comment::on_label_reply_linkActivated(const QString &link)
{
    //qDebug()<<link;
    QDesktopServices::openUrl(QUrl(link));
}

//点击了评论中的网页链接
void View_Item_Comment::on_label_textComment_linkActivated(const QString &link)
{
    //qDebug()<<link;
    QDesktopServices::openUrl(QUrl(link));
}
