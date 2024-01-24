#include "../content/server.h"
#include <iostream>
#include <cstring>

server::server() : serverHost(nullptr) {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet." << std::endl;
        exit(EXIT_FAILURE);
    }
}

server::~server() {
    enet_host_destroy(serverHost);
    enet_deinitialize();
}

void server::start() {
    ENetAddress address;
    ENetEvent event;

    address.host = ENET_HOST_ANY;
    address.port = 1234;

    serverHost = enet_host_create(&address, 32, 2, 0, 0);

    if (serverHost == nullptr) {
        std::cerr << "Failed to create ENet server host." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "server started on port 1234." << std::endl;

    while (true) {
        if (enet_host_service(serverHost, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "A new client connected." << std::endl;
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    handlePacket(event.packet);
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "A client disconnected." << std::endl;
                    break;

                default:
                    break;
            }
        }

        broadcastPlayerUpdates();
    }
}

void server::handlePacket(ENetPacket* packet) {
    playerUpdate receivedUpdate;
    memcpy(&receivedUpdate, packet->data, sizeof(playerUpdate));

    // store player in vector
    players.push_back(receivedUpdate);

    std::cout << "Received player update from client " << receivedUpdate.playerId << std::endl;
}

void server::broadcastPlayerUpdates() {
    if (!players.empty()) {
        // Create a packet to send player updates
        ENetPacket* packet = enet_packet_create(&players[0], players.size() * sizeof(playerUpdate), ENET_PACKET_FLAG_RELIABLE);

        // Broadcast the packet to all connected clients
        enet_host_broadcast(serverHost, 0, packet);

        // Clear the vector after broadcasting
        players.clear();
    }
}

void server::stop() {
    std::cout << "server shutting down." << std::endl;
    enet_host_destroy(serverHost);
    enet_deinitialize();
    exit(EXIT_SUCCESS);
}