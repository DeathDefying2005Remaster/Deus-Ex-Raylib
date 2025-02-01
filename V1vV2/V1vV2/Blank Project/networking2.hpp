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
#include <charconv>
#include <vector>
#include <iostream>
using namespace std;

class Observer
{
	public:

	virtual void Update(string* hellspawn, bool server) = 0;
	virtual void SetByString(string str) = 0;
	virtual int Name() = 0;
	virtual bool Owner() = 0;
	virtual string Value() = 0;
};

class MotherOfTheTrucker
{
	public:

	string hellspawn;
	vector<Observer*> observers;

	MotherOfTheTrucker()
	{
		//
	}

	void Broadcast(ENetPeer* peer, bool server)
	{
		hellspawn = "";

		for (int i = 0; i < observers.size(); i++)
		{
			observers[i]->Update(&hellspawn, server);
		}

		const char* c = hellspawn.c_str();
		ENetPacket* packet = enet_packet_create(c, strlen(c) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);

		hellspawn = "";
	}

	void Recieve(enet_uint8* data)
	{
		string str;
		for (int i = 0; i < strlen((char*)data); i++)
		{
			str += data[i];
		}

		while (str.length() > 0)
		{
			size_t border = str.find('|');
			string name = "";
			for (int j = 0; j < border; j++)
			{
				name += str[j];
			}
			size_t border2 = str.find(':');
			string value = "";
			for (int j = border + 1; j < border2; j++)
			{
				value += str[j];
			}

			for (int j = 0; j < observers.size(); j++)
			{
				if (observers[j]->Name() == stoi(name) && !observers[j]->Owner())
				{
					observers[j]->SetByString(value);
					break;
				}
			}

			str.erase(0, border2 + 1);
		}
	}

	void Subscribe(Observer* observer)
	{
		observers.push_back(observer);
	}
	void Unsubscribe(Observer* observer)
	{
		auto it = find(observers.begin(), observers.end(), observer);
		observers.erase(it);
	}
};

template <typename T> class NetworkVariable : public Observer
{
	public:

	int name;
	T value;
	bool owner;

	NetworkVariable()
	{
		//name = "unwanted neglected variable, only considered worthy of a name once an owner has been chosen for it.";
	}
	NetworkVariable(T _value, int _name, bool _owner, MotherOfTheTrucker* mother, Observer* thissonofabeep)
	{
		name = _name;
		value = _value;
		owner = _owner;

		mother->Subscribe(thissonofabeep);
	}

	void Update(string* hellspawn, bool server) override
	{
		if (owner || server)
		{
			string alaska = to_string(name) + "|" + PackVar(value);
			hellspawn->append(alaska);
		}
	}
	void SetByString(string str) override
	{
		UnpackVar(str, &value);
	}
	int Name() override
	{
		return name;
	}
	bool Owner() override
	{
		return owner;
	}
	string Value()
	{
		return to_string(value.y);
	}

	template <typename T> NetworkVariable<T>& operator= (T _value)
	{
		this->value = _value;
		return *this;
	}
};

class Client : public MotherOfTheTrucker
{
	public:

	ENetHost* client;
	ENetPeer* peer;
	ENetAddress address;
	ENetEvent event;

	Client() {}

	int Start(string connectAddress)
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

		enet_address_set_host(&address, connectAddress.c_str());
		address.port = 11775;

		peer = enet_host_connect(client, &address, 1, 0);
		if (peer == NULL)
		{
			fprintf(stderr, "No available peers for initiating connection. You are all alone.\n");
			return EXIT_FAILURE;
		}

		if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			printf("Connection to ");
			printf(connectAddress.c_str());
			printf(" succeeded. Yay!\n");
			enet_host_flush(client);
		}
		else
		{
			enet_peer_reset(peer);
			printf("Connection to ");
			printf(connectAddress.c_str());
			printf(" failed. Awwww dangit.\n");
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}
	void Run()
	{
		Broadcast(peer, false);

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
				Recieve(event.packet->data);
			}
		}
	}
	int Kill()
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

class Server : public MotherOfTheTrucker
{
	public:

	ENetHost* server;
	vector<ENetPeer*> peers;
	ENetAddress address;
	ENetEvent event;

	Server() {}

	int Start()
	{
		if (enet_initialize() != 0)
		{
			printf("An error occurred while initializing ENet.\n");
			return EXIT_FAILURE;
		}
		atexit(enet_deinitialize);

		address.host = ENET_HOST_ANY;
		address.port = 11775;

		server = enet_host_create(&address, 3, 1, 0, 0);

		if (server == NULL)
		{
			printf("An error occurred while creating an ENet server.\n");
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}
	void Run()
	{
		for (int i = 0; i < peers.size(); i++)
		{
			Broadcast(peers[i], true);
		}

		while (enet_host_service(server, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A client connected from %x:%u.\n", event.peer->address.host, event.peer->address.host);
				peers.push_back(event.peer);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
				Recieve(event.packet->data);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("Client %x:%u disconnected.\n", event.peer->address.host, event.peer->address.host);
				break;
			}
		}
	}
	int Kill()
	{
		printf("Destroying server.\n");
		enet_host_destroy(server);
		printf("Server destroyed.\n");
		return EXIT_SUCCESS;
	}
};

//---- Serialization ----//

string PackVar(Vector2 v)
{
	string x = to_string(v.x);
	/*while (x.length() < 10) { x.append("0"); }
	while (x.length() > 10) { x.erase(x.end()); }*/
	string y = to_string(v.y);
	/*while (y.length() < 10) { y.append("0"); }
	while (y.length() > 10) { y.erase(y.end()); }*/
	return "V " + x + " " + y + ":";
}
string PackVar(enet_uint32 v)
{
	string str = to_string(v);
	while (str.length() < 10)
	{
		str.insert(0, "0");
	}
	return "I " + str + ":";
}
string PackVar(bool v)
{
	return "B " + to_string(v) + ":";
}

void UnpackVar(string str, Vector2* v)
{
	string strX = "";
	string strY = "";
	size_t border = str.find(' ', 2);
	for (int j = 2; j < border; j++)
	{
		strX += str[j];
	}
	for (int j = border + 1; j < str.length(); j++)
	{
		strY += str[j];
	}
	*v = { stof(strX), stof(strY) };
}
void UnpackVar(string str, enet_uint32* v)
{
	string var = "";
	for (int i = 2; i < str.length(); i++)
	{
		var += str[i];
	}
	*v = stoul(var);
}
void UnpackVar(string str, enet_uint8* v)
{
	string var = "";
	for (int i = 2; i < str.length(); i++)
	{
		var += str[i];
	}
	*v = stoi(var);
}
void UnpackVar(string str, bool* v)
{
	bool barf = stoi(to_string(str[2]));
	*v = barf;
}

void PutsVar(Vector2 v)
{
	puts(to_string(v.x).c_str());
	puts(to_string(v.y).c_str());
}