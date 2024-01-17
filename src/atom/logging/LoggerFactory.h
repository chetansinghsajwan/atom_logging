#pragma once
#include "atom/logging/Logger.h"
#include "atom/logging/SimpleLogger.h"

namespace Atom::Logging
{
    class LoggerFactory
    {
    public:
        auto CreateLogger(String name) -> LoggerPtr
        {
            return MAKE_LOGGER<SimpleLoggerST>(mov(name));
        }
    };

    inline auto GET_LOGGER_FACTORY() -> LoggerFactory&
    {
        static LoggerFactory instance;
        return instance;
    }

    inline auto CREATE_LOGGER(auto&&... args) -> LoggerPtr
    {
        return GET_LOGGER_FACTORY().CreateLogger(fwd(args)...);
    }
}
