#include "discord.hpp"

#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/system/window.hpp>

#include <array>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <QString>

#include "source/discord.h"

namespace nxi::modules
{
    discord::discord(nxi::user& user)
        : module("discord", module_type::compiled)
        , user_{ user }
    {}

    void discord::on_load()
    {
        nxi::command_data share;
        share.module = "discord";
        share.action = "share";
        share.description = "Share this page with a discord friend";
        share.function = [this](const nxi::values& values) {
            nxi_assert(values.size() > 1);
            auto id = values.get(1).toLongLong();
            send_invite(id);
        };

        share.parameters = { { "friend_name", [this](nxi::suggestion_vector& suggestion) {
                                  for (auto f: friends_) suggestion.push_back(f);
                              } },
                             { "id" } };

        session_.command_system().add(std::move(share));

        auto f = [this] {
            do
            {
                state.core->RunCallbacks();

                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            } while (true);
        };

        init();

        std::thread t(f);
        t.detach();
    }

    void discord::init()
    {
        auto result = ::discord::Core::Create(807065370573668372, DiscordCreateFlags_Default, &core);
        state.core.reset(core);
        if (!state.core)
        {
            std::cout << "Failed to instantiate discord core! (err " << static_cast<int>(result) << ")\n";
            std::exit(-1);
        }

        state.core->SetLogHook(::discord::LogLevel::Debug, [](::discord::LogLevel level, const char* message) {
            std::cerr << "Log(" << static_cast<uint32_t>(level) << "): " << message << "\n";
        });

        ::discord::User user;
        state.core->RelationshipManager().OnRefresh.Connect([this]() {
            state.core->RelationshipManager().Filter([this](const ::discord::Relationship& rel) -> bool {
                contact c;
                c.id = rel.GetUser().GetId();
                c.name = rel.GetUser().GetUsername();
                friends_.push_back(discord_suggestion{ c.id, QString::fromStdString(c.name) });
                return true;
            });
        });
        /*
        state.core->RelationshipManager().OnRelationshipUpdate.Connect(
        [](const ::discord::Relationship& rel) { std::cout << rel.GetUser().GetUsername() << "\n"; });*/

        state.core->ActivityManager().OnActivityInvite.Connect([](::discord::ActivityActionType, ::discord::User const&, ::discord::Activity const&) {
            std::cout << "accept";
            // system("D:\\project\\nk\\nxi\\bin\\nxi\\nxi.exe");
        });

        state.core->ActivityManager().AcceptInvite(820766957171245077, [](::discord::Result) { system("D:\\project\\nk\\nxi\\bin\\nxi\\nxi.exe"); });
        state.core->ActivityManager().RegisterCommand("nxi");
    }

    void discord::send_invite(::discord::UserId id)
    {
        ::discord::LobbyTransaction l;
        state.core->LobbyManager().GetLobbyCreateTransaction(&l);
        l.SetCapacity(10);
        l.SetType(::discord::LobbyType::Public);

        auto url = session_.page_system().focus().command();
        ::discord::User self;
        state.core->UserManager().GetCurrentUser(&self);
        invite_message_ = "**" + QString{ self.GetUsername() } + "** shared with you this page from **nxi browser**\n<" + url + ">";


        state.core->LobbyManager().CreateLobby(l, [id, this](::discord::Result result, ::discord::Lobby const& lobby) {
            ::discord::Activity activity{};
            activity.SetName("zeta");
            activity.SetType(::discord::ActivityType::Playing);
            activity.SetState("nxi");
            activity.SetDetails("[details][]");
            activity.GetTimestamps().SetStart(std::time(nullptr));
            activity.GetTimestamps().SetEnd(std::time(nullptr) + 1000);
            activity.GetAssets().SetLargeImage("large");
            activity.GetAssets().SetSmallImage("large");
            activity.GetAssets().SetLargeText("__");
            activity.GetAssets().SetSmallText("__");
            activity.GetParty().SetId("200");
            activity.GetParty().GetSize().SetCurrentSize(3);
            activity.GetParty().GetSize().SetMaxSize(10);
            activity.GetSecrets().SetJoin(lobby.GetSecret());
            activity.GetSecrets().SetSpectate("lobby.GetSecret()");
            // activity.GetSecrets().SetMatch(lobby.GetSecret());

            state.core->ActivityManager().UpdateActivity(activity, [](::discord::Result result) { std::cout << "ok"; });

            //session_.error(id.toULongLong());
            state.core->ActivityManager().SendInvite(id, ::discord::ActivityActionType::Join, invite_message_.toStdString().c_str(),
                                                     [this](::discord::Result result){ session_.error("invite sent"); });
        });
    }
} // namespace nxi::modules