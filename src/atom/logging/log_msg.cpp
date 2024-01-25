export module atom.logging:log_msg;
import :core;
import atom.core;

namespace atom::logging
{
    /// --------------------------------------------------------------------------------------------
    /// log_level is used to define the level of the log.
    /// --------------------------------------------------------------------------------------------
    export enum class log_level : _byte
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
    export class log_msg
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
