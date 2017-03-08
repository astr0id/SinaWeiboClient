/*
 * 文件名：view_dailog_screenshot.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了View_Dialog_ScreenShot类的全部成员函数。
 */

#include "view_dialog_screenshot.h"
#include "ui_view_dialog_screenshot.h"

//构造函数
View_Dialog_ScreenShot::View_Dialog_ScreenShot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::View_Dialog_ScreenShot)
{
    ui->setupUi(this);
    pixmap = new QPixmap("ScreenShotTemp");
    isDrawing=false;
    rubber = new QRubberBand(QRubberBand::Rectangle,this);
    label  = new QLabel(this);
}

//析构函数
View_Dialog_ScreenShot::~View_Dialog_ScreenShot()
{
    delete pixmap;
    delete ui;
}

//显示全屏的截图
void View_Dialog_ScreenShot::displayScreenShot(){
    ui->label->setPixmap(*pixmap);
    this->showFullScreen();
    ui->label->adjustSize();//自动调整label大小
    ui->label->showFullScreen();

    setCursor(Qt::CrossCursor);//鼠标改为十字交叉形状
}

//键盘点击事件
void View_Dialog_ScreenShot::keyPressEvent(QKeyEvent * event){
    if(event->key()==Qt::Key_Escape){//esc键按下
        this->showNormal();
        this->close();
    }
}

//鼠标点击事件
void View_Dialog_ScreenShot::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){//左键按下
        isDrawing=true;
        startPoint=event->pos();
        rubber->setGeometry(startPoint.x(), endPoint.y(), 0, 0);
        rubber->show();
    }
}

//鼠标按压并拖动事件
void View_Dialog_ScreenShot::mouseMoveEvent(QMouseEvent *event){
    endPoint = event->pos();

    //调整要显示的区域
    int w = abs(endPoint.x() - startPoint.x());
    int h = abs(endPoint.y() - startPoint.y());
    int x = startPoint.x() < endPoint.x() ? startPoint.x() : endPoint.x();
    int y = startPoint.y() < endPoint.y() ? startPoint.y() : endPoint.y();

    rubber->setGeometry(x,y,w,h);
    // 动态显示选区大小
    setLabel();
}

//鼠标释放事件
void View_Dialog_ScreenShot::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        isDrawing = false;
        endPoint = event->pos();
        // 记录终点
        rubber->hide();
        // 截屏
        grabScreen();
        // 截屏后关闭程序和标签栏
        label->close();

        // 如果使用close()，当主窗口隐藏时，主窗口也会close()，如果你需要将他作为一个模块，使用hide而不是close
        this->showNormal();
        this ->close();
    }
}

//设置label
void View_Dialog_ScreenShot::setLabel(){
    // 调整标签显示位置
    int wid = abs(startPoint.x() - endPoint.x());
    int height = abs(startPoint.y() - endPoint.y());
    int x = startPoint.x() < endPoint.x() ? startPoint.x() : endPoint.x();
    int y = startPoint.y() < endPoint.y() ? startPoint.y() : endPoint.y();

    // 设置标签显示内容
    QString str = QString(" %1 x %2      ").arg(wid).arg(height);
    label->setText(str);
    // 设置标签位置
    //QRect rect(label->contentsRect());
    label->adjustSize();
    label->move(QPoint(x,y - label->height()));
    label->show();
}

//截屏
void View_Dialog_ScreenShot::grabScreen(){
    // 调整要截取的区域
    int wid = abs(startPoint.x() - endPoint.x());
    int height = abs(startPoint.y() - endPoint.y());
    int x = startPoint.x() < endPoint.x() ? startPoint.x() : endPoint.x();
    int y = startPoint.y() < endPoint.y() ? startPoint.y() : endPoint.y();

    // 对全屏背景进行截图
    QPixmap pic = pixmap->copy(x,y,wid,height);
    pic.save("ScreenShot.jpg", "jpg");
}
