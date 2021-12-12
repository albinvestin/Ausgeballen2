#ifndef NETWORKHANDLER_HPP
#define NETWORKHANDLER_HPP
#if defined(__APPLE__) || defined(__linux__)
// Some other place
#else
#include <enet/enet.h>
#include <cereal/archives/portable_binary.hpp>
#endif
#include <sstream>

class NetworkHandler
{
private:
    ENetEvent _event;
    ENetAddress _address;
    ENetHost* _server = NULL;
    ENetHost* _client = NULL;
    ENetPeer* _peer = NULL;

    std::string GetIPFromAdress(ENetAddress address);
public:
    void PollAllServerEvents();
    NetworkHandler();
    ~NetworkHandler();
    void Host();
    void Join();
    void Disconnect();
};





#endif /* NETWORKHANDLER_HPP */
