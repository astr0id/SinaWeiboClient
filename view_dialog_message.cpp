/*
 * 文件名：view_dialog_message.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件中实现了View_Dialog_Message类的成员函数。
 */

#include "view_dialog_message.h"
#include "ui_view_dialog_message.h"

//构造函数
View_Dialog_Message::View_Dialog_Message(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::View_Dialog_Message)
{
    ui->setupUi(this);
}

//析构函数
View_Dialog_Message::~View_Dialog_Message()
{
    delete ui;
}

//关闭窗口
void View_Dialog_Message::on_pushButton_Close_clicked()
{
    this->close();
}

//显示提示信息
void View_Dialog_Message::displayMessage(QString message){
    ui->label->setText(message);
    this->exec();
}
