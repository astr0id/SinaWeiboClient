/*
 * 文件名：weibo.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了Weibo类中各模块公用的部分函数。主要包括处理
 * 原始的网络请求等待。
 */

#include "weibo.h"

//构造函数
Weibo::Weibo(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager;
    request = new QNetworkRequest;
    //reply = new QNetworkReply;

    nowStatus=unknown;//设置初始状态

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestResult(QNetworkReply*)));
}

//处理返回json的初始状态
void Weibo::requestResult(QNetworkReply * reply){

    if(reply->error() == QNetworkReply::NoError){//若返回的内容无误

        QByteArray bytes = reply->readAll();//读取返回数据

        QString result(bytes);
        //result=QObject::tr(result);
        qDebug()<<result;

        //根据当前状态选择一个parser
        if(nowStatus==oauth2_access_token){
            parseOauthAccessToken(result);
            getUserInfo();
            //emit userShow();
        }
        else if(nowStatus==statuses_update){
            //qDebug()<<"微博发送成功";
        }
        else if(nowStatus==statuses_public_timeline){
            //qDebug()<<"getStatusesPublicTimeline";
        }
        else if(nowStatus==statuses_friends_timeline){
            parseStatusesFriendsTimeline(result);
        }
        else if(nowStatus==statuses_user_timeline){
            parseStatusesUserTimeline(result);
        }
        else if(nowStatus==statuses_mentions){
            parseStatusesMentions(result);
        }
        else if(nowStatus==statuses_show){
            parseStatusesShow(result);
        }
        else if(nowStatus==statuses_repost){
            parseStatusesRepost(result);
        }
        else if(nowStatus==statuses_destroy){
            parseStatusesDestroy(result);
        }
        else if(nowStatus==statuses_upload){
            parseStatusesUpload(result);
        }

        else if(nowStatus==comments_show){
            parseCommentsShow(result);
        }
        else if(nowStatus==comments_by_me){
            parseCommentsByMe(result);
        }
        else if(nowStatus==comments_to_me){
            parseCommentsToMe(result);
        }
        else if(nowStatus==comments_timeline){
            parseCommentsTimeline(result);
        }
        else if(nowStatus==comments_mentions){
            parseCommentsMentions(result);
        }
        else if(nowStatus==comments_create){
            parseCommentsCreate(result);
        }
        else if(nowStatus==comments_destroy){
            parseCommentsDestroy(result);
        }
        else if(nowStatus==comments_reply){
            parseCommentsReply(result);
        }

        else if(nowStatus==friendships_friends){
            parseFriendshipsFriends(result);
        }
        else if(nowStatus==friendships_followers){
            parseFriendshipsFollowers(result);
        }
        else if(nowStatus==friendships_create){
            parseFriendshipsCreate(result);
        }
        else if(nowStatus==friendships_destroy){
            parseFriendshipsDestroy(result);
        }

        else if(nowStatus==remind_unread_count){
            parseRemindUnreadCount(result);
        }

        else if(nowStatus==users_show){
            parseGetUsersShow(result);
            emit userShow();
        }
        else if(nowStatus==search_suggestions_users){
            parseSearchUser(result);
            
        }
        else{
            qDebug()<<"unknown_operation";
        }

    }
    else{//若返回的数据有误
        qDebug()<<"reply error"<<reply->error()<<reply->errorString();
        if(nowStatus!=remind_unread_count){
            emit displayMessage("网络请求出错\n请检查网络状态和接口权限");
        }
        if(nowStatus==statuses_friends_timeline){
            emit moveList(0);
        }
        else if(nowStatus==comments_mentions){
            emit moveList(1);
        }
    }
    reply->deleteLater();
    //nowStatus=unknown;
}

//获取当前登录用户的access_token
QString Weibo::getAccessToken(){
    return this->accessToken;
}

//获取当前登录用户令牌的剩余时间
QString Weibo::getRemindIn(){
    return this->remind_in;
}

//获取当前登录用户的uid
QString Weibo::getUid(){
    return this->uid;
}

//设置当前登录用户的令牌
void Weibo::setAccessToken(QString _accessToken){
    this->accessToken=_accessToken;
}

//设置剩余时间
void Weibo::setRemindIn(QString _remind_in){
    this->remind_in=_remind_in;
}

//设置用户的uid
void Weibo::setUid(QString _uid){
    this->uid=_uid;
}


