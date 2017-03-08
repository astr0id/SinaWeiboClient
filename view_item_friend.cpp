/*
 * 文件名：view_item_friend.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件中实现了View_Item_Friend类中的所有成员函数。
 */

#include "view_item_friend.h"
#include "ui_view_item_friend.h"

//构造函数
View_Item_Friend::View_Item_Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::View_Item_Friend)
{
    ui->setupUi(this);
}

//析构函数
View_Item_Friend::~View_Item_Friend()
{
    delete ui;
}

//设置表项内容
void View_Item_Friend::setItemContent(QString _id, QString _screen_name, QString _description,
                                      QString _profile_image_url, QString _button_content){
    //设置成员变量
    id=_id;
    screen_name=_screen_name;
    description=_description;
    profile_image_url=_profile_image_url;

    //设置头像
    QPixmap head;
    QPixmap mask;

    head.load(profile_image_url);
    mask.load(mask_path);
    ui->label_avatar->setScaledContents(true);
    ui->label_avatar->setPixmap(head);
    ui->label_avatar->setMask(mask.mask());//设置遮罩

    //设置文字内容
    ui->label_screen_name->setText(screen_name);
    ui->label_description->setWordWrap(true);
    if(description==""){
        ui->label_description->setText("暂无介绍");
    }
    else{

        ui->label_description->setText(description);
    }

    ui->label_description_2->setVisible(false);

    ui->pushButton_follow->setText(_button_content);
}

//获取显示用户的id
QString View_Item_Friend::getItemIID(){
    return this->id;
}

//获取显示用户的昵称
QString View_Item_Friend::getItemScreenName(){
    return this->screen_name;
}






