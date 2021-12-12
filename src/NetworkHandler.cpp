#include "../inc/NetworkHandler.hpp"

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

void NetworkHandler::PollAllServerEvents()
{
    if (_server == NULL)
    {
        return;
    }
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service (_server, &event, 5) > 0)
    {
        printf("Got an event\n");
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                event.peer->data = (void*)"FirstConnection";
                printf("A new client connected from %x:%u. Giving it name \"%s\"\n",
                    event.peer->address.host,
                    event.peer->address.port,
                    (char*)event.peer->data);
                break;

            // case ENET_EVENT_TYPE_RECEIVE:
            //     if (_event.packet->dataLength > 0)
            //     {
            //         std::stringstream ss((char*)_event.packet->data); // Use std::ios::binary if filestream
            //         cereal::PortableBinaryInputArchive inArchive(ss);
            //         char header;
            //         inArchive(header); // DeSerialize
            //         printf("Got header: %u\n", header);
                    
            //         switch (header)
            //         {
            //         case ENET_UINT16:
            //             UINT16 result;
            //             inArchive(result); // DeSerialize

            //             printf ("A packet of length %u containing \"%#X\" was received from %s on channel %u.\n",
            //                 event.packet->dataLength,
            //                 result,
            //                 event.peer->data,
            //                 event.channelID);
                            
            //             break;

            //         case ENET_FLOAT:
            //             float inputdata;
            //             inArchive(inputdata); // DeSerialize
            //             printf("Got float: %f\n", inputdata);

            //         default:
            //             break;
            //         }

            //         /* Clean up the packet now that we're done using it. */
            //         enet_packet_destroy (_event.packet);
            //     }

            //     // SEND A REPLY! :D
            //     // SendPacket(_event.peer, "HELLO!"); // This adds the packet to a queue

            // break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf ("%s disconnected.\n", event.peer -> data);
                /* Reset the peer's client information. */
                event.peer -> data = NULL;
                break;
        }
    }

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