/*
 * 文件名：weibo_url.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件中定义了一系列常量，用于表示微博各个接口的URL。
 */

#ifndef WEIBO_URL
#define WEIBO_URL

/*包含头文件*/

#include <QString>

/*常量定义*/

const QString AppKey = "2900848875";
const QString AppSecret = "43b8b2478048c1b169080f7df258d163";

const QString url_weibo_api = "https://api.weibo.com";

const QString url_redirect = "https://api.weibo.com/oauth2/default.html"; // weibo default Blank RedirectUrl
const QString url_oauth_getcode = url_weibo_api + "/oauth2/authorize?client_id=" +
                                AppKey + "&redirect_uri=" + url_redirect + "&response_type=code";

const QString url_oauth2_authorize = "https://api.weibo.com/oauth2/authorize";
const QString url_oauth2_access_token = "https://api.weibo.com/oauth2/access_token";
const QString url_oauth2_get_token_info = "https://api.weibo.com/oauth2/get_token_info";

const QString url_statuses_public_timeline = "https://api.weibo.com/2/statuses/public_timeline.json";
const QString url_statuses_friends_timeline = "https://api.weibo.com/2/statuses/friends_timeline.json";
//const QString url_statuses_home_timeline = "";
//const QString url_statuses_friends_timeline_ids = "";
const QString url_statuses_user_timeline = "https://api.weibo.com/2/statuses/user_timeline.json";
//const QString url_statuses_repost_timeline = "https://api.weibo.com/2/statuses/repost_timeline.json";
//const QString url_statuses_repost_timeline_ids = "";
const QString url_statuses_mentions = "https://api.weibo.com/2/statuses/mentions.json";
//const QString url_statuses_mentions_ids = "";


const QString url_statuses_show = "https://api.weibo.com/2/statuses/show.json";
const QString url_statuses_repost = "https://api.weibo.com/2/statuses/repost.json";
const QString url_statuses_destroy = "https://api.weibo.com/2/statuses/destroy.json";
const QString url_statuses_upload = "https://upload.api.weibo.com/2/statuses/upload.json";
const QString url_comments_show = "https://api.weibo.com/2/comments/show.json";
const QString url_comments_create = "https://api.weibo.com/2/comments/create.json";
const QString url_comments_destroy = "https://api.weibo.com/2/comments/destroy.json";
const QString url_comments_reply = "https://api.weibo.com/2/comments/reply.json";

//const QString url_statuses_querymid = "";
//const QString url_statuses_count = "";
//const QString url_statuses_go = "";
//const QString url_emotions = "";
const QString url_statuses_update = "https://api.weibo.com/2/statuses/update.json";

const QString url_comments_by_me = "https://api.weibo.com/2/comments/by_me.json";
const QString url_comments_to_me = "https://api.weibo.com/2/comments/to_me.json";
const QString url_comments_timeline = "https://api.weibo.com/2/comments/timeline.json";
const QString url_comments_mentions = "https://api.weibo.com/2/comments/mentions.json";
//const QString url_comments_show_batch = "";

//const QString url_comments_destroy_batch = "";

const QString url_users_show = "https://api.weibo.com/2/users/show.json";
//const QString url_users_domain_show = "";
//const QString url_users_counts = "";

const QString url_friendships_friends = "https://api.weibo.com/2/friendships/friends.json";
//const QString url_friendships_friends_in_common = "";
//const QString url_friendships_friends_bilateral = "";
//const QString url_friendships_friends_bilateral_ids = "";
//const QString url_friendships_friends_ids = "";
const QString url_friendships_followers = "https://api.weibo.com/2/friendships/followers.json";
//const QString url_friendships_followers_ids = "";
//const QString url_friendships_show = "";
const QString url_friendships_create = "https://api.weibo.com/2/friendships/create.json";
const QString url_friendships_destroy = "https://api.weibo.com/2/friendships/destroy.json";

//const QString url_account_rate_limit_status = "";
//const QString url_account_get_uid = "";

//const QString url_favorites = "";
//const QString url_favorites_ids = "";
//const QString url_favorites_show = "";
//const QString url_favorites_by_tags = "";
//const QString url_favorites_tags = "";
//const QString url_favorites_by_tags_ids = "";
//const QString url_favorites_create = "";
//const QString url_favorites_destroy = "";
//const QString url_favorites_destroy_batch = "";
//const QString url_favorites_update = "";
//const QString url_favorites_update_batch = "";
//const QString url_favorites_tags_update_batch = "";
//const QString url_favorites_tags_destroy_batch = "";

const QString url_search_suggestions_users = "https://api.weibo.com/2/search/suggestions/users.json";
//const QString url_search_suggestions_schools = "";
//const QString url_search_suggestions_companyies = "";
//const QString url_search_suggestions_apps = "";
//const QString url_search_suggestions_at_users = "";
//const QString url_search_topics = "";

const QString url_remind_unread_count = "https://rm.api.weibo.com/2/remind/unread_count.json";


#endif // WEIBO_URL

