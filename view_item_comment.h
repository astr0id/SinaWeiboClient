/*
 * 文件名：view_item_comment.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该类定义了一个名为View_Item_Comment的类，用于显示评论
 * 列表中的表项。
 */

#ifndef VIEW_ITEM_COMMENT_H
#define VIEW_ITEM_COMMENT_H

/*包含头文件*/

#include <QWidget>
#include <QDebug>
#include <QDesktopServices>
#include <QPixmap>
#include <QBitmap>

#include <QUrl>
#include <QMenu>

/*指定命名空间*/

namespace Ui {
class View_Item_Comment;
}

/*类声明*/

class View_Item_Comment : public QWidget
{
    Q_OBJECT

public:
    explicit View_Item_Comment(QWidget *parent = 0);
    ~View_Item_Comment();

    void setItemContent(QString _id_comment, QString _id_status,//设置表项内容
                        QString _id_user, QString _screen_name_comment,
                        QString _screen_name_status, QString _profile_image_url,
                        QString _text_comment, QString _text_status,
                        bool _isReply, QString _text_reply,
                        QString _screen_name_reply);

signals:
    void postCommentsDestroy(QString, QString);//删除评论
    void postCommentsReply(QString, QString);//回复评论

private slots:
    void on_pushButton_Reply_clicked();//回复按钮被按下

    void deleteCurComment();//删除当前评论

    void on_label_reply_linkActivated(const QString &link);//网页链接被点击

    void on_label_textComment_linkActivated(const QString &link);//文字内容被点击

private:
    Ui::View_Item_Comment *ui;

    void contextMenuEvent(QContextMenuEvent * event);//菜单
    QMenu* popMenu;
    QAction * deleteWeibo;

    //保存评论内容的变量
    QString id_comment;
    QString id_status;
    QString id_user;
    QString screen_name_comment;
    QString screen_name_status;
    QString profile_image_url;
    QString text_comment;
    QString text_status;
    bool isReply;
    QString text_reply;
    QString screen_name_reply;

    QString lexicalAnalysis(QString str);//词法分析，找出用户名和网页链接

    void mousePressEvent(QMouseEvent * event);//鼠标点击事件
};

#endif // VIEW_ITEM_COMMENT_H
