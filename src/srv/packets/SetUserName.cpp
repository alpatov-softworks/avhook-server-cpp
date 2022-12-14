//
// Created by nullifiedvlad on 06.12.2022.
//
#include "SetUserName.h"
#include <fmt/format.h>
#include "../../lib/sqlite/connection.h"
#include "exceptions.h"
namespace Web::Packet
{

    SetUserName::SetUserName(const nlohmann::json &data) : BasePacket(data)
    {
        try
        {
            m_sNewUserName = m_Data["name"].get<std::string>();
        }
        catch (...)
        {
            throw Exception::CorruptedPacket();
        }
    }

    nlohmann::json SetUserName::ExecutePayload(int userId)
    {
        if (!IsUsernameValid(m_sNewUserName))
            throw Exception::InValidUserName();

		sql::Connection::Get()->Query(
				fmt::format("UPDATE `users` SET `name`= \"{}\" WHERE `id` = {}", m_sNewUserName, userId));

        return "";
    }

    bool SetUserName::IsUsernameValid(const std::string &name)
    {
		return !std::any_of(name.begin(), name.end(), [](char chr) {return (chr == ' ' or chr == '\n' or chr == '\t'); });
    }
} // packet