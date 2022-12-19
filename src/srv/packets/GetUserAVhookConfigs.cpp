//
// Created by nullifiedvlad on 20.12.2022.
//
#include "GetUserAVhookConfigs.h"
#include "exceptions.h"
#include "../../lib/sqlite/connection.h"
#include "fmt/format.h"


namespace server::packet
{
    std::string GetUserAVhookConfigs::execute_payload(int userId)
    {
        std::vector<nlohmann::json> cfgs;
        nlohmann::json out;
        for (const auto& cfgData : sql::Connection::get()->query(fmt::format("SELECT `id`, `data` FROM `avhook-configs` WHERE `owner_id` = {}", userId)))
        {
            nlohmann::json tmp;
            tmp["id"]   = std::stoi(cfgData[0]);
            tmp["data"] = nlohmann::json::parse(cfgData[1]);
            cfgs.push_back(tmp);
        }

        out["configs"] = cfgs;

        return out.dump();
    }

    GetUserAVhookConfigs::GetUserAVhookConfigs(const nlohmann::json &data) : Base(data)
    {}
} // packet