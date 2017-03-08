/*
 * 文件名：weibo_comments.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了weibo类关于评论模块的api调用及解析json。
 */

#include "weibo.h"

//获取评论列表
void Weibo::getCommentsShow(QString id){
    nowStatus=comments_show;
    request->setUrl(QUrl(url_comments_show+"?access_token="+accessToken+
                         "&id=" + id));
    reply=manager->get(*request);
}

//解析json值
void Weibo::parseCommentsShow(QString json){
    parseCommentsInfo(json);
    emit returnCommentsShow();
}

//获取发出的评论列表
void Weibo::getCommentsByMe(){
    nowStatus=comments_by_me;
    request->setUrl(QUrl(url_comments_by_me+"?access_token="+accessToken));
    reply=manager->get(*request);
    //qDebug()<<"before finish Weibo::getCommentsByMe()";
}

//解析json
void Weibo::parseCommentsByMe(QString json){
    //parseCommentsInfo(json);
    parseCommentsInfoLikeStatus(json);
    emit returnCommentsByMe();
}

//获取收到的评论列表
void Weibo::getCommentsToMe(){
    nowStatus=comments_to_me;
    request->setUrl(QUrl(url_comments_to_me+"?access_token="+accessToken));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseCommentsToMe(QString json){
    //parseCommentsInfo(json);
    parseCommentsInfoLikeStatus(json);
    emit returnCommentsToMe();
}

//获取评论时间线
void Weibo::getCommentsTimeline(){
    nowStatus=comments_timeline;
    request->setUrl(QUrl(url_comments_timeline+"?access_token="+accessToken));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseCommentsTimeline(QString json){
    parseCommentsInfo(json);
    emit returnCommentsTimeline();
}

//获取mentions时间线
void Weibo::getCommentsMentions(){
    nowStatus=comments_mentions;

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::UnknownProtocol);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    request->setSslConfiguration(config);


    request->setUrl(QUrl(url_comments_mentions+"?access_token="+accessToken));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseCommentsMentions(QString json){
    //parseCommentsInfo(json);
    parseCommentsInfoLikeStatus(json);
    emit returnCommentsMentions();
}

//发评论
void Weibo::postCommentsCreate(QString comment, QString id){
    nowStatus=comments_create;
    request->setUrl(QUrl(url_comments_create +
                         "?access_token=" + accessToken +
                         "&comment=" + comment.toUtf8().toPercentEncoding() +
                         "&id=" + id +
                         ""));
    QByteArray postData;
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->post(*request, postData);
}

//解析json
void Weibo::parseCommentsCreate(QString json){

    emit returnCommentsCreate();
}

//删评论
void Weibo::postCommentsDestroy(QString cid){
    nowStatus=comments_destroy;
    request->setUrl(QUrl(url_comments_destroy +
                         "?access_token=" + accessToken +
                         "&cid=" + cid +
                         ""));
    QByteArray postData;
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->post(*request, postData);
}

//解析json
void Weibo::parseCommentsDestroy(QString json){

    emit returnCommentsDestroy();
}

//回复评论
void Weibo::postCommentsReply(QString cid, QString id, QString comment){
    nowStatus=comments_reply;
    request->setUrl(QUrl(url_comments_reply +
                         "?access_token=" + accessToken +
                         "&comment=" + comment.toUtf8().toPercentEncoding() +
                         "&id=" + id +
                         "&cid=" + cid +
                         "&without_mention=1"
                         ""));
    QByteArray postData;
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    reply = manager->post(*request, postData);
}

//解析json
void Weibo::parseCommentsReply(QString json){

    emit returnCommentsReply();
}

//解析json
void Weibo::parseCommentsInfo(QString json){
    commentsList.clear();
    commentInfoT commentInfo;
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error==QJsonParseError::NoError){
        //qDebug()<<parse_doucment.isArray();
        if(parse_doucment.isObject()){
            //qDebug()<<"isObject";
            QJsonObject objAll=parse_doucment.object();
            QJsonValue arrayValue=objAll.take("comments");
            QJsonArray array=arrayValue.toArray();
            int size=array.size();
            //qDebug()<<"search size is"<<size;
            for(int i=0; i<size; i++){

                QJsonValue value=array.at(i);
                QJsonObject obj=value.toObject();

                if(obj.contains("id")){
                    commentInfo.id_comment=obj.take("id").toVariant().toString();
                }
                if(obj.contains("text")){
                    commentInfo.text_comment=obj.take("text").toString();
                }
                if(obj.contains("user")){
                    QJsonObject userObj=obj.take("user").toObject();
                    commentInfo.id_user=userObj.take("id").toVariant().toString();
                    commentInfo.screen_name_comment=userObj.take("screen_name").toString();
                    commentInfo.profile_image_url=userObj.take("profile_image_url").toString();

                }
                if(obj.contains("status")){
                    QJsonObject statusObj=obj.take("status").toObject();
                    //qDebug()<<statusObj.contains("retweeted");
                    commentInfo.id_status=statusObj.take("id").toVariant().toString();
                    commentInfo.text_status=statusObj.take("text").toString();
                    commentInfo.screen_name_status=statusObj.take("user").toObject().take("screen_name").toString();
                }
                if(obj.contains("reply_comment")){
                    //qDebug()<<"contains reply_comment";
                    QJsonObject replyObj=obj.take("reply_comment").toObject();
                    commentInfo.isReply=true;
                    commentInfo.screen_name_reply=replyObj.take("user").toObject().take("screen_name").toString();
                    commentInfo.text_reply=replyObj.take("text").toString();
                }
                else{
                    commentInfo.isReply=false;
                }

                commentsList.push_back(commentInfo);
//                qDebug()<<"";
//                qDebug()<<commentInfo.id_comment;
//                qDebug()<<commentInfo.id_status;
//                qDebug()<<commentInfo.id_user;
//                qDebug()<<commentInfo.screen_name_comment.toStdString().c_str();
//                qDebug()<<commentInfo.screen_name_status.toStdString().c_str();
//                qDebug()<<commentInfo.text_comment.toStdString().c_str();
//                qDebug()<<commentInfo.text_status.toStdString().c_str();
//                qDebug()<<commentInfo.profile_image_url;
//                qDebug()<<"-------------------reply--------------------";
//                if(commentInfo.isReply){
//                    qDebug()<<commentInfo.text_reply;
//                    qDebug()<<commentInfo.screen_name_reply;
//                }
            }

        }
        else{
            qDebug()<<"Weibo::parseFriendshipsFriends()中 json解析结果有误";
        }
    }

}

//解析json
void Weibo::parseCommentsInfoLikeStatus(QString json){
    statusesList.clear();
    statusInfoT status;
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error==QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            QJsonObject objAll=parse_doucment.object();
            QJsonValue arrayValue=objAll.take("comments");
            QJsonArray array=arrayValue.toArray();
            int size=array.size();
            //qDebug()<<"search size is"<<size;
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
                if(obj.contains("status")){
                    status.isRepost=true;

                    QJsonObject retweetedObj=obj.take("status").toObject();
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
    //std::list<statusInfoT>::iterator itor;
    //for(itor=statusesList.begin(); itor!=statusesList.end(); itor++){
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
    //}
}





















