/*
 * 文件名：view_item_status.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该类定义了一个名为View_Item_Status的类，用于显示微博
 * 时间线列表的表项。
 */

#ifndef VIEW_ITEM_STATUS_H
#define VIEW_ITEM_STATUS_H

/*包含头文件*/

#include <QWidget>
#include <QLabel>
#include <QObject>
#include <QString>
#include <QPixmap>
#include <QBitmap>
#include <QImage>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMouseEvent>
#include <QWebView>
#include <QMenu>
#include <QCursor>

#include <list>
#include <iterator>

/*常量定义*/

const QString mask_path_status=":/image/image/mask";

/*指定命名空间*/

namespace Ui {
class View_Item_Status;
}

/*类声明*/

class View_Item_Status : public QWidget
{
    Q_OBJECT

public:
    explicit View_Item_Status(QWidget *parent = 0);
    ~View_Item_Status();

    void setItemContent(QString _id_status, QString _id_user_status,//设置表项内容
                        QString _text_status, QString _screen_name_user_status,
                        QString _profile_image_url_status, bool _isRepost,
                        QString _text_retweeted, QString _id_retweeted,
                        QString _id_user_retweeted, QString _screen_name_user_retweeted,
                        QString _profile_image_url_retweeted, std::list<QString> pictureList,
                        bool isNew);
    QString getIdStatus();//获取表项的微博id
    QString getIdUserStatus();//获取表项的用户id
    QString getTextStatus();//获取表项的微博内容
    QString getScreenNameUserStatus();//获取表项的用户名
    QString getProfileImageUrlStatus();//获取表项用户的头像链接
    bool getIsRepost();//返回是否时转发微博
    QString getTextRetweeted();//获取转发的文字内容
    QString getIdRetweeted();//获取转发微博的id
    QString getIdUserRetweeted();//获取转发用户的id
    QString getScreenNameUserRetweeted();//获取转发用户的用户名
    QString getProfileImageUrlRetweeted();//获取转发用户的头像链接
    std::list<QString> getPictureList();//获取图片列表

    void setDetailsDisvisible();//设置细节内容不可见

signals:
    void getStatusesShow(QString);//单独显示微博内容

    void postStatusesDestroy(QString, QString);//删除微博

    void getCommentsShow(QString, View_Item_Status *);//显示评论列表

    void postStatusesRepost(QString);//转发该微博

    void postCommentsCreate(QString);//在该微博下发表评论

private slots:
    void on_label_status_text_linkActivated(const QString &link);//微博内容被点击

    void on_label_retweeted_text_linkActivated(const QString &link);//转发内容被点击

    void deleteCurWeibo();//删除当前微博

    void on_pushButton_Details_clicked();//单独显示微博内容的按钮被按下

    void on_pushButton_Retweeted_clicked();//转发按钮被按下

    void on_pushButton_Reply_clicked();//评论按钮被按下

private:
    Ui::View_Item_Status *ui;

    void setPictures();//设置微博配图

    int picPosX;//图片x坐标
    int picPosY;//图片y坐标

    QWebView * webView;//网页浏览器

    void mousePressEvent(QMouseEvent * event);//鼠标点击事件
    void mouseDoubleClickEvent(QMouseEvent * event);//鼠标双击事件
    void mouseMoveEvent(QMouseEvent * event);//按压并移动鼠标事件

    bool mouseInLabelRect(QLabel * label, int x, int y);//判断鼠标是否在某一区域内

    void contextMenuEvent(QContextMenuEvent * event);//设置右键菜单
    QMenu* popMenu;
    QAction * deleteWeibo;

    //保存表项内容的变量
    QString id_status;
    QString id_user_status;
    QString text_status;
    QString screen_name_user_status;
    QString profile_image_url_status;
    bool isRepost;
    QString text_retweeted;
    QString id_retweeted;
    QString id_user_retweeted;
    QString screen_name_user_retweeted;
    QString profile_image_url_retweeted;

    std::list<QString> pictureList;//图片列表
    std::list<QLabel *> labelList;//图片对应的qlabel列表

    QString lexicalAnalysis(QString str);//词法分析，标示出用户名和网页链接

    void gradientShowButtons();//淡化显示出按钮
    void gradientHideButtons();//淡化隐藏按钮



};

#endif // VIEW_ITEM_STATUS_H
