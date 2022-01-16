#include "../inc/NetworkHandler.hpp"
#include "../inc/Constants.hpp"
// #include "../inc/SerializeBaseData.hpp"
#include "../inc/Vector2.hpp"
#include "../inc/Entities.hpp"
#include <cereal/archives/portable_binary.hpp>

NetworkHandler::NetworkHandler()
{
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
    }
    atexit(enet_deinitialize);
}

NetworkHandler::~NetworkHandler()
{
    if (_server != NULL)
    {
        enet_host_destroy(_server);
    }
        if (_client != NULL)
    {
        enet_host_destroy(_client);
    }
}

void NetworkHandler::Host()
{
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    _address.host = ENET_HOST_ANY; // This allows
    /* Bind the server to port 7777. */
    _address.port = 7777;

    _server = enet_host_create (&_address	/* the address to bind the server host to */,
                    32	/* allow up to 32 clients and/or outgoing connections */,
                    1	/* allow up to 1 channel to be used, 0. */,
                    0	/* assume any amount of incoming bandwidth */,
                    0	/* assume any amount of outgoing bandwidth */);

    if (_server == NULL)
    {
        printf("An error occurred while trying to create an ENet server host.");
    }
    else
    {
        printf("Host is running at 127.0.0.1 Port %u\n", _address.port);
    }
    PollAllServerEvents();
}

void NetworkHandler::setEntetiesHandler(EntityHandler* entities)
{
    _entities = entities;
}

std::vector<uint8_t> NetworkHandler::PollAllServerEvents()
{
    std::vector<uint8_t> recivedActions;
    if (_server == NULL)
    {
        return recivedActions;
    }

    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service (_server, &_event, 5) > 0) // This adds a variable delta time, TODO sleep so that we have constant 60 Hz.
    {
        printf("Got an event\n");
        switch (_event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                _event.peer->data = (void*)"FirstConnection";
                printf("A new client connected from %x:%u. Giving it name \"%s\"\n",
                    _event.peer->address.host,
                    _event.peer->address.port,
                    (char*)_event.peer->data);
                _peer = _event.peer; // Set first connected as _peer
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                printf("Recived data\n");
                if (_event.packet->dataLength > 0)
                {
                    printf("Recived data longer than 0\n");
                    std::string input((unsigned char*)_event.packet->data, _event.packet->data + _event.packet->dataLength);
                    std::istringstream iss(input, std::ios_base::in|std::ios_base::binary);
                    uint8_t header = 0;
                    {
                        // Encapsulate archive to make sure all content is flused
                        cereal::PortableBinaryInputArchive inArchive(iss);
                        inArchive(header); // DeSerialize header only
                    }
                    iss.seekg(0, iss.beg); // Reset read position
                    
                    printf("Got header: %u\n", header);
                    
                    switch (header)
                    {
                    case NETWORK_TYPE_UINT16:
                    {
                        // TODO make a template?
                        uint16_t inputdata;
                        {
                            // Encapsulate archive to make sure all content is flused
                            cereal::PortableBinaryInputArchive inArchive(iss);
                            inArchive(header, inputdata); // DeSerialize
                        }
                        break;
                        // printf ("A packet of length %u containing \"%#X\" was received from %s on channel %u.\n",
                        //     _event.packet->dataLength,
                        //     result,
                        //     _event.peer->data,
                        //     _event.channelID);
                    }

                    case NETWORK_TYPE_FLOAT:
                    {
                        float inputdata;
                        {
                            // Encapsulate archive to make sure all content is flused
                            cereal::PortableBinaryInputArchive inArchive(iss);
                            inArchive(header, inputdata); // DeSerialize
                        }
                        break;
                        // printf("Got float: %f\n", inputdata);
                    }

                    case NETWORK_TYPE_UINT8:
                    {
                        uint8_t inputdata;
                        {
                            // Encapsulate archive to make sure all content is flused
                            cereal::PortableBinaryInputArchive inArchive(iss);
                            inArchive(header, inputdata); // DeSerialize
                        }
                        printf("Got uint8_t: %u\n", inputdata);
                        recivedActions.push_back(inputdata);
                        break;
                        
                        // inArchive(inputdata); // DeSerialize
                    }

                    default:
                        break;
                    }

                }
                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy (_event.packet);

            break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                printf ("%s disconnected.\n", _event.peer -> data);
                /* Reset the peer's client information. */
                _event.peer -> data = NULL;
                break;
            }
        }
    }

    return recivedActions;
}

void NetworkHandler::Join()
{
    _client = enet_host_create(NULL, 1, 1, 0, 0);

    if(_client == NULL)
    {
        fprintf(stderr, "An error occurred while trying to create an ENet client host!\n");
    }

    enet_address_set_host(&_address, "127.0.0.1");
    _address.port = 7777;

    _peer = enet_host_connect(_client, &_address, 1, 0);
    if(_peer == NULL)
    {
        fprintf(stderr, "No available peers for initiating an ENet connection!\n");
    }

    if(enet_host_service(_client, &_event, 5000) > 0
        && _event.type == ENET_EVENT_TYPE_CONNECT)
    {
        printf("Connection to 127.0.0.1 Port %u succeeded.\n", _address.port);
        /* Enet bufferers protocol messages to reduce traffic. 
        This means it may not send the connection event until you send the first packet. 
        If you call enet_host_flush in the client after is accepts the connection it 
        will force enet to send the connection message to the server. */
        enet_host_flush(_client);
    }
    else
    {
        enet_peer_reset(_peer);
        printf("Connection to 127.0.0.1 Port %u failed.\n", _address.port);
    }
}

void NetworkHandler::PollAllClientEvents()
{
    if (_client == NULL)
    {
        return;
    }
    while(enet_host_service(_client, &_event, 5) > 0)
    {
        switch(_event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                printf("Recived data\n");
                if (_event.packet->dataLength > 0)
                {
                    printf("Data length %u\n", (unsigned int)_event.packet->dataLength);
                    std::string input((unsigned char*)_event.packet->data, _event.packet->data + _event.packet->dataLength);
                    std::istringstream iss(input, std::ios_base::in|std::ios_base::binary);
                    uint8_t header = 0;
                    {
                        // Encapsulate archive to make sure all content is flused
                        cereal::PortableBinaryInputArchive inArchive(iss);
                        inArchive(header); // DeSerialize header only
                    }
                    iss.seekg(0, iss.beg); // Reset read position
                    printf("Got header: %u\n", header);
                    
                    switch (header)
                    {
                    case NETWORK_TYPE_UINT16:
                    {
                        uint16_t inputdata;
                        {
                            // Encapsulate archive to make sure all content is flused
                            cereal::PortableBinaryInputArchive inArchive(iss);
                            inArchive(header, inputdata);
                        }

                        printf ("A packet of length %u containing \"%#X\" was received from %s on channel %u.\n",
                            _event.packet->dataLength,
                            inputdata,
                            _event.peer->data,
                            _event.channelID);
                            
                        break;
                    }
                    case NETWORK_TYPE_FLOAT:
                    {
                        float inputdata;
                        {
                            // Encapsulate archive to make sure all content is flused
                            cereal::PortableBinaryInputArchive inArchive(iss);
                            inArchive(header, inputdata);
                        }
                        printf("Got float: %f\n", inputdata);
                        break;
                    }
                    case NETWORK_TYPE_GAMESNAPSHOT:
                    {
                        GameSnapshot gs;
                        {
                            // Encapsulate archive to make sure all content is flused
                            cereal::PortableBinaryInputArchive inArchive(iss);
                            inArchive(header, gs);
                        }

                        printf("Got BulletRecoilPlayerIndex\n");
                        // TODO make the EntityHandle handle these calls.
                        _entities->HandleNetworkGameSnapshot(gs);
                        break;
                    }
                    case NETWORK_TYPE_UINT8:
                    {
                        uint8_t inputdata;
                        {
                            // Encapsulate archive to make sure all content is flused
                            cereal::PortableBinaryInputArchive inArchive(iss);
                            inArchive(header, inputdata);
                        }
                        printf("Got uint8_t: %u\n", inputdata);

                        break;
                    }

                    default:
                        break;
                    }

                }
                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy (_event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("Got Disconnected!.");
                break;
        }
    }
}

void NetworkHandler::Disconnect()
{
    if (_client == NULL)
    {
        return;
    }
    printf("Trying to disconnect...");
    enet_peer_disconnect(_peer, 0);
    /* Enet bufferers protocol messages to reduce traffic. 
    This means it may not send the connection event until you send the first packet. 
    If you call enet_host_flush in the client after is accepts the connection it 
    will force enet to send the connection message to the server. */
    enet_host_flush(_client);

    while(enet_host_service(_client, &_event, 3000) > 0)
    {
        switch(_event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(_event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("Disconnection succeeded.");
                return;
                break;
        }
    }

    /* We've arrived here, so the disconnect attempt didn't */
    /* succeed yet.  Force the connection down.             */
    enet_peer_reset(_peer);
}

std::string NetworkHandler::GetIPFromAdress(ENetAddress address)
{
    std::string IP = "X.X.X.X";
    IP[0] = address.host & 0xff;
    address.host >>= 8;
    IP[2] = address.host & 0xff;
    address.host >>= 8;
    IP[4] = address.host & 0xff;
    address.host >>= 8;
    IP[6] = address.host & 0xff;
    return IP;
}

void NetworkHandler::Shoot()
{
    uint8_t header = NETWORK_TYPE_UINT8;
    uint8_t action = NETWORK_ACTION_SHOOT_P2;
    printf("Sending shoot: uint32_t = %u\n", action);
    std::ostringstream oss(std::ios_base::out|std::ios_base::binary);
    {
        // Encapsulate archive to make sure all content is flused
        cereal::PortableBinaryOutputArchive outArchive(oss);
        outArchive(header, action); // Serialize
    }

    SendPacket(oss);
}


void NetworkHandler::S2CGameSnapshot(const GameSnapshot& gs)
{
    uint8_t header = NETWORK_TYPE_GAMESNAPSHOT;
    std::ostringstream oss(std::ios_base::out|std::ios::binary);
    {
        // Encapsulate archive to make sure all content is flused
        cereal::PortableBinaryOutputArchive outArchive(oss);
        outArchive(header, gs); // Serialize
    }

    SendPacket(oss);
}

void NetworkHandler::SendPacket(std::ostringstream& data)
{
    ENetPacket* packet;
    int dataLength = data.tellp();
    if (dataLength > 0)
    {
        packet = enet_packet_create(data.str().c_str(), dataLength, ENET_PACKET_FLAG_RELIABLE);
    }
    // enet_packet_create takes a const void * for data to be sent.
    
    // Second arg is channel:
    if (_peer == NULL)
    {
        printf("Peer not initialized! at SendPacket");
        return;
    }
    enet_peer_send(_peer, 0, packet);
    if (_client != NULL)
    {
        enet_host_service(_client, &_event, 10);
    }
    else if (_server != NULL)
    {
        enet_host_service(_server, &_event, 10);
    }
}
