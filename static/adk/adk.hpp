#ifndef MODULE_STATIC_ADK_ADK_HPP_NXI
#define MODULE_STATIC_ADK_ADK_HPP_NXI

#include <nxi/module.hpp>

#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineUrlRequestInfo>

namespace nxi
{
    class user;
}

struct interceptor;

namespace nxi::modules
{
    class adk : public nxi::module
    {
    public:
        explicit adk(nxi::user&);

        void on_load() override;
        void on_unload() override;

    private:
        nxi::user& user_;
        ::interceptor* interceptor_;
    };
} // nxi::modules

#endif // MODULE_STATIC_ADK_ADK_HPP_NXI