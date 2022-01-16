#ifndef NETWORKHANDLER_HPP
#define NETWORKHANDLER_HPP
#if defined(__APPLE__) || defined(__linux__)
// Some other place
#else
#include <enet/enet.h>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#endif
#include <sstream>
#include <stdint.h>
#include "Entities.hpp"
#include "EntityHandler.hpp"

// Forward declaration
class EntityHandler;

class NetworkHandler
{
private:
    ENetEvent _event;
    ENetAddress _address;
    ENetHost* _server = NULL;
    ENetHost* _client = NULL;
    ENetPeer* _peer = NULL;
    EntityHandler* _entities = NULL;
    
    std::string GetIPFromAdress(ENetAddress address);
    void SendPacket(std::ostringstream& data);
public:
    std::vector<uint8_t> PollAllServerEvents();
    void PollAllClientEvents();
    void setEntetiesHandler(EntityHandler* entities);
    NetworkHandler();
    ~NetworkHandler();
    void Host();
    void Join();
    void Disconnect();
    void Shoot();
    void S2CGameSnapshot(const GameSnapshot& gs);
};

#endif /* NETWORKHANDLER_HPP */
