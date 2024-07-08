#ifdef _WIN32

#include "THzCommon/network/socketwin32.hpp"

#include "THzCommon/utility/workerThread.hpp"

#define WIN32_LEAN_AND_MEAN

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>
#include <gtest/gtest.h>
#include <iostream>
#include <system_error>

namespace Terrahertz::UnitTests {

struct NetworkSocketWin32 : public testing::Test
{
    static void SetUpTestSuite()
    {
        std::cout << "Starting Server ..." << std::endl;
        _server.thread = std::thread([]() {
            WSADATA    wsaData{};
            auto const result = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (result != 0)
            {
                throw std::system_error({result, std::iostream_category()}, "WSAStartup failed.");
            }

            sockaddr_in service{};
            service.sin_family = AF_INET;
            service.sin_port   = htons(47111);
            auto serverSocket  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            unsigned long mode = 1U;
            if (serverSocket == INVALID_SOCKET)
            {
                _server.shutdownFlag = true;
            }
            else if (InetPton(AF_INET, "127.0.0.1", &service.sin_addr.s_addr) != 1)
            {
                _server.shutdownFlag = true;
            }
            else if (bind(serverSocket, (SOCKADDR *)&service, sizeof(service)) == SOCKET_ERROR)
            {
                _server.shutdownFlag = true;
            }
            else if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
            {
                _server.shutdownFlag = true;
            }
            // Set socket to non-blocking so we are able to shutdown easily
            else if (ioctlsocket(serverSocket, FIONBIO, &mode) == SOCKET_ERROR)
            {
                _server.shutdownFlag = true;
            }

            WorkerThread::UniqueLock lock{_server.mutex};
            while (!_server.shutdownFlag)
            {
                auto connectionsocket = accept(serverSocket, nullptr, nullptr);
                if (connectionsocket == INVALID_SOCKET)
                {
                    _server.wakeUp.wait_for(lock, std::chrono::milliseconds{10U});
                    continue;
                }
                std::cout << "Connected to socket" << std::endl;
            }

            if (serverSocket != INVALID_SOCKET)
            {
                closesocket(serverSocket);
            }
            WSACleanup();
        });
        std::cout << "Server started" << std::endl;
    }

    static void TearDownTestSuite()
    {
        std::cout << "Shutting down Server ..." << std::endl;
        _server.shutdown();
        std::cout << "Server shut down" << std::endl;
    }

    static WorkerThread _server;
};

WorkerThread NetworkSocketWin32::_server{};

TEST_F(NetworkSocketWin32, InitDeinit)
{
    SocketWin32 sut{};
    EXPECT_FALSE(sut.isConnected());
}

TEST_F(NetworkSocketWin32, ConnectingToInvalidAddressReturnsFalse)
{
    SocketWin32 sut{};
    EXPECT_FALSE(sut.connectTo("127.0.0.1", 47112));
    EXPECT_FALSE(sut.isConnected());
}

TEST_F(NetworkSocketWin32, ConnectingToServerReturnsTrue)
{
    SocketWin32 sut{};
    EXPECT_TRUE(sut.connectTo("127.0.0.1", 8000));
    EXPECT_TRUE(sut.isConnected());
}

TEST_F(NetworkSocketWin32, CallingConnectOnAlreadyConnectedSocketReturnsFalse)
{
    SocketWin32 sut{};
    EXPECT_TRUE(sut.connectTo("127.0.0.1", 47111));
    EXPECT_TRUE(sut.isConnected());
    EXPECT_FALSE(sut.connectTo("127.0.0.1", 47111));
}

} // namespace Terrahertz::UnitTests

#endif // !_WIN32
