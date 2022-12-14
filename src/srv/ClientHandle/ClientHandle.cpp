//
// Created by Vlad on 27.12.2022.
//
#include "ClientHandle.h"
#include "../packets/Auth.h"
#include "../packets/exceptions.h"
#include "../../lib/sqlite/connection.h"

#include "../observers/OnUserConnected.h"
#include "../observers/OnUserDisconnected.h"
#include "../observers/OnUserAuth.h"
#include "../observers/OnPacket.h"


#include <fmt/format.h>


void Web::ClientHandle::Listen()
{
    AuthClient();
    while (true)
    {
        try
        {
            OnPacket(m_clientSocket.RecvPacket());
        }
        catch (const Web::Packet::Exception::BasePacketException& ex)
        {
            nlohmann::json jsn;

            jsn["success"]    = false;
            jsn["error_code"] = ex.what();

            m_clientSocket.SendJson(jsn);
        }
    }
}

Web::ClientHandle::~ClientHandle()
{

}

void Web::ClientHandle::AuthClient()
{
	while (true)
	{
		try
		{
			auto pAuthPacket = m_clientSocket.RecvPacket();

			if (!dynamic_cast<Web::Packet::Auth*>(pAuthPacket.get()))
				throw Web::Packet::Exception::ExceptedAutPacket();

			m_iUserIdInDataBase = pAuthPacket->ExecutePayload(NULL)["user_id"].get<int>();


			nlohmann::json jsn;
			jsn["success"] = true;
			NotifyObserver<Observers::OnUserAuth>();
            m_clientSocket.SendJson(jsn);

			return;
		}
		catch(const Web::Packet::Exception::BasePacketException& ex)
		{
			nlohmann::json jsn;
			jsn["success"] = false;
			jsn["error_code"] = ex.what();
            m_clientSocket.SendJson(jsn);
		}

	}
}

void Web::ClientHandle::OnPacket(const std::shared_ptr<Web::Packet::BasePacket>& pPacket) const
{
	NotifyObserver<Observers::OnPacket>();
	auto jsn = pPacket->ExecutePayload(m_iUserIdInDataBase);
	jsn["success"] = true;

    m_clientSocket.SendJson(jsn);
}

Web::ClientHandle::ClientHandle(const Network::Socket& soc)
{
	m_clientSocket = soc;
}
