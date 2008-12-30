
#include <stdio.h>
#include <stdlib.h>

#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#include "systemoption.h"
#include "snsnotify.h"
#include "qlog.h"
#include "utils.h"

int main(int argc, char **argv)
{
    int ev ;
    QApplication *a = NULL;
    QTranslator * qtTranslator = NULL;
    SNSNotify * note = NULL;
    QString main_module_path;
    QLibrary * so_handle = NULL;
    char *get_func_name = "get_sns_gui";
    char *free_func_name = "free_sns_gui";
    SNSNotify * (*get_sns_gui)() = NULL;
    void (*free_sns_gui)(SNSNotify *) = NULL;

 restart_again:
    
    qInstallMsgHandler(qlog);
    a = new QApplication(argc, argv);
    a->addLibraryPath(QApplication::applicationDirPath());
    a->addLibraryPath(QApplication::applicationDirPath()+"/modules");
    a->addLibraryPath("z:/cross/tidy/bin");
    a->addLibraryPath("z:/Qt/VC-4.4.1/bin");
    q_debug()<<a->libraryPaths();
    //so = new SystemOption(0);
    
    //qtTranslator = new QTranslator();
    
    main_module_path = QApplication::applicationDirPath();
    main_module_path += "/" + Utils::get_so_file_name("snsmain");
    q_debug()<<"Loading "<<main_module_path;
    so_handle = new QLibrary(main_module_path);

    so_handle->load();
    q_debug()<<so_handle;
    q_debug()<<"Load lib error:"<<so_handle->errorString();
    *(void**)(&get_sns_gui) = so_handle->resolve(get_func_name);
    *(void**)(&free_sns_gui) = so_handle->resolve(free_func_name);
    if(get_sns_gui == 0 || free_sns_gui == 0) {
        Q_ASSERT(get_sns_gui != 0);
        Q_ASSERT(free_sns_gui != 0);
        q_debug()<<"Load lib error:"<<so_handle->errorString();
    }

    q_debug()<<"here"<<get_sns_gui<<free_sns_gui<<so_handle->errorString();
    note = get_sns_gui();
    //QObject::connect(note, SIGNAL(close()), a, SLOT(quit()));
    note->show();
    q_debug()<<note;    

    ev = a->exec();

    qDebug()<<"app exec return";
    {
        free_sns_gui(note);
        so_handle->unload();
        delete so_handle;
        delete a;
    }
    //goto restart_again;

    return 0;
}

// int main(int argc, char **argv)
// {
//     qInstallMsgHandler(qlog);

//     QApplication a(argc, argv);
//     so = new SystemOption(0);

//     QTranslator qtTranslator;
//     qtTranslator.load("qt_" + QLocale::system().name(), so->appPath + "/translations/");
//     //                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//     a.installTranslator(&qtTranslator);
    
//     QTranslator myappTranslator;
//     myappTranslator.load("nsm_" + QLocale::system().name(),  so->appPath + "/translations/");
//     a.installTranslator(&myappTranslator);

//     SNSNotify *note = new SNSNotify();
//     note->show();

//     int ev = a.exec();

//     delete note;

//     return ev;
// }
