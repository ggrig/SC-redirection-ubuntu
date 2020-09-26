#include <QCoreApplication>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QSettings>

#include "scd_smartcardserver.h"

#define  echo QTextStream(stderr) <<

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   //echo "SC-Develop Smart Card Web Socket Server v1.0\n";
   //echo "Copyright (c) 2019 (MIT) Ing. Salvatore Cerami - dev.salvatore.cerami@gmail.com\n";
   //echo "https://github.com/sc-develop - git.sc.develop@gmail.com\n\n";

   QSettings config(QCoreApplication::applicationDirPath() + "/config.cfg",QSettings::IniFormat);

   qint16 port = config.value("port",10522).toInt();

   config.setValue("port",port);

   config.sync();

   SCD_SmartCardServer server;

   if (server.start())
   {
     return a.exec();
   }

   return 0;
}

#ifdef TCP_TUNNEL_STANDALONE
// ./tcptunnel --local-port=3240 --remote-port=3240 --remote-host=192.168.1.7 --log --stay-alive
int main(int argc, char *argv[])
{
#ifdef __MINGW32__
    WSADATA info;
    if (WSAStartup(MAKEWORD(1,1), &info) != 0)
    {
        perror("main: WSAStartup()");
        exit(1);
    }
#endif

    name = argv[0];

    set_options(argc, argv);

    if (build_server() == 1)
    {
        exit(1);
    }

#ifndef __MINGW32__
    signal(SIGCHLD, SIG_IGN);
#endif

    do
    {
        if (wait_for_clients() == 0)
        {
            handle_client();
        }
    }
    while (settings.stay_alive);

    close(rc.server_socket);

    return 0;
}
#endif
