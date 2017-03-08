/*
 * 文件名：view_item_friend.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件定义了一个名为View_Item_Friend的类，用于显示关注
 * 人或粉丝列表的表项。
 */

#ifndef VIEW_ITEM_FRIEND_H
#define VIEW_ITEM_FRIEND_H

/*包含头文件*/

#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QBitmap>
#include <QImage>

/*常量定义*/

const QString mask_path=":/image/image/mask";

/*指定命名空间*/

namespace Ui {
class View_Item_Friend;
}

/*类声明*/

class View_Item_Friend : public QWidget
{
    Q_OBJECT

public:
    explicit View_Item_Friend(QWidget *parent = 0);
    ~View_Item_Friend();

    void setItemContent(QString _id, QString _screen_name, QString _description,//设置表项内容
                        QString _profile_image_url, QString _button_content);
    QString getItemIID();//获取id
    QString getItemScreenName();//获取昵称

private:
    Ui::View_Item_Friend *ui;

    //保存表项内容的变量
    QString id;
    QString screen_name;
    QString description;
    QString profile_image_url;
};

#endif // VIEW_ITEM_FRIEND_H
