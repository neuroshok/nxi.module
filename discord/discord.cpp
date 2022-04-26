#include "discord.hpp"

#include <nxi/core.hpp>
#include <nxi/system/window.hpp>
#include <nxi/log.hpp>


#include <QString>
#include <array>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "source/discord.h"

static std::string data;

#if defined(_WIN32)
#pragma pack(push, 1)
struct BitmapImageHeader {
    uint32_t const structSize{sizeof(BitmapImageHeader)};
    int32_t width{0};
    int32_t height{0};
    uint16_t const planes{1};
    uint16_t const bpp{32};
    uint32_t const pad0{0};
    uint32_t const pad1{0};
    uint32_t const hres{2835};
    uint32_t const vres{2835};
    uint32_t const pad4{0};
    uint32_t const pad5{0};

    BitmapImageHeader& operator=(BitmapImageHeader const&) = delete;
};

struct BitmapFileHeader {
    uint8_t const magic0{'B'};
    uint8_t const magic1{'M'};
    uint32_t size{0};
    uint32_t const pad{0};
    uint32_t const offset{sizeof(BitmapFileHeader) + sizeof(BitmapImageHeader)};

    BitmapFileHeader& operator=(BitmapFileHeader const&) = delete;
};
#pragma pack(pop)
#endif

struct DiscordState {
    discord::User currentUser;

    std::unique_ptr<discord::Core> core;
};

namespace {
volatile bool interrupted{false};
}


void f()
{
    DiscordState state{};
    DiscordCreateParams p{};
    discord::Core* core{};
    auto result = discord::Core::Create(807065370573668372, DiscordCreateFlags_Default, &core);
    state.core.reset(core);
    if (!state.core)
    {
        std::cout << "Failed to instantiate discord core! (err " << static_cast<int>(result)
                  << ")\n";
        std::exit(-1);
    }

    state.core->SetLogHook(
      discord::LogLevel::Debug, [](discord::LogLevel level, const char* message) {
          std::cerr << "Log(" << static_cast<uint32_t>(level) << "): " << message << "\n";
      });


    discord::User user;
    state.core->RelationshipManager().OnRefresh.Connect([&state]()
    {
          std::cout << "refresh\n";
        state.core->RelationshipManager().Filter([](const discord::Relationship& rel) -> bool
        {
                                                       data += rel.GetUser().GetUsername() + std::string("\n");
              std::cout << rel.GetUser().GetUsername() << "\n";
              return true;
        });
    });
    state.core->RelationshipManager().OnRelationshipUpdate.Connect([](const discord::Relationship& rel)
    {
          std::cout << rel.GetUser().GetUsername() << "\n";
    });





    discord::LobbyTransaction l;
    state.core->LobbyManager().GetLobbyCreateTransaction(&l);
    l.SetCapacity(10);
    l.SetType(discord::LobbyType::Public);

    state.core->ActivityManager().OnActivityInvite.Connect([](discord::ActivityActionType, discord::User const &, discord::Activity const &) {
        std::cout << "accept";
        system("D:\\project\\nk\\nxi\\bin\\nxi\\nxi.exe");
    });

    /*
    state.core->LobbyManager().CreateLobby(l, [&state](discord::Result result, discord::Lobby const& lobby)
    {
        discord::Activity activity{};
        activity.SetName("zeta");
        activity.SetType(discord::ActivityType::Playing);
        activity.SetState("nxi");
        activity.SetDetails("[sniper][bomberman]");
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
        //activity.GetSecrets().SetMatch(lobby.GetSecret());

        state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result)
                                                     {
                                                         std::cout << "ok";
                                                     });
        state.core->ActivityManager().SendInvite(820766957171245077, discord::ActivityActionType::Join,"", [](discord::Result result)
        {
            std::cout << (int)result;
        });

        state.core->ActivityManager().AcceptInvite(820766957171245077, [](discord::Result)
                                                   {
                                                       system("D:\\project\\nk\\nxi\\bin\\nxi\\nxi.exe");
                                                   });
        state.core->ActivityManager().RegisterCommand("nxi");

    });*/





    do {
        state.core->RunCallbacks();


        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } while (true);
}

namespace nxi::modules
{
    discord::discord(nxi::session& session)
        : module("discord", module_type::compiled)
        , session_{ session }
    {}

    void discord::on_load()
    {
        auto cmd = nxi::command("discord", "friend", [this](const nxi::values&)
        {
            session_.error(QString::fromStdString(::data));
        });

        session_.command_system().add(std::move(cmd));

        std::thread t(&f);
        t.detach();
    }

} // nxi::modules