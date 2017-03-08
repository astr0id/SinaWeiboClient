/*
 * 文件名：weibo_remind.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了weibo类关于提醒模块的api调用及解析json。
 */

#include "weibo.h"

//获取未读信息数
void Weibo::getRemindUnreadCount(){
    nowStatus=remind_unread_count;

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::UnknownProtocol);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    request->setSslConfiguration(config);

    request->setUrl(QUrl(url_remind_unread_count+
                         "?access_token="+ accessToken +
                         "&uid=" + uid +
                         ""));
    reply=manager->get(*request);
}

//解析json
void Weibo::parseRemindUnreadCount(QString json){
    unreadCount.status=-1;
    unreadCount.followers=-1;
    unreadCount.cmt=-1;
    unreadCount.dm=-1;
    unreadCount.mention_status=-1;
    unreadCount.mention_cmt=-1;
    unreadCount.group=-1;
    unreadCount.private_group=-1;
    unreadCount.notice=-1;
    unreadCount.invite=-1;
    unreadCount.badge=-1;
    unreadCount.photo=-1;
    unreadCount.msgbox=-1;
    QByteArray byte_array;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
    if(json_error.error == QJsonParseError::NoError){
        if(parse_doucment.isObject()){
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("status")){
                unreadCount.status=obj.take("status").toInt();
            }
            if(obj.contains("follower")){
                unreadCount.followers=obj.take("follower").toInt();
            }
            if(obj.contains("cmt")){
                unreadCount.cmt=obj.take("cmt").toInt();
            }
            if(obj.contains("dm")){
                unreadCount.dm=obj.take("dm").toInt();
            }
            if(obj.contains("mention_status")){
                unreadCount.mention_status=obj.take("mention_status").toInt();
            }
            if(obj.contains("mention_cmt")){
                unreadCount.mention_cmt=obj.take("mention_cmt").toInt();
            }
            if(obj.contains("group")){
                unreadCount.group=obj.take("group").toInt();
            }
            if(obj.contains("private_group")){
                unreadCount.private_group=obj.take("private_group").toInt();
            }
            if(obj.contains("notice")){
                unreadCount.notice=obj.take("notice").toInt();
            }
            if(obj.contains("invite")){
                unreadCount.invite=obj.take("invite").toInt();
            }
            if(obj.contains("badge")){
                unreadCount.badge=obj.take("badge").toInt();
            }
            if(obj.contains("photo")){
                unreadCount.photo=obj.take("photo").toInt();
            }
            if(obj.contains("msgbox")){
                unreadCount.msgbox=obj.take("msgbox").toInt();
            }
        }
    }
//    qDebug()<<unreadCount.status;
//    qDebug()<<unreadCount.followers;
//    qDebug()<<unreadCount.cmt;
//    qDebug()<<unreadCount.dm;
//    qDebug()<<unreadCount.mention_status;
//    qDebug()<<unreadCount.mention_cmt;
//    qDebug()<<unreadCount.group;
//    qDebug()<<unreadCount.private_group;
//    qDebug()<<unreadCount.notice;
//    qDebug()<<unreadCount.invite;
//    qDebug()<<unreadCount.badge;
//    qDebug()<<unreadCount.photo;
//    qDebug()<<unreadCount.msgbox;
    emit returnRemindUnreadCount(unreadCount);
}




