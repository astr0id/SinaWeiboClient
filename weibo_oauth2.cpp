/*
 * 文件名：weibo_oauth2.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了weibo类关于认证模块的api调用及解析json。
 */

#include "weibo.h"

//进行oauth2认证，获取access_token
void Weibo::postOauth2AccessToken(QString code){
    nowStatus=oauth2_access_token;
    request->setUrl(QUrl(url_oauth2_access_token +
                         "?client_id=" + AppKey +
                         "&client_secret=" + AppSecret +
                         "&grant_type=authorization_code" +
                         "&redirect_uri=" + url_redirect +
                         "&code=" + code));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray postData;
    reply=manager->post(*request, postData);
}

//解析json
void Weibo::parseOauthAccessToken(QString json){
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("access_token")){
                QJsonValue access_token_value = obj.take("access_token");
                if(access_token_value.isString()){
                    accessToken = access_token_value.toString();
                }
            }
            if(obj.contains("remind_in")){
                QJsonValue remind_in_value = obj.take("remind_in");
                if(remind_in_value.isString()){
                    remind_in = remind_in_value.toString();
                }
            }
            if(obj.contains("expires_in")){
                qDebug()<<"in expires_in";
                QJsonValue expires_in_value = obj.take("expires_in");
                if(expires_in_value.isDouble()){
                    qDebug()<<"in if";
                    expires_in = expires_in_value.toDouble();
                }
            }
            if(obj.contains("uid")){
                QJsonValue uid_value = obj.take("uid");
                if(uid_value.isString()){
                    uid = uid_value.toString();
                }
            }
        }
    }
//    qDebug()<<"access_token: "<<accessToken;
//    qDebug()<<"remind_in: "<<remind_in;
//    qDebug()<<"expires_in: "<<expires_in;
//    qDebug()<<"uid: "<<uid;
    //getUserInfo();
}
