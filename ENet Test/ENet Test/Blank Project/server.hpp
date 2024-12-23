#include <iostream>

#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <enet/enet.h>

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#undef PlaySound
#endif

#include <raylib-cpp.hpp>
#include <vector>
#include <string>
#include "playerData.hpp"

class Server
{
	public:

	ENetAddress address;
	ENetHost* server;
	ENetEvent event;
	std::vector<ENetPeer*> peers;

	Server()
	{

	}
	~Server()
	{

	}

	Vector2 UnpackVector2(enet_uint8* data)
	{
		char xStr[11];
		char yStr[11];
		strncpy_s(xStr, (char*)event.packet->data, 10);
		strncpy_s(yStr, (char*)event.packet->data + 11, 10);

		Vector2 result;
		result.y = atof(yStr);
		result.x = atof(xStr);
		return result;
	}

	void SendPacket(ENetPeer* _peer, Vector2 position, std::vector<PlayerData>* clientInfo)
	{
		std::string bongo = std::to_string((unsigned char)clientInfo->size()) + "|" + std::to_string(position.x) + " " + std::to_string(position.y) + "|";
		for (int i = 0; i < clientInfo->size(); i++)
		{
			if (clientInfo->at(i).connectId != _peer->connectID)
			{
				std::string caca = std::to_string(clientInfo->at(i).position.x) + " " + std::to_string(clientInfo->at(i).position.y)
					+ ":" + std::to_string(clientInfo->at(i).connectId) + "|";
				bongo.append(caca);
			}
		}

		const char* c = bongo.c_str();
		ENetPacket* packet = enet_packet_create(c, strlen(c) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(_peer, 0, packet);
	}

	void AddClient(ENetPeer* client, std::vector<PlayerData>* clientInfo)
	{
		bool tru = true;
		for (int i = 0; i < peers.size(); i++)
		{
			ENetPeer peep = *peers[i];
			enet_uint32 id = peep.connectID;
			if (id == client->connectID)
			{
				tru = false;
			}
		}
		if (true) 
		{
			peers.push_back(event.peer);
			clientInfo->push_back(PlayerData(client->connectID, {}));
		}
	}

	void RecieveClientPositions(ENetEvent event, std::vector<PlayerData>* clientInfo)
	{
		for (int i = 0; i < peers.size(); i++)
		{
			if (peers[i]->connectID == event.peer->connectID)
			{
				clientInfo->at(i).position = UnpackVector2(event.packet->data);
				break;
			}
		}
	}

	int StartServer()
	{
		if (enet_initialize() != 0)
		{
			printf("An error occurred while initializing ENet.\n");
			return EXIT_FAILURE;
		}
		atexit(enet_deinitialize);

		address.host = ENET_HOST_ANY;
		address.port = 7777;

		server = enet_host_create(&address, 3, 1, 0, 0);

		if (server == NULL)
		{
			printf("An error occurred while creating an ENet server.\n");
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}

	void RunServer(std::vector<PlayerData>* clientInfo, Vector2 serverPos)
	{
		while (enet_host_service(server, &event, 0) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
					printf("A client connected from %x:%u.\n", event.peer->address.host, event.peer->address.host);
					AddClient(event.peer, clientInfo);
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					printf("A packet of length %u containing %s was received from %s on channel %u.\n",
							event.packet->dataLength,
							event.packet->data,
							event.peer->data,
							event.channelID);
					RecieveClientPositions(event, clientInfo);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					printf("Client %x:%u disconnected.\n", event.peer->address.host, event.peer->address.host);
					break;
			}
		}

		for (int i = 0; i < peers.size(); i++)
		{
			SendPacket(peers[i], serverPos, clientInfo);
		}
	}

	int DestroyServer()
	{
		printf("Destroying server.\n");
		enet_host_destroy(server);
		printf("Server destroyed.\n");
		return EXIT_SUCCESS;
	}
};