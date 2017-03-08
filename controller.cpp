/*
 * 文件名：controller.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了Controller类中的部分成员函数。
 */

#include "controller.h"

//构造函数
Controller::Controller(QObject *parent) : QObject(parent)
{
    weibo = new Weibo;
    viewMainWindow = new View_MainWindow;
    viewPostWeibo = new View_PostWeibo;
    viewOauth2 = new View_Oauth2;

    messageDialog = new View_Dialog_Message;

    createDatabase();//创建数据库

    //初始化成员变量
    inHttping=false;

    haveUnread=false;
    viewMainWindow->hideUnreadPot();


    watcherThread = new QThread(this);

    //设置监视线程
    timer = new QTimer;
    timer->setSingleShot(false);
    timer->setInterval(interval);
    timer->moveToThread(watcherThread);//将定时器移到监视线程

    //设置缓存文件
    cacheDir = new QDir;
    cacheDir->setPath(cacheDirPath);
    if(!cacheDir->exists()){
        cacheDir->mkpath(cacheDirPath);
    }

    //创建链接
    setConnections();

}

//析构函数，delete所有的指针
Controller::~Controller(){
    delete weibo;
    delete viewMainWindow;
    delete viewPostWeibo;
    delete viewOauth2;

    delete messageDialog;
    delete watcherThread;
    delete timer;

    delete cacheDir;
}

//设置应用图标
void Controller::setApplicationIcon(QApplication * application){
    application->setWindowIcon(QIcon(":/image/image/LOGO_64x64.png"));
}

//显示欢迎界面
void Controller::splashAppear(){
    QSplashScreen *splash =new QSplashScreen;
    splash->setPixmap(QPixmap(":/image/image/SinaWeibo300x240.png"));
    splash->show();

    QEventLoop loop;
    QTimer::singleShot(3000, &loop, SLOT(quit()));
    loop.exec();

    splash->finish(NULL);
}

//设置应用风格
void Controller::setAppStyle(QStyle *style){
    this->appStyle=style;
}

//测试函数
void Controller::test(){

    //weibo->postStatusesUpload("gaoshiqing", cacheDirPath + "/" + "screenshot1.jpg");


    //not permitted
    QHttpMultiPart * multiPart=new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg("access_token")));
    textPart.setBody(QString("2.00WsRDhFF_f_KD2acd51214eCd9zID").toUtf8());
    multiPart->append(textPart);

    QHttpPart textPart1;
    textPart1.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    textPart1.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg("status")));
    textPart1.setBody(QString("upload image").toUtf8());
    multiPart->append(textPart1);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"pic\"; filename=\""+cacheDirPath + "/" + "screenshot1.jpg"+"\""));

    QFile * file=new QFile(cacheDirPath + "/" + "screenshot1.jpg");
    if(!file->open(QFile::ReadOnly)){
        qDebug()<<"文件打开不成功";
        return;
    }
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);



    QUrl url("https://upload.api.weibo.com/2/statuses/upload.json");
////             "?access_token=2.00WsRDhFF_f_KD2acd51214eCd9zID"
////             "&status=upload image");
////             "&pic="+cacheDirPath + "/" + "screenshot1.jpg"
////             ","+cacheDirPath + "/" + "screenshot2.jpg");
    QNetworkRequest request;
    request.setUrl(url);

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, multiPart);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() == QNetworkReply::NoError){
        qDebug()<<"no error";

        qDebug()<<reply->readAll();

    }
    else{
        qDebug()<<"reply error"<<reply->error()<<reply->errorString();
    }
}

//创建信号和槽函数的链接
void Controller::setConnections(){
    connect(viewMainWindow, SIGNAL(test()), this, SLOT(test()));

    connect(weibo, SIGNAL(moveList(int)), this, SLOT(moveList(int)));

    connect(weibo, SIGNAL(displayMessage(QString)), this, SLOT(displayMessage(QString)));

    connect(viewPostWeibo, SIGNAL(displayMessage(QString)), this, SLOT(displayMessage(QString)));
    connect(viewMainWindow, SIGNAL(displayMessage(QString)), this, SLOT(displayMessage(QString)));

    connect(viewMainWindow, SIGNAL(logOut()), this, SLOT(userLogOut()));

    connect(viewOauth2, SIGNAL(urlChange(QUrl)), this, SLOT(getAccessToken(QUrl)));

    connect(viewMainWindow, SIGNAL(writeWeibo()), this, SLOT(writeNewWeibo()));
    connect(viewPostWeibo, SIGNAL(postNewWeibo(QString)), this, SLOT(postNewWeibo(QString)));
    connect(viewMainWindow, SIGNAL(postStatusesRepost(QString)), this, SLOT(writeNewRepost(QString)));
    connect(viewPostWeibo, SIGNAL(postStatusesRepost(QString)), this, SLOT(postStatusesRepost(QString)));

    connect(viewPostWeibo, SIGNAL(postStatusesUpload(QString,QString)), this, SLOT(postStatusesUpload(QString,QString)));

    connect(viewMainWindow, SIGNAL(postCommentsCreate(QString)), this, SLOT(writeNewComment(QString)));
    connect(viewPostWeibo, SIGNAL(postCommentsCreate(QString)), this, SLOT(postCommentsCreate(QString)));
    connect(viewMainWindow, SIGNAL(postCommentsReply(QString,QString)), this, SLOT(writeNewReply(QString,QString)));
    connect(viewPostWeibo, SIGNAL(postCommentsReply(QString)), this, SLOT(postCommentsReply(QString)));

    connect(viewMainWindow, SIGNAL(searchUser(QString)), this, SLOT(searchUser(QString)));
    connect(weibo, SIGNAL(returnSearchUsers()), this, SLOT(returnSearchUsers()));
    connect(weibo, SIGNAL(userShow()), this, SLOT(displayUserInfo()));

    connect(timer,SIGNAL(timeout()),this,SLOT(unreadWatcher()), Qt::DirectConnection);
    connect(watcherThread, SIGNAL(started()), timer, SLOT(start()));
    connect(weibo, SIGNAL(returnRemindUnreadCount(unreadCountT)), this, SLOT(returnRemindUnreadCount(unreadCountT)));

    connect(viewMainWindow, SIGNAL(getStatusesFriendsTimeline()), this, SLOT(getStatusesFriendsTimeline()));
    connect(weibo, SIGNAL(returnStatusesFriendsTimeline()), this, SLOT(returnStatusesFriendsTimeline()));
    connect(viewMainWindow, SIGNAL(getStatusesUserTimeline()), this, SLOT(getStatusesUserTimeline()));
    connect(weibo, SIGNAL(returnStatusesUserTimeline()), this, SLOT(returnStatusesUserTimeline()));
    connect(viewMainWindow, SIGNAL(getStatusesMentions()), this, SLOT(getStatusesMentions()));
    connect(weibo, SIGNAL(returnStatusesMentions()), this, SLOT(returnStatusesMentions()));

    connect(weibo, SIGNAL(returnStatusesMentions()), this, SLOT(returnStatusesMentions()));

    connect(viewMainWindow, SIGNAL(getStatusesShow(QString)), this, SLOT(getStatusesShow(QString)));
    connect(weibo, SIGNAL(returnStatusesShow()), this, SLOT(returnStatusesShow()));

    connect(viewMainWindow, SIGNAL(postStatusesDestroy(QString, QString)), this, SLOT(postStatusesDestroy(QString,QString)));

    connect(viewMainWindow, SIGNAL(getCommentsShow(QString)), this, SLOT(getCommentsShow(QString)));
    connect(weibo, SIGNAL(returnCommentsShow()), this, SLOT(returnCommentsShow()));
    connect(viewMainWindow, SIGNAL(getCommentsMentions()), this, SLOT(getCommentsMentions()));
    connect(weibo, SIGNAL(returnCommentsMentions()), this, SLOT(returnCommentsMentions()));
    connect(viewMainWindow, SIGNAL(getCommentsToMe()), this, SLOT(getCommentsToMe()));
    connect(weibo, SIGNAL(returnCommentsToMe()), this, SLOT(returnCommentsToMe()));
    connect(viewMainWindow, SIGNAL(getCommentsByMe()), this, SLOT(getCommentsByMe()));
    connect(weibo, SIGNAL(returnCommentsByMe()), this, SLOT(returnCommentsByMe()));

    connect(viewMainWindow, SIGNAL(postCommentsDestroy(QString,QString)), this, SLOT(postCommentsDestroy(QString,QString)));

    connect(viewMainWindow, SIGNAL(getFriendshipsFriends(QString)), this, SLOT(getFriendshipsFriends(QString)));
    connect(viewMainWindow, SIGNAL(getFriendshipsFollowers(QString)), this, SLOT(getFriendshipsFollowers(QString)));

    connect(weibo, SIGNAL(returnFriendshipsFriends()), this, SLOT(returnFriendshipsFriends()));
    connect(weibo, SIGNAL(returnFriendshipsFollowers()), this, SLOT(returnFriendshipsFollowers()));

}

//显示提示信息
void Controller::displayMessage(QString message){
    messageDialog->displayMessage(message);
}

//发送获取未读信息的网络请求
void Controller::unreadWatcher(){
    if(!inHttping){
        weibo->getRemindUnreadCount();
    }
}

//返回未读信息
void Controller::returnRemindUnreadCount(unreadCountT unreadInfo){
    if(unreadInfo.status>0){
        //qDebug()<<"-------------------------有新的微博--------------------------";
        haveUnread=true;
        viewMainWindow->showUnreadPot();
    }
    else{
        //qDebug()<<"-------------------------没有新的微博--------------------------";
        //qDebug()<<unreadInfo.status;
    }

    if(haveUnread){
        viewMainWindow->setTrayPictureOn();
    }
    else{
        viewMainWindow->setTrayPictureOff();
    }
}

//注销
void Controller::userLogOut(){
    QSettings settings(IniFileName, QSettings::IniFormat);//打开文件
    settings.clear();
    clearDababase();
    viewMainWindow->clearAllListWidgets();
    hideMainWindow();
    showOauth2();

}

//显示登录认证页面
void Controller::showOauth2(){
    viewOauth2->loadUrl(url_oauth_getcode);
    viewOauth2->gradientShow();
}

//隐藏登录认证页面
void Controller::hideOauth2(){
    viewOauth2->gradientHide();
}

//显示主界面
void Controller::showMainWindow(){
    viewMainWindow->gradientShow();
}

//隐藏主界面
void Controller::hideMainWindow(){
    viewMainWindow->gradientHide();
}

//显示微博编辑界面
void Controller::showWriteWeibo(){
    viewPostWeibo->gradientShow();
    viewPostWeibo->setFunction(postContentType::weibo);
}

//隐藏微博编辑界面
void Controller::hideWriteWeibo(){
    viewPostWeibo->gradientHide();
    viewPostWeibo->clearTextEditor();
}

//获取access_token
void Controller::getAccessToken(QUrl url){
    //qDebug()<<"in Controller::getAccessToken-------------------";
    QString urlQString=url.toString();
    QString code;
    try{
        if(urlQString.contains(RedirectUrl) && urlQString.contains("code=")){
            code=urlQString.mid(urlQString.lastIndexOf('=')+1);
            //qDebug()<<code;
            weibo->postOauth2AccessToken(code);
            hideOauth2();
            watcherThread->start();
            //timer->start(interval);
        }
        else{

        }
    }
    catch(...){

    }
}

//写微博
void Controller::writeNewWeibo(){
    showWriteWeibo();
}

//写转发微博
void Controller::writeNewRepost(QString id){
    idRepostOrCommentOrReply=id;
    viewPostWeibo->gradientShow();
    viewPostWeibo->setFunction(postContentType::repost);
}

//写新评论
void Controller::writeNewComment(QString id){
    idRepostOrCommentOrReply=id;
    viewPostWeibo->gradientShow();
    viewPostWeibo->setFunction(postContentType::comment_create);
}

//写评论回复
void Controller::writeNewReply(QString id_comment, QString id_status){
    idRepostOrCommentOrReply=id_comment;
    idStatusForReply=id_status;
    viewPostWeibo->gradientShow();
    viewPostWeibo->setFunction(postContentType::comment_reply);
}

//发布微博
void Controller::postNewWeibo(QString newWeibo){
    //qDebug()<<"in Controller::postNewWeibo()"<<newWeibo;
    weibo->postNewWeibo(newWeibo);
}

//发布转发微博
void Controller::postStatusesRepost(QString status){
    weibo->postStatusesRepost(idRepostOrCommentOrReply, status);

}

//发布新评论
void Controller::postCommentsCreate(QString comment){
    weibo->postCommentsCreate(comment, idRepostOrCommentOrReply);
}

//回复评论
void Controller::postCommentsReply(QString reply){
    weibo->postCommentsReply(this->idRepostOrCommentOrReply, this->idStatusForReply, reply);
}

//查找用户
void Controller::searchUser(QString userName){
    //qDebug()<<"in Controller::searchUser()"<<userName;
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
    }
    else{
        inHttping=true;
        weibo->searchUser(userName);
    }
}

//返回用户搜索结果
void Controller::returnSearchUsers(){
    viewMainWindow->displaySearchUsers(weibo->userSearchResultList);
    inHttping=false;
}

//显示用户信息
void Controller::displayUserInfo(){
    saveIniFile();//在配置文件中保存信息

    curUserId=weibo->userInfo.id;
    //缓存用户头像
    imageDownloader(weibo->userInfo.profile_image, "current_user_"+weibo->userInfo.screen_name+".jpg");
    weibo->userInfo.profile_image=cacheDirPath+"/"+"current_user_"+weibo->userInfo.screen_name+".jpg";
    viewMainWindow->displayUserProfile(weibo->userInfo);

    viewPostWeibo->setProfileImage(weibo->userInfo.profile_image);

    showMainWindow();//显示主页面
}

//返回关注人列表
void Controller::returnFriendshipsFriends(){
    std::list<friendsInfoT>::iterator itor;
    for(itor=weibo->friendsList.begin(); itor!=weibo->friendsList.end(); itor++){
        imageDownloader(itor->profile_image_url, itor->id+"_"+itor->screen_name+".jpg");//缓存图片
        itor->profile_image_url=cacheDirPath+"/"+itor->id+"_"+itor->screen_name+".jpg";
        //qDebug()<<itor->profile_image_url.toStdString().c_str();
    }
    viewMainWindow->displayFriendsList(weibo->friendsList, "Following");
    inHttping=false;
}

//返回粉丝列表
void Controller::returnFriendshipsFollowers(){
    std::list<friendsInfoT>::iterator itor;
    for(itor=weibo->friendsList.begin(); itor!=weibo->friendsList.end(); itor++){
        imageDownloader(itor->profile_image_url, itor->id+"_"+itor->screen_name+".jpg");//缓存图片
        itor->profile_image_url=cacheDirPath+"/"+itor->id+"_"+itor->screen_name+".jpg";
        //qDebug()<<itor->profile_image_url.toStdString().c_str();
    }
    viewMainWindow->displayFriendsList(weibo->friendsList, "Follower");
    inHttping=false;
}

//图片下载器
void Controller::imageDownloader(QString url, QString fileName){
    QFile f(cacheDirPath+"/"+fileName);
    if(f.exists()){
        qDebug()<<"图片已存在";
        return;
    }

    //qDebug()<<url;
    QNetworkAccessManager manager;//网络请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;//等待网络请求返回
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (reply->error() != QNetworkReply::NoError){
        qDebug()<<"reply error";
        return;
    }

    if(!f.open(QIODevice::WriteOnly)){//保存图片
        qDebug()<<"can not write";
        return;
    }
    f.write(reply->readAll());
    f.close();
    delete reply;
    //qDebug()<<fileName;
}

//URL转换
QString Controller::urlChangeThumbnailToBmiddle(QString url){
    QString result=url;
    result.replace("thumbnail", "bmiddle");
    return result;
}

//缓存图片
void Controller::cacheImages(std::list<statusInfoT> &statusesList){
    std::list<statusInfoT>::iterator itor;
    for(itor=statusesList.begin(); itor!=statusesList.end(); itor++){
        imageDownloader(itor->profile_image_url_status, itor->id_user_status+"_"+itor->screen_name_user_status+".jpg");
        std::list<QString>::iterator itor1;
        int i=0;
        for(itor1=itor->pictureList.begin(); itor1!=itor->pictureList.end(); itor1++, i++){
            *itor1=urlChangeThumbnailToBmiddle(*itor1);
            imageDownloader(*itor1, itor->id_status+"_"+QString::number(i)+".jpg");
        }
        itor->profile_image_url_status=cacheDirPath+"/"+itor->id_user_status+"_"+itor->screen_name_user_status+".jpg";
        //qDebug()<<itor->profile_image_url_status.toStdString().c_str();
    }
}

//获取关注人列表
void Controller::getFriendshipsFriends(QString count){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
    }
    else{
        inHttping=true;
        weibo->getFriendshipsFriends(count);

    }

}

//获取粉丝列表
void Controller::getFriendshipsFollowers(QString count){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
    }
    else{
        inHttping=true;
        weibo->getFriendshipsFollowers(count);
    }

}

//返回主时间轴
void Controller::returnStatusesFriendsTimeline(){

    cacheImages(weibo->statusesList);//缓存图片
    saveTimeline("friendstimeline", weibo->statusesList);//保存时间线到数据库
    viewMainWindow->displayStatusesFriendsTimeline(weibo->statusesList, true);
    viewMainWindow->setTrayPictureOff();
    haveUnread=false;
    viewMainWindow->hideUnreadPot();
    inHttping=false;
}

//返回用户微博的时间线
void Controller::returnStatusesUserTimeline(){

    cacheImages(weibo->statusesList);//缓存图片
    saveTimeline("usertimeline", weibo->statusesList);//保存时间线到数据库
    viewMainWindow->displayStatusesUserTimeline(weibo->statusesList);
    inHttping=false;
}

//返回@的时间线
void Controller::returnStatusesMentions(){
    cacheImages(weibo->statusesList);//缓存图片
    saveTimeline("usermentions", weibo->statusesList);//保存时间线到数据库
    viewMainWindow->displayStatusesMentions(weibo->statusesList);
    inHttping=false;
}

//返回某微博内容
void Controller::returnStatusesShow(){
    qDebug()<<"in Controller::returnStatusesShow()";
    inHttping=false;

}

//返回评论列表
void Controller::returnCommentsShow(){
    qDebug()<<"in Controller::returnCommentsShow()";
    std::list<commentInfoT>::iterator itor;
    for(itor=weibo->commentsList.begin(); itor!=weibo->commentsList.end(); itor++){
        imageDownloader(itor->profile_image_url, itor->id_user+"_"+itor->screen_name_comment+".jpg");//缓存图片
        itor->profile_image_url=cacheDirPath+"/"+itor->id_user+"_"+itor->screen_name_comment+".jpg";
    }

    viewMainWindow->displayCommentsShow(weibo->commentsList);
    inHttping=false;

}

//返回含有@的评论时间线
void Controller::returnCommentsMentions(){
    cacheImages(weibo->statusesList);//缓存图片
    saveTimeline("mentionstimeline", weibo->statusesList);//保存时间线内容到数据库
    viewMainWindow->displayCommentsMentions(weibo->statusesList, true);
    inHttping=false;
}

//返回收到的评论列表
void Controller::returnCommentsToMe(){
    cacheImages(weibo->statusesList);
    saveTimeline("commentstome", weibo->statusesList);
    viewMainWindow->displayCommentsToMe(weibo->statusesList);
    inHttping=false;
}

//返回发出的评论列表
void Controller::returnCommentsByMe(){
    cacheImages(weibo->statusesList);
    saveTimeline("commentsbyme", weibo->statusesList);
    viewMainWindow->displayCommentsByMe(weibo->statusesList);
    inHttping=false;
}

//获取主时间线
void Controller::getStatusesFriendsTimeline(){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
        //主时间线滚回
        viewMainWindow->moveList(0);
    }
    else{
        inHttping=true;
        weibo->getStatusesFriendsTimeline("50");
    }
}

//获取用户微博的时间线
void Controller::getStatusesUserTimeline(){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");

    }
    else{
        inHttping=true;
        weibo->getStatusesUserTimeline("20");
    }
}

//获取@时间线
void Controller::getStatusesMentions(){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
    }
    else{
        inHttping=true;
        weibo->getStatusesMentions("50");
    }
}

//获取某一微博
void Controller::getStatusesShow(QString id){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
        //返回主时间线页面，这个接口没有调
    }
    else{
        inHttping=true;
        weibo->getStatusesShow(id);
    }
}

//删除某一微博
void Controller::postStatusesDestroy(QString id, QString id_user){
    //qDebug()<<"Controller::"<<id<<id_user;
    if(id_user!=curUserId){
        displayMessage("只能删除自己的微博");
        //qDebug()<<"只能删除自己的微博";
        return;
    }
    weibo->postStatusesDestroy(id);
}

//发布带有图片的微博
void Controller::postStatusesUpload(QString status, QString picturePath){
    weibo->postStatusesUpload(status, picturePath);
}

//显示评论列表
void Controller::getCommentsShow(QString id){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
        //返回主时间线页面
        viewMainWindow->backToMainTimeline();
    }
    else{
        inHttping=true;
        weibo->getCommentsShow(id);
    }
}

//显示含@的评论
void Controller::getCommentsMentions(){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
        //Mentions 滚回
        viewMainWindow->moveList(1);
    }
    else{
        inHttping=true;
        weibo->getCommentsMentions();
    }
}

//显示收到的评论列表
void Controller::getCommentsToMe(){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
        //
    }
    else{
        inHttping=true;
        weibo->getCommentsToMe();
    }
}

//显示发布的评论列表
void Controller::getCommentsByMe(){
    if(inHttping){
        displayMessage("请勿频繁刷新列表");
        //
    }
    else{
        inHttping=true;
        weibo->getCommentsByMe();
    }
}

//删除某一评论
void Controller::postCommentsDestroy(QString id_comment, QString id_user){
    if(id_user!=curUserId){
        displayMessage("只能删除自己的评论");
        //qDebug()<<"只能删除自己的评论";
        return;
    }
    weibo->postCommentsDestroy(id_comment);
}

void Controller::moveList(int id){
    viewMainWindow->moveList(id);
    inHttping=false;
}

void Controller::setInHttping(bool b){
    inHttping=false;
}

