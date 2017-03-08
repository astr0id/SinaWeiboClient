/*
 * 文件名：weibo.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件中声明了一个名为Weibo的类，即MVC模型中的Model。用于
 * 和新浪后台进行网络通信。
 */

#ifndef WEIBO_H
#define WEIBO_H

/*包含头文件*/

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QListWidget>
#include <QDateTime>
#include <QDebug>
#include <QDate>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSplashScreen>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QtGui>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCompleter>
#include <QStringList>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTextStream>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPainter>
#include <QPoint>
#include <QStyle>
#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QSslConfiguration>
#include <QWebView>
#include <QSslSocket>

#include "weibo_structure.h"
#include "weibo_url.h"

/*类声明*/

class Weibo : public QObject
{
    Q_OBJECT
public:
    explicit Weibo(QObject *parent = 0);

    QString getAccessToken();//获取access_token
    QString getRemindIn();//获取令牌生命周期
    QString getUid();//获取登录用户的uid
    void setAccessToken(QString _accessToken);//设置access_token
    void setRemindIn(QString _remind_in);//设置令牌生命周期
    void setUid(QString _uid);//设置登录用户的uid

    void postOauth2AccessToken(QString);//通过网络请求，获取access_token

    void getRemindUnreadCount();//获取未读信息数

    void postNewWeibo(QString);//发新微博
    void getStatusesPublicTimeline();//获取公共时间线
    void getStatusesFriendsTimeline(QString count);//获取主时间线
    void getStatusesUserTimeline(QString count);//获取用户的时间线
    void getStatusesMentions(QString count);//获取@时间线
    void getStatusesShow(QString id);//获取某一微博的内容
    void postStatusesRepost(QString id, QString status);//转发一条微博
    void postStatusesDestroy(QString id);//删除一条微博
    void postStatusesUpload(QString status, QString picturePath);//发布一条带图片的微博

    void searchUser(QString);//搜索用户

    void getUserInfo();//获取用户信息

    void getCommentsShow(QString id);//获取评论列表
    void getCommentsByMe();//获取登录用户发出的评论时间线
    void getCommentsToMe();//获取登录用户收到的评论时间线
    void getCommentsTimeline();//获取用户的评论时间线
    void getCommentsMentions();//获取@登录用户的评论时间线
    void postCommentsCreate(QString comment, QString id);//写评论
    void postCommentsDestroy(QString cid);//删评论
    void postCommentsReply(QString cid, QString id, QString comment);//回复评论

    void getFriendshipsFriends(QString count);//获取关注人列表
    void getFriendshipsFollowers(QString count);//获取粉丝列表
    void postFriendshipsCreate(QString uid, QString screen_name);//关注一个用户
    void postFriendshipsDestroy(QString uid, QString screen_name);//取消关注一个用户

    userInfoT userInfo;//登录用户的个人信息
    std::list<userSearchResultT> userSearchResultList;//用户搜索结果
    std::list<friendsInfoT> friendsList;//用户列表

    statusInfoT singleStatusInfo;//单个微博的内容信息
    std::list<statusInfoT> statusesList;//微博列表

    std::list<commentInfoT> commentsList;//评论列表

    unreadCountT unreadCount;//未读信息数量

signals:
    void displayMessage(QString);
    void moveList(int);

    void userShow();//返回用户的个人信息
    void returnSearchUsers();//返回搜索结果

    void returnRemindUnreadCount(unreadCountT unreadInfo);//返回未读消息的信息

    //返回Statuses接口的数据
    void returnStatusesFriendsTimeline();
    void returnStatusesUserTimeline();
    void returnStatusesMentions();
    void returnStatusesShow();
    void returnStatusesRepost();
    void returnStatusesDestroy();
    void returnStatusesUpload();

    //返回评论接口的数据
    void returnCommentsShow();
    void returnCommentsByMe();
    void returnCommentsToMe();
    void returnCommentsTimeline();
    void returnCommentsMentions();
    void returnCommentsCreate();
    void returnCommentsDestroy();
    void returnCommentsReply();

    //返回好友接口的数据
    void returnFriendshipsFriends();
    void returnFriendshipsFollowers();
    void returnFriendshipsCreate();
    void returnFriendshipsDestroy();

public slots:

private slots:
    void requestResult(QNetworkReply *);//处理返回结果

private:
    //网络请求变量
    QNetworkAccessManager * manager;
    QNetworkRequest * request;
    QNetworkReply * reply;

    WeiboStatus nowStatus;//记录当前网络请求的接口

    //保存登录信息
    QString accessToken;
    QString remind_in;
    double expires_in;
    QString uid;

    void parseRemindUnreadCount(QString json);//解析未读消息接口的返回结果

    void parseOauthAccessToken(QString);//解析access_token的返回结果
    void parseGetUsersShow(QString);//解析用户个人信息的返回结果
    void parseSearchUser(QString);//解析搜索用户的返回结果

    //解析Statuses接口的返回内容
    void parseStatusesFriendsTimeline(QString json);
    void parseStatusesUserTimeline(QString json);
    void parseStatusesMentions(QString json);
    void parseStatusesShow(QString json);
    void parseStatusesRepost(QString json);
    void parseStatusesDestroy(QString json);
    void parseStatusesUpload(QString json);
    void parseStatusesStatusInfo(QString json);
    void parseStatusesSingleStatus(QString json);
    void parseStatusesRepostInfo(QString json);

    //解析评论接口的返回内容
    void parseCommentsShow(QString json);
    void parseCommentsByMe(QString json);
    void parseCommentsToMe(QString json);
    void parseCommentsTimeline(QString json);
    void parseCommentsMentions(QString json);
    void parseCommentsCreate(QString json);
    void parseCommentsDestroy(QString json);
    void parseCommentsReply(QString json);
    void parseCommentsInfo(QString json);
    void parseCommentsInfoLikeStatus(QString json);

    //解析好友接口的返回内容
    void parseFriendshipsFriends(QString json);
    void parseFriendshipsFollowers(QString json);
    void parseFriendshipsCreate(QString json);
    void parseFriendshipsDestroy(QString json);

};

#endif // WEIBO_H
