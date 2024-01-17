#pragma once
// To Do: Remove this.
// #include "fmt/chrono.h"

#include "Internal/Core.h"

// import atom.core;

namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// ELogLevel is used to define the level of the log.
    /// --------------------------------------------------------------------------------------------
    enum class ELogLevel : byte
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,

        /// Used to indicate logging is off.
        OFF
    };

    /// --------------------------------------------------------------------------------------------
    /// Lightweight object containing all the details of the log.
    /// --------------------------------------------------------------------------------------------
    class LogMsg
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Message of the log.
        /// ----------------------------------------------------------------------------------------
        StringView msg;

        /// ----------------------------------------------------------------------------------------
        /// Name of the logger through which this message was logged.
        /// ----------------------------------------------------------------------------------------
        StringView loggerName;

        /// ----------------------------------------------------------------------------------------
        /// Level of this message.
        /// ----------------------------------------------------------------------------------------
        ELogLevel lvl;

        /// ----------------------------------------------------------------------------------------
        /// Time when this message was logged.
        ///
        /// This is necessary to store as the message may be logged asynchronously or may face
        /// some latency in writing.
        /// ----------------------------------------------------------------------------------------
        TimePoint time;
    };
}

namespace Atom
{
    template <>
    class StringViewConverter<Logging::ELogLevel>
    {
    public:
        constexpr auto Convert(Logging::ELogLevel lvl) -> StringView
        {
            switch (lvl)
            {
                case Logging::ELogLevel::Trace: return MakeRange("Trace"); break;
                case Logging::ELogLevel::Debug: return MakeRange("Debug"); break;
                case Logging::ELogLevel::Info:  return MakeRange("Info"); break;
                case Logging::ELogLevel::Warn:  return MakeRange("Warn"); break;
                case Logging::ELogLevel::Error: return MakeRange("Error"); break;
                case Logging::ELogLevel::Fatal: return MakeRange("Fatal"); break;
                case Logging::ELogLevel::OFF:   return MakeRange("OFF"); break;
                default:                        return MakeRange("UNKNOWN");
            }
        }
    };

    static_assert(RStringFmtArgFmtable<Logging::ELogLevel>, "ELogLevel is not formattable.");
}
