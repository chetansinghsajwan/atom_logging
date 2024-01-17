#pragma once
#include "atom/logging/logger.h"
#include "atom/logging/simple_logger.h"

namespace atom::logging
{
    class logger_factory
    {
    public:
        auto create_logger(string name) -> logger_ptr
        {
            return make_logger<simple_logger_st>(mov(name));
        }
    };

    inline auto get_logger_factory() -> logger_factory&
    {
        static logger_factory instance;
        return instance;
    }

    inline auto create_logger(auto&&... args) -> logger_ptr
    {
        return get_logger_factory().create_logger(fwd(args)...);
    }
}
