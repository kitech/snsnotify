#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include <QtCore>
#include <QtGui>

int main(int argc, char **argv)
{
    QApplication *a = new QApplication(argc, argv);

    

    return 0;
}
// int main(int argc, char **argv)
// {
//  restart_from_here:    

//     QApplication *a = new QApplication(argc, argv);

//     dlopen("gui main obj.so", LD_LAZY);
//     void get_gui_main_func = dlsym(handle, "get_gui_main_fun");
//     QObject *obj = new get_gui_main_func();
    
//     obj->show();

//     a->exec();

//     if(obj->property("restart") == QString("yes")) {
//         qDebug()<<"Restart.";
//         dlclose(handle);
//         //update gui_main_obj.so to the newest version
//         goto restart_from_here;
//     }else{
//         qDebug()<<"Exit.";
//     }
    
//     return 0;
// }

// 第一种方法,据点，这个程序与界面程序仍旧属于同一个程序，不过GUI是可以重新启动的。
// #include <QtCore>
// #include <QtGui>

// int main(int argc, char **argv)
// {
//  restart_from_here:    

//     QApplication *a = new QApplication(argc, argv);
//     QObject *obj = new QObject();

//     a->exec();

//     if(obj->property("restart") == QString("yes")) {
//         qDebug()<<"Restart.";
//         goto restart_from_here;
//     }else{
//         qDebug()<<"Exit.";
//     }
    
//     return 0;
// }
