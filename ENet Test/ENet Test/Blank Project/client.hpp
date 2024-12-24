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
#include "packet.hpp"
#include "playerData.hpp"

class Client
{
	public:

	ENetHost* client;
	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;

	Client()
	{

	}
	~Client()
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

	void SendPacket(ENetPeer* _peer, Vector2 position)
	{
		string caca = to_string(position.x) + " " + to_string(position.y);
		const char* c = caca.c_str();
		ENetPacket* packet = enet_packet_create(c, strlen(c) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(_peer, 0, packet);
	}

	void RecievePositions(enet_uint8* data, vector<PlayerData>* peerInfo)
	{
		char countStr[2];
		char xStr[11];
		char yStr[11];
		strncpy_s(countStr, (char*)event.packet->data, 1);
		strncpy_s(xStr, (char*)event.packet->data + 2, 10);
		strncpy_s(yStr, (char*)event.packet->data + 13, 10);
		peerInfo->at(0).position.x = atof(xStr);
		peerInfo->at(0).position.y = atof(yStr);

		char peerCount = atof(countStr);

		for (int i = 0; i < peerCount - 1; i++)
		{
			char xStr2[11];
			char yStr2[11];
			char idStr[11];
			strncpy_s(xStr2, (char*)event.packet->data + 24 + (33 * i), 10);
			strncpy_s(yStr2, (char*)event.packet->data + 35 + (33 * i), 10);
			strncpy_s(idStr, (char*)event.packet->data + 46 + (33 * i), 10);

			Vector2 pos = { atof(xStr2), atof(yStr2) };

			if (peerInfo->size() < i + 2)
			{
				peerInfo->push_back(PlayerData((enet_uint32)atof(idStr), pos));
			}
			peerInfo->at(i + 1).position.x = pos.x;
			peerInfo->at(i + 1).position.y = pos.y;
		}
	}

	int StartClient(vector<PlayerData>* peerInfo)
	{
		if (enet_initialize() != 0)
		{
			fprintf(stderr, "An error occurred while initializing ENet.\n");
			return EXIT_FAILURE;
		}
		atexit(enet_deinitialize);

		client = enet_host_create(NULL, 1, 1, 0, 0);
		if (client == NULL)
		{
			fprintf(stderr, "An error occurred while creating an ENet client.\n");
			return EXIT_FAILURE;
		}

		enet_address_set_host(&address, "127.0.0.1");
		address.port = 7777;

		peer = enet_host_connect(client, &address, 1, 0);
		if (peer == NULL)
		{
			fprintf(stderr, "No available peers for initiating connection. You are all alone.\n");
			return EXIT_FAILURE;
		}
		peerInfo->push_back(PlayerData(0, Vector2()));

		if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			puts("Connection to 127.0.0.1:7777 succeeded. Yay!");
			enet_host_flush(client);
		}
		else
		{
			enet_peer_reset(peer);
			puts("Connection to 127.0.0.1:7777 failed. Awwww dangit.");
			return EXIT_SUCCESS;
		}

		return EXIT_SUCCESS;
	}

	void RunClient(vector<PlayerData>* peerInfo, Vector2 clientPos)
	{
		while (enet_host_service(client, &event, 0) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					printf("A packet of length %u containing %s was received from %s on channel %u.\n",
							event.packet->dataLength,
							event.packet->data,
							event.peer->data,
							event.channelID);
					RecievePositions(event.packet->data, peerInfo);
			}
		}
		SendPacket(peer, clientPos);
	}

	int DisconnectClient()
	{
		enet_peer_disconnect(peer, 0);
		while (enet_host_service(client, &event, 3000) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					puts("Disconnection successful. Yippee!");
					return EXIT_SUCCESS;
			}
		}
		enet_peer_reset(peer);
		return EXIT_SUCCESS;
	}
};