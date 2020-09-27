#include "tcptunnelworker.h"
#include "tcptunnel.h"

void TCPTunnelWorker::process() { // Process. Start processing data.
    // allocate resources using new here

    set_option(LOCAL_PORT_OPTION, "3240");
    set_option(REMOTE_PORT_OPTION, "23240");
    set_option(REMOTE_HOST_OPTION, "192.168.1.7");
    set_option(STAY_ALIVE_OPTION, "");
    set_option(LOG_OPTION, "");

    tcptunnel_loop(pServer);

    emit finished();
}
