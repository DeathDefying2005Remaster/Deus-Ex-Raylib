#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || 
PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

#if PLATFORM == PLATFORM_WINDOWS
#pragma comment( lib, "wsock32.lib" )
#endif

#include <stdio.h>
using namespace std;

bool InitializeSockets()
{
    #if PLATFORM == PLATFORM_WINDOWS
        WSADATA WsaData;
        return WSAStartup(MAKEWORD(2, 2),
                          &WsaData)
            == NO_ERROR;
    #else
        return true;
    #endif
}

void ShutdownSockets()
{
    #if PLATFORM == PLATFORM_WINDOWS
        WSACleanup();
    #endif
}

bool CreateSocket()
{
    int handle = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    if (handle <= 0)
    {
        printf("failed to create socket\n");
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port =
        htons((unsigned short)30000);

    if (bind(handle,
        (const sockaddr*)&address,
        sizeof(sockaddr_in)) < 0)
    {
        printf("failed to bind socket\n");
        return false;
    }

    #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

        int nonBlocking = 1;
        if (fcntl(handle,
            F_SETFL,
            O_NONBLOCK,
            nonBlocking) == -1)
        {
            printf("failed to set non-blocking\n");
            return false;
        }

    #elif PLATFORM == PLATFORM_WINDOWS

        DWORD nonBlocking = 1;
        if (ioctlsocket(handle,
            FIONBIO,
            &nonBlocking) != 0)
        {
            printf("failed to set non-blocking\n");
            return false;
        }

    #endif
}

bool Send(SOCKET handle, char* packet_data, int packet_size, sockaddr address)
{
    int sent_bytes =
        sendto(handle,
            (const char*)packet_data,
            packet_size,
            0,
            (sockaddr*)&address,
            sizeof(sockaddr_in));

    if (sent_bytes != packet_size)
    {
        printf("failed to send packet\n");
        return false;
    }
}

sockaddr_in CreateAddress(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned short port)
{
    unsigned int address = (a << 24) |
        (b << 16) |
        (c << 8) |
        d;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(address);
    addr.sin_port = htons(port);
    return addr;
}

void RecievePackets(SOCKET socket)
{
    while (true)
    {
        unsigned char packet_data[256];

        unsigned int max_packet_size =
            sizeof(packet_data);

        #if PLATFORM == PLATFORM_WINDOWS
                typedef int socklen_t;
        #endif

        sockaddr_in from;
        socklen_t fromLength = sizeof(from);

        int bytes = recvfrom(socket,
            (char*)packet_data,
            max_packet_size,
            0,
            (sockaddr*)&from,
            &fromLength);

        if (bytes <= 0)
            break;

        unsigned int from_address =
            ntohl(from.sin_addr.s_addr);

        unsigned int from_port =
            ntohs(from.sin_port);
    }
}

void DestroySocket(SOCKET socket)
{
    #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
            close(socket);
    #elif PLATFORM == PLATFORM_WINDOWS
        closesocket(socket);
    #endif
}

/* class Address
{
    public:

    Address();

    Address(unsigned char a,
            unsigned char b,
            unsigned char c,
            unsigned char d,
            unsigned short port);

    Address(unsigned int address,
            unsigned short port);

    unsigned int GetAddress() const;

    unsigned char GetA() const;
    unsigned char GetB() const;
    unsigned char GetC() const;
    unsigned char GetD() const;

    unsigned short GetPort() const;

    private:

    unsigned int address;
    unsigned short port;
};

class Socket
{
    public:

    Socket();

    ~Socket();

    bool Open(unsigned short port)
    {
        handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (handle <= 0)
        {
            printf("failed to create socket\n");
            return false;
        }

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons((unsigned short) port);

        if (bind(handle,
            (const sockaddr*)&address,
            sizeof(sockaddr_in)) < 0)
        {
            printf("failed to bind socket\n");
            return false;
        }

        #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

            int nonBlocking = 1;
            if (fcntl(handle,
                F_SETFL,
                O_NONBLOCK,
                nonBlocking) == -1)
            {
                printf("failed to set non-blocking\n");
                return false;
            }

        #elif PLATFORM == PLATFORM_WINDOWS

            DWORD nonBlocking = 1;
            if (ioctlsocket(handle,
                FIONBIO,
                &nonBlocking) != 0)
            {
                printf("failed to set non-blocking\n");
                return false;
            }

        #endif
    }

    void Close()
    {
        #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
                close(socket);
        #elif PLATFORM == PLATFORM_WINDOWS
                closesocket(handle);
        #endif
    }

    bool IsOpen() const;

    bool Send(const Address& destination, const void* data, int size)
    {
        int sent_bytes = sendto(handle,
                                (const char*) data,
                                size,
                                0,
                                (sockaddr*)&destination,
                                sizeof(sockaddr_in));

        if (sent_bytes != size)
        {
            printf("failed to send packet\n");
            return false;
        }
    }

    int Receive(Address& sender, void* data, int size)
    {
        while (true)
        {
            #if PLATFORM == PLATFORM_WINDOWS
                typedef int socklen_t;
            #endif

            sockaddr_in from;
            socklen_t fromLength = sizeof(from);

            int bytes = recvfrom(handle,
                                (char*)data,
                                size,
                                0,
                                (sockaddr*)&from,
                                &fromLength);

            if (bytes <= 0)
                break;

            unsigned int from_address =
                ntohl(from.sin_addr.s_addr);

            unsigned int from_port =
                ntohs(from.sin_port);
        }
    }

    private:

    int handle;
}; */