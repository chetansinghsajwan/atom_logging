#pragma once
// import atom.core;
#include "atom/logging/LogMsg.h"

namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// Abstract class defining a log target (like a file, console, network).
    /// --------------------------------------------------------------------------------------------
    class LogTarget
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Writes the message into underlying target.
        /// ----------------------------------------------------------------------------------------
        virtual auto Write(const LogMsg& logMsg) -> void = 0;

        /// ----------------------------------------------------------------------------------------
        /// If storing messages in a buffer, flushes it.
        /// ----------------------------------------------------------------------------------------
        virtual auto Flush() -> void = 0;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ptr type used to manage LogTarget in this logging api. It's recommended to use this
    /// to type to store LogTarget instance. This keeps compatibility with the logging api.
    /// --------------------------------------------------------------------------------------------
    using LogTargetPtr = SharedPtr<LogTarget>;
}
