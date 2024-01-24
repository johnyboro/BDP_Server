#ifndef BDP_SERVER_SERVER_H
#define BDP_SERVER_SERVER_H

#include <vector>
#include <enet/enet.h>
#include "playerUpdate.h"

class server {
public:
    server();
    ~server();

    void start();
    void stop();

private:
    ENetHost* serverHost;
    std::vector<playerUpdate> players;

    void handlePacket(ENetPacket* packet);
    void broadcastPlayerUpdates();
};

#endif //BDP_SERVER_SERVER_H
