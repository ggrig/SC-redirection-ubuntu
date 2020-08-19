#include "scd_smartcardserver.h"

/**
 * @brief SCD_SmartCardServer::SCD_SmartCardServer
 * @param port
 * @param parent
 */

SCD_SmartCardServer::SCD_SmartCardServer(qint16 port, ServerType type, QObject *parent) : QObject(parent), type(type), port(port)
{
   messages.insert(SM_UNKNOWN,          "Unknown");

   commands.insert(C_SERVERTYPE, "SERVERTYPE");
   commands.insert(C_ATR,        "ATRCODE");
   commands.insert(C_LOGIN,      "LOGINCODE");
   commands.insert(C_CERT,       "VIEWCERT");
   commands.insert(C_AUTH,       "AUTHENTICATE");
   commands.insert(C_SIGNED,     "SIGNED");
   commands.insert(C_TIMEOUT,    "POLLTIMEOUT");

}

/**
 * @brief SCD_SmartCardServer::start
 * @return
 */
int SCD_SmartCardServer::start()
{
   cardServer = new QWebSocketServer("ATR",QWebSocketServer::NonSecureMode,NULL);

   connect(cardServer, SIGNAL(newConnection()), this, SLOT(onConnect()));
   connect(cardServer, SIGNAL(closed()),        cardServer, SLOT(deleteLater()));

   cardServer->setMaxPendingConnections(1);

   if (cardServer->listen(QHostAddress::Any,port))
   {
      lastError =  "Server listening on port: " + QString::number(port) + "\n";

      qDebug() << lastError;

      return 1;
   }

   lastError = "unable to start server omn port: " + QString::number(port) + "\n";

   qDebug() << lastError;

   return 1;
}

/**
 * @brief SCD_SmartCardServer::stop
 * @return
 */
void SCD_SmartCardServer::stop()
{
   cardServer->close();
}

/**
 * @brief SCD_SmartCardServer::onConnect
 */
void SCD_SmartCardServer::onConnect()
{
   socket = cardServer->nextPendingConnection(); // get the new connection

   connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(onCheckCardMessageReceived(QString)));
   connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));

   lastCardError.clear();

   lastPollStatus = SM_UNKNOWN;
}

/**
 * @brief SCD_SmartCardServer::onCheckCardMessageReceived
 * @param message
 */
void SCD_SmartCardServer::onCheckCardMessageReceived(const QString &message)
{
   QWebSocket *socket = static_cast<QWebSocket *>(sender());

   lastCardError.clear();

   messageParse(socket, message);
}

/**
 * @brief SCD_SmartCardServer::messageParse
 * @param socket
 * @param message
 */
void SCD_SmartCardServer::messageParse(QWebSocket *socket, const QString &message)
{
   QStringList msg=message.split(":");

   qDebug() << QString::number(timer++) << " Message Received: " << message <<"\n";

   //socket->sendTextMessage("Ubuntu|Message Received");

   if (msg.count()!=2)
   {
      return;
   }

   // Require to change polling tmeout interval -------------------------------------------

   if (msg[0]==commands.at(C_TIMEOUT))
   {
      return;
   }

   // Require server type ------------------------------------------------------------------

   if (msg[0]==commands.at(C_SERVERTYPE))
   {
      return;
   }

   // Require ATR authentication code -----------------------------------------------------

   if (msg[0]==commands.at(C_LOGIN))
   {
      return;
   }

   // Authentication check ----------------------------------------------------------------

   if (msg[0]==commands.at(C_CERT))
   {
      qDebug() << QString::number(timer++) << " C_CERT Received: " <<"\n";
      return;
   }

   if (msg[0]==commands.at(C_AUTH))
   {
       qDebug() << QString::number(timer++) << " Authentication Request: " <<"\n";
       QString msgToSign = "Message to sign";
       socket->sendTextMessage("\{\"__MESSAGE__\":\"CMD|TOSIGN:" + msgToSign + "\"\}"); // send message to be signed
       return;
   }

   if (msg[0]==commands.at(C_SIGNED))
   {
       qDebug() << QString::number(timer++) << " To Verify: " << msg[1] << "\n";

       system("openssl x509 -pubkey -noout -in mysite.local.cer  > pubkey.pem");
       system("openssl dgst -verify pubkey.pem -keyform PEM -sha256 -signature sig.bin -binary msg");
       return;
   }

}

/**
 * @brief SCD_SmartCardServer::resetAuthentication
 */
void SCD_SmartCardServer::resetAuthentication()
{
   isAuthenticated = 0;
   atr = "";
}
