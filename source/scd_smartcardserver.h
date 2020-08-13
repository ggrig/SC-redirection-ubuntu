#ifndef SCD_SMARTCARDSERVER_H
#define SCD_SMARTCARDSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>

class SCD_SmartCardServer : public QObject
{
   Q_OBJECT

   private:

     enum ServerType {ST_UNKNOWN, ST_STANDALONE, ST_INTEGRATED};

     enum StatusMess {SM_AUTHENTICATED , SM_NOTAUTHENTICATED,SM_VALIDATED,
                      SM_NOTVALIDATED  , SM_ALREADYAUTH     ,SM_SESSIONTIMEOUT,
                      SM_UNKNOWNCOMMAND, SM_INTEGRATED      ,SM_STANDALONE,
                      SM_UNKNOWN,SM_ERROR};

     enum Commands {C_SERVERTYPE, C_ATR, C_LOGIN, C_CHECK, C_AUTH, C_TIMEOUT};

     QStringList messages;
     QStringList commands;
     QString lastCardError;

     StatusMess  lastPollStatus = SM_UNKNOWN;

     ServerType type;

     QWebSocketServer *cardServer;
     QWebSocket *socket;

     QString lastError;
     QString atr = "";

     qint16 port;

     int timer = 0;
     int isAuthenticated = 0;

     bool permanentConnection;

     QTimer pollTimer;

     void resetAuthentication();

     void messageParse(QWebSocket *socket, const QString &message);

   public:

     explicit SCD_SmartCardServer(qint16 port=10522, ServerType type=ST_STANDALONE , QObject *parent = nullptr);

     int  start();
     void stop();

   signals:

     void status(QString command, StatusMess status, bool logout);
     void error(QString command, QString error);
     void serverType(ServerType type);
     void loginCode(QByteArray ATR);

   private slots:

     void onConnect();

     void onCheckCardMessageReceived(const QString &message);    

};

#endif // SCD_SMARTCARDSERVER_H
