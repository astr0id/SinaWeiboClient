/*
 * 文件名：macro.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件包含了所有需要的头文件，供各类使用。在其他文件中只需
 * 包含该文件即可。该文件中还声明了必要的枚举类型和常量。
 */

#ifndef MACRO
#define MACRO

/*包含头文件*/

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QListWidget>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QDate>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QSplashScreen>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QtGui>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCompleter>
#include <QStringList>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTextStream>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPainter>
#include <QPoint>
#include <Qbuffer>
#include <QDataStream>
#include <QScreen>
#include <QMovie>
#include <QPushButton>
#include <QStyle>
#include <QMacNativeWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QWebView>
#include <QSslSocket>
#include <QDesktopServices>
#include <QFileDialog>
#include <QPixmap>
#include <QBitmap>
#include <QImage>
#include <QThread>
#include <QListWidget>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <list>
#include <iterator>
#include <vector>
#include <map>
#include <queue>

/*宏定义*/

#define AppearTime 300//界面淡化显示时间
#define DisappearTime 300//界面淡化隐藏时间

#define IniFileName "UserInfo.ini"//配置文件文件名
#define DatabaseName "database.db"//数据库文件名

/*枚举类型定义*/

enum postContentType{//发布微博的属性
    weibo,//普通微博
    repost,//转发微博
    comment_create,//新评论
    comment_reply,//回复评论
    weibo_upload//带图片的微博
};

enum errorType{//错误类型

};

/*常量定义*/

const QString AppKey1 = "2900848875";//应用的AppKey
const QString AppSecret1 = "43b8b2478048c1b169080f7df258d163";//应用的AppSecret

const QString WeiboApiUrl = "https://api.weibo.com";//微博api的url

const QString RedirectUrl = "https://api.weibo.com/oauth2/default.html"; // weibo default Blank RedirectUrl
const QString OauthGetCodeUrl = WeiboApiUrl + "/oauth2/authorize?client_id=" +//获取code的url
                                AppKey1 + "&redirect_uri=" + RedirectUrl + "&response_type=code";

const int interval=60*1000;//定时器触发频率
const QString cacheDirPath="cache_image";//缓存路径


#endif // MACRO

