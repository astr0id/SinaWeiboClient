/*
 * 文件名：main.cpp
 * Coder：李一鸣-2014211302-2014212070
 * ---------------------------------------------------
 * 文件内容说明：
 *    该文件包含了主函数，是整个程序的入口。
 */


/*包含头文件*/

#include "view_mainwindow.h"
#include <QApplication>

#include "controller.h"

/*主函数*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller * controller = new Controller;//new出controller，由controller完成程序控制
    controller->setApplicationIcon(&a);//设置应用图标
    controller->splashAppear();//显示欢迎界面
    controller->setAppStyle(a.style());//设置应用风格
    if(!controller->notPastDue())//判断令牌是否过期
        controller->showOauth2();//过期重新认证
    else{

    }
    return a.exec();
}
