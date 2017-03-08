/*
 * 文件名：view_postweibo.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该类定义了一个名为View_PostWeibo的类，用于发送微博、转发
 * 微博、发表评论和回复评论。
 */

#ifndef VIEW_POSTWEIBO_H
#define VIEW_POSTWEIBO_H

/*包含头文件*/

#include <QMainWindow>

#include "macro.h"

/*指定命名空间*/

namespace Ui {
class View_PostWeibo;
}

/*类声明*/

class View_PostWeibo : public QMainWindow
{
    Q_OBJECT

public:
    explicit View_PostWeibo(QWidget *parent = 0);
    ~View_PostWeibo();

    void clearTextEditor();//清除编辑器内容

    void setFunction(postContentType _type);//设置当前功能

    void gradientShow();//淡化显示
    void gradientHide();//淡化隐藏

    void setProfileImage(QString url);//设置头像

signals:
    void postNewWeibo(QString);//发微博
    void postStatusesUpload(QString, QString);//发带图片的微博
    void postStatusesRepost(QString);//转发微博
    void postCommentsCreate(QString);//写评论
    void postCommentsReply(QString);//回复评论
    void displayMessage(QString);//提示信息
    void displayImage();//显示要上传的图片

private slots:
    void menuTriggered(QAction *);//菜单被触发
    void timeOut();//定时器触发
    void selectLocalImage();//选择本地图片
    void screenShot();//截屏

    void on_buttonCancel_clicked();//取消按钮

    void on_buttonPosition_clicked();//位置按钮

    void on_buttonEmotions_clicked();//表情按钮

    void on_buttonSend_clicked();//发送按钮

    void on_newWeiboEditor_textChanged();//编辑器内容改变

    void displayImageSelected();//显示选择的图片

private:
    Ui::View_PostWeibo *ui;

    void adjustPositions();//调整控件位置

    postContentType type;//记录当前发布内容的类型

    void mousePressEvent(QMouseEvent * event);//鼠标点击事件
    void mouseMoveEvent(QMouseEvent * event);//鼠标按压并移动事件
    QPoint windowPos;//记录窗口位置
    QPoint mousePos;
    QPoint dPos;

    void setRoundRect();//设置窗口圆角

    void setPictureMenu();//设置菜单图片
    QMenu * pictureMenu;//按钮菜单
    QAction * selectImage;
    QAction * captureScreen;
    bool inScreenShoting;//是否正在截图
    bool hasPicture;//是否已选择图片
    QString picturePath;//图片的路径

    QTimer * timer;//定时器，用于控制截屏
    bool isShoting;//正在截图
    bool isSelecting;//正在选本地图片

    bool containPoint(int x, int y, QPushButton * widget);//鼠标是否在某一按钮上

};

#endif // VIEW_POSTWEIBO_H
