#include <QtCore/QCoreApplication>
#include <iostream>

#include "server.h"
#include "RoomController.h"
#include "Enums.h"

#include <plog/Log.h> 
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#include "MessageSaver_Service.h"
#include "SwearHelper.h"

#include "RoomRepository.h"
#include "UserRepository.h"
#include "DBRoom.h"
#include "DBUser.h"
#include "MediaSaver_Service.h"

#if defined (Q_OS_WIN)
#include "AsyncConsoleWin.h"
#else

#endif


Server server;  //create server instace
AsyncConsoleWin* asyncConsole;

static void shutdown_routine()
{
    qDebug() << "The server shatdown.";
}

static void startup_routine()
{

    // add QT post exit rotine
    qAddPostRoutine(shutdown_routine);

    // set up some application settings
    qApp->setOrganizationName("C++ Final Project"); // through macro
    qApp->setApplicationName("InetChat");

    // add a routine to be executed just before application exits through signal
    QObject::connect(qApp, &QCoreApplication::aboutToQuit, []()
        {
            qDebug() << "AboutToQuit : InetChat finishing...";
        }
    );

    // register some meta types
    qRegisterMetaType<RoomController>();

    static plog::RollingFileAppender<plog::TxtFormatter> fileAppender("log.txt", 1000000, 5);
    static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender; 
    plog::init(plog::debug, &consoleAppender).addAppender(&fileAppender);
    PLOGD << "Console logging enabled";
    
    PLOGD << "Server application starting. Logging is enabled.";



#if defined (Q_OS_WIN)
    //(void)new asyncConsoleWin(qApp);
    asyncConsole = new AsyncConsoleWin(qApp);
    QObject::connect(asyncConsole, &AsyncConsoleWin::startServer, &server, &Server::startServer);
    QObject::connect(asyncConsole, &AsyncConsoleWin::stopServer, &server, &Server::stopServer);
#else
    qDebug() << "AsyncConsoleWin can't be loaded! Curent OS doesnt support.";
    PLOGD << "AsyncConsoleWin can't be loaded! Curent OS doesnt support.";
#endif
    QTimer::singleShot(0, [&]()
        {
            //MessageSaver_Service::start(0.05);
        });
    QTimer::singleShot(0, [&]()
        {
            server.startServer();
        });
}
Q_COREAPP_STARTUP_FUNCTION(startup_routine)

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    


    return a.exec();


}


//Logger severity
//enum Severity
//{
//    none = 0,
//    fatal = 1,
//    error = 2,
//    warning = 3,..
//    info = 4,
//    debug = 5,
//    verbose = 6
//};

//Short simple macros
//PLOGV << "verbose";
//PLOGD << "debug";
//PLOGI << "info";
//PLOGW << "warning";
//PLOGE << "error";
//PLOGF << "fatal";
//PLOGN << "none";

//Conditional macros
//PLOGV_IF(cond) << "verbose";
//PLOGD_IF(cond) << "debug";
//PLOGI_IF(cond) << "info";
//PLOGW_IF(cond) << "warning";
//PLOGE_IF(cond) << "error";
//PLOGF_IF(cond) << "fatal";
//PLOGN_IF(cond) << "none";