#pragma once
#include "Atom/Logging/LogTarget.h"

namespace Atom::Logging::Internal
{
    /// --------------------------------------------------------------------------------------------
    /// Base class for LogTargets with base functionality like formatting, thread safety and
    /// level filtering.
    ///
    /// @TODO Add thread safety.
    /// @TODO Make default log and flush level global.
    /// --------------------------------------------------------------------------------------------
    class LogTargetBase: public LogTarget
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// # Default Constructor
        /// ----------------------------------------------------------------------------------------
        LogTargetBase():
            _logLevel(ELogLevel::Debug), _flushLevel(ELogLevel::Info), _hasWritten(false),
            _alwaysFlush(false)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// Filters and Formats the LogMsg and passes it to write.
        /// ----------------------------------------------------------------------------------------
        virtual auto Write(const LogMsg& logMsg) -> void override final
        {
            if (CheckLogLevel(logMsg.lvl))
            {
                // TODO: Add chrono support for Atom.Fmt.
                // Str result = StrFmter().Fmt("[{}] [{}] {}: {}\n",
                //     logMsg.time, logMsg.lvl, logMsg.loggerName, logMsg.msg);

                Str result =
                    StrFmter().Fmt("[{}] {}: {}\n", logMsg.lvl, logMsg.loggerName, logMsg.msg);

                _hasWritten = true;
                _Write(logMsg, result);

                if (CheckFlushLevel(logMsg.lvl))
                {
                    _Flush();
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes if `[`ShouldFlush()`] == true`.
        /// ----------------------------------------------------------------------------------------
        virtual auto Flush() -> void override final
        {
            if (ShouldFlush())
            {
                _Flush();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the log level.
        /// ----------------------------------------------------------------------------------------
        auto GetLogLevel() const -> ELogLevel
        {
            return _logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the log level.
        /// ----------------------------------------------------------------------------------------
        auto SetLogLevel(ELogLevel lvl)
        {
            _logLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if we should log the message of specified level.
        /// ----------------------------------------------------------------------------------------
        auto CheckLogLevel(ELogLevel lvl) const -> bool
        {
            if (lvl == ELogLevel::OFF)
                return false;
            if (lvl < _logLevel)
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Gets the flush level.
        /// ----------------------------------------------------------------------------------------
        auto GetFlushLevel() const -> ELogLevel
        {
            return _flushLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the flush level.
        /// ----------------------------------------------------------------------------------------
        auto SetFlushLevel(ELogLevel lvl)
        {
            _flushLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if should flush after logging the message of specified level.
        /// It also asks [`ShouldFlush()`].
        /// ----------------------------------------------------------------------------------------
        auto CheckFlushLevel(ELogLevel lvl) const -> bool
        {
            if (!_hasWritten)
                return false;
            if (lvl == ELogLevel::OFF)
                return false;
            if (lvl < _flushLevel)
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if we should flush.
        ///
        /// # Returns
        /// `true` if there has been a log since last flush, else `false`.
        /// ----------------------------------------------------------------------------------------
        auto ShouldFlush() const -> bool
        {
            return _alwaysFlush || _hasWritten;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Write implementation.
        ///
        /// @PARAM[IN] logMsg Log message object passed for logging.
        /// @PARAM[IN] formattedMsg Formatted message generated from {logMsg}.
        /// ----------------------------------------------------------------------------------------
        virtual auto _Write(const LogMsg& logMsg, StrView formattedMsg) -> void abstract;

        /// ----------------------------------------------------------------------------------------
        /// Flush implementation.
        /// ----------------------------------------------------------------------------------------
        virtual auto _Flush() -> void abstract;

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Log Level used to filter log messages.
        /// ----------------------------------------------------------------------------------------
        ELogLevel _logLevel;

        /// ----------------------------------------------------------------------------------------
        /// Flush level used to check if to call flush after logging.
        /// ----------------------------------------------------------------------------------------
        ELogLevel _flushLevel;

        /// ----------------------------------------------------------------------------------------
        /// Value used to check if there has been any write since last flush.
        /// ----------------------------------------------------------------------------------------
        bool _hasWritten;

        /// ----------------------------------------------------------------------------------------
        /// If true always calls underlying flush _Flush(), even if not necessary.
        /// This doesn't override CheckFlushLevel(ELogLevel lvl) check. It only affects
        /// ShouldFlush().
        /// ----------------------------------------------------------------------------------------
        bool _alwaysFlush;
    };
}
