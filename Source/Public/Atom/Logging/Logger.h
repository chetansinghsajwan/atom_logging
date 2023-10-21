#pragma once
#include "Atom/CoreAll.h"
#include "LogMsg.h"

namespace Atom::Logging
{
    class Logger
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Get the name of the logger.
        /// ----------------------------------------------------------------------------------------
        virtual auto Name() const -> StrView abstract;

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Trace, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        auto LogTrace(LogStr<TArgs...> msg, TArgs&&... args)
        {
            Log(ELogLevel::Trace, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Debug, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        auto LogDebug(LogStr<TArgs...> msg, TArgs&&... args)
        {
            Log(ELogLevel::Debug, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Info, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        auto LogInfo(LogStr<TArgs...> msg, TArgs&&... args)
        {
            Log(ELogLevel::Info, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Warn, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        auto LogWarn(LogStr<TArgs...> msg, TArgs&&... args)
        {
            Log(ELogLevel::Warn, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Error, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        auto LogError(LogStr<TArgs...> msg, TArgs&&... args)
        {
            Log(ELogLevel::Error, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Fatal, msg, fwd(args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        auto LogFatal(LogStr<TArgs...> msg, TArgs&&... args)
        {
            Log(ELogLevel::Fatal, msg, fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Logs message if level is high enough to pass the filter level. Flushes if level passes
        /// flush filter level.
        ///
        /// @PARAM[IN] lvl ELogLevel to log at.
        /// @PARAM[IN] msg Log message containing format of the msg.
        /// @PARAM[IN] args... Arguments used with {msg} to construct the formatted message.
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        auto Log(ELogLevel lvl, LogStr<TArgs...> msg, TArgs&&... args)
        {
            if (CheckLogLevel(lvl))
            {
                Str formattedMsg = StrFmter().Fmt(msg, fwd(args)...);
                LogMsg logMsg{
                    .msg = formattedMsg,
                    .loggerName = Name(),
                    .lvl = lvl,
                    .time = Time::Now(),
                };

                Log(logMsg);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Pure virtual function.
        ///
        /// Logs the LogMsg object.
        /// ----------------------------------------------------------------------------------------
        virtual auto Log(LogMsg& logMsg) -> void abstract;

        /// ----------------------------------------------------------------------------------------
        /// Pure virtual function.
        ///
        /// Flushes the logs of this logger.
        /// ----------------------------------------------------------------------------------------
        virtual auto Flush() -> void abstract;

        /// ----------------------------------------------------------------------------------------
        /// Pure virtual function.
        ///
        /// Check if the message should be passed for logging.
        /// ----------------------------------------------------------------------------------------
        virtual auto CheckLogLevel(ELogLevel lvl) const -> bool abstract;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ptr type used by this logging api to manage a Logger. It's recommended to use this
    /// type to store logger instance. This keeps compatibility with this logging api.
    /// --------------------------------------------------------------------------------------------
    using LoggerPtr = SharedPtr<Logger>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <RDerivedFrom<Logger> TLogger>
    LoggerPtr MAKE_LOGGER(auto&&... args)
    {
        return MakeShared<TLogger>(fwd(args)...);
    }
}
