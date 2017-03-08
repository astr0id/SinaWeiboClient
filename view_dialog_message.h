/*
 * 文件名：view_dialog_message.h
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件定义了一个名为View_Dialog_Message的类，用于实现
 * 显示警告和提示信息。
 */

#ifndef VIEW_DIALOG_MESSAGE_H
#define VIEW_DIALOG_MESSAGE_H

/*包含头文件*/

#include <QDialog>

/*指定命名空间*/

namespace Ui {
class View_Dialog_Message;
}

/*类声明*/

class View_Dialog_Message : public QDialog
{
    Q_OBJECT

public:
    explicit View_Dialog_Message(QWidget *parent = 0);
    ~View_Dialog_Message();

    void displayMessage(QString message);//显示提示信息

private slots:
    void on_pushButton_Close_clicked();//关闭窗口

private:
    Ui::View_Dialog_Message *ui;
};

#endif // VIEW_DIALOG_MESSAGE_H
