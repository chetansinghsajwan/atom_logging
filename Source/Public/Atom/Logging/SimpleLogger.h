#pragma once
#include "Atom/Logging/LogTarget.h"
#include "Atom/Logging/LogTargets/MultiLogTarget.h"
#include "Atom/Logging/Logger.h"

namespace Atom::Logging::Internal
{
    /// --------------------------------------------------------------------------------------------
    /// --- DOC_TEMPLATE
    /// --------------------------------------------------------------------------------------------
    /// Basic Logger object.
    ///
    /// @THREAD_SAFETY SAFE
    /// --------------------------------------------------------------------------------------------
    template <bool ST>
    class SimpleLoggerTemplate: public Logger
    {
        using MultiLogTarget = TTI::TConditional<ST, MultiLogTargetST, MultiLogTargetMT>;
        using AtomicLogLevel = TTI::TConditional<ST, ELogLevel, Atomic<ELogLevel>>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        ///
        /// @PARAM[IN] name Name of this logger.
        /// ----------------------------------------------------------------------------------------
        explicit SimpleLoggerTemplate(Str name)
            : _name(mov(name))
            , targets()
        {}

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget objects.
        /// Constructs {targets(targets)}.
        ///
        /// @PARAM[IN] name Name of this logger.
        /// @PARAM[IN] targets LogTarget objects to add.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        SimpleLoggerTemplate(Str name, const TRange& targets)
            requires(RRangeOf<TRange, LogTargetPtr>)
            : _name(mov(name))
            , targets(targets)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get the name of this Logger.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto Name() const -> StrView override final
        {
            return _name;
        }

        /// ----------------------------------------------------------------------------------------
        /// Writes to all LogTarget objects.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto Log(LogMsg& logMsg) -> void override
        {
            targets.Write(logMsg);
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes all LogTarget objects.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto Flush() -> void override
        {
            targets.Flush();
        }

        /// ----------------------------------------------------------------------------------------
        /// Set log level.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto SetLogLevel(ELogLevel lvl)
        {
            _logLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get log level.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto GetLogLevel(ELogLevel lvl) -> ELogLevel
        {
            return _logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if Log of level {lvl} should be logged.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto CheckLogLevel(ELogLevel lvl) const -> bool override final
        {
            return lvl != ELogLevel::OFF && lvl >= _logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Set flush level.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto SetFlushLevel(ELogLevel lvl)
        {
            _flushLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get flush level.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto GetFlushLevel() const -> ELogLevel
        {
            return _flushLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if Log of level {lvl} should be flushed.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        auto CheckFlushLevel(ELogLevel lvl) const -> bool
        {
            return lvl != ELogLevel::OFF && lvl >= _flushLevel;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// The object used to manage LogTarget objects. Accessing it is thread safe.
        ///
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        MultiLogTarget targets;

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Name of this logger.
        /// ----------------------------------------------------------------------------------------
        const Str _name;

        /// ----------------------------------------------------------------------------------------
        /// ELogLevel used to filter logs.
        /// ----------------------------------------------------------------------------------------
        AtomicLogLevel _logLevel;

        /// ----------------------------------------------------------------------------------------
        /// ELogLevel used to filter flush after logs.
        /// ----------------------------------------------------------------------------------------
        AtomicLogLevel _flushLevel;
    };
}

namespace Atom::Logging
{
    using SimpleLoggerST = Internal::SimpleLoggerTemplate<true>;
    using SimpleLoggerMT = Internal::SimpleLoggerTemplate<false>;
}
