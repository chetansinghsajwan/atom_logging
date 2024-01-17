#pragma once
#include "atom/logging/logger_registry.h"

namespace atom::logging
{
    inline logger_ptr get_root_logger()
    {
        return get_registry().get_default_logger();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// runtime logging
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// ----------------------------------------------------------------------------------------
    /// checks if root_logger should log.
    ///
    /// @param[in] lvl log_level to check against.
    /// ----------------------------------------------------------------------------------------
    inline auto check_log_level(log_level lvl) -> bool
    {
        return get_root_logger()->check_log_level(lvl);
    }

    /// ----------------------------------------------------------------------------------------
    /// calls log on get_root_logger().
    /// ----------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto log(log_level lvl, log_str<targs...> msg, targs&&... args)
    {
        get_root_logger()->log(lvl, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::trace, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto log_trace(log_str<targs...> msg, targs&&... args)
    {
        log(log_level::trace, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::debug, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto log_debug(log_str<targs...> msg, targs&&... args)
    {
        log(log_level::debug, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::info, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto log_info(log_str<targs...> msg, targs&&... args)
    {
        log(log_level::info, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::warn, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto log_warn(log_str<targs...> msg, targs&&... args)
    {
        log(log_level::warn, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::error, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto log_error(log_str<targs...> msg, targs&&... args)
    {
        log(log_level::error, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::fatal, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto log_fatal(log_str<targs...> msg, targs&&... args)
    {
        log(log_level::fatal, msg, fwd(args)...);
    }

    inline auto flush_logs()
    {
        get_root_logger()->flush();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// static logging
    //// logging is checked at compile time and hopefully removed at runtime.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// # to do
    ///
    /// - add this as option.
    /// --------------------------------------------------------------------------------------------
    constexpr log_level static_log_level = log_level::debug;

    /// --------------------------------------------------------------------------------------------
    /// checks at compile time if message of specified level {lvl} should be logged.
    ///
    /// @tparam[in] lvl log_level of the msg to check for.
    /// --------------------------------------------------------------------------------------------
    template <log_level lvl>
    constexpr auto static_check_log_level() -> bool
    {
        return lvl >= static_log_level && lvl != log_level::off;
    }

    /// --------------------------------------------------------------------------------------------
    /// checks at compile time if message should be logged. if not it's just an empty function
    /// and hopefully compiler will optimize it away, however it's yet to check.
    ///
    /// this is done to afn the use of macros. but if this approach fails we may fallback to
    /// macros.
    /// --------------------------------------------------------------------------------------------
    template <log_level lvl, rlog_arg... targs>
    inline auto static_log(log_str<targs...> msg, targs&&... args)
    {
        if constexpr (static_check_log_level<lvl>())
        {
            log(lvl, msg, fwd(args)...);
        }
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::trace>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto static_log_trace(log_str<targs...> msg, targs&&... args)
    {
        static_log<log_level::trace>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::debug>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto static_log_debug(log_str<targs...> msg, targs&&... args)
    {
        static_log<log_level::debug>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::info>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto static_log_info(log_str<targs...> msg, targs&&... args)
    {
        static_log<log_level::info>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::warn>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto static_log_warn(log_str<targs...> msg, targs&&... args)
    {
        static_log<log_level::warn>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::error>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto static_log_error(log_str<targs...> msg, targs&&... args)
    {
        static_log<log_level::error>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::fatal>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <rlog_arg... targs>
    inline auto static_log_fatal(log_str<targs...> msg, targs&&... args)
    {
        static_log<log_level::fatal>(msg, fwd(args)...);
    }
}
