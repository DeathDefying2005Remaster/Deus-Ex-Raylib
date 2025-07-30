#pragma once

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
#include <charconv>
#include <vector>
#include <iostream>
#include <string>

//---- Serialization ----//

std::string PackVar(Vector2 v);
std::string PackVar(enet_uint32 v);
std::string PackVar(int v);
std::string PackVar(bool v);

void UnpackVar(std::string str, Vector2* v);
void UnpackVar(std::string str, enet_uint32* v);
void UnpackVar(std::string str, enet_uint8* v);
void UnpackVar(std::string str, int* v);
void UnpackVar(std::string str, bool* v);

//---- Other Stuff ----//

class Observer
{
	public:

	virtual void Update(std::string* hellspawn, bool server) = 0;
	virtual std::string Pack() = 0;
	virtual void SetByString(std::string str) = 0;
	virtual int Name() = 0;
	virtual bool Owner() = 0;
};

class Event
{
	public:

	int name;
	std::vector<Observer*> parameters;
	std::string* hellspawn = nullptr;

	void Send()
	{
		std::string alaska = "E" + std::to_string(name) + "[";
		for (int i = 0; i < parameters.size(); i++)
		{
			alaska.append(parameters[i]->Pack());
		}
		alaska.append("]");
		hellspawn->append(alaska);
	}

	void Recieve(std::string str)
	{
		str.erase(0, 3);
		printf(str.c_str());
		int i = 0;
		while (str.length() > 0)
		{
			size_t border = str.find(':');
			std::string jimjam = "";
			for (int j = 0; j < border; j++)
			{
				jimjam += str[j];
			}
			parameters[i]->SetByString(jimjam);
			str.erase(0, border + 1);
			i++;
		}

		Trigger();
	}

	virtual void Trigger() = 0;
};

class MotherOfTheTrucker
{
	public:

	std::string hellspawn;
	std::vector<Observer*> observers;
	std::vector<Event*> events;

	MotherOfTheTrucker()
	{
		//
	}

	void Broadcast(ENetPeer* peer, bool server)
	{
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
		std::string str;
		for (int i = 0; i < strlen((char*)data); i++)
		{
			str += data[i];
		}

		while (str[0] == 'E')
		{
			int name = str[1] - '0';
			size_t border = str.find(']');
			std::string event = str.substr(0, border);
			str.erase(0, border + 1);
			for (int i = 0; i < events.size(); i++)
			{
				printf("ooh");
				if (events[i]->name == name)
				{
					printf("AH!");
					events[i]->Recieve(event);
					break;
				}
			}
		}

		while (str.length() > 0)
		{
			size_t border = str.find('|');
			std::string name = "";
			for (int j = 0; j < border; j++)
			{
				name += str[j];
			}
			size_t border2 = str.find(':');
			std::string value = "";
			for (int j = int(border + 1); j < border2; j++)
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
	void Subscribe(Event* event)
	{
		events.push_back(event);
		event->hellspawn = &hellspawn;
	}
	void Unsubscribe(Observer* observer)
	{
		auto it = find(observers.begin(), observers.end(), observer);
		observers.erase(it);
	}
	void Unsubscribe(Event* event)
	{
		auto it = find(events.begin(), events.end(), event);
		events.erase(it);
		event->hellspawn = nullptr;
	}

	virtual int Start(std::string connectAddress) { return 0; };
	virtual void Run() {};
	virtual int Kill() { return 0; };
};

//---- <MOTHER> ----//

extern MotherOfTheTrucker* networking;

//---- >MOTHER< ----//

template <typename T> class NetworkVariable : public Observer
{
	public:

	int name;
	T value;
	bool owner;

	NetworkVariable()
	{
		name = -1;
		value = T();
		owner = false;
	}
	NetworkVariable(T _value, int _name, bool _owner, Observer* justapointertothisvariable)
	{
		name = _name;
		value = _value;
		owner = _owner;

		networking->Subscribe(justapointertothisvariable);
	}

	void Update(std::string* hellspawn, bool server) override
	{
		if (owner || server)
		{
			std::string alaska = std::to_string(name) + "|" + PackVar(value);
			hellspawn->append(alaska);
		}
	}
	std::string Pack()
	{
		return PackVar(value);
	}
	void SetByString(std::string str) override
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

	int Start(std::string connectAddress) override
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
					(unsigned int)event.packet->dataLength,
					(char*)event.packet->data,
					(char*)event.peer->data,
					(unsigned int)event.channelID);
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
	std::vector<ENetPeer*> peers;
	ENetAddress address;
	ENetEvent event;

	Server() {}

	int Start(std::string connectAddress) override
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

		printf("Server created successfully.\n");
		return EXIT_SUCCESS;
	}
	void Run() override
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
					(unsigned int)event.packet->dataLength,
					(char*)event.packet->data,
					(char*)event.peer->data,
					(unsigned int)event.channelID);
				Recieve(event.packet->data);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("Client %x:%u disconnected.\n", event.peer->address.host, event.peer->address.host);
				break;
			}
		}
	}
	int Kill() override
	{
		printf("Destroying server.\n");
		enet_host_destroy(server);
		printf("Server destroyed.\n");
		return EXIT_SUCCESS;
	}
};
