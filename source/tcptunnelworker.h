#ifndef TCPTUNNELWORKER_H
#define TCPTUNNELWORKER_H

#include <QObject>

class TCPTunnelWorker : public QObject
{
    Q_OBJECT
public:
    TCPTunnelWorker(){}
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
