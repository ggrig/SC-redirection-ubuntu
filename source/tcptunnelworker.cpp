#include "tcptunnelworker.h"
#include "tcptunnel.h"

void TCPTunnelWorker::process() { // Process. Start processing data.
    // allocate resources using new here

#ifdef TCP_TUNNEL_STANDALONE
// ./tcptunnel --local-port=3240 --remote-port=3240 --remote-host=192.168.1.7 --log --stay-alive

    name = argv[0];

    set_options(argc, argv);

    if (build_server() == 1)
    {
        exit(1);
    }

    do
    {
        if (wait_for_clients() == 0)
        {
            handle_client();
        }
    }
    while (settings.stay_alive);

    close(rc.server_socket);
#endif

    qDebug("Hello World!");
    emit finished();
}
