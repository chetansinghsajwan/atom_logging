export module atom.logging:log_target;
import :log_msg;
import atom.core;

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

    /// --------------------------------------------------------------------------------------------
    /// ptr type used to manage log_target in this logging api. it's recommended to use this
    /// to type to store log_target instance. this keeps compatibility with the logging api.
    /// --------------------------------------------------------------------------------------------
    export using log_target_ptr = shared_ptr<log_target>;
}
