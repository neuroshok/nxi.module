#include "adk.hpp"

#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/page/web.hpp>
#include <nxi/system/window.hpp>
#include <nxi/user.hpp>
#include <nxi/web_session.hpp>

#include <ui/core.hpp>

#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineUrlRequestInfo>
#include <QWebEngineProfile>

struct interceptor : QWebEngineUrlRequestInterceptor
{
    virtual void interceptRequest(QWebEngineUrlRequestInfo& info) override
    {
        //qDebug() << info.requestUrl();
        if (info.requestUrl().toString().contains("adsbygoogle")) info.block(true);
    }
};

namespace nxi::modules
{
    adk::adk(nxi::user_session& session)
        : module("adk", module_type::compiled)
        , session_{ session }
        , interceptor_{ new ::interceptor }
    {
    }

    void adk::on_load()
    {
        session_.web_session().setUrlRequestInterceptor(interceptor_);

        auto cmd = nxi::command("adk", "main", [this](const nxi::values&)
        {
            session_.page_system().open<nxi::web_page>(nxi::core::module_path() + "/adk.html");
        });

        session_.command_system().add(std::move(cmd) /*, this (source) */ );
    }

    void adk::on_unload()
    {
        session_.web_session().setUrlRequestInterceptor(nullptr);


        // session_.command_system().del(this); // delete all commands from this module
    }

} // nxi::modules