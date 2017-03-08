/*
 * 文件名：view_oauth2.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该类声明了一个名为View_Oauth2的类，用于客户端的登录认证
 * 界面。
 */

#ifndef VIEW_OAUTH2_H
#define VIEW_OAUTH2_H

/*包含头文件*/

#include <QMainWindow>

#include "macro.h"

/*指定命名空间*/

namespace Ui {
class View_Oauth2;
}

/*类声明*/

class View_Oauth2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit View_Oauth2(QWidget *parent = 0);
    ~View_Oauth2();

    void loadUrl(QString);//加载URL
    QUrl getCodeUrl();//获取URL中的code
    void gradientShow();//淡化显示
    void gradientHide();//淡化隐藏


signals:
    void urlChange(QUrl);//URL内容发生了改变

private slots:
    void setUrlWithCode(QUrl);//设置URL

private:
    Ui::View_Oauth2 *ui;

    int counter;

    QUrl urlWithCode;//重定位后的URL

    void mousePressEvent(QMouseEvent * event);//鼠标点击事件
    void mouseMoveEvent(QMouseEvent * event);//鼠标按压并移动事件

    QPoint windowPos;//用于记录窗口坐标
    QPoint mousePos;
    QPoint dPos;
};

#endif // VIEW_OAUTH2_H
