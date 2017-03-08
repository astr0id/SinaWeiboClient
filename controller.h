/*
 * 文件名：controller.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    Controller类即MVC中的controller，用于实现程序的控制。
 * 同时也完成 本地数据的保存和网络图片缓存的功能。
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

/*包含头文件*/

#include "weibo.h"

#include "macro.h"

#include "view_mainwindow.h"
#include "ui_view_mainwindow.h"
#include "view_postweibo.h"
#include "ui_view_postweibo.h"
#include "view_oauth2.h"
#include "ui_view_oauth2.h"

#include "view_dialog_message.h"
#include "ui_view_dialog_message.h"

/*类声明*/

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);
    ~Controller();

    void setAppStyle(QStyle * style);//设置应用的风格

    bool notPastDue();//判断access_token是否过期

    void showOauth2();//显示oauth2界面

    void setApplicationIcon(QApplication * application);//设置应用的icon
    void splashAppear();//显示欢迎界面


public slots:
    void getAccessToken(QUrl);//获取access_token

    void searchUser(QString);//搜索用户
    void displayUserInfo();//显示登录用户信息

    void returnRemindUnreadCount(unreadCountT unreadInfo);//返回未读信息数量

    void test();//测试函数

private slots:
    void moveList(int);
    void setInHttping(bool);

    void userLogOut();//用户注销

    void displayMessage(QString message);//显示警告、提醒等信息

    void writeNewWeibo();//写新微博
    void writeNewRepost(QString);//转发微博
    void writeNewComment(QString id);//写新评论
    void writeNewReply(QString id_comment, QString id_status);//写回复

    void postNewWeibo(QString);//发布微博
    void postStatusesRepost(QString);//转发微博

    void returnSearchUsers();//返回用户搜索结果

    void returnFriendshipsFriends();//返回关注人列表
    void returnFriendshipsFollowers();//返回粉丝列表

    void getFriendshipsFriends(QString count);//获取关注人列表
    void getFriendshipsFollowers(QString count);//获取粉丝列表

    void getStatusesFriendsTimeline();//获取登录用户的主时间线
    void getStatusesUserTimeline();//获取登录用户自己的时间线
    void getStatusesMentions();//获取所有@登录用户的微博时间线
    void getStatusesShow(QString id);//显示一条微博的内容
    void postStatusesDestroy(QString, QString);//删除一条微博
    void postStatusesUpload(QString, QString);//发布一条带图片的微博

    void getCommentsShow(QString id);//获取评论列表内容
    void getCommentsMentions();//获取提到登录用户的微博
    void getCommentsToMe();//获取用户收到的评论列表
    void getCommentsByMe();//获取用户发出的评论列表
    void postCommentsCreate(QString comment);//写一条新评论
    void postCommentsDestroy(QString id_comment, QString id_user);//删除一条评论
    void postCommentsReply(QString reply);//回复一条评论

    void returnStatusesFriendsTimeline();//返回登录用户的主时间线
    void returnStatusesUserTimeline();//返回登录用户自己的时间线
    void returnStatusesMentions();//返回@登录用户的微博时间线
    void returnStatusesShow();//返回某一微博的内容

    void returnCommentsShow();//返回一条微博的评论列表
    void returnCommentsMentions();//返回提到登录用户的评论时间线
    void returnCommentsToMe();//返回登录用户收到的评论时间线
    void returnCommentsByMe();//返回登录用户发出的评论时间线

    void unreadWatcher();//监视线程函数

private:
    Weibo * weibo;//微博类的指针，Model

    View_Dialog_Message * messageDialog;//显示提示或警告信息的对话框

    QStyle * appStyle;//应用风格

    bool inHttping;//是否有网络请求正在进行，用于网络请求互斥

    bool haveUnread;//是否有未读信息

    void saveIniFile();//保存配置文件信息

    View_MainWindow * viewMainWindow;//主界面
    View_PostWeibo * viewPostWeibo;//发布微博界面
    View_Oauth2 * viewOauth2;//登录界面

    QThread * watcherThread;//监视线程

    QTimer * timer;//定时器，用于监视线程

    QDir * cacheDir;//缓存文件夹

    QString curUserId;//当前登录用户的uid

    QString idRepostOrCommentOrReply;//转发、评论或回复的微博的id
    QString idStatusForReply;//评论微博的id

    void hideOauth2();//隐藏登录界面
    void showMainWindow();//显示主界面
    void hideMainWindow();//隐藏主界面
    void showWriteWeibo();//显示发微博界面
    void hideWriteWeibo();//隐藏发微博界面

    void setConnections();//设置链接

    void imageDownloader(QString url, QString fileName);//下载网页图片
    QString urlChangeThumbnailToBmiddle(QString url);//url转换
    void cacheImages(std::list<statusInfoT> &statusesList);//缓存图片

    void createDatabase();//创建数据库
    bool dbIsExist(QString dbName);//数据库是否已存在
    bool dbOpenSpecified(QSqlDatabase &db, QString dbName);//打开特定的数据库
    void saveTimeline(QString table, std::list<statusInfoT> statusesList);//保存所有的时间线
    std::list<statusInfoT> recoverTimeline(QString table);//恢复所有的时间线
    void clearDababase();//清除数据库内容


};

#endif // CONTROLLER_H
