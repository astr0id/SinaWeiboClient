/*
 * 文件名：weibo_friendships.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了weibo类关于好友模块的api调用及解析json。
 */

#include "weibo.h"

//获取关注人列表
void Weibo::getFriendshipsFriends(QString count){
    nowStatus=friendships_friends;
    request->setUrl(QUrl(url_friendships_friends+"?access_token="+accessToken+
                         "&uid=" + uid +
                         "&count" + count));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseFriendshipsFriends(QString json){
    friendsList.clear();
    friendsInfoT friendInfo;
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error==QJsonParseError::NoError){
        if(parse_doucment.isObject()){

            QJsonObject objAll=parse_doucment.object();
            QJsonValue arrayValue=objAll.take("users");
            QJsonArray array=arrayValue.toArray();

            int size=array.size();
            for(int i=0; i<size; i++){
                QJsonValue value=array.at(i);
                QJsonObject obj=value.toObject();
                if(obj.contains("id")){
                    friendInfo.id=obj.take("id").toVariant().toString();
                }
                if(obj.contains("screen_name")){
                    friendInfo.screen_name=obj.take("screen_name").toString();
                }
                if(obj.contains("description")){
                    friendInfo.description=obj.take("description").toString();
                }
                if(obj.contains("profile_image_url")){
                    friendInfo.profile_image_url=obj.take("profile_image_url").toString();
                }
                friendsList.push_back(friendInfo);
            }

        }
        else{
            qDebug()<<"Weibo::parseFriendshipsFriends()中 json解析结果有误";
        }
    }

    emit returnFriendshipsFriends();
}

//获取粉丝列表
void Weibo::getFriendshipsFollowers(QString count){
    nowStatus=friendships_followers;
    request->setUrl(QUrl(url_friendships_followers+"?access_token="+accessToken+
                         "&uid=" + uid +
                         "&count=" + count));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseFriendshipsFollowers(QString json){
    friendsList.clear();
    friendsInfoT friendInfo;
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error==QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            //qDebug()<<"is array";
            QJsonObject objAll=parse_doucment.object();
            //qDebug()<<"users is array?"<<objAll.contains("users");
            QJsonValue arrayValue=objAll.take("users");
            //qDebug()<<"array?"<<arrayValue.isArray();
            //qDebug()<<arrayValue.toVariant().toString();
            QJsonArray array=arrayValue.toArray();

            int size=array.size();
            qDebug()<<"search size is"<<size;
            for(int i=0; i<size; i++){
                QJsonValue value=array.at(i);
                QJsonObject obj=value.toObject();
                if(obj.contains("id")){
                    friendInfo.id=obj.take("id").toVariant().toString();
                }
                if(obj.contains("screen_name")){
                    friendInfo.screen_name=obj.take("screen_name").toString();
                }
                if(obj.contains("description")){
                    friendInfo.description=obj.take("description").toString();
                }
                if(obj.contains("profile_image_url")){
                    friendInfo.profile_image_url=obj.take("profile_image_url").toString();
                }
                friendsList.push_back(friendInfo);
            }
        }
        else{
            qDebug()<<"Weibo::parseFriendshipsFollowers()中 json解析结果有误";
        }
    }
    emit returnFriendshipsFollowers();
}

//关注一个用户
void Weibo::postFriendshipsCreate(QString uid, QString screen_name){
    nowStatus=friendships_create;
    request->setUrl(QUrl(url_friendships_create +
                         "?access_token=" + accessToken +
                         "&uid=" + uid +
                         "&screen_name=" + screen_name.toUtf8().toPercentEncoding() +
                         ""));
    QByteArray postData;
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->post(*request, postData);
}

//解析json
void Weibo::parseFriendshipsCreate(QString json){

}

//取消关注一个用户
void Weibo::postFriendshipsDestroy(QString uid, QString screen_name){
    nowStatus=friendships_destroy;
    request->setUrl(QUrl(url_friendships_destroy +
                         "?access_token=" + accessToken +
                         "&uid=" + uid +
                         "&screen_name=" + screen_name.toUtf8().toPercentEncoding() +
                         ""));
    QByteArray postData;
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->post(*request, postData);
}

//解析json
void Weibo::parseFriendshipsDestroy(QString json){

}
