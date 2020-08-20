#include "scd_smartcardserver.h"

void saveToFile(QString qStr, QString qPath)
{
    QFile qFile(qPath);
    if (qFile.open(QIODevice::WriteOnly)) {
      QTextStream out(&qFile); out << qStr;
      qFile.close();
    }
}

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

   if (msg.count()!=2)
   {
       qDebug() << QString::number(timer++) << " Wrongly formatted: " << message <<"\n";
       return;
   }

   int pos = msg[1].lastIndexOf(QChar('\"'));
   msg[1] = msg[1].left(pos);

   qDebug() << QString::number(timer++) << " Message Received: ";
   qDebug() << " Command: " << msg[0];
   qDebug() << " Data: " << msg[1] <<"\n";


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
      QString certificate = msg[1];
      QString qPath("cert_sc.cer");
      saveToFile(certificate, qPath);

      return;
   }

   if (msg[0]==commands.at(C_AUTH))
   {
       QString certificate = msg[1];
       QString qPath("cert.cer");
       saveToFile(certificate, qPath);

       QString msgToSign = "CryptoAPI is a good way to handle security";
       qPath = "toSign.txt";
       saveToFile(msgToSign, qPath);
       socket->sendTextMessage("{\"__MESSAGE__\":\"CMD|TOSIGN:" + msgToSign + "\"}"); // send message to be signed
       return;
   }

   if (msg[0]==commands.at(C_SIGNED))
   {
       QString sig64 = msg[1];
       sig64 = sig64.replace("\\r\\n","\r\n");
       sig64 = sig64.replace("\\n","\r\n");
       QString qPath("sig64");
       saveToFile(sig64, qPath);

       system("openssl base64 -d -in sig64 -out sig256");
       system("openssl x509 -pubkey -noout -in mysite.local.cer  > pubkey.pem");
       system("openssl dgst -verify pubkey.pem -keyform PEM -sha256 -signature sig256 -binary toSign.txt");
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
