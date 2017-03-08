#-------------------------------------------------
#
# Project created by QtCreator 2016-10-23T09:56:27
#
#-------------------------------------------------

QT       += core gui sql network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeiboClient
TEMPLATE = app


SOURCES += main.cpp\
        view_mainwindow.cpp \
    view_traysystem.cpp \
    view_postweibo.cpp \
    weibo_statuses.cpp \
    weibo_comments.cpp \
    weibo_users.cpp \
    weibo_friendships.cpp \
    weibo_account.cpp \
    weibo_favorites.cpp \
    weibo_search.cpp \
    weibo_oauth2.cpp \
    weibo_remind.cpp \
    controller.cpp \
    view_oauth2.cpp \
    weibo.cpp \
    view_item_status.cpp \
    view_item_friend.cpp \
    view_item_comment.cpp \
    view_dialog_message.cpp \
    controller_filemanage.cpp \
    view_dialog_screenshot.cpp

HEADERS  += view_mainwindow.h \
    macro.h \
    view_postweibo.h \
    weibo_structure.h \
    controller.h \
    view_oauth2.h \
    weibo.h \
    weibo_url.h \
    view_item_status.h \
    view_item_friend.h \
    view_item_comment.h \
    view_dialog_message.h \
    view_dialog_screenshot.h

FORMS    += view_mainwindow.ui \
    view_postweibo.ui \
    view_oauth2.ui \
    view_item_status.ui \
    view_item_friend.ui \
    view_item_comment.ui \
    view_dialog_message.ui \
    view_dialog_screenshot.ui

RESOURCES += \
    image.qrc
