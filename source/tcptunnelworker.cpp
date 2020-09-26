#include "tcptunnelworker.h"

TCPTunnelWorker::TCPTunnelWorker()
{

}

void TCPTunnelWorker::process() { // Process. Start processing data.
    // allocate resources using new here
    qDebug("Hello World!");
    emit finished();
}
