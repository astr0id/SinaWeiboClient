/*
 * 文件名：weibo_statuses.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了weibo类关于statuses模块的api调用及解析json。
 */

#include "weibo.h"

//发文字微博
void Weibo::postNewWeibo(QString newWeibo){
    //qDebug()<<"in Weibo::postNewWeibo()"<<newWeibo;

    nowStatus=statuses_update;
    request->setUrl(QUrl(url_statuses_update +
                         "?access_token=" + accessToken +
                         "&status=" + newWeibo.toUtf8().toPercentEncoding() +
                         ""));
    QByteArray postData;

    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->post(*request, postData);
}

//获取公共时间线
void Weibo::getStatusesPublicTimeline(){
    nowStatus=statuses_public_timeline;
    request->setUrl(QUrl(url_statuses_public_timeline+"?access_token="+accessToken));
    reply=manager->get(*request);
}

//获取登录用户的主时间线
void Weibo::getStatusesFriendsTimeline(QString count){
    nowStatus=statuses_friends_timeline;
    request->setUrl(QUrl(url_statuses_friends_timeline+"?access_token=" + accessToken +
                         "&count=" + count));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseStatusesFriendsTimeline(QString json){
    parseStatusesStatusInfo(json);
    emit returnStatusesFriendsTimeline();
}

//获取用户自己微博的时间线
void Weibo::getStatusesUserTimeline(QString count){
    nowStatus=statuses_user_timeline;
    request->setUrl(QUrl(url_statuses_user_timeline+"?access_token=" + accessToken +
                         "&count=" + count));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseStatusesUserTimeline(QString json){
    parseStatusesStatusInfo(json);
    emit returnStatusesUserTimeline();
}

//获取mentions时间线
void Weibo::getStatusesMentions(QString count){
    nowStatus=statuses_mentions;
    request->setUrl(QUrl(url_statuses_mentions+"?access_token=" + accessToken +
                         "&count=" + count));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseStatusesMentions(QString json){
    parseStatusesStatusInfo(json);
    emit returnStatusesMentions();
}

//获取某一微博的内容
void Weibo::getStatusesShow(QString id){
    nowStatus=statuses_show;
    request->setUrl(QUrl(url_statuses_show+"?access_token=" + accessToken +
                         "&id=" + id));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseStatusesShow(QString json){
    parseStatusesSingleStatus(json);
    emit returnStatusesShow();
}

//转发微博
void Weibo::postStatusesRepost(QString id, QString status){
    nowStatus=statuses_repost;
    request->setUrl(QUrl(url_statuses_repost +
                         "?access_token=" + accessToken +
                         "&id=" + id +
                         "&status=" + status.toUtf8().toPercentEncoding() +
                         ""));
    QByteArray postData;
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->post(*request, postData);

}

//解析json
void Weibo::parseStatusesRepost(QString json){
    emit returnStatusesRepost();
}

//删除微博
void Weibo::postStatusesDestroy(QString id){

    nowStatus=statuses_destroy;
    request->setUrl(QUrl(url_statuses_destroy +
                         "?access_token=" + accessToken +
                         "&id=" + id +
                         ""));
    QByteArray postData;
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->post(*request, postData);
}

//解析json
void Weibo::parseStatusesDestroy(QString json){
    emit returnStatusesDestroy();
}

//发布带图片的微博
void Weibo::postStatusesUpload(QString status, QString picturePath){
    nowStatus=statuses_upload;

    QHttpMultiPart * multiPart=new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg("access_token")));
    textPart.setBody(accessToken.toUtf8());
    multiPart->append(textPart);

    QHttpPart textPart1;
    textPart1.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    textPart1.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg("status")));
    textPart1.setBody(status.toUtf8());
    multiPart->append(textPart1);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"pic\"; filename=\""+picturePath+"\""));

    QFile * file=new QFile(picturePath);
    if(!file->open(QFile::ReadOnly)){
        qDebug()<<"文件打开不成功";
        return;
    }

    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);


    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::UnknownProtocol);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    request->setSslConfiguration(config);

    QUrl url("https://upload.api.weibo.com/2/statuses/upload.json");
    request->setUrl(url);

    reply = manager->post(*request, multiPart);
}

//解析json
void Weibo::parseStatusesUpload(QString json){
    emit returnStatusesUpload();
}

//解析json
void Weibo::parseStatusesStatusInfo(QString json){
    statusesList.clear();
    statusInfoT status;
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error==QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            QJsonObject objAll=parse_doucment.object();
            QJsonValue arrayValue=objAll.take("statuses");
            QJsonArray array=arrayValue.toArray();
            int size=array.size();
            for(int i=0; i<size; i++){
                QJsonValue value=array.at(i);
                QJsonObject obj=value.toObject();
                if(obj.contains("id")){
                    status.id_status=obj.take("id").toVariant().toString();
                }
                if(obj.contains("text")){
                    status.text_status=obj.take("text").toString();
                }
                if(obj.contains("user")){
                    QJsonObject userObj=obj.take("user").toObject();
                    status.id_user_status=userObj.take("id").toVariant().toString();
                    status.screen_name_user_status=userObj.take("screen_name").toString();
                    status.profile_image_url_status=userObj.take("profile_image_url").toString();
                }
                if(obj.contains("retweeted_status")){
                    status.isRepost=true;

                    QJsonObject retweetedObj=obj.take("retweeted_status").toObject();
                    status.text_retweeted=retweetedObj.take("text").toString();
                    status.id_retweeted=retweetedObj.take("id").toVariant().toString();
                    QJsonObject userObj=retweetedObj.take("user").toObject();
                    status.id_user_retweeted=userObj.take("id").toVariant().toString();
                    status.screen_name_user_retweeted=userObj.take("screen_name").toString();
                    status.profile_image_url_retweeted=userObj.take("profile_image_url").toString();

                    status.pictureList.clear();
                    if(retweetedObj.contains("pic_urls")){
                        QJsonArray picArray=retweetedObj.take("pic_urls").toArray();
                        for(int i=0; i<picArray.size(); i++){
                            QJsonValue picValue=picArray.at(i);
                            QJsonObject picObj=picValue.toObject();
                            status.pictureList.push_back(picObj.take("thumbnail_pic").toString());
                        }
                    }
                }
                else{
                    status.isRepost=false;

                    status.pictureList.clear();
                    if(obj.contains("pic_urls")){
                        QJsonArray picArray=obj.take("pic_urls").toArray();
                        for(int i=0; i<picArray.size(); i++){
                            QJsonValue picValue=picArray.at(i);
                            QJsonObject picObj=picValue.toObject();
                            status.pictureList.push_back(picObj.take("thumbnail_pic").toString());
                        }
                    }
                }

                statusesList.push_back(status);
            }
        }
        else{
            qDebug()<<"Weibo::parseStatusesStatusInfo()中 json解析结果有误";
        }
    }
//    std::list<statusInfoT>::iterator itor;
//    for(itor=statusesList.begin(); itor!=statusesList.end(); itor++){
//        qDebug()<<"";
//        qDebug()<<itor->id_status;
//        qDebug()<<itor->id_user_status;
//        qDebug()<<itor->text_status.toStdString().c_str();
//        qDebug()<<itor->screen_name_user_status.toStdString().c_str();
//        qDebug()<<itor->profile_image_url_status;
//        if(itor->isRepost){
//            qDebug()<<"-----------retweeted------------";
//            qDebug()<<itor->text_retweeted.toStdString().c_str();
//            qDebug()<<itor->id_retweeted;
//            qDebug()<<itor->id_user_retweeted;
//            qDebug()<<itor->screen_name_user_retweeted.toStdString().c_str();
//            qDebug()<<itor->profile_image_url_retweeted;
//        }
//        else{
//            qDebug()<<"非转发微博";
//        }
//        qDebug()<<"--------------pic_urls--------------";
//        std::list<QString>::iterator itor1;
//        for(itor1=itor->pictureList.begin(); itor1!=itor->pictureList.end(); itor1++){
//            qDebug()<<*itor1;
//        }
//    }

}

//解析json
void Weibo::parseStatusesSingleStatus(QString json){
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("id")){
                singleStatusInfo.id_status=obj.take("id").toVariant().toString();
            }
            if(obj.contains("text")){
                singleStatusInfo.text_status=obj.take("text").toString();
            }
            if(obj.contains("user")){
                QJsonObject userObj=obj.take("user").toObject();
                if(userObj.contains("id")){
                    singleStatusInfo.id_user_status=userObj.take("id").toVariant().toString();
                }
                if(userObj.contains("screen_name")){
                    singleStatusInfo.screen_name_user_status=userObj.take("screen_name").toString();
                }
                if(userObj.contains("profile_image_url")){
                    singleStatusInfo.profile_image_url_status=userObj.take("profile_image_url").toString();
                }
            }
            if(obj.contains("retweeted_status")){
                singleStatusInfo.isRepost=true;
                QJsonObject retweetedObj=obj.take("retweeted_status").toObject();
                singleStatusInfo.text_retweeted=retweetedObj.take("text").toString();
                singleStatusInfo.id_retweeted=retweetedObj.take("id").toVariant().toString();
                QJsonObject userObj=retweetedObj.take("user").toObject();
                singleStatusInfo.id_user_retweeted=userObj.take("id").toVariant().toString();
                singleStatusInfo.screen_name_user_retweeted=userObj.take("screen_name").toString();
                singleStatusInfo.profile_image_url_retweeted=userObj.take("profile_image_url").toString();

            }
            else{
                singleStatusInfo.isRepost=false;
            }
        }
        else{
            qDebug()<<"Weibo::parseStatusesSingleStatus()中 json解析结果有误";
        }
    }
//    qDebug()<<singleStatusInfo.id_status;
//    qDebug()<<singleStatusInfo.id_user_status;
//    qDebug()<<singleStatusInfo.text_status.toStdString().c_str();
//    qDebug()<<singleStatusInfo.screen_name_user_status.toStdString().c_str();
//    qDebug()<<singleStatusInfo.profile_image_url_status;
//    if(singleStatusInfo.isRepost){
//        qDebug()<<"-----------retweeted------------";
//        qDebug()<<singleStatusInfo.text_retweeted.toStdString().c_str();
//        qDebug()<<singleStatusInfo.id_retweeted;
//        qDebug()<<singleStatusInfo.id_user_retweeted;
//        qDebug()<<singleStatusInfo.screen_name_user_retweeted.toStdString().c_str();
//        qDebug()<<singleStatusInfo.profile_image_url_retweeted;
//    }
//    else{
//        qDebug()<<"非转发微博";
//    }
}

//解析json
void Weibo::parseStatusesRepostInfo(QString json){
    //no need now
}





































