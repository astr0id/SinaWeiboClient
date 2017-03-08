/*
 * 文件名：controller_filemanager.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了Controller类的部分成员函数。
 */

#include "controller.h"

//判断用户的令牌是否过期
bool Controller::notPastDue(){
    QSettings settings(IniFileName, QSettings::IniFormat);//打开文件

    if(!(settings.contains("UserInfo/remind_in")&&settings.contains("UserInfo/currentTime"))){
        qDebug()<<"ini文件为空";
        return false;
    }

    //获取配置文件内容
    QDateTime lastCurrentTime=settings.value("UserInfo/currentTime").toDateTime();
    qint64 lastRemindIn=settings.value("UserInfo/remind_in").toLongLong();
    qDebug()<<"lastCurrentTime"<<lastCurrentTime;
    qDebug()<<"lastRemindIn"<<lastRemindIn;
    if(lastCurrentTime.addSecs(lastRemindIn)>QDateTime::currentDateTime()){//若仍未过期
        qDebug()<<"access_token尚未过期";
        userInfoT info;
        info.id=settings.value("UserInfo/id").toString();
        info.screen_name=settings.value("UserInfo/screen_name").toString();
        info.location=settings.value("UserInfo/location").toString();
        info.description=settings.value("UserInfo/description").toString();
        info.gender=settings.value("UserInfo/gender").toString();
        info.followers_count=settings.value("UserInfo/followers_count").toInt();
        info.friends_count=settings.value("UserInfo/friends_count").toInt();
        info.statuses_count=settings.value("UserInfo/statuses_count").toInt();
        info.favourites_count=settings.value("UserInfo/favourites_count").toInt();
        info.profile_image=settings.value("UserInfo/profile_image").toString();
        info.avatar_large=settings.value("UserInfo/avatar_large").toString();

        weibo->setAccessToken(settings.value("UserInfo/accessToken").toString());
        weibo->setRemindIn(settings.value("UserInfo/remind_in").toString());
        weibo->setUid(settings.value("UserInfo/uid").toString());

        imageDownloader(info.profile_image, "current_user_"+info.screen_name+".jpg");
        info.profile_image=cacheDirPath+"/"+"current_user_"+info.screen_name+".jpg";

        curUserId=info.id;
        viewMainWindow->displayUserProfile(info);
        viewPostWeibo->setProfileImage(info.profile_image);

        std::list<statusInfoT> statusesList;
        statusesList=recoverTimeline("friendstimeline");
        viewMainWindow->displayStatusesFriendsTimeline(statusesList, false);

        showMainWindow();

        statusesList=recoverTimeline("mentionstimeline");
        viewMainWindow->displayCommentsMentions(statusesList, false);
        statusesList=recoverTimeline("commentstome");
        viewMainWindow->displayCommentsToMe(statusesList);
        statusesList=recoverTimeline("commentsbyme");
        viewMainWindow->displayCommentsByMe(statusesList);
        statusesList=recoverTimeline("usertimeline");
        viewMainWindow->displayStatusesUserTimeline(statusesList);
        statusesList=recoverTimeline("usermentions");
        viewMainWindow->displayStatusesMentions(statusesList);

        return true;
    }
    else{//已过期
        qDebug()<<"access_token已过期";
        return false;
    }
}

//保存配置文件内容
void Controller::saveIniFile(){
    QSettings settings(IniFileName, QSettings::IniFormat);//打开文件
    settings.beginGroup("UserInfo");
    settings.setValue("id", weibo->userInfo.id);
    settings.setValue("screen_name", weibo->userInfo.screen_name);
    settings.setValue("location", weibo->userInfo.location);
    settings.setValue("description", weibo->userInfo.description);
    settings.setValue("gender", weibo->userInfo.gender);
    settings.setValue("followers_count", weibo->userInfo.followers_count);
    settings.setValue("friends_count", weibo->userInfo.friends_count);
    settings.setValue("status_count", weibo->userInfo.statuses_count);
    settings.setValue("favourites_count", weibo->userInfo.favourites_count);
    settings.setValue("profile_image", weibo->userInfo.profile_image);
    settings.setValue("avatar_large", weibo->userInfo.avatar_large);

    settings.setValue("currentTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    settings.setValue("accessToken", weibo->getAccessToken());
    settings.setValue("remind_in", weibo->getRemindIn());
    settings.setValue("uid", weibo->getUid());
    settings.endGroup();
}

//创建数据库
void Controller::createDatabase(){
    QSqlDatabase db;
    QFileInfo dbFileInfo(DatabaseName);
    bool isExist=dbFileInfo.exists();
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DatabaseName);

    if(db.open()==false){//打开数据库
        messageDialog->displayMessage("数据库打开失败");
        return;
    }

    QSqlQuery query;
    if(isExist==false){//创建table
        query.exec("CREATE TABLE friendstimeline ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "id_status NVARCHAR,"
                   "id_user_status NVARCHAR,"
                   "text_status NVARCHAR,"
                   "screen_name_user_status NVARCHAR,"
                   "profile_image_url_status NVARCHAR,"
                   "isRepost BOOL,"
                   "text_retweeted NVARCHAR,"
                   "id_retweeted NVARCHAR,"
                   "id_user_retweeted NVARCHAR,"
                   "screen_name_user_retweeted NVARCHAR,"
                   "profile_image_url_retweeted NVARCHAR,"
                   "pictureNumber INT)"
                   );
        query.exec("CREATE TABLE friendstimelinePictures ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name NVARCHAR,"
                   "url NVARCHAR)"
                   );
        query.exec("CREATE TABLE mentionstimeline ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "id_status NVARCHAR,"
                   "id_user_status NVARCHAR,"
                   "text_status NVARCHAR,"
                   "screen_name_user_status NVARCHAR,"
                   "profile_image_url_status NVARCHAR,"
                   "isRepost BOOL,"
                   "text_retweeted NVARCHAR,"
                   "id_retweeted NVARCHAR,"
                   "id_user_retweeted NVARCHAR,"
                   "screen_name_user_retweeted NVARCHAR,"
                   "profile_image_url_retweeted NVARCHAR,"
                   "pictureNumber INT)"
                   );
        query.exec("CREATE TABLE mentionstimelinePictures ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name NVARCHAR,"
                   "url NVARCHAR)"
                   );
        query.exec("CREATE TABLE commentsbyme ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "id_status NVARCHAR,"
                   "id_user_status NVARCHAR,"
                   "text_status NVARCHAR,"
                   "screen_name_user_status NVARCHAR,"
                   "profile_image_url_status NVARCHAR,"
                   "isRepost BOOL,"
                   "text_retweeted NVARCHAR,"
                   "id_retweeted NVARCHAR,"
                   "id_user_retweeted NVARCHAR,"
                   "screen_name_user_retweeted NVARCHAR,"
                   "profile_image_url_retweeted NVARCHAR,"
                   "pictureNumber INT)"
                   );
        query.exec("CREATE TABLE commentsbymePictures ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name NVARCHAR,"
                   "url NVARCHAR)"
                   );
        query.exec("CREATE TABLE commentstome ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "id_status NVARCHAR,"
                   "id_user_status NVARCHAR,"
                   "text_status NVARCHAR,"
                   "screen_name_user_status NVARCHAR,"
                   "profile_image_url_status NVARCHAR,"
                   "isRepost BOOL,"
                   "text_retweeted NVARCHAR,"
                   "id_retweeted NVARCHAR,"
                   "id_user_retweeted NVARCHAR,"
                   "screen_name_user_retweeted NVARCHAR,"
                   "profile_image_url_retweeted NVARCHAR,"
                   "pictureNumber INT)"
                   );
        query.exec("CREATE TABLE commentstomePictures ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name NVARCHAR,"
                   "url NVARCHAR)"
                   );
        query.exec("CREATE TABLE usertimeline ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "id_status NVARCHAR,"
                   "id_user_status NVARCHAR,"
                   "text_status NVARCHAR,"
                   "screen_name_user_status NVARCHAR,"
                   "profile_image_url_status NVARCHAR,"
                   "isRepost BOOL,"
                   "text_retweeted NVARCHAR,"
                   "id_retweeted NVARCHAR,"
                   "id_user_retweeted NVARCHAR,"
                   "screen_name_user_retweeted NVARCHAR,"
                   "profile_image_url_retweeted NVARCHAR,"
                   "pictureNumber INT)"
                   );
        query.exec("CREATE TABLE usertimelinePictures ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name NVARCHAR,"
                   "url NVARCHAR)"
                   );
        query.exec("CREATE TABLE usermentions ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "id_status NVARCHAR,"
                   "id_user_status NVARCHAR,"
                   "text_status NVARCHAR,"
                   "screen_name_user_status NVARCHAR,"
                   "profile_image_url_status NVARCHAR,"
                   "isRepost BOOL,"
                   "text_retweeted NVARCHAR,"
                   "id_retweeted NVARCHAR,"
                   "id_user_retweeted NVARCHAR,"
                   "screen_name_user_retweeted NVARCHAR,"
                   "profile_image_url_retweeted NVARCHAR,"
                   "pictureNumber INT)"
                   );
        query.exec("CREATE TABLE usermentionsPictures ("
                   "num INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "name NVARCHAR,"
                   "url NVARCHAR)"
                   );
    }
    db.close();//关闭数据库
}

//判断数据库是否存在
bool Controller::dbIsExist(QString dbName){
    QFileInfo dbFileInfo(dbName);
    return dbFileInfo.exists();
}

//打开某一特定的数据库
bool Controller::dbOpenSpecified(QSqlDatabase &db, QString dbName){
    if(dbIsExist(dbName) == false){//数据库是否存在
        messageDialog->displayMessage("数据库不存在");
        return false;
    }
    if(QSqlDatabase::contains("qt_sql_default_connection"))//是否已有连接
        db=QSqlDatabase::database("qt_sql_default_connection");
    else
        db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(db.open() == false){//数据库打开失败
        messageDialog->displayMessage("数据库打开失败");
        return false;
    }
    return true;
}

//保存时间线
void Controller::saveTimeline(QString table, std::list<statusInfoT> statusesList){
    QSqlDatabase db;
    if(!dbOpenSpecified(db, DatabaseName)){//开数据库
        return;
    }

    QSqlQuery query;
    query.exec("DELETE FROM "+table);//选table
    query.exec("DELETE FROM "+table+"Pictures");
    std::list<statusInfoT>::iterator itor;
    for(itor=statusesList.begin(); itor!=statusesList.end(); itor++){//遍历list，保存
        query.prepare("INSERT INTO "+table+"(id_status, id_user_status, text_status,"
                                           "screen_name_user_status, profile_image_url_status,"
                                           "isRepost, text_retweeted, id_retweeted,"
                                           "id_user_retweeted, screen_name_user_retweeted,"
                                           "profile_image_url_retweeted, pictureNumber)"
                      " VALUES(:id_status, :id_user_status, :text_status,"
                                           ":screen_name_user_status, :profile_image_url_status,"
                                           ":isRepost, :text_retweeted, :id_retweeted,"
                                           ":id_user_retweeted, :screen_name_user_retweeted,"
                                           ":profile_image_url_retweeted, :pictureNumber)");
        query.bindValue(":id_status", itor->id_status);
        query.bindValue(":id_user_status", itor->id_user_status);
        query.bindValue(":text_status", itor->text_status);
        query.bindValue(":screen_name_user_status", itor->screen_name_user_status);
        query.bindValue(":profile_image_url_status", itor->profile_image_url_status);
        query.bindValue(":isRepost", itor->isRepost);
        query.bindValue(":text_retweeted", itor->text_retweeted);
        query.bindValue(":id_retweeted", itor->id_retweeted);
        query.bindValue(":id_user_retweeted", itor->id_user_retweeted);
        query.bindValue(":screen_name_user_retweeted", itor->screen_name_user_retweeted);
        query.bindValue(":profile_image_url_retweeted", itor->profile_image_url_retweeted);
        int num=itor->pictureList.size();
        query.bindValue(":pictureNumber", num);
        query.exec();

        std::list<QString>::iterator itor1;
        int i=0;
        for(itor1=itor->pictureList.begin(); itor1!=itor->pictureList.end(); itor1++, i++){
            query.prepare("INSERT INTO "+table+"Pictures(name, url)"
                          " VALUES(:name, :url)");
            query.bindValue(":name", itor->id_status+"_"+QString::number(i)+".jpg");
            query.bindValue(":url", *itor1);
            query.exec();
        }
    }

    db.close();//关闭数据库
}

//从数据库中恢复时间线
std::list<statusInfoT> Controller::recoverTimeline(QString table){
    std::list<statusInfoT> statusesList;
    statusInfoT status;

    QSqlDatabase db;//开数据库
    if(!dbOpenSpecified(db, DatabaseName)){
        return statusesList;
    }

    QSqlQuery query;
    query.exec("SELECT * FROM "+table);
    while(query.next()){//遍历数据库的查询结果
        //读出数据库中的数据
        status.id_status=query.value("id_status").toString();
        status.id_user_status=query.value("id_user_status").toString();
        status.text_status=query.value("text_status").toString();
        status.screen_name_user_status=query.value("screen_name_user_status").toString();
        status.profile_image_url_status=query.value("profile_image_url_status").toString();
        status.isRepost=query.value("isRepost").toBool();
        status.text_retweeted=query.value("text_retweeted").toString();
        status.id_retweeted=query.value("id_retweeted").toString();
        status.id_user_retweeted=query.value("id_user_retweeted").toString();
        status.screen_name_user_retweeted=query.value("screen_name_user_retweeted").toString();
        status.profile_image_url_retweeted=query.value("profile_image_url_retweeted").toString();
        int number=query.value("pictureNumber").toInt();
        status.pictureList.clear();
        for(int i=0; i<number; i++){
            QString url;
            QSqlQuery query1;
            qDebug()<<query1.exec("SELECT * FROM "+table+"Pictures WHERE name = '"+status.id_status+"_"+QString::number(i)+".jpg'");
            query1.next();
            url=query1.value("url").toString();
            qDebug()<<url;

            status.pictureList.push_back(url);
        }

        statusesList.push_back(status);
    }
    db.close();//关闭数据库
    return statusesList;
}

//清除数据库中的所有内容
void Controller::clearDababase(){
    QSqlDatabase db;
    if(!dbOpenSpecified(db, DatabaseName)){//开数据库
        return;
    }

    QSqlQuery query;//清除
    query.exec("DELETE FROM friendstimeline");
    query.exec("DELETE FROM friendstimelinePictures");
    query.exec("DELETE FROM mentionstimeline");
    query.exec("DELETE FROM mentionstimelinePictures");
    query.exec("DELETE FROM commentstome");
    query.exec("DELETE FROM commentstomePictures");
    query.exec("DELETE FROM commentsbyme");
    query.exec("DELETE FROM commentsbymePictures");
    query.exec("DELETE FROM usertimeline");
    query.exec("DELETE FROM usertimelinePictures");
    query.exec("DELETE FROM usermentions");
    query.exec("DELETE FROM usermentionsPictures");
}
