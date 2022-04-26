#ifndef EXP_MODULE_TEST_H_NXI
#define EXP_MODULE_TEST_H_NXI

#include <nxi/module.hpp>

namespace nxi { class session; }

namespace nxi::modules
{
    class discord : public nxi::module
    {
    public:
        explicit discord(nxi::session&);

        void on_load();

    private:
        nxi::session& session_;

    };
} // nxi::modules

#endif // EXP_MODULE_TEST_H_NXI