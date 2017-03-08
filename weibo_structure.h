/*
 * 文件名：weibo_structure.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件中定义了一些枚举类型，以及微博内容的结构体。
 */

#ifndef WEIBO_STRUCTURE
#define WEIBO_STRUCTURE

/*包含头文件*/

#include <QString>

/*定义枚举类型*/

enum WeiboStatus{//记录当前正在进行的网络请求的接口
    unknown,

    oauth2_access_token,

    statuses_update,
    statuses_public_timeline,
    statuses_friends_timeline,
    statuses_user_timeline,
    statuses_mentions,
    statuses_show,
    statuses_repost,
    statuses_destroy,
    statuses_upload,

    search_suggestions_users,

    comments_show,
    comments_by_me,
    comments_to_me,
    comments_timeline,
    comments_mentions,
    comments_create,
    comments_destroy,
    comments_reply,

    friendships_friends,
    friendships_followers,
    friendships_create,
    friendships_destroy,

    remind_unread_count,

    users_show
};

/*定义结构体*/

typedef struct{//记录登录用户的个人信息
    QString id;
    QString screen_name;
    QString location;
    QString description;
    QString gender;
    int followers_count;
    int friends_count;//following
    int statuses_count;
    int favourites_count;
    QString profile_image;
    QString avatar_large;

}userInfoT;

typedef struct{//记录搜索用户结果
    QString screen_name;
    QString followers_count;
    QString uid;
}userSearchResultT;

typedef struct{//记录好友信息
    QString id;
    QString screen_name;
    QString description;
    QString profile_image_url;

}friendsInfoT;

typedef struct{//记录每条微博的内容
    QString id_status;
    QString id_user_status;
    QString text_status;
    QString screen_name_user_status;
    QString profile_image_url_status;
    bool isRepost;
    QString text_retweeted;
    QString id_retweeted;
    QString id_user_retweeted;
    QString screen_name_user_retweeted;
    QString profile_image_url_retweeted;

    std::list<QString> pictureList;
}statusInfoT;

typedef struct{//记录每条评论的内容
    QString id_comment;
    QString id_status;
    QString id_user;
    QString screen_name_comment;
    QString screen_name_status;
    QString profile_image_url;
    QString text_comment;
    QString text_status;

    bool isReply;
    QString text_reply;
    QString screen_name_reply;
}commentInfoT;


typedef struct {//记录未读消息的结构体
    int status;
    int followers;
    int cmt;
    int dm;
    int mention_status;
    int mention_cmt;
    int group;
    int private_group;
    int notice;
    int invite;
    int badge;
    int photo;
    int msgbox;
}unreadCountT;

#endif // WEIBO_STRUCTURE

