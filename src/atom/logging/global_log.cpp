export module atom.logging:global_log;
import :logger;
import :log_msg;
import :logger_registry;
import atom.core;

export namespace atom::logging
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
    template <typename... arg_types>
    inline auto log(log_level lvl, format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        get_root_logger()->log(lvl, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::trace, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto log_trace(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        log(log_level::trace, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::debug, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto log_debug(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        log(log_level::debug, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::info, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto log_info(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        log(log_level::info, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::warn, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto log_warn(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        log(log_level::warn, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::error, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto log_error(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        log(log_level::error, msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls log(log_level::fatal, msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto log_fatal(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
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
    template <log_level lvl, typename... arg_types>
    inline auto static_log(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        if constexpr (static_check_log_level<lvl>())
        {
            log(lvl, msg, fwd(args)...);
        }
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::trace>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto static_log_trace(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        static_log<log_level::trace>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::debug>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto static_log_debug(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        static_log<log_level::debug>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::info>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto static_log_info(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        static_log<log_level::info>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::warn>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto static_log_warn(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        static_log<log_level::warn>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::error>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto static_log_error(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        static_log<log_level::error>(msg, fwd(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// calls static_log<log_level::fatal>(msg, fwd(args)...).
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    inline auto static_log_fatal(format_string<arg_types...> msg, arg_types&&... args)
        requires(rstring_formattable<arg_types> and ...)
    {
        static_log<log_level::fatal>(msg, fwd(args)...);
    }
}
