/*
 * 文件名：view_traysystem.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了系统托盘的功能。属于View_MainWindow类。
 */

#include <view_mainwindow.h>
#include <ui_view_mainwindow.h>

//初始化托盘
void View_MainWindow::initSystemTray(){
    createActions();
    createSystemTray();
    setTraySlots();
}

//删除托盘，用于析构函数
void View_MainWindow::deleteSystemTray(){
    delete actionTimeline;
    delete actionMentions;
    delete actionComments;
    delete actionMessages;
    delete actionFollowers;
    delete actionNewWeibo;
    delete trayMenu;
    delete systemTray;
}

//托盘显示有未读信息
void View_MainWindow::setTrayPictureOn(){
    systemTray->setIcon(QIcon(":/image/image/statusbar_on"));
}

//托盘显示无未读信息
void View_MainWindow::setTrayPictureOff(){
    systemTray->setIcon(QIcon(":/image/image/statusbar_off"));
}

//创建托盘的点击动作
void View_MainWindow::createActions(){
    actionTimeline = new QAction(this);
    actionMentions = new QAction(this);
    actionComments = new QAction(this);
    actionMessages = new QAction(this);
    actionFollowers = new QAction(this);
    actionNewWeibo = new QAction(this);

    //设置icon
    actionTimeline->setIcon(QIcon(":/image/image/menu-timeline"));
    actionMentions->setIcon(QIcon(":/image/image/menu-mentions"));
    actionComments->setIcon(QIcon(":/image/image/menu-comments"));
    actionMessages->setIcon(QIcon(":/image/image/menu-messages"));
    actionFollowers->setIcon(QIcon(":/image/image/menu-followers"));

    //设置文字
    actionTimeline->setText(QString("  Timeline                    "));
    actionMentions->setText(QString("  Mentions                    "));
    actionComments->setText(QString("  Comments                    "));
    actionMessages->setText(QString("  Messages                    "));
    actionFollowers->setText(QString("  Followers                    "));
    actionNewWeibo->setText(QString("        New Weibo                    "));

    trayMenu = new QMenu(this);

    //添加动作
    trayMenu->addAction(actionTimeline);
    trayMenu->addAction(actionMentions);
    trayMenu->addAction(actionComments);
    trayMenu->addAction(actionMessages);
    trayMenu->addAction(actionFollowers);
    trayMenu->addSeparator();
    trayMenu->addAction(actionNewWeibo);
}

//创建系统托盘
void View_MainWindow::createSystemTray(){
    systemTray = new QSystemTrayIcon();
    systemTray->setIcon(QIcon(":/image/image/statusbar_off"));
    systemTray->setContextMenu(trayMenu);
    systemTray->show();
}

//设置槽函数
void View_MainWindow::setTraySlots(){

    connect(actionTimeline, SIGNAL(triggered()), this, SLOT(on_timeline_clicked()));
    connect(actionMentions, SIGNAL(triggered()), this, SLOT(on_mentions_clicked()));
    connect(actionComments, SIGNAL(triggered()), this, SLOT(on_comments_clicked()));
    connect(actionMessages, SIGNAL(triggered()), this, SLOT(on_messages_clicked()));
    connect(actionFollowers, SIGNAL(triggered()), this, SLOT(on_pushButton_profile_follower_clicked()));
    connect(actionNewWeibo, SIGNAL(triggered()), this, SLOT(on_postWeibo_clicked()));

}


