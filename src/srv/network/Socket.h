//
// Created by Vlad on 06.01.2023.
//
#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "../packets/BasePacket.h"

#if defined(_WIN32)
typedef unsigned long long SOCKET;
#else
typedef int SOCKET;
#endif
namespace Web::Network
{

    class Socket
    {
    public:
        Socket(SOCKET socket);
        Socket();
        SOCKET GetRawSocket() const;

        Socket(int af, int type, int protocol);
        std::string RecvString() const;
        nlohmann::json RecvJson() const;
        void SendString(const std::string& str) const;
        void SendJson(const nlohmann::json& jsn) const;
        void Bind(const std::string& ip, int iPort);
        Socket Listen();
        std::shared_ptr<Web::Packet::BasePacket> RecvPacket() const;

    private:
        std::shared_ptr<SOCKET> m_pRawSocket;
    };

} // Network
