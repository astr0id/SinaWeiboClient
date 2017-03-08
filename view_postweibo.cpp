/*
 * 文件名：view_postweibo.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件实现了View_PostWeibo类中所有的成员函数。
 */

#include "view_postweibo.h"
#include "ui_view_postweibo.h"

#include "view_dialog_screenshot.h"
#include "ui_view_dialog_screenshot.h"

//构造函数
View_PostWeibo::View_PostWeibo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View_PostWeibo)
{
    ui->setupUi(this);

    pictureMenu = new QMenu();//初始化菜单
    selectImage = new QAction(pictureMenu);
    captureScreen = new QAction(pictureMenu);

    //初始化成员变量
    inScreenShoting=false;
    hasPicture=false;
    ui->label_image->setVisible(false);

    setPictureMenu();

    isShoting=false;
    isSelecting=false;
    timer = new QTimer;
    timer->setSingleShot(false);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));

    connect(this, SIGNAL(displayImage()), this, SLOT(displayImageSelected()));

    setWindowFlags(Qt::FramelessWindowHint);//设为无边框

    setRoundRect();//设置圆角
}

//析构函数
View_PostWeibo::~View_PostWeibo()
{
    delete pictureMenu;
    delete selectImage;
    delete captureScreen;
    delete ui;
}

//判断矩形中是否包含某一点
bool View_PostWeibo::containPoint(int x, int y, QPushButton * widget){
    return x>widget->x() && y>widget->y() && x<widget->x()+widget->width() && y<widget->y()+widget->height();
}

//鼠标点击事件
void View_PostWeibo::mousePressEvent(QMouseEvent *event){
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;

    if(hasPicture){
        if(containPoint(event->x(), event->y(), ui->buttonCancel)){
            on_buttonCancel_clicked();
        }
        else if(containPoint(event->x(), event->y(), ui->buttonEmotions)){
            on_buttonEmotions_clicked();
        }
        else if(containPoint(event->x(), event->y(), ui->buttonPosition)){
            on_buttonPosition_clicked();
        }
        else if(containPoint(event->x(), event->y(), ui->buttonSend)){
            on_buttonSend_clicked();
        }
    }

}

//鼠标按压并拖动
void View_PostWeibo::mouseMoveEvent(QMouseEvent *event){
    this->move(event->globalPos()-this->dPos);
}

//设置圆角
void View_PostWeibo::setRoundRect(){
    QBitmap bmp(this->size());
    bmp.fill(this,0,0);
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(bmp.rect(),5,5,Qt::AbsoluteSize);
    setMask(bmp);
}

//设置菜单图片
void View_PostWeibo::setPictureMenu(){
    pictureMenu->addAction(selectImage);
    pictureMenu->addAction(captureScreen);
    selectImage->setText("Select Image");
    captureScreen->setText("Capture Screen");

    ui->buttonImage->setMenu(pictureMenu);

    ui->buttonImage->setStyleSheet("QPushButton::menu-indicator{image:None;}");

    connect(pictureMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));
}

//计时器触发的槽函数
void View_PostWeibo::timeOut(){
    if(isSelecting){//选择图片
        timer->stop();
        selectLocalImage();
        isSelecting=false;
        timer->start(1000);
    }
    else if(isShoting){//截图
        timer->stop();
        screenShot();
        isShoting=false;
        timer->start(1000);

    }
}

//菜单被点击
void View_PostWeibo::menuTriggered(QAction *action){
    //qDebug()<<ui->buttonImage->isChecked();
    if(action == selectImage){
        //selectLocalImage();
        isSelecting=true;
    }
    else if(action == captureScreen){
        //screenShot();
        isShoting=true;
    }
}

//选择本地图片
void View_PostWeibo::selectLocalImage(){

    QFileDialog * fileDialog=new QFileDialog(this);
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter("*.jpg *.png");
    if(fileDialog->exec()==QDialog::Accepted){

        picturePath=fileDialog->selectedFiles()[0];
        hasPicture=true;
        type=postContentType::weibo_upload;

        displayImageSelected();
    }
    delete fileDialog;
}

//截图
void View_PostWeibo::screenShot(){
    //qDebug()<<"in screenShot()";
    QEventLoop loop;
    QTimer::singleShot(1000, &loop, SLOT(quit()));
    loop.exec();

    QScreen * screen=QGuiApplication::primaryScreen();
    screen->grabWindow(0).save("ScreenShotTemp.jpg", "jpg");

    View_Dialog_ScreenShot * screenshotDialog=new View_Dialog_ScreenShot(this);
    screenshotDialog->displayScreenShot();
    screenshotDialog->exec();
    delete screenshotDialog;

    hasPicture=true;
    type=postContentType::weibo_upload;
    picturePath="ScreenShot.jpg";

    displayImageSelected();
}

//显示选择的图片
void View_PostWeibo::displayImageSelected(){
    //显示选中的图片
    QPixmap pic(picturePath);
    ui->label_image->setScaledContents(true);
    ui->label_image->setPixmap(pic);
    ui->label_image->setVisible(true);
    ui->label_image->setGeometry(270, 102, 45, 45);

    //调整元素位置

    ui->buttonCancel->move(10, 155);
    ui->buttonPosition->move(130, 155);
    ui->buttonEmotions->move(170, 155);
    ui->buttonImage->move(210, 155);
    ui->buttonSend->move(250, 155);

    ui->label_bar->setGeometry(0, 150, 330, 40);
    ui->centralwidget->setGeometry(0, 0, 330, 190);
    this->setGeometry(this->x(), this->y(), 330, 190);

}

//调整界面上元素的位置
void View_PostWeibo::adjustPositions(){
    //qDebug()<<"adjustPositions";
    picturePath="";
    hasPicture=false;

    ui->label_image->setVisible(false);

    ui->centralwidget->setGeometry(0, 0, 330, 140);
    this->setGeometry(this->x(), this->y(), 330, 140);

    ui->label_bar->setGeometry(0, 100, 330, 40);
    ui->buttonCancel->setGeometry(10, 105, 80, 30);
    ui->buttonPosition->setGeometry(130, 105, 30, 30);
    ui->buttonEmotions->setGeometry(170, 105, 30, 30);
    ui->buttonImage->setGeometry(210, 105, 30, 30);
    ui->buttonSend->setGeometry(250, 105, 70, 30);
}

//设置当前编辑文字的功能：发微博、转发、评论、回复
void View_PostWeibo::setFunction(postContentType _type){
    this->type=_type;
}

//取消按钮被点击
void View_PostWeibo::on_buttonCancel_clicked()
{
    this->gradientHide();
    ui->newWeiboEditor->clear();
    adjustPositions();
}

//位置按钮被点击
void View_PostWeibo::on_buttonPosition_clicked()
{
    //qDebug()<<"询问是否获得地址权限，许可则请求地址";
    emit displayMessage("新浪微博暂未提供位置接口");
}

//表情按钮被点击
void View_PostWeibo::on_buttonEmotions_clicked()
{
    //qDebug()<<"在该按钮下显示表情选项";
    emit displayMessage("新浪微博暂未提供表情支持");

}

//发送按钮被点击
void View_PostWeibo::on_buttonSend_clicked()
{
    if(ui->newWeiboEditor->toPlainText().length()>140){
        emit displayMessage("字数不允许超过140");
        return;
    }

    if(type==postContentType::weibo){
        //qDebug()<<"post新微博内容";
        QString newWeibo=ui->newWeiboEditor->toPlainText();
        emit postNewWeibo(newWeibo);
    }
    else if(type==postContentType::comment_create){
        //qDebug()<<"post新评论";
        QString newComment=ui->newWeiboEditor->toPlainText();
        emit postCommentsCreate(newComment);
    }
    else if(type==postContentType::comment_reply){
        qDebug()<<"回复他人的评论";
        QString newReply=ui->newWeiboEditor->toPlainText();
        emit postCommentsReply(newReply);
    }
    else if(type==postContentType::repost){
        qDebug()<<"repost别人的微博";
        QString newRepost=ui->newWeiboEditor->toPlainText();
        emit postStatusesRepost(newRepost);
    }
    else if(type==postContentType::weibo_upload){
        QString newWeibo=ui->newWeiboEditor->toPlainText();
        emit postStatusesUpload(newWeibo, picturePath);
    }
    else{
        qDebug()<<"post类型错误";
        return;
    }

    this->hide();
    ui->newWeiboEditor->clear();
    adjustPositions();

}

//清除编辑区的文字
void View_PostWeibo::clearTextEditor(){
    ui->newWeiboEditor->clear();
}

//淡化显示
void View_PostWeibo::gradientShow(){
    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(AppearTime);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    this->show();

    timer->start(1000);
}

//淡化隐藏
void View_PostWeibo::gradientHide(){
    QPropertyAnimation * animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(DisappearTime);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    this->hide();

    timer->stop();
}

//显示登录用户的头像
void View_PostWeibo::setProfileImage(QString url){
    QPixmap head;

    head.load(url);

    ui->label_avatar->setScaledContents(true);
    ui->label_avatar->setPixmap(head);
}

//计算剩余的字数，上限140字
void View_PostWeibo::on_newWeiboEditor_textChanged()
{
    int textLength=ui->newWeiboEditor->toPlainText().length();
    if(textLength>0){
        ui->buttonSend->setChecked(true);
    }
    else{
        ui->buttonSend->setChecked(false);
    }
    if(textLength>140){
        ui->label_counter->setStyleSheet("color: rgba(255, 102, 102, 191);");
    }
    else{
        ui->label_counter->setStyleSheet("color: rgba(204, 204, 204, 191);");
    }
    ui->label_counter->setText(QString::number(140-textLength));

    ui->label_counter->setGeometry(ui->label_counter->x(), ui->newWeiboEditor->y()+ui->newWeiboEditor->height()-50,
                                   ui->label_counter->width(), ui->label_counter->height());

}




