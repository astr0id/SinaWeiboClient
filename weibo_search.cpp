/*
 * 文件名：weibo_search.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了weibo类关于搜索模块的api调用及解析json。
 */

#include "weibo.h"

//搜索用户
void Weibo::searchUser(QString userName){
    nowStatus=search_suggestions_users;
    request->setUrl(QUrl(url_search_suggestions_users +
                         "?access_token=" + accessToken +
                         "&q=" + userName.toUtf8().toPercentEncoding() +
                         ""));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->get(*request);
}

//解析json
void Weibo::parseSearchUser(QString json){
    friendsList.clear();
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error==QJsonParseError::NoError){
        if(parse_doucment.isArray()){
            QJsonArray array=parse_doucment.array();
            int size=array.size();
            userSearchResultT result;
            for(int i=0; i<size; i++){
                QJsonValue value=array.at(i);
                QJsonObject obj=value.toObject();
                if(obj.contains("screen_name")){
                    result.screen_name=obj.take("screen_name").toString();
                }
                if(obj.contains("followers_count")){
                    QVariant variant=obj.take("followers_count").toVariant();
                    result.followers_count=variant.toString();
                }
                if(obj.contains("uid")){
                    QVariant variant=obj.take("uid").toVariant();
                    result.uid=variant.toString();
                }
                userSearchResultList.push_back(result);
            }
        }
        else{
            qDebug()<<"Weibo::parseSearchUser()中 json解析结果有误";
        }
    }
    emit returnSearchUsers();
}








