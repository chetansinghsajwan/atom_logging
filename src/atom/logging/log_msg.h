#pragma once
// to do: remove this.
// #include "fmt/chrono.h"

#include "internal/core.h"

// import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// log_level is used to define the level of the log.
    /// --------------------------------------------------------------------------------------------
    enum class log_level : _byte
    {
        trace,
        debug,
        info,
        warn,
        error,
        fatal,

        /// used to indicate logging is off.
        off
    };

    /// --------------------------------------------------------------------------------------------
    /// lightweight object containing all the details of the log.
    /// --------------------------------------------------------------------------------------------
    class log_msg
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// message of the log.
        /// ----------------------------------------------------------------------------------------
        string_view msg;

        /// ----------------------------------------------------------------------------------------
        /// name of the logger through which this message was logged.
        /// ----------------------------------------------------------------------------------------
        string_view logger_name;

        /// ----------------------------------------------------------------------------------------
        /// level of this message.
        /// ----------------------------------------------------------------------------------------
        log_level lvl;

        /// ----------------------------------------------------------------------------------------
        /// time when this message was logged.
        ///
        /// this is necessary to store as the message may be logged asynchronously or may face
        /// some latency in writing.
        /// ----------------------------------------------------------------------------------------
        time_point time;
    };
}

namespace atom
{
    template <>
    class string_view_converter<logging::log_level>
    {
    public:
        constexpr auto convert(logging::log_level lvl) -> string_view
        {
            switch (lvl)
            {
                case logging::log_level::trace: return make_range("trace"); break;
                case logging::log_level::debug: return make_range("debug"); break;
                case logging::log_level::info:  return make_range("info"); break;
                case logging::log_level::warn:  return make_range("warn"); break;
                case logging::log_level::error: return make_range("error"); break;
                case logging::log_level::fatal: return make_range("fatal"); break;
                case logging::log_level::off:   return make_range("off"); break;
                default:                        return make_range("unknown");
            }
        }
    };

    static_assert(rstring_fmt_arg_fmtable<logging::log_level>, "log_level is not formattable.");
}
