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
#include <cstdint>
#include <cstddef>
#include <vector>
using namespace std;

// BASIC IDEA: 
// The NetworkVariable class holds every overloaded function for converting types to and from the packet string.
// The rules for the packets are as follows: 
// - Owners are separated by '|', and variables by ':'
// - Each variable has one character (e.g. 'V') that indicates the type, followed by a space and then the variable
// - A digit at the beginning indicates how many owners are expected, followed by '|'

//template <typename T> class NetworkVariable
//{
//	public:
//
//	T value;
//
//	NetworkVariable<enet_uint32>()
//	{
//
//	}
//	NetworkVariable<Vector2>()
//	{
//
//	}
//};

class PlayerData
{
	public:

	enet_uint32 connectId;
	Vector2 position;

	PlayerData(enet_uint32 id, Vector2 pos)
	{
		connectId = id;
		position = pos;
	}
};

inline Vector2 UnpackVector2(enet_uint8* data)
{
	char xStr[11];
	char yStr[11];
	strncpy_s(xStr, (char*)data, 10);
	strncpy_s(yStr, (char*)data + 11, 10);

	Vector2 result;
	result.y = atof(yStr);
	result.x = atof(xStr);
	return result;
}

inline string FloatToString(float value)
{
	string str = to_string(value);
	while (str.length() < 10)
	{
		str.append("0");
	}
	while (str.length() > 10)
	{
		str.erase(str.end());
	}
	return str;
}

class Server
{
	public:

	ENetAddress address;
	ENetHost* server;
	ENetEvent event;
	vector<ENetPeer*> peers;

	Server()
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

	void SendPacket(ENetPeer* _peer, Vector2 position, vector<PlayerData>* clientInfo)
	{
		string bongo = to_string((unsigned char)clientInfo->size()) + "|" + FloatToString(position.x) + " " + FloatToString(position.y) + "|";
		for (int i = 0; i < clientInfo->size(); i++)
		{
			if (clientInfo->at(i).connectId != _peer->connectID)
			{
				string caca = FloatToString(clientInfo->at(i).position.x) + " " + FloatToString(clientInfo->at(i).position.y)
					+ ":" + to_string(clientInfo->at(i).connectId) + "|";
				bongo.append(caca);
			}
		}

		const char* c = bongo.c_str();
		ENetPacket* packet = enet_packet_create(c, strlen(c) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(_peer, 0, packet);
	}

	void AddClient(ENetPeer* client, vector<PlayerData>* clientInfo)
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

	void RecieveClientPositions(ENetEvent event, vector<PlayerData>* clientInfo)
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

	void RunServer(vector<PlayerData>* clientInfo, Vector2 serverPos)
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

	void SendPacket(ENetPeer* _peer, Vector2 position)
	{
		string caca = FloatToString(position.x) + " " + FloatToString(position.y);
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
			enet_uint32 id = atof(idStr);

			if (peerInfo->size() < i + 2)
			{
				peerInfo->push_back(PlayerData(id, pos));
			}

			for (int j = 0; j < peerInfo->size(); j++)
			{
				if (peerInfo->at(j).connectId == id)
				{
					peerInfo->at(j).position.x = pos.x;
					peerInfo->at(j).position.y = pos.y;
					break;
				}
			}
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