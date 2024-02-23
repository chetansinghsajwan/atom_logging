export module atom.logging:log_target;
import :log_msg;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// abstract class defining a log target (like a file, console, network).
    /// --------------------------------------------------------------------------------------------
    export class log_target
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
