export module atom_logging:log_typearget;

import :log_msg;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// abstract class defining a log target (like a file, console, network).
    /// --------------------------------------------------------------------------------------------
    export class log_typearget
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// writes the message into underlying target.
        /// ----------------------------------------------------------------------------------------
        virtual auto write(const log_msg& log_msg) -> void = 0;

        /// ----------------------------------------------------------------------------------------
        /// if storing messages in a buffer, flushes it.
        /// ----------------------------------------------------------------------------------------
        virtual auto flush() -> void = 0;
    };
}
