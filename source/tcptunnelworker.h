#ifndef TCPTUNNELWORKER_H
#define TCPTUNNELWORKER_H

#include <QObject>
#include "scd_smartcardserver.h"

class TCPTunnelWorker : public QObject
{
    Q_OBJECT
    SCD_SmartCardServer * pServer;
public:
    TCPTunnelWorker(SCD_SmartCardServer * p):pServer(p){}
    virtual ~TCPTunnelWorker(){}
public slots:
    void process();
signals:
    void finished();
    void error(QString err);
private:
    // add your variables here
};

#endif // TCPTUNNELWORKER_H
