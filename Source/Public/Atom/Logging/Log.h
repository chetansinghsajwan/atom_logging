#pragma once
#include "Atom/Logging/LoggerRegistry.h"

namespace Atom::Logging
{
    inline LoggerPtr GET_ROOT_LOGGER()
    {
        return GET_REGISTRY().GetDefaultLogger();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Runtime Logging
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// ----------------------------------------------------------------------------------------
    /// Checks if RootLogger should log.
    ///
    /// @PARAM[IN] lvl ELogLevel to check against.
    /// ----------------------------------------------------------------------------------------
    inline auto CHECK_LOG_LEVEL(ELogLevel lvl) -> bool
    {
        return GET_ROOT_LOGGER()->CheckLogLevel(lvl);
    }

    /// ----------------------------------------------------------------------------------------
    /// Calls Log on GET_ROOT_LOGGER().
    /// ----------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto LOG(ELogLevel lvl, LogStr<TArgs...> msg, TArgs&&... args)
    {
        GET_ROOT_LOGGER()->Log(lvl, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Trace, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto LOG_TRACE(LogStr<TArgs...> msg, TArgs&&... args)
    {
        LOG(ELogLevel::Trace, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Debug, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto LOG_DEBUG(LogStr<TArgs...> msg, TArgs&&... args)
    {
        LOG(ELogLevel::Debug, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Info, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto LOG_INFO(LogStr<TArgs...> msg, TArgs&&... args)
    {
        LOG(ELogLevel::Info, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Warn, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto LOG_WARN(LogStr<TArgs...> msg, TArgs&&... args)
    {
        LOG(ELogLevel::Warn, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Error, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto LOG_ERROR(LogStr<TArgs...> msg, TArgs&&... args)
    {
        LOG(ELogLevel::Error, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Fatal, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto LOG_FATAL(LogStr<TArgs...> msg, TArgs&&... args)
    {
        LOG(ELogLevel::Fatal, msg, fwd(args)...);
    }

    inline auto FLUSH_LOGS()
    {
        GET_ROOT_LOGGER()->Flush();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Static Logging
    //// Logging is checked at compile time and hopefully removed at runtime.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// @TODO Add this as option.
    /// --------------------------------------------------------------------------------------------
    constexpr ELogLevel STATIC_LOG_LEVEL = ELogLevel::Debug;

    /// --------------------------------------------------------------------------------------------
    /// Checks at compile time if message of specified level {lvl} should be logged.
    ///
    /// @TPARAM[IN] lvl ELogLevel of the msg to check for.
    /// --------------------------------------------------------------------------------------------
    template <ELogLevel lvl>
    constexpr auto STATIC_CHECK_LOG_LEVEL() -> bool
    {
        return lvl >= STATIC_LOG_LEVEL && lvl != ELogLevel::OFF;
    }

    /// --------------------------------------------------------------------------------------------
    /// Checks at compile time if message should be logged. If not it's just an empty function
    /// and hopefully compiler will optimize it away, however it's yet to check.
    ///
    /// This is done to afn the use of macros. But if this approach fails we may fallback to
    /// macros.
    /// --------------------------------------------------------------------------------------------
    template <ELogLevel lvl, RLogArg... TArgs>
    inline auto STATIC_LOG(LogStr<TArgs...> msg, TArgs&&... args)
    {
        if constexpr (STATIC_CHECK_LOG_LEVEL<lvl>())
        {
            LOG(lvl, msg, fwd(args)...);
        }
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Trace>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto STATIC_LOG_TRACE(LogStr<TArgs...> msg, TArgs&&... args)
    {
        STATIC_LOG<ELogLevel::Trace>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Debug>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto STATIC_LOG_DEBUG(LogStr<TArgs...> msg, TArgs&&... args)
    {
        STATIC_LOG<ELogLevel::Debug>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Info>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto STATIC_LOG_INFO(LogStr<TArgs...> msg, TArgs&&... args)
    {
        STATIC_LOG<ELogLevel::Info>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Warn>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto STATIC_LOG_WARN(LogStr<TArgs...> msg, TArgs&&... args)
    {
        STATIC_LOG<ELogLevel::Warn>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Error>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto STATIC_LOG_ERROR(LogStr<TArgs...> msg, TArgs&&... args)
    {
        STATIC_LOG<ELogLevel::Error>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Fatal>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline auto STATIC_LOG_FATAL(LogStr<TArgs...> msg, TArgs&&... args)
    {
        STATIC_LOG<ELogLevel::Fatal>(msg, fwd(args)...);
    }
}
