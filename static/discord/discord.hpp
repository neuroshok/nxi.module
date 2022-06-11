#ifndef EXP_MODULE_TEST_H_NXI
#define EXP_MODULE_TEST_H_NXI

#include <nxi/module.hpp>
#include <vector>
#include <nxi/suggestion/vector.hpp>
#include "source/discord.h"


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

namespace nxi
{
    class user;
}

namespace nxi::modules
{
    class discord : public nxi::module
    {
        struct contact
        {
            ::discord::UserId id;
            std::string name;
        };

        struct discord_suggestion : text_suggestion
        {
            discord_suggestion(::discord::UserId user_id_, QString name)
                : text_suggestion(std::move(name))
                , user_id{ user_id_ }
            {}
            ::discord::UserId user_id;
        };

    public:
        explicit discord(nxi::user&);

        void on_load();
        void init();
        void send_invite(::discord::UserId);

    private:
        nxi::user& user_;
        QString invite_message_;

        //std::vector<contact> friends_;
        std::vector<discord_suggestion> friends_;

        DiscordState state{};
        DiscordCreateParams p{};
        ::discord::Core* core{};
    };
} // nxi::modules

#endif // EXP_MODULE_TEST_H_NXI