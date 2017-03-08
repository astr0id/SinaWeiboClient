/*
 * 文件名：view_mainwindow.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件定义了一个名为View_MainWindow的类，用于显示微博的
 * 主界面。
 */

#ifndef VIEW_MAINWINDOW_H
#define VIEW_MAINWINDOW_H

/*包含头文件*/

#include <QMainWindow>

#include "macro.h"
#include "weibo_structure.h"

#include "view_item_friend.h"
#include "ui_view_item_friend.h"
#include "view_item_status.h"
#include "ui_view_item_status.h"
#include "view_item_comment.h"
#include "ui_view_item_comment.h"

/*指定命名空间*/

namespace Ui {
class View_MainWindow;
}

/*类声明*/

class View_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit View_MainWindow(QWidget *parent = 0);
    ~View_MainWindow();


    QString access_token;//保存access_token

    void displayUserProfile(userInfoT);//显示登录用户的个人信息
    void displaySearchUsers(std::list<userSearchResultT>);//显示搜索用户的结果

    void displayFriendsList(std::list<friendsInfoT> friendsList, QString buttonContent);//显示关注人列表

    void displayStatusesFriendsTimeline(std::list<statusInfoT> statusesList, bool slide);//显示主时间线
    void displayStatusesUserTimeline(std::list<statusInfoT> statusesList);//显示登录用户自己的微博时间线
    void displayStatusesMentions(std::list<statusInfoT> statusesList);//显示@登录用户的时间轴

    void displayCommentsShow(std::list<commentInfoT> commentsList);//显示评论列表
    void displayCommentsMentions(std::list<statusInfoT> statusesList, bool slide);//显示评论中@登录用户的时间线
    void displayCommentsByMe(std::list<statusInfoT> statusesList);//显示用户发出评论的时间线
    void displayCommentsToMe(std::list<statusInfoT> statusesList);//显示用户收到评论的时间线

    void clearAllListWidgets();//清除所有的列表内容

    void gradientShow();//淡化显示
    void gradientHide();//淡化隐藏

    void setTrayPictureOn();//设置托盘图片有新微博
    void setTrayPictureOff();//取消显示

    void showUnreadPot();//显示标识未读信息的圆点
    void hideUnreadPot();//隐藏标识未读信息的圆点

    void moveList(int list);//滑动列表
    void backToMainTimeline();//返回主时间线

signals:
    void logOut();//注销

    void displayMessage(QString);//显示消息

    void writeWeibo();//写微博
    void searchUser(QString);//搜索用户

    void getFriendshipsFriends(QString count);//获取关注人列表
    void getFriendshipsFollowers(QString count);//获取粉丝列表

    void getStatusesFriendsTimeline();//获取主时间线
    void getStatusesUserTimeline();//获取用户自己的时间线
    void getStatusesMentions();//获取@用户的微博时间线
    void getStatusesShow(QString);//获取某一微博的内容
    void postStatusesDestroy(QString, QString);//删除某一微博
    void postStatusesRepost(QString);//转发某条微博

    void getCommentsShow(QString);//获取某微博的评论列表
    void getCommentsMentions();//获取评论中提到登录用户的列表
    void getCommentsByMe();//获取用户发出评论的列表
    void getCommentsToMe();//获取用户收到评论的列表
    void postCommentsCreate(QString);//发出评论
    void postCommentsDestroy(QString, QString);//删除评论
    void postCommentsReply(QString, QString);//回复评论

    void test();//测试

private slots:
    //按钮点击的槽函数
    void on_pushButton_clicked();

    void on_timeline_clicked();

    void on_comments_clicked();

    void on_mentions_clicked();

    void on_messages_clicked();

    void on_profile_clicked();

    void on_search_clicked();

    void on_postWeibo_clicked();

    void on_buttonMyTimeline_clicked();

    void on_buttonMyMentions_clicked();

    void on_buttonMyProfile_clicked();

    void on_buttonMyFavourites_clicked();

    void on_searchLineEdit_returnPressed();//开始搜索

    void on_listWidget_FriendsList_itemClicked(QListWidgetItem *item);//用户列表的表项被点击

    void on_pushButton_profile_following_clicked();

    void on_pushButton_profile_follower_clicked();

    void on_listWidget_FriendsTimeline_itemClicked(QListWidgetItem *item);//时间线的列表表项被点击

    void on_pushButton_FriendsList_Back_clicked();

    void on_label_test_linkActivated(const QString &link);//测试label

    void on_pushButton_RefreshFriendsTimeline_clicked();

    void on_pushButton_Mentions_clicked();

    void on_tabWidget_Comments_tabBarClicked(int index);

    void on_listWidget_FriendsTimeline_itemDoubleClicked(QListWidgetItem *item);//时间线列表表项被双击

    void statusesShow(QString id);//显示某一微博

    void commentsShow(QString id, View_Item_Status * _widget);//显示评论列表

    void commentsCreate(QString id);//发出评论

    void commentsReply(QString id_comment, QString id_status);//回复评论

    void statusesDestroy(QString id, QString id_user);//删除微博

    void commentsDestroy(QString id_comment, QString id_user);//删除评论

    void statusesRepost(QString id);//转发微博

    void on_pushButton_Status_Back_clicked();

    void on_actionClear_Chche_triggered();

    void on_actionLog_Out_triggered();

    void on_postWeibo_2_clicked();

private:    
    Ui::View_MainWindow *ui;

    QMovie * movieFriendsTimeline;//主时间线的gif
    QMovie * movieMentionsTimeline;//@时间线的gif

    void paintEvent( QPaintEvent* ) ;//绘制透明区域
    void buttonIconController(int);
    void buttonIconControllerProfile(int);

    //托盘系统
    QSystemTrayIcon * systemTray;//用指针传递，把最小化到托盘功能改到WindowManager
    QAction * actionTimeline;
    QAction * actionMentions;
    QAction * actionComments;
    QAction * actionMessages;
    QAction * actionFollowers;
    QAction * actionNewWeibo;
    QMenu * trayMenu;

    //初始化托盘
    void initSystemTray();
    void deleteSystemTray();
    void createActions();
    void createSystemTray();
    void setTraySlots();

    //各列表的备忘录，用于记录指针，释放堆区空间
    std::map<View_Item_Friend *, QListWidgetItem *> memoFriendsList;
    std::map<QListWidgetItem *, View_Item_Friend *> memoFriendsListReverse;
    std::map<View_Item_Status *, QListWidgetItem *> memoStatusesTimeline;
    std::map<QListWidgetItem *, View_Item_Status *> memoStatusesTimelineReverse;

    View_Item_Status * memoCommentsStatusWidget;
    QListWidgetItem * memoCommentsStatusItem;
    std::map<View_Item_Comment *, QListWidgetItem *> memoCommentsShow;
    std::map<QListWidgetItem *, View_Item_Comment *> memoCommentsShowReverse;

    void setStatusesList(std::list<statusInfoT> statusesList, QListWidget * listWidget);//设置时间线

    void clearStatusesListWidgets(QListWidget * listWidget);//清除微博时间线
    void clearCommentsListWidgets();//清除评论列表
    void clearFriendsListWidgets();//清除用户列表

};

#endif // VIEW_MAINWINDOW_H
