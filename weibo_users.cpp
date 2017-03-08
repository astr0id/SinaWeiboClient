/*
 * 文件名：weibo_users.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了weibo类关于user模块的api调用及解析json。
 */

#include "weibo.h"

//获取用户信息
void Weibo::getUserInfo(){
    qDebug()<<"in Weibo::getUserInfo()"<<uid;
    nowStatus=users_show;
    request->setUrl(QUrl(url_users_show+
                         "?access_token="+ accessToken +
                         "&uid=" + uid +
                         ""));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseGetUsersShow(QString json){
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("id")){
                QJsonValue id_value = obj.take("id");
                userInfo.id=id_value.toVariant().toString();
            }
            if(obj.contains("screen_name")){
                QJsonValue screen_name_value = obj.take("screen_name");
                if(screen_name_value.isString()){
                    userInfo.screen_name = screen_name_value.toString();
                }
            }
            if(obj.contains("location")){
                QJsonValue location_value = obj.take("location");
                if(location_value.isString()){
                    userInfo.location = location_value.toString();
                }
            }
            if(obj.contains("description")){
                QJsonValue description_value = obj.take("description");
                if(description_value.isString()){
                    userInfo.description = description_value.toString();
                }
            }
            if(obj.contains("gender")){
                QJsonValue gender_value = obj.take("gender");
                if(gender_value.isString()){
                    userInfo.gender = gender_value.toString();
                }
            }
            if(obj.contains("followers_count")){
                QJsonValue followers_count_value = obj.take("followers_count");
                userInfo.followers_count = followers_count_value.toInt();

            }
            if(obj.contains("friends_count")){
                QJsonValue friends_count_value = obj.take("friends_count");

                userInfo.friends_count = friends_count_value.toInt();

            }
            if(obj.contains("statuses_count")){
                QJsonValue statuses_count_value = obj.take("statuses_count");

                userInfo.statuses_count = statuses_count_value.toInt();

            }
            if(obj.contains("favourites_count")){
                QJsonValue favourites_count_value = obj.take("favourites_count");

                userInfo.favourites_count = favourites_count_value.toInt();

            }
            if(obj.contains("profile_image_url")){
                QJsonValue profile_image_value = obj.take("profile_image_url");
                if(profile_image_value.isString()){
                    userInfo.profile_image = profile_image_value.toString();
                }
            }
            if(obj.contains("avatar_large")){
                QJsonValue avatar_large_value = obj.take("avatar_large");
                if(avatar_large_value.isString()){
                    userInfo.avatar_large = avatar_large_value.toString();
                }
            }

        }
    }
//    qDebug()<<userInfo.screen_name;
//    qDebug()<<userInfo.location;
//    qDebug()<<userInfo.description;
//    qDebug()<<userInfo.gender;
//    qDebug()<<userInfo.followers_count;
//    qDebug()<<userInfo.friends_count;
//    qDebug()<<userInfo.statuses_count;
//    qDebug()<<userInfo.favourites_count;
//    qDebug()<<userInfo.profile_image;
//    qDebug()<<userInfo.avatar_large;
    //qDebug()<<userInfo;


}


