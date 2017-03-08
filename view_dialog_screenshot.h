/*
 * 文件名：view_dialog_screenshot.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该类定义了一个名为View_Dialog_ScreenShot的类，用于实现
 * 截屏功能。
 */

#ifndef VIEW_DIALOG_SCREENSHOT_H
#define VIEW_DIALOG_SCREENSHOT_H

/*包含头文件*/

#include <QDialog>
#include <QPixmap>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QRubberBand>
#include <QLabel>

/*指定命名空间*/

namespace Ui {
class View_Dialog_ScreenShot;
}

/*类声明*/

class View_Dialog_ScreenShot : public QDialog
{
    Q_OBJECT

public:
    explicit View_Dialog_ScreenShot(QWidget *parent = 0);
    ~View_Dialog_ScreenShot();

    void displayScreenShot();//显示全屏的截图

private:
    Ui::View_Dialog_ScreenShot *ui;
    QPixmap * pixmap;//保存图片

    QPoint startPoint;//截屏起始点
    QPoint endPoint;//截屏终止点

    bool isDrawing;//是否在画矩形

    QRubberBand * rubber;//用于选择矩形区域
    QLabel * label;//用于显示图片

    void setLabel();//设置label的内容
    void grabScreen();//截屏

    void keyPressEvent(QKeyEvent * event);//追踪键盘事件

    void mousePressEvent(QMouseEvent * event);//鼠标点击事件
    void mouseMoveEvent(QMouseEvent * event);//鼠标点击并移动
    void mouseReleaseEvent(QMouseEvent * event);//鼠标释放事件
};

#endif // VIEW_DIALOG_SCREENSHOT_H
