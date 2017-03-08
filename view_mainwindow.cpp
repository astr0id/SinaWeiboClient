/*
 * 文件名：view_mainwindow.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了View_MainWindow类的全部成员函数。
 */

#include "view_mainwindow.h"
#include "ui_view_mainwindow.h"

//构造函数
View_MainWindow::View_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View_MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setVisible(false);//测试按钮不可见

    buttonIconController(0);
    ui->PageStack->setCurrentIndex(0);
    on_buttonMyProfile_clicked();

    initSystemTray();

    memoCommentsStatusItem=NULL;
    memoCommentsStatusWidget=NULL;

    //list初始化
    ui->listWidget_FriendsTimeline->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget_CommentsRecieved->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget_CommentsSend->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget_FriendsList->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget_Mentions->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget_MentionsProfile->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget_StatusShow->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget_UserTimeline->setAttribute(Qt::WA_MacShowFocusRect, false);

    //设置GIF
    movieFriendsTimeline=new QMovie(":/image/image/7.gif");
    ui->label_loading_friendstimeline->setMovie(movieFriendsTimeline);
    ui->label_loading_friendstimeline->setScaledContents(true);
    movieMentionsTimeline=new QMovie(":/image/image/8.gif");
    ui->label_loading_mentions->setMovie(movieMentionsTimeline);
    ui->label_loading_mentions->setScaledContents(true);
}

//析构函数
View_MainWindow::~View_MainWindow()
{
    deleteSystemTray();

    delete ui;
}

//显示小蓝点
void View_MainWindow::showUnreadPot(){
    ui->label_unreadPot->setVisible(true);
}

//隐藏小蓝点
void View_MainWindow::hideUnreadPot(){
    ui->label_unreadPot->setVisible(false);
}

//查看时间线按钮被点击
void View_MainWindow::on_timeline_clicked()
{
    if(this->isHidden())
        this->gradientShow();
    buttonIconController(0);
    ui->PageStack->setCurrentIndex(0);
}

//评论时间线按钮被点击
void View_MainWindow::on_comments_clicked()
{
    if(this->isHidden())
        this->gradientShow();
    buttonIconController(1);
    ui->PageStack->setCurrentIndex(1);
}

//mentions按钮被点击
void View_MainWindow::on_mentions_clicked()
{
    if(this->isHidden())
        this->gradientShow();
    buttonIconController(2);
    ui->PageStack->setCurrentIndex(2);
}

//消息按钮被点击
void View_MainWindow::on_messages_clicked()
{
    if(this->isHidden())
        this->gradientShow();
    buttonIconController(3);
    ui->PageStack->setCurrentIndex(3);
}

//主页按钮被点击
void View_MainWindow::on_profile_clicked()
{
    buttonIconController(4);
    ui->PageStack->setCurrentIndex(4);
}

//搜索按钮被点击
void View_MainWindow::on_search_clicked()
{
    buttonIconController(5);
    ui->PageStack->setCurrentIndex(5);
}

void View_MainWindow::on_buttonMyTimeline_clicked()
{
    ui->ProfilePage->setCurrentIndex(0);
    buttonIconControllerProfile(0);
    emit getStatusesUserTimeline();

}

void View_MainWindow::on_buttonMyMentions_clicked()
{
    ui->ProfilePage->setCurrentIndex(1);
    buttonIconControllerProfile(1);
    emit getStatusesMentions();

}

void View_MainWindow::on_buttonMyFavourites_clicked()
{
    ui->ProfilePage->setCurrentIndex(2);
    buttonIconControllerProfile(2);
}

void View_MainWindow::on_buttonMyProfile_clicked()
{
    ui->ProfilePage->setCurrentIndex(3);
    buttonIconControllerProfile(3);
}

//显示用户个人资料
void View_MainWindow::displayUserProfile(userInfoT userInfo){

    //设置头像
    QPixmap head;
    QPixmap mask;

    head.load(userInfo.profile_image);
    mask.load(mask_path);
    ui->label_avaterLeftbar->setScaledContents(true);
    ui->label_avaterProfilePage->setScaledContents(true);
    ui->label_avaterLeftbar->setPixmap(head);
    ui->label_avaterProfilePage->setPixmap(head);
    ui->label_avaterLeftbar->setMask(mask.mask());
    ui->label_avaterProfilePage->setMask(mask.mask());

    ui->label_screenName->setText(userInfo.screen_name);//设置个人信息
    ui->label_location->setText(userInfo.location);

    //设置个人说明
    ui->label_description->setText(userInfo.description);
    ui->label_description->setWordWrap(true);
    ui->label_description->adjustSize();

    //设置位置信息
    ui->label_location2->setText(userInfo.location);
    ui->label_location2->adjustSize();
    ui->label_location2->setGeometry(ui->label_description->x(), ui->label_description->y()+ui->label_description->height()+10,
                                     ui->label_location2->width(), ui->label_location2->height());

    //设置性别
    if(userInfo.gender=="m"){
        ui->label_gender2->setText("Male");
    }
    else{
        ui->label_gender2->setText("Female");
    }
    ui->label_gender2->adjustSize();
    ui->label_gender2->setGeometry(ui->label_location2->x(), ui->label_location2->y()+ui->label_location2->height()+10,
                                     ui->label_gender2->width(), ui->label_gender2->height());
    ui->label_gender->setGeometry(ui->label_gender->x(), ui->label_gender2->y(),
                                  ui->label_gender->width(), ui->label_gender->height());

    //设置following和follower按钮
    ui->pushButton_profile_following->setText("Following      "+QString::number(userInfo.friends_count));
    ui->pushButton_profile_following->setGeometry(ui->label_gender->x(),
                                                  ui->label_gender2->y()+ui->label_gender2->height()+30,
                                                  ui->pushButton_profile_following->width(),
                                                  ui->pushButton_profile_following->height());
    ui->pushButton_profile_follower->setText( "Follower        "+QString::number(userInfo.followers_count));
    ui->pushButton_profile_follower->setGeometry(ui->label_gender->x(),
                                                  ui->pushButton_profile_following->y()+ui->pushButton_profile_following->height()+10,
                                                  ui->pushButton_profile_follower->width(),
                                                  ui->pushButton_profile_follower->height());


}

//清除所有的list
void View_MainWindow::clearAllListWidgets(){
    clearStatusesListWidgets(ui->listWidget_FriendsTimeline);
    clearStatusesListWidgets(ui->listWidget_Mentions);
    clearStatusesListWidgets(ui->listWidget_CommentsSend);
    clearStatusesListWidgets(ui->listWidget_CommentsRecieved);
    clearStatusesListWidgets(ui->listWidget_UserTimeline);
    clearStatusesListWidgets(ui->listWidget_MentionsProfile);
    clearCommentsListWidgets();
    clearFriendsListWidgets();
}

//清除主时间线
void View_MainWindow::clearStatusesListWidgets(QListWidget * listWidget){
    for(int i=listWidget->count()-1; i>=0; i--){
        QListWidgetItem * item=NULL;
        View_Item_Status * widget=NULL;
        item=listWidget->item(i);
        widget=memoStatusesTimelineReverse[item];
        memoStatusesTimeline.erase(memoStatusesTimeline.find(widget));
        memoStatusesTimelineReverse.erase(memoStatusesTimelineReverse.find(item));
        delete item;
        delete widget;
    }
    listWidget->clear();
}

//清除评论列表
void View_MainWindow::clearCommentsListWidgets(){

    for(int i=ui->listWidget_StatusShow->count()-1; i>0; i--){
        QListWidgetItem * item=NULL;
        View_Item_Comment * widget=NULL;
        item=ui->listWidget_StatusShow->item(i);
        widget=memoCommentsShowReverse[item];
        memoCommentsShow.erase(memoCommentsShow.find(widget));
        memoCommentsShowReverse.erase(memoCommentsShowReverse.find(item));
        delete item;
        delete widget;
    }

    delete memoCommentsStatusItem;
    delete memoCommentsStatusWidget;
    ui->listWidget_StatusShow->clear();
    memoCommentsStatusItem=NULL;
    memoCommentsStatusWidget=NULL;
}

//清除好友列表
void View_MainWindow::clearFriendsListWidgets(){
    for(int i=ui->listWidget_FriendsList->count()-1; i>=0; i--){
        QListWidgetItem * item=NULL;
        View_Item_Friend * widget=NULL;
        item=ui->listWidget_FriendsList->item(i);
        widget=memoFriendsListReverse[item];
        memoFriendsList.erase(memoFriendsList.find(widget));
        memoFriendsListReverse.erase(memoFriendsListReverse.find(item));
        delete item;
        delete widget;
    }
    ui->listWidget_FriendsList->clear();
}

//写新微博
void View_MainWindow::on_postWeibo_clicked()
{
    emit writeWeibo();
}

//写新微博
void View_MainWindow::on_postWeibo_2_clicked()
{
    emit writeWeibo();
}

//绘图事件，实现侧栏背景半透明
void View_MainWindow::paintEvent( QPaintEvent* )
{
    QPainter p(this );
    p.setCompositionMode( QPainter::CompositionMode_Clear );
    p.fillRect( 0, 0, 70, 687, Qt::SolidPattern );
}

//按钮颜色高亮控制器
void View_MainWindow::buttonIconController(int blue_num){
    if(blue_num==0) ui->timeline->setIcon(QIcon(":/image/image/sidebar-timeline@2x copy.png"));
    else ui->timeline->setIcon(QIcon(":/image/image/sidebar-timeline@2x.png"));
    if(blue_num==1) ui->comments->setIcon(QIcon(":/image/image/sidebar-comments@2x copy.png"));
    else ui->comments->setIcon(QIcon(":/image/image/sidebar-comments@2x.png"));
    if(blue_num==2) ui->mentions->setIcon(QIcon(":/image/image/sidebar-mentions@2x copy.png"));
    else ui->mentions->setIcon(QIcon(":/image/image/sidebar-mentions@2x.png"));
    if(blue_num==3) ui->messages->setIcon(QIcon(":/image/image/sidebar-messages@2x copy.png"));
    else ui->messages->setIcon(QIcon(":/image/image/sidebar-messages@2x.png"));
    if(blue_num==4) ui->profile->setIcon(QIcon(":/image/image/sidebar-profile@2x copy.png"));
    else ui->profile->setIcon(QIcon(":/image/image/sidebar-profile@2x.png"));
    if(blue_num==5) ui->search->setIcon(QIcon(":/image/image/sidebar-search@2x copy.png"));
    else ui->search->setIcon(QIcon(":/image/image/sidebar-search@2x.png"));
}

//主页按钮颜色高亮控制器
void View_MainWindow::buttonIconControllerProfile(int blue_num){
    if(blue_num==0) ui->buttonMyTimeline->setIcon(QIcon(":/image/image/clock copy.png"));
    else ui->buttonMyTimeline->setIcon(QIcon(":/image/image/clock.png"));
    if(blue_num==1) ui->buttonMyMentions->setIcon(QIcon(":/image/image/at copy.png"));
    else ui->buttonMyMentions->setIcon(QIcon(":/image/image/at.png"));
    if(blue_num==2) ui->buttonMyFavourites->setIcon(QIcon(":/image/image/star copy.png"));
    else ui->buttonMyFavourites->setIcon(QIcon(":/image/image/star.png"));
    if(blue_num==3) ui->buttonMyProfile->setIcon(QIcon(":/image/image/person copy.png"));
    else ui->buttonMyProfile->setIcon(QIcon(":/image/image/person.png"));
}

//测试按钮被点击
void View_MainWindow::on_pushButton_clicked()
{
    emit test();
}

//开始搜索
void View_MainWindow::on_searchLineEdit_returnPressed()
{
    QString searchContent=ui->searchLineEdit->text();
    emit searchUser(searchContent);

}

//显示搜索信息
void View_MainWindow::displaySearchUsers(std::list<userSearchResultT> resultsList){
    qDebug()<<"in View_MainWindow::displaySearchUsers()";
    std::list<userSearchResultT>::iterator itor;
    for(itor=resultsList.begin(); itor!=resultsList.end(); itor++){
        qDebug()<<itor->screen_name.toStdString().c_str();
        qDebug()<<itor->followers_count;
        qDebug()<<itor->uid;
        //qDebug()<<itor->profile_image_url;
    }

}

//显示好友列表
void View_MainWindow::displayFriendsList(std::list<friendsInfoT> friendsList, QString buttonContent){
    QListWidgetItem * item;
    View_Item_Friend * widget;
    std::list<friendsInfoT>::iterator itor;
    ui->listWidget_FriendsList->clear();
    for(itor=friendsList.begin(); itor!=friendsList.end(); itor++){
        item=new QListWidgetItem(ui->listWidget_FriendsList);
        widget=new View_Item_Friend;
        widget->setItemContent(itor->id, itor->screen_name, itor->description,
                               itor->profile_image_url, buttonContent);
        ui->listWidget_FriendsList->setItemWidget(item, widget);
        item->setSizeHint (QSize(widget->rect().width(),widget->rect().height()));
        memoFriendsList[widget]=item;
        memoFriendsListReverse[item]=widget;
    }
    ui->PageStack->setCurrentIndex(6);
}

//好友列表表项被点击
void View_MainWindow::on_listWidget_FriendsList_itemClicked(QListWidgetItem *item)
{
    item->setSelected(false);
}

//关注人列表按钮被点击
void View_MainWindow::on_pushButton_profile_following_clicked()
{
    emit getFriendshipsFriends("100");

}

//粉丝按钮被点击
void View_MainWindow::on_pushButton_profile_follower_clicked()
{
    if(this->isHidden())
        this->gradientShow();
    emit getFriendshipsFollowers("100");

}

//显示主时间轴
void View_MainWindow::displayStatusesFriendsTimeline(std::list<statusInfoT> statusesList, bool slide){
    setStatusesList(statusesList, ui->listWidget_FriendsTimeline);

    if(slide){//滑动效果
        QEventLoop loop;
        for(int i=1; i<69; i++){
            ui->listWidget_FriendsTimeline->move(ui->listWidget_FriendsTimeline->x(), ui->listWidget_FriendsTimeline->y()-1);
            QTimer::singleShot(40, &loop, SLOT(quit()));
            loop.exec();
        }
        movieFriendsTimeline->stop();
    }

}

//显示用户微博时间轴
void View_MainWindow::displayStatusesUserTimeline(std::list<statusInfoT> statusesList){
    setStatusesList(statusesList, ui->listWidget_UserTimeline);
}

//显示@微博时间轴
void View_MainWindow::displayStatusesMentions(std::list<statusInfoT> statusesList){
    setStatusesList(statusesList, ui->listWidget_MentionsProfile);
}

//显示评论列表
void View_MainWindow::displayCommentsShow(std::list<commentInfoT> commentsList){

    QListWidgetItem * item;
    View_Item_Comment * widget;
    std::list<commentInfoT>::iterator itor;
    for(itor=commentsList.begin(); itor!=commentsList.end(); itor++){
        item=new QListWidgetItem(ui->listWidget_StatusShow);
        widget=new View_Item_Comment(ui->listWidget_StatusShow);

        widget->setItemContent(itor->id_comment, itor->id_status,
                               itor->id_user, itor->screen_name_comment,
                               itor->screen_name_status, itor->profile_image_url,
                               itor->text_comment, itor->text_status,
                               itor->isReply, itor->text_reply,
                               itor->screen_name_reply);
        ui->listWidget_StatusShow->setItemWidget(item, widget);
        item->setSizeHint (QSize(widget->rect().width(),widget->rect().height()));
        connect(widget, SIGNAL(postCommentsDestroy(QString,QString)), this, SLOT(commentsDestroy(QString,QString)));
        connect(widget, SIGNAL(postCommentsReply(QString, QString)), this, SLOT(commentsReply(QString,QString)));
        memoCommentsShow[widget]=item;
        memoCommentsShowReverse[item]=widget;

    }
}

//显示mention列表
void View_MainWindow::displayCommentsMentions(std::list<statusInfoT> statusesList, bool slide){
    setStatusesList(statusesList, ui->listWidget_Mentions);

    if(slide){//滑动效果
        QEventLoop loop;
        for(int i=1; i<69; i++){
            ui->listWidget_Mentions->move(ui->listWidget_Mentions->x(), ui->listWidget_Mentions->y()-1);
            QTimer::singleShot(40, &loop, SLOT(quit()));
            loop.exec();
        }
        movieMentionsTimeline->stop();
    }
}

//显示收到评论列表
void View_MainWindow::displayCommentsToMe(std::list<statusInfoT> statusesList){
    setStatusesList(statusesList, ui->listWidget_CommentsRecieved);
}

//显示发出评论列表
void View_MainWindow::displayCommentsByMe(std::list<statusInfoT> statusesList){
    setStatusesList(statusesList, ui->listWidget_CommentsSend);
}

//时间线表项被点击
void View_MainWindow::on_listWidget_FriendsTimeline_itemClicked(QListWidgetItem *item)
{
    //qDebug()<<"itemClicked";
    //item->setSelected(false);
}

//返回主时间线界面
void View_MainWindow::on_pushButton_FriendsList_Back_clicked()
{
    ui->PageStack->setCurrentIndex(4);
    clearFriendsListWidgets();
}

//测试label被点击
void View_MainWindow::on_label_test_linkActivated(const QString &link)
{
    //qDebug()<<link.toStdString().c_str();
    //QDesktopServices::openUrl(QUrl(link));
}

//刷新主时间线按钮被点击
void View_MainWindow::on_pushButton_RefreshFriendsTimeline_clicked()
{
    ui->listWidget_FriendsTimeline->scrollToTop();//回顶部

    movieFriendsTimeline->start();

    QEventLoop loop;//滑动效果
    for(int i=1; i<69; i++){
        ui->listWidget_FriendsTimeline->move(ui->listWidget_FriendsTimeline->x(), ui->listWidget_FriendsTimeline->y()+1);
        QTimer::singleShot(3, &loop, SLOT(quit()));
        loop.exec();
    }
    emit getStatusesFriendsTimeline();
}

//设置时间线列表
void View_MainWindow::setStatusesList(std::list<statusInfoT> statusesList, QListWidget *listWidget){
    clearStatusesListWidgets(listWidget);

    QListWidgetItem * item;
    View_Item_Status * widget;
    std::list<statusInfoT>::iterator itor;
    listWidget->clear();
    for(itor=statusesList.begin(); itor!=statusesList.end(); itor++){
        item=new QListWidgetItem(listWidget);
        widget=new View_Item_Status(listWidget);

//        widget->setItemContent(itor->id, itor->screen_name, itor->description,
//                               itor->profile_image_url, buttonContent);
        widget->setItemContent(itor->id_status, itor->id_user_status,
                               itor->text_status, itor->screen_name_user_status,
                               itor->profile_image_url_status, itor->isRepost,
                               itor->text_retweeted, itor->id_retweeted,
                               itor->id_user_retweeted, itor->screen_name_user_retweeted,
                               itor->profile_image_url_retweeted, itor->pictureList,
                               true);

        listWidget->setItemWidget(item, widget);
        item->setSizeHint (QSize(widget->rect().width(),widget->rect().height()));
        memoStatusesTimeline[widget]=item;
        memoStatusesTimelineReverse[item]=widget;
        //connect(widget, SIGNAL(getStatusesShow(QString)), this, SLOT(statusesShow(QString)));
        connect(widget, SIGNAL(getCommentsShow(QString,View_Item_Status*)), this, SLOT(commentsShow(QString,View_Item_Status*)));
        connect(widget, SIGNAL(postStatusesDestroy(QString, QString)), this, SLOT(statusesDestroy(QString, QString)));
        connect(widget, SIGNAL(postStatusesRepost(QString)), this ,SLOT(statusesRepost(QString)));
        connect(widget, SIGNAL(postCommentsCreate(QString)), this, SLOT(commentsCreate(QString)));
    }
}

//刷新mentions列表
void View_MainWindow::on_pushButton_Mentions_clicked()
{
    ui->listWidget_Mentions->scrollToTop();//回顶部

    movieMentionsTimeline->start();

    QEventLoop loop;//滑动效果
    for(int i=1; i<69; i++){
        ui->listWidget_Mentions->move(ui->listWidget_Mentions->x(), ui->listWidget_Mentions->y()+1);
        QTimer::singleShot(3, &loop, SLOT(quit()));
        loop.exec();
    }

    emit getCommentsMentions();

}

//tab被点击
void View_MainWindow::on_tabWidget_Comments_tabBarClicked(int index)
{
    if(index==0){
        //qDebug()<<"Recieved";
        emit getCommentsToMe();

    }
    else if(index==1){
        //qDebug()<<"Send";
        emit getCommentsByMe();

    }
}

//表项被双击
void View_MainWindow::on_listWidget_FriendsTimeline_itemDoubleClicked(QListWidgetItem *item)
{
    //qDebug()<<"itemDoubleClicked";
    //qDebug()<<memoStatusesTimelineReverse[item]->getScreenNameUserStatus();
}

//显示某一微博内容
void View_MainWindow::statusesShow(QString id){
    //qDebug()<<id;
    emit getStatusesShow(id);
}

//显示评论列表
void View_MainWindow::commentsShow(QString id, View_Item_Status * _widget){
    if(memoCommentsStatusItem) delete memoCommentsStatusItem;
    if(memoCommentsStatusWidget) delete memoCommentsStatusWidget;
    ui->listWidget_StatusShow->clear();

    QListWidgetItem * item=new QListWidgetItem(ui->listWidget_StatusShow);
    View_Item_Status * widget=new View_Item_Status(ui->listWidget_StatusShow);
    widget->setItemContent(_widget->getIdStatus(), _widget->getIdUserStatus(),
                           _widget->getTextStatus(), _widget->getScreenNameUserStatus(),
                           _widget->getProfileImageUrlStatus(), _widget->getIsRepost(),
                           _widget->getTextRetweeted(), _widget->getIdRetweeted(),
                           _widget->getIdUserRetweeted(), _widget->getScreenNameUserRetweeted(),
                           _widget->getProfileImageUrlRetweeted(), _widget->getPictureList(),
                           false);
    ui->listWidget_StatusShow->setItemWidget(item, widget);
    item->setSizeHint (QSize(widget->rect().width(),widget->rect().height()));
    memoCommentsStatusWidget=widget;
    memoCommentsStatusItem=item;
    connect(widget, SIGNAL(postStatusesRepost(QString)), this ,SLOT(statusesRepost(QString)));
    connect(widget, SIGNAL(postCommentsCreate(QString)), this, SLOT(commentsCreate(QString)));

    widget->setDetailsDisvisible();

    ui->PageStack->setCurrentIndex(7);

    emit getCommentsShow(id);

}

//删除微博
void View_MainWindow::statusesDestroy(QString id, QString id_user){
    emit postStatusesDestroy(id, id_user);
}

//转发微博
void View_MainWindow::statusesRepost(QString id){
    emit postStatusesRepost(id);
}

//写评论
void View_MainWindow::commentsCreate(QString id){
    emit postCommentsCreate(id);
}

//删评论
void View_MainWindow::commentsDestroy(QString id_comment, QString id_user){
    emit postCommentsDestroy(id_comment, id_user);
}

//回复评论
void View_MainWindow::commentsReply(QString id_comment, QString id_status){
    emit postCommentsReply(id_comment, id_status);
}

//查看微博界面的back按钮被按下
void View_MainWindow::on_pushButton_Status_Back_clicked()
{
    buttonIconController(0);
    ui->PageStack->setCurrentIndex(0);
    clearCommentsListWidgets();
}

//窗口淡化显示
void View_MainWindow::gradientShow(){
    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(AppearTime);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    this->show();
}

//窗口淡化隐藏
void View_MainWindow::gradientHide(){
    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(DisappearTime);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    this->hide();
}

//清除缓存图片
void View_MainWindow::on_actionClear_Chche_triggered()
{
    //Clear the cache file's images
    QDir dir("cache_image");
    if(!dir.exists())
        return;
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList=dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileList) {
        if(fileInfo.isFile()){
            //qDebug()<<fileInfo.fileName();
            if(!fileInfo.fileName().contains("current_user"));
                fileInfo.dir().remove(fileInfo.fileName());
        }
        else{
            ;
        }
    }
    emit displayMessage("cache清理完毕");
    //qDebug()<<"Cache清理完毕";
}

//注销
void View_MainWindow::on_actionLog_Out_triggered()
{
    emit logOut();
}

//移动list，用于网络互斥
void View_MainWindow::moveList(int list){
    if(list==0){
        QEventLoop loop;
        for(int i=1; i<69; i++){
            ui->listWidget_FriendsTimeline->move(ui->listWidget_FriendsTimeline->x(), ui->listWidget_FriendsTimeline->y()-1);
            QTimer::singleShot(3, &loop, SLOT(quit()));
            loop.exec();
        }
    }
    else if(list==1){
        QEventLoop loop;
        for(int i=1; i<69; i++){
            ui->listWidget_Mentions->move(ui->listWidget_Mentions->x(), ui->listWidget_Mentions->y()-1);
            QTimer::singleShot(3, &loop, SLOT(quit()));
            loop.exec();
        }
    }
}

//返回主时间线页面
void View_MainWindow::backToMainTimeline(){
    on_pushButton_Status_Back_clicked();
}



